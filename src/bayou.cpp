#include "bayou.h"
#include "board_action_handler.h"
#include "bayou_state.h"
#include "bayou_texture_bank.h"
#include "network_game_manager.h"

#include "game_loop.h"
#include "game_piece.h"
#include "game_state_visualizer.h"
#include "input_binding_registrar.h"
#include "input_manager.h"
#include "texture.h"
#include "sprite.h"
#include "utility.h"
#include "window.h"

namespace atom
{
	void c_bayou::set_network_manager(std::shared_ptr<c_network_game_manager> network_manager)
	{
		m_network_manager = network_manager;
	}
	
	std::shared_ptr<c_bayou_state> c_bayou::get_game_state() const
	{
		return m_game_state;
	}

	bool c_bayou::go()
	{
		// Create main game objects
		auto window = std::make_unique<c_window>("Bayou Game", 1280, 1024);
		
		// Create the texture bank
		c_bayou_texture_bank texture_bank;
		
		// Set up input system
		auto input_manager = std::make_shared<c_input_manager>();
		window->add_connection<i_input_manager>(input_manager);

		// Create a registrar for binding
		auto registrar = std::make_unique<c_input_binding_registrar>();
		auto registrar_ptr = registrar.get();

		// Add the registrar as a child to the input manager
		input_manager->add_child(std::move(registrar));

		// Create game state
		m_game_state = std::make_shared<c_bayou_state>();
		
		// Create game visualizer
		auto game_visualizer = std::make_unique<c_bayou_game_visualizer>(m_game_state);
		game_visualizer->set_cell_size(96);
		game_visualizer->set_board_position(224, 160);
		auto game_visualizer_ptr = game_visualizer.get();
		m_game_state->add_child(std::move(game_visualizer));

		// Connect visualizer to window for rendering
		window->add_connection<i_drawable>(m_game_state);

		// Register bindings for gameplay context
		static constexpr uint32_t GAMEPLAY_CONTEXT = "gameplay"_h;
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, c_board_action_handler::DRAG_START, sf::Mouse::Left, e_activation_type::pressed);
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, c_board_action_handler::DRAG, sf::Mouse::Left, e_activation_type::held);
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, c_board_action_handler::DROP, sf::Mouse::Left, e_activation_type::released);

		// Start with gameplay context active
		input_manager->push_context(GAMEPLAY_CONTEXT);

		// Create and connect the action handler
		auto board_action_handler = std::make_unique<c_board_action_handler>();
		auto board_action_handler_ptr = board_action_handler.get();
		m_game_state->add_child(std::move(board_action_handler));

		// Set up networking if available
		if (m_network_manager)
		{
			// Connect the network manager to the game state
			m_network_manager->set_game_state(m_game_state);
			
			// Set the network manager in the board action handler
			board_action_handler_ptr->set_network_manager(m_network_manager);
			
			// If we're a client, wait for initial game state from server
			if (m_network_manager->is_client())
			{
				// Wait for initial game state (timeout after 5 seconds)
				int timeout_ms = 5000;
				int wait_interval_ms = 100;
				int elapsed_ms = 0;
				
				while (!m_network_manager->has_received_initial_state() && elapsed_ms < timeout_ms)
				{
					// Update the network manager to process messages
					m_network_manager->update();
					
					// Sleep for a short interval
					sf::sleep(sf::milliseconds(wait_interval_ms));
					elapsed_ms += wait_interval_ms;
				}
				
				if (!m_network_manager->has_received_initial_state())
				{
					// Failed to receive initial state, fall back to standalone mode
					errorln("Failed to receive initial game state from server. Falling back to standalone mode.");
					m_network_manager->stop_client();
				}
			}
			else if (m_network_manager->is_server())
			{
				// Create some game pieces for army 0
				m_game_state->add_piece(std::make_shared<c_game_piece>(*texture_bank.get_texture("tinkeringTom_blue"_h), "tinkering_tom"_h, 0, 3, 3));
				m_game_state->add_piece(std::make_shared<c_game_piece>(*texture_bank.get_texture("tinkeringTom_blue"_h), "tinkering_tom"_h, 0, 2, 2));
				
				// Create some game pieces for army 1
				m_game_state->add_piece(std::make_shared<c_game_piece>(*texture_bank.get_texture("tinkeringTom_blue"_h), "tinkering_tom"_h, 1, 4, 6));
				m_game_state->add_piece(std::make_shared<c_game_piece>(*texture_bank.get_texture("tinkeringTom_blue"_h), "tinkering_tom"_h, 1, 6, 6));
				m_game_state->add_piece(std::make_shared<c_game_piece>(*texture_bank.get_texture("tinkeringTom_blue"_h), "tinkering_tom"_h, 1, 3, 7));
				
				// Note: Initial game state will be sent to each client when they connect
			}
			else
			{
				// Standalone mode - create default pieces
				// Create some game pieces for army 0
				m_game_state->add_piece(std::make_shared<c_game_piece>(*texture_bank.get_texture("tinkeringTom_blue"_h), "tinkering_tom"_h, 0, 3, 3));
				m_game_state->add_piece(std::make_shared<c_game_piece>(*texture_bank.get_texture("tinkeringTom_blue"_h), "tinkering_tom"_h, 0, 2, 2));
				
				// Create some game pieces for army 1
				m_game_state->add_piece(std::make_shared<c_game_piece>(*texture_bank.get_texture("tinkeringTom_blue"_h), "tinkering_tom"_h, 1, 4, 6));
				m_game_state->add_piece(std::make_shared<c_game_piece>(*texture_bank.get_texture("tinkeringTom_blue"_h), "tinkering_tom"_h, 1, 6, 6));
				m_game_state->add_piece(std::make_shared<c_game_piece>(*texture_bank.get_texture("tinkeringTom_blue"_h), "tinkering_tom"_h, 1, 3, 7));
			}
		}
		else
		{
			// Standalone mode - create default pieces
			// Create some game pieces for army 0
			m_game_state->add_piece(std::make_shared<c_game_piece>(*texture_bank.get_texture("tinkeringTom_blue"_h), "tinkering_tom"_h, 0, 3, 3));
			m_game_state->add_piece(std::make_shared<c_game_piece>(*texture_bank.get_texture("tinkeringTom_blue"_h), "tinkering_tom"_h, 0, 2, 2));
			
			// Create some game pieces for army 1
			m_game_state->add_piece(std::make_shared<c_game_piece>(*texture_bank.get_texture("tinkeringTom_blue"_h), "tinkering_tom"_h, 1, 4, 6));
			m_game_state->add_piece(std::make_shared<c_game_piece>(*texture_bank.get_texture("tinkeringTom_blue"_h), "tinkering_tom"_h, 1, 6, 6));
			m_game_state->add_piece(std::make_shared<c_game_piece>(*texture_bank.get_texture("tinkeringTom_blue"_h), "tinkering_tom"_h, 1, 3, 7));
		}

		// Update the visualization
		game_visualizer_ptr->update_visualization();

		input_manager->add_connection<i_action_handler>(m_game_state);

		// Create the game loop and start
		auto game_loop = std::make_unique<c_game_loop>();
		
		// Add network manager to the game loop if available
		if (m_network_manager)
		{
			game_loop->add_update_callback([this]() {
				if (m_network_manager)
				{
					m_network_manager->update();
				}
				return true;
			});
		}
		
		game_loop->add_child(std::move(window));
		return game_loop->go() ? 0 : 1;
	}
} // namespace atom