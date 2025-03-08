#pragma once

#include "atom.h"
#include <SFML/Network.hpp>
#include <string>
#include <functional>
#include <vector>

namespace atom
{
	// Network message types
	enum class e_message_type : uint8_t
	{
		CONNECT,
		DISCONNECT,
		GAME_STATE_UPDATE,
		MOVE_PIECE,
		CHAT_MESSAGE,
		PLAYER_JOINED,
		PLAYER_LEFT
	};

	// Network message structure
	struct s_network_message
	{
		e_message_type type;
		uint32_t sender_id;
		std::vector<uint8_t> data;
	};

	// Network interface for sending/receiving messages
	class i_network_interface
	{
	public:
		virtual ~i_network_interface() = default;
		
		// Send a message to a specific client (server->client) or to the server (client->server)
		virtual bool send_message(const s_network_message& message, uint32_t client_id = 0) = 0;
		
		// Set callback for when a message is received
		virtual void set_message_callback(std::function<void(const s_network_message&, uint32_t)> callback) = 0;
		
		// Update network state (process incoming messages, etc.)
		virtual bool update() = 0;
		
		// Check if connected
		virtual bool is_connected() const = 0;
	};
} // namespace atom
