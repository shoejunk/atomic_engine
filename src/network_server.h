#pragma once

#include "network_interface.h"
#include "atom.h"
#include <SFML/Network.hpp>
#include <unordered_map>
#include <mutex>
#include <atomic>
#include <thread>
#include <memory>

namespace atom
{
	class c_network_server : public c_atom, public i_network_interface
	{
	public:
		c_network_server(uint16_t port = 54321);
		~c_network_server() override;

		// i_network_interface implementation
		bool send_message(const s_network_message& message, uint32_t client_id = 0) override;
		void set_message_callback(std::function<void(const s_network_message&, uint32_t)> callback) override;
		bool update() override;
		bool is_connected() const override;

		// Server-specific methods
		bool start();
		void stop();
		uint32_t get_client_count() const;

	private:
		// Client information structure
		struct s_client_info
		{
			std::unique_ptr<sf::TcpSocket> socket;
			uint32_t id;
			std::string name;
			
			s_client_info() : socket(std::make_unique<sf::TcpSocket>()), id(0) {}
			s_client_info(s_client_info&& other) noexcept
				: socket(std::move(other.socket)), id(other.id), name(std::move(other.name)) {}
			s_client_info& operator=(s_client_info&& other) noexcept {
				socket = std::move(other.socket);
				id = other.id;
				name = std::move(other.name);
				return *this;
			}
		};

		// Server thread function
		void server_thread_func();
		
		// Handle new client connections
		void handle_new_connections();
		
		// Handle client messages
		void handle_client_messages();
		
		// Send message to specific client
		bool send_message_to_client(const s_network_message& message, s_client_info& client);
		
		// Server socket and listener
		sf::TcpListener m_listener;
		std::unordered_map<uint32_t, s_client_info> m_clients;
		mutable std::mutex m_clients_mutex;
		
		// Server thread
		std::thread m_server_thread;
		std::atomic<bool> m_running;
		
		// Server port
		uint16_t m_port;
		
		// Message callback
		std::function<void(const s_network_message&, uint32_t)> m_message_callback;
		
		// Selector for handling multiple sockets
		sf::SocketSelector m_selector;
		
		// Next client ID
		uint32_t m_next_client_id;
	};
} // namespace atom
