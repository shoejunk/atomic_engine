#include "network_game_manager.h"
#include "game_piece.h"
#include "utility.h"
#include <iostream>

namespace atom
{
	c_network_game_manager::c_network_game_manager(std::shared_ptr<c_bayou_state> game_state)
		: m_game_state(game_state)
		, m_network_interface(nullptr)
		, m_is_server(false)
		, m_is_client(false)
		, m_has_received_initial_state(false)
		, m_pending_disconnect(false)
	{
	}

	c_network_game_manager::~c_network_game_manager()
	{
		stop();
	}

	bool c_network_game_manager::start_server(uint16_t port)
	{
		// Already running
		if (m_is_server || m_is_client)
		{
			return false;
		}

		// Create server
		m_server = std::make_unique<c_network_server>(port);
		
		// Set message callback
		m_server->set_message_callback([this](const s_network_message& message, uint32_t sender_id) {
			handle_message(message, sender_id);
		});
		
		// Start server
		if (!m_server->start())
		{
			m_server.reset();
			return false;
		}
		
		// Set network interface
		m_network_interface = m_server.get();
		m_is_server = true;
		
		logln("Started server on port {}", port);
		return true;
	}

	bool c_network_game_manager::start_client(const std::string& server_ip, uint16_t port)
	{
		// Already running
		if (m_is_server || m_is_client)
		{
			return false;
		}

		// Create client
		m_client = std::make_unique<c_network_client>();
		
		// Set message callback
		m_client->set_message_callback([this](const s_network_message& message, uint32_t sender_id) {
			handle_message(message, sender_id);
		});
		
		// Connect to server
		if (!m_client->connect(server_ip, port))
		{
			m_client.reset();
			return false;
		}
		
		// Set network interface
		m_network_interface = m_client.get();
		m_is_client = true;
		
		// Send connect message
		s_network_message connect_message;
		connect_message.type = e_message_type::CONNECT;
		connect_message.sender_id = 0; // Will be set by server
		m_network_interface->send_message(connect_message);
		
		logln("Connected to server at {}:{}", server_ip, port);
		return true;
	}

	void c_network_game_manager::stop()
	{
		if (m_is_server && m_server)
		{
			logln("Stopping server");
			m_server->stop();
			m_server.reset();
			m_is_server = false;
			
			if (m_network_interface == m_server.get())
			{
				m_network_interface = nullptr;
			}
		}
		
		if (m_is_client && m_client)
		{
			logln("Stopping client");
			m_client->disconnect();
			m_client.reset();
			m_is_client = false;
			
			if (m_network_interface == m_client.get())
			{
				m_network_interface = nullptr;
			}
		}
	}
	
	void c_network_game_manager::stop_client()
	{
		if (m_is_client && m_client)
		{
			logln("Stopping client connection");
			m_client->disconnect();
			m_client.reset();
			m_is_client = false;
			
			if (m_network_interface == m_client.get())
			{
				m_network_interface = nullptr;
			}
		}
	}

	bool c_network_game_manager::update()
	{
		// Not running
		if (!m_network_interface)
		{
			return false;
		}
		
		// Update network interface
		m_network_interface->update();
		
		// Check for pending disconnect
		if (m_pending_disconnect)
		{
			stop_client();
			m_pending_disconnect = false;
		}
		
		// Synchronize game state (server only)
		if (m_is_server && m_last_sync_time.getElapsedTime().asMilliseconds() >= 100)
		{
			// Send game state to all clients
			s_network_message state_message;
			state_message.type = e_message_type::GAME_STATE_UPDATE;
			state_message.sender_id = 0; // Server
			state_message.data = serialize_game_state();
			m_network_interface->send_message(state_message);
			
			m_last_sync_time.restart();
		}
		
		return false; // Return false to indicate no further updates needed
	}

	bool c_network_game_manager::is_connected() const
	{
		return m_network_interface && m_network_interface->is_connected();
	}

