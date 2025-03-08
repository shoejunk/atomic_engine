#pragma once

#include "network_interface.h"
#include "atom.h"
#include <SFML/Network.hpp>
#include <thread>
#include <atomic>
#include <mutex>

namespace atom
{
	class c_network_client : public c_atom, public i_network_interface
	{
	public:
		c_network_client();
		~c_network_client() override;

		// i_network_interface implementation
		bool send_message(const s_network_message& message, uint32_t client_id = 0) override;
		void set_message_callback(std::function<void(const s_network_message&, uint32_t)> callback) override;
		bool update() override;
		bool is_connected() const override;

		// Client-specific methods
		bool connect(const std::string& server_ip, uint16_t port = 54321);
		void disconnect();
		uint32_t get_client_id() const { return m_client_id; }

	private:
		// Client thread function
		void client_thread_func();

		// Client socket
		sf::TcpSocket m_socket;
		
		// Client thread
		std::thread m_client_thread;
		std::atomic<bool> m_running;
		
		// Client ID assigned by server
		uint32_t m_client_id;
		
		// Message callback
		std::function<void(const s_network_message&, uint32_t)> m_message_callback;
		
		// Connection state
		std::atomic<bool> m_connected;
		
		// Mutex for thread safety
		std::mutex m_mutex;
	};
} // namespace atom
