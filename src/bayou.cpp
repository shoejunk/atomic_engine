#include "bayou.h"
#include "board_action_handler.h"
#include "bayou_state.h"
#include "bayou_texture_bank.h"

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
		auto game_state = std::make_shared<c_bayou_state>();
		
		// Create game visualizer
		auto game_visualizer = std::make_unique<c_bayou_game_visualizer>(game_state);
		game_visualizer->set_cell_size(96);
		game_visualizer->set_board_position(224, 160);
		auto game_visualizer_ptr = game_visualizer.get();
		game_state->add_child(std::move(game_visualizer));

		// Connect visualizer to window for rendering
		window->add_connection<i_drawable>(game_state);

		// Register bindings for gameplay context
		static constexpr uint32_t GAMEPLAY_CONTEXT = "gameplay"_h;
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, c_board_action_handler::MOVE_LEFT, sf::Keyboard::A, e_activation_type::pressed);
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, c_board_action_handler::MOVE_RIGHT, sf::Keyboard::D, e_activation_type::pressed);
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, c_board_action_handler::MOVE_UP, sf::Keyboard::W, e_activation_type::pressed);
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, c_board_action_handler::MOVE_DOWN, sf::Keyboard::S, e_activation_type::pressed);
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, c_board_action_handler::SELECT, sf::Mouse::Left, e_activation_type::pressed);

		// Arrow key alternatives
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, c_board_action_handler::MOVE_LEFT, sf::Keyboard::Left, e_activation_type::pressed);
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, c_board_action_handler::MOVE_RIGHT, sf::Keyboard::Right, e_activation_type::pressed);
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, c_board_action_handler::MOVE_UP, sf::Keyboard::Up, e_activation_type::pressed);
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, c_board_action_handler::MOVE_DOWN, sf::Keyboard::Down, e_activation_type::pressed);

		// Start with gameplay context active
		input_manager->push_context(GAMEPLAY_CONTEXT);

		// Create some game pieces for army 0
		game_state->add_piece(std::make_shared<c_game_piece>(*texture_bank.get_texture("tinkeringTom_blue"_h), "tinkering_tom"_h, 0, 3, 3));
		game_state->add_piece(std::make_shared<c_game_piece>(*texture_bank.get_texture("tinkeringTom_blue"_h), "tinkering_tom"_h, 0, 2, 2));
		
		// Create some game pieces for army 1
		game_state->add_piece(std::make_shared<c_game_piece>(*texture_bank.get_texture("tinkeringTom_blue"_h), "tinkering_tom"_h, 4, 6));
		game_state->add_piece(std::make_shared<c_game_piece>(*texture_bank.get_texture("tinkeringTom_blue"_h), "tinkering_tom"_h, 6, 6));
		game_state->add_piece(std::make_shared<c_game_piece>(*texture_bank.get_texture("tinkeringTom_blue"_h), "tinkering_tom"_h, 3, 7));

		// Update the visualization
		game_visualizer_ptr->update_visualization();
		
		// Create and connect the action handler
		auto board_action_handler = std::make_unique<c_board_action_handler>();
		auto board_action_handler_ptr = board_action_handler.get();
		game_state->add_child(std::move(board_action_handler));

		input_manager->add_connection<i_action_handler>(game_state);

		// Create the game loop and start
		auto game_loop = std::make_unique<c_game_loop>();
		game_loop->add_child(std::move(window));
		return game_loop->go() ? 0 : 1;
	}
} // namespace atom