	uint32_t c_network_game_manager::get_client_id() const
	{
		return m_is_client ? m_client->get_client_id() : 0;
	}

	uint32_t c_network_game_manager::get_client_count() const
	{
		return m_is_server ? m_server->get_client_count() : 0;
	}

	void c_network_game_manager::handle_message(const s_network_message& message, uint32_t sender_id)
	{
		switch (message.type)
		{
			case e_message_type::CONNECT:
				if (m_is_server)
				{
					// Send current game state to new client
					s_network_message state_message;
					state_message.type = e_message_type::GAME_STATE_UPDATE;
					state_message.sender_id = 0; // Server
					state_message.data = serialize_game_state();
					m_network_interface->send_message(state_message, sender_id);
					
					// Assign client ID
					s_network_message id_message;
					id_message.type = e_message_type::CONNECT;
					id_message.sender_id = 0; // Server
					
					// Pack client ID into data
					id_message.data.resize(4);
					*reinterpret_cast<uint32_t*>(id_message.data.data()) = sender_id;
					
					m_network_interface->send_message(id_message, sender_id);
					
					logln("Client {} connected", sender_id);
				}
				break;
				
			case e_message_type::DISCONNECT:
				if (m_is_client)
				{
					logln("Disconnected from server");
					m_pending_disconnect = true;
				}
				break;
				
			case e_message_type::GAME_STATE_UPDATE:
				if (m_is_client)
				{
					// Update game state from server
					deserialize_game_state(message.data);
				}
				break;
				
			case e_message_type::MOVE_PIECE:
				if (m_is_server)
				{
					// Process move piece action from client
					deserialize_move_piece(message.data);
					
					// Broadcast to all clients
					m_network_interface->send_message(message);
				}
				else if (m_is_client)
				{
					// Process move piece action from server
					deserialize_move_piece(message.data);
				}
				break;
				
			case e_message_type::PLAYER_JOINED:
				logln("Player {} joined the game", sender_id);
				break;
				
			case e_message_type::PLAYER_LEFT:
				logln("Player {} left the game", sender_id);
				break;
				
			default:
				break;
		}
	}

	void c_network_game_manager::send_initial_game_state()
	{
		if (!m_is_server || !m_network_interface || !m_game_state)
		{
			return;
		}
		
		// Send game state to all clients
		s_network_message state_message;
		state_message.type = e_message_type::GAME_STATE_UPDATE;
		state_message.sender_id = 0; // Server
		state_message.data = serialize_game_state();
		m_network_interface->send_message(state_message);
		
		logln("Sent initial game state to clients");
	}
	
	bool c_network_game_manager::send_message(const s_network_message& message, uint32_t client_id)
	{
		if (!m_network_interface)
		{
			return false;
		}
		
		return m_network_interface->send_message(message, client_id);
	}

	std::vector<uint8_t> c_network_game_manager::serialize_game_state()
	{
		std::vector<uint8_t> data;
		
		// Get all pieces from game state
		auto pieces = m_game_state->get_pieces();
		
		// Write piece count
		uint32_t piece_count = static_cast<uint32_t>(pieces.size());
		data.resize(sizeof(uint32_t));
		*reinterpret_cast<uint32_t*>(data.data()) = piece_count;
		
		// Write each piece data
		for (const auto& piece : pieces)
		{
			// Get piece data
			uint32_t piece_id = piece->get_id();
			uint32_t piece_type = piece->get_type();
			uint32_t army_id = piece->get_army_id();
			uint8_t x = 0, y = 0;
			
			// Get board position
			if (auto board_pos = piece->as<i_board_position>())
			{
				x = board_pos->get_board_x();
				y = board_pos->get_board_y();
			}
			
			// Append to data
			size_t offset = data.size();
			data.resize(offset + sizeof(uint32_t) * 3 + sizeof(uint8_t) * 2);
			
			*reinterpret_cast<uint32_t*>(data.data() + offset) = piece_id;
			*reinterpret_cast<uint32_t*>(data.data() + offset + sizeof(uint32_t)) = piece_type;
			*reinterpret_cast<uint32_t*>(data.data() + offset + sizeof(uint32_t) * 2) = army_id;
			*(data.data() + offset + sizeof(uint32_t) * 3) = x;
			*(data.data() + offset + sizeof(uint32_t) * 3 + sizeof(uint8_t)) = y;
		}
		
		return data;
	}

