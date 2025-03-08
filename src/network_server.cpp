#include "network_server.h"
#include "utility.h"
#include <iostream>

namespace atom
{
	c_network_server::c_network_server(uint16_t port)
		: m_port(port)
		, m_running(false)
		, m_next_client_id(1) // Start from 1, 0 is reserved for server
	{
	}

	c_network_server::~c_network_server()
	{
		stop();
	}

	bool c_network_server::start()
	{
		if (m_running)
		{
			return true; // Already running
		}

		// Start listening for connections
		if (m_listener.listen(m_port) != sf::Socket::Done)
		{
			errorln("Failed to listen on port {}", m_port);
			return false;
		}

		// Add listener to selector
		m_selector.add(m_listener);

		// Start server thread
		m_running = true;
		m_server_thread = std::thread(&c_network_server::server_thread_func, this);

		logln("Server started on port {}", m_port);

		return true;
	}

	void c_network_server::stop()
	{
		if (!m_running)
		{
			return; // Already stopped
		}

		// Stop server thread
		m_running = false;
		if (m_server_thread.joinable())
		{
			m_server_thread.join();
		}

		// Clear clients
		{
			std::lock_guard<std::mutex> lock(m_clients_mutex);
			m_clients.clear();
		}

		// Close listener
		m_listener.close();

		logln("Server stopped");
	}

	bool c_network_server::send_message(const s_network_message& message, uint32_t client_id)
	{
		// Lock clients map
		std::lock_guard<std::mutex> lock(m_clients_mutex);

		// If client_id is 0, broadcast to all clients
		if (client_id == 0)
		{
			bool success = true;
			for (auto& client : m_clients)
			{
				if (!send_message_to_client(message, client.second))
				{
					success = false;
				}
			}
			return success;
		}
		else
		{
			// Send to specific client
			auto it = m_clients.find(client_id);
			if (it != m_clients.end())
			{
				return send_message_to_client(message, it->second);
			}
		}

		return false;
	}

	bool c_network_server::send_message_to_client(const s_network_message& message, s_client_info& client)
	{
		// Create packet
		sf::Packet packet;
		packet << static_cast<uint8_t>(message.type) << message.sender_id << static_cast<uint32_t>(message.data.size());

		// Add data if present
		if (!message.data.empty())
		{
			packet.append(message.data.data(), message.data.size());
		}

		// Send packet
		if (client.socket->send(packet) != sf::Socket::Done)
		{
			errorln("Failed to send message to client {}", client.id);
			return false;
		}

		return true;
	}

	void c_network_server::set_message_callback(std::function<void(const s_network_message&, uint32_t)> callback)
	{
		m_message_callback = callback;
	}

	bool c_network_server::update()
	{
		// Nothing to do here, server thread handles everything
		return true;
	}

	bool c_network_server::is_connected() const
	{
		return m_running;
	}

	uint32_t c_network_server::get_client_count() const
	{
		std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(m_clients_mutex));
		return static_cast<uint32_t>(m_clients.size());
	}

	void c_network_server::server_thread_func()
	{
		while (m_running)
		{
			// Wait for activity on sockets
			if (m_selector.wait(sf::milliseconds(100)))
			{
				// Check for new connections
				if (m_selector.isReady(m_listener))
				{
					handle_new_connections();
				}

				// Check for client messages
				handle_client_messages();
			}
		}
	}

	void c_network_server::handle_new_connections()
	{
		// Accept new connection
		auto client = std::make_unique<s_client_info>();
		if (m_listener.accept(*client->socket) == sf::Socket::Done)
		{
			// Set non-blocking mode
			client->socket->setBlocking(false);

			// Assign client ID
			client->id = m_next_client_id++;

			// Add to clients map
			{
				std::lock_guard<std::mutex> lock(m_clients_mutex);
				m_clients[client->id] = std::move(*client);
				m_selector.add(*m_clients[client->id].socket);
			}

			// Convert IP address to string before formatting
			std::string ip_address = m_clients[client->id].socket->getRemoteAddress().toString();
			logln("Client {} connected from {}", client->id, ip_address);

			// Notify about new client
			if (m_message_callback)
			{
				s_network_message message;
				message.type = e_message_type::PLAYER_JOINED;
				message.sender_id = client->id;
				m_message_callback(message, client->id);
			}

			// Send connect message with client ID
			s_network_message connect_message;
			connect_message.type = e_message_type::CONNECT;
			connect_message.sender_id = 0; // Server
			connect_message.data.resize(sizeof(uint32_t));
			*reinterpret_cast<uint32_t*>(connect_message.data.data()) = client->id;
			send_message(connect_message, client->id);
		}
	}

	void c_network_server::handle_client_messages()
	{
		std::lock_guard<std::mutex> lock(m_clients_mutex);
		
		for (auto it = m_clients.begin(); it != m_clients.end(); )
		{
			if (m_selector.isReady(*it->second.socket))
			{
				// Receive packet
				sf::Packet packet;
				sf::Socket::Status status = it->second.socket->receive(packet);

				if (status == sf::Socket::Done)
				{
					// Process packet
					uint8_t type;
					uint32_t sender_id, data_size;

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

					// Call message callback
					if (m_message_callback)
					{
						m_message_callback(message, it->first);
					}

					++it;
				}
				else if (status == sf::Socket::Disconnected)
				{
					// Client disconnected
					logln("Client {} disconnected", it->first);
					
					// Notify about disconnection
					if (m_message_callback)
					{
						s_network_message message;
						message.type = e_message_type::PLAYER_LEFT;
						message.sender_id = it->first;
						m_message_callback(message, it->first);
					}
					
					// Remove from selector and clients map
					m_selector.remove(*it->second.socket);
					it = m_clients.erase(it);
				}
				else
				{
					++it;
				}
			}
			else
			{
				++it;
			}
		}
	}
} // namespace atom
