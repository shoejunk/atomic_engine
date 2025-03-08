#pragma once

#include "atom.h"
#include "network_interface.h"
#include "network_server.h"
#include "network_client.h"
#include "bayou_state.h"
#include "game_state_visualizer.h"
#include <memory>
#include <string>
#include <SFML/System/Clock.hpp>

namespace atom
{
	// Network game manager that handles server/client mode and game state synchronization
	class c_network_game_manager : public c_atom
	{
	public:
		c_network_game_manager(std::shared_ptr<c_bayou_state> game_state);
		~c_network_game_manager() override;

		// Start server mode
		bool start_server(uint16_t port = 54321);
		
		// Start client mode and connect to server
		bool start_client(const std::string& server_ip, uint16_t port = 54321);
		
		// Stop networking (server or client)
		void stop();
		
		// Stop client specifically
		void stop_client();
		
		// Update network state
		bool update() override;
		
		// Check if running as server
		bool is_server() const { return m_is_server; }
		
		// Check if running as client
		bool is_client() const { return m_is_client; }
		
		// Check if connected
		bool is_connected() const;
		
		// Get client ID (if client)
		uint32_t get_client_id() const;
		
		// Get client count (if server)
		uint32_t get_client_count() const;
		
		// Set the game state reference
		void set_game_state(std::shared_ptr<c_bayou_state> game_state) { m_game_state = game_state; }
		
		// Send initial game state to clients (server only)
		void send_initial_game_state();
		
		// Check if client has received initial game state
		bool has_received_initial_state() const { return m_has_received_initial_state; }
		
		// Send a network message
		bool send_message(const s_network_message& message, uint32_t client_id = 0);

	private:
		// Handle network messages
		void handle_message(const s_network_message& message, uint32_t sender_id);
		
		// Serialize game state for network transmission
		std::vector<uint8_t> serialize_game_state();
		
		// Deserialize game state from network data
		void deserialize_game_state(const std::vector<uint8_t>& data);
		
		// Serialize move piece action
		std::vector<uint8_t> serialize_move_piece(uint32_t piece_id, uint8_t x, uint8_t y);
		
		// Deserialize move piece action
		void deserialize_move_piece(const std::vector<uint8_t>& data);

		// Network components
		std::unique_ptr<c_network_server> m_server;
		std::unique_ptr<c_network_client> m_client;
		i_network_interface* m_network_interface;
		
		// Game state reference
		std::shared_ptr<c_bayou_state> m_game_state;
		
		// Mode flags
		bool m_is_server;
		bool m_is_client;
		
		// Flag indicating if client has received initial game state
		bool m_has_received_initial_state;
		
		// Flag indicating a pending disconnect request
		bool m_pending_disconnect;
		
		// Last update time for state synchronization
		sf::Clock m_last_sync_time;
	};
} // namespace atom
