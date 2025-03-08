#include "network_client.h"
#include "utility.h"
#include <iostream>

namespace atom
{
	c_network_client::c_network_client()
		: m_running(false)
		, m_client_id(0)
		, m_connected(false)
	{
	}

	c_network_client::~c_network_client()
	{
		disconnect();
	}

	bool c_network_client::connect(const std::string& server_ip, uint16_t server_port)
	{
		// Already connected
		if (m_connected)
		{
			return true;
		}

		// Connect to server
		sf::Socket::Status status = m_socket.connect(server_ip, server_port, sf::seconds(5));
		if (status != sf::Socket::Done)
		{
			errorln("Failed to connect to server at {}:{}", server_ip, server_port);
			return false;
		}

		// Set socket to non-blocking mode
		m_socket.setBlocking(false);

		// Start client thread
		m_running = true;
		m_connected = true;
		m_client_thread = std::thread(&c_network_client::client_thread_func, this);

		logln("Connected to server at {}:{}", server_ip, server_port);
		return true;
	}

	void c_network_client::disconnect()
	{
		// Not connected
		if (!m_connected)
		{
			return;
		}

		// Stop client thread
		m_running = false;
		if (m_client_thread.joinable())
		{
			m_client_thread.join();
		}

		// Disconnect socket
		m_socket.disconnect();
		m_connected = false;
		m_client_id = 0;

		logln("Disconnected from server");
	}

	bool c_network_client::send_message(const s_network_message& message, uint32_t client_id)
	{
		// Not connected
		if (!m_connected)
		{
			return false;
		}

		std::lock_guard<std::mutex> lock(m_mutex);

		// Prepare packet
		sf::Packet packet;
		packet << static_cast<uint8_t>(message.type);
		packet << message.sender_id;
		packet << static_cast<uint32_t>(message.data.size());
		if (!message.data.empty())
		{
			packet.append(message.data.data(), message.data.size());
		}

		// Send packet
		return (m_socket.send(packet) == sf::Socket::Done);
	}

	void c_network_client::set_message_callback(std::function<void(const s_network_message&, uint32_t)> callback)
	{
		m_message_callback = callback;
	}

	bool c_network_client::update()
	{
		// Nothing to do here, client thread handles everything
		return false; // Return false to indicate no further updates needed
	}

	bool c_network_client::is_connected() const
	{
		return m_connected;
	}

	void c_network_client::client_thread_func()
	{
		while (m_running)
		{
			// Check for messages from server
			sf::Packet packet;
			sf::Socket::Status status = m_socket.receive(packet);

			if (status == sf::Socket::Done)
			{
				// Process packet
				uint8_t type;
				uint32_t sender_id;
				uint32_t data_size;

				packet >> type >> sender_id >> data_size;

				s_network_message message;
				message.type = static_cast<e_message_type>(type);
				message.sender_id = sender_id;

				// Extract data if present
				if (data_size > 0)
				{
					message.data.resize(data_size);
					const char* packet_data = static_cast<const char*>(packet.getData());
					
					// Calculate the correct offset: header size is sizeof(uint8_t) + 2 * sizeof(uint32_t)
					size_t header_size = sizeof(uint8_t) + 2 * sizeof(uint32_t); // type (uint8_t), sender_id (uint32_t), data_size (uint32_t)
					if (packet.getDataSize() >= header_size + data_size)
					{
						std::memcpy(message.data.data(), packet_data + header_size, data_size);
					}
					else
					{
						errorln("Invalid packet size: expected at least {} bytes, got {}", header_size + data_size, packet.getDataSize());
						message.data.clear();
					}
				}

				// Handle connect message (server assigns client ID)
				if (message.type == e_message_type::CONNECT && m_client_id == 0)
				{
					// First 4 bytes of data contain client ID
					if (message.data.size() >= 4)
					{
						m_client_id = *reinterpret_cast<uint32_t*>(message.data.data());
						logln("Assigned client ID: {}", m_client_id);
					}
				}

				// Call message callback
				if (m_message_callback)
				{
					m_message_callback(message, sender_id);
				}
			}
			else if (status == sf::Socket::Disconnected)
			{
				// Server disconnected
				m_connected = false;
				logln("Disconnected from server");
				
				// Notify about disconnection
				if (m_message_callback)
				{
					s_network_message message;
					message.type = e_message_type::DISCONNECT;
					message.sender_id = 0; // Server
					m_message_callback(message, 0);
				}
				
				break;
			}

			// Sleep to avoid high CPU usage
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}
} // namespace atom