	void c_network_game_manager::deserialize_game_state(const std::vector<uint8_t>& data)
	{
		if (data.size() < sizeof(uint32_t) || !m_game_state)
		{
			return;
		}
		
		// Read piece count
		uint32_t piece_count = *reinterpret_cast<const uint32_t*>(data.data());
		
		// Clear current pieces
		m_game_state->clear_pieces();
		
		// Read each piece data
		size_t offset = sizeof(uint32_t);
		for (uint32_t i = 0; i < piece_count; ++i)
		{
			if (offset + sizeof(uint32_t) * 3 + sizeof(uint8_t) * 2 > data.size())
			{
				break;
			}
			
			// Read piece data
			uint32_t piece_id = *reinterpret_cast<const uint32_t*>(data.data() + offset);
			uint32_t piece_type = *reinterpret_cast<const uint32_t*>(data.data() + offset + sizeof(uint32_t));
			uint32_t army_id = *reinterpret_cast<const uint32_t*>(data.data() + offset + sizeof(uint32_t) * 2);
			uint8_t x = *(data.data() + offset + sizeof(uint32_t) * 3);
			uint8_t y = *(data.data() + offset + sizeof(uint32_t) * 3 + sizeof(uint8_t));
			
			// Create piece
			auto piece = std::make_shared<c_game_piece>(
				*m_game_state->get_texture_for_piece_type(piece_type),
				piece_type,
				army_id,
				x,
				y
			);
			
			// Set piece ID
			piece->set_id(piece_id);
			
			// Add piece to game state
			m_game_state->add_piece(piece);
			
			offset += sizeof(uint32_t) * 3 + sizeof(uint8_t) * 2;
		}
		
		// Update visualization
		if (auto visualizer = m_game_state->as<c_bayou_game_visualizer>())
		{
			visualizer->update_visualization();
		}
		
		// Mark that we've received the initial state
		m_has_received_initial_state = true;
	}

	std::vector<uint8_t> c_network_game_manager::serialize_move_piece(uint32_t piece_id, uint8_t x, uint8_t y)
	{
		std::vector<uint8_t> data(sizeof(uint32_t) + sizeof(uint8_t) * 2);
		
		// Write piece ID
		*reinterpret_cast<uint32_t*>(data.data()) = piece_id;
		
		// Write new position
		*(data.data() + sizeof(uint32_t)) = x;
		*(data.data() + sizeof(uint32_t) + sizeof(uint8_t)) = y;
		
		return data;
	}

	void c_network_game_manager::deserialize_move_piece(const std::vector<uint8_t>& data)
	{
		if (data.size() < sizeof(uint32_t) + sizeof(uint8_t) * 2)
		{
			return;
		}
		
		// Read piece ID
		uint32_t piece_id = *reinterpret_cast<const uint32_t*>(data.data());
		
		// Read new position
		uint8_t x = *(data.data() + sizeof(uint32_t));
		uint8_t y = *(data.data() + sizeof(uint32_t) + sizeof(uint8_t));
		
		// Find piece and move it
		auto piece = m_game_state->get_piece_by_id(piece_id);
		if (piece)
		{
			if (auto board_pos = piece->as<i_board_position>())
			{
				board_pos->set_board_position(x, y);
			}
		}
		
		// Update visualization
		if (auto visualizer = m_game_state->as<c_bayou_game_visualizer>())
		{
			visualizer->update_visualization();
		}
	}
} // namespace atom
