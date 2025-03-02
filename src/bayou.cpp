#include "bayou.h"
#include "game_loop.h"
#include "input_binding_registrar.h"
#include "input_manager.h"
#include "menu_action_handler.h"
#include "player.h"
#include "texture.h"
#include "sprite.h"
#include "utility.h"
#include "window.h"
#include "game_state.h"
#include "game_state_visualizer.h"
#include "game_piece.h"

namespace atom
{
	bool c_bayou::go()
	{
		// Create main game objects
		auto window = std::make_unique<c_window>("Bayou Game", 1280, 1024);
		
		// Load textures
		auto piece_texture = std::make_unique<c_texture>("assets/texture.png");
		auto terrain_texture = std::make_unique<c_texture>("assets/terrain.png");
		
		// Set up input system
		auto input_manager = std::make_shared<c_input_manager>();
		window->add_connection<i_input_manager>(input_manager);

		// Create a registrar for binding
		auto registrar = std::make_unique<c_input_binding_registrar>();
		auto registrar_ptr = registrar.get();

		// Add the registrar as a child to the input manager
		input_manager->add_child(std::move(registrar));

		// Create game state
		auto game_state = std::make_shared<c_bayou_game_state>();
		
		// Create game visualizer
		auto game_visualizer = std::make_shared<c_bayou_game_visualizer>(game_state);
		game_visualizer->set_cell_size(64.0f);
		game_visualizer->set_board_position(224.0f, 160.0f);

		// Connect visualizer to window for rendering
		window->add_connection<i_drawable>(game_visualizer);

		// Define action and context hash constants
		constexpr uint32_t GAMEPLAY_CONTEXT = "gameplay"_h;
		constexpr uint32_t MOVE_LEFT = "move_left"_h;
		constexpr uint32_t MOVE_RIGHT = "move_right"_h;
		constexpr uint32_t MOVE_UP = "move_up"_h;
		constexpr uint32_t MOVE_DOWN = "move_down"_h;
		constexpr uint32_t SELECT = "select"_h;

		// Register bindings for gameplay context
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, MOVE_LEFT, sf::Keyboard::A, e_activation_type::pressed);
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, MOVE_RIGHT, sf::Keyboard::D, e_activation_type::pressed);
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, MOVE_UP, sf::Keyboard::W, e_activation_type::pressed);
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, MOVE_DOWN, sf::Keyboard::S, e_activation_type::pressed);
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, SELECT, sf::Keyboard::Space, e_activation_type::pressed);

		// Arrow key alternatives
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, MOVE_LEFT, sf::Keyboard::Left, e_activation_type::pressed);
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, MOVE_RIGHT, sf::Keyboard::Right, e_activation_type::pressed);
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, MOVE_UP, sf::Keyboard::Up, e_activation_type::pressed);
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, MOVE_DOWN, sf::Keyboard::Down, e_activation_type::pressed);

		// Start with gameplay context active
		input_manager->push_context(GAMEPLAY_CONTEXT);

		// Create some game pieces
		std::vector<std::shared_ptr<c_game_piece>> game_pieces;
		
		// Add a "player" piece
		auto player_piece = std::make_shared<c_game_piece>(*piece_texture, "player", 3, 3);
		game_pieces.push_back(player_piece);
		game_state->add_piece(player_piece);
		
		// Add some obstacle pieces
		auto obstacle1 = std::make_shared<c_game_piece>(*piece_texture, "obstacle", 1, 1);
		game_pieces.push_back(obstacle1);
		game_state->add_piece(obstacle1);
		
		auto obstacle2 = std::make_shared<c_game_piece>(*piece_texture, "obstacle", 5, 5);
		game_pieces.push_back(obstacle2);
		game_state->add_piece(obstacle2);
		
		auto obstacle3 = std::make_shared<c_game_piece>(*piece_texture, "obstacle", 2, 6);
		game_pieces.push_back(obstacle3);
		game_state->add_piece(obstacle3);

		// Update the visualization
		game_visualizer->update_visualization();
		
		// Create player action handler for board movement
		class c_board_action_handler : public c_atom, public i_action_handler
		{
		public:
			c_board_action_handler(
				std::shared_ptr<c_bayou_game_state> game_state,
				std::shared_ptr<c_game_piece> player_piece,
				std::shared_ptr<c_bayou_game_visualizer> visualizer)
				: m_game_state(game_state)
				, m_player_piece(player_piece)
				, m_visualizer(visualizer)
			{
				register_aspect<i_action_handler>(this);
				
				m_handled_actions = {
					MOVE_LEFT, MOVE_RIGHT, MOVE_UP, MOVE_DOWN, SELECT
				};
			}
			
			std::vector<uint32_t> get_aspect_types() const override
			{
				return { i_action_handler::type() };
			}
			
			bool can_handle(uint32_t action_hash) const override
			{
				return std::find(m_handled_actions.begin(), m_handled_actions.end(), action_hash)
					!= m_handled_actions.end();
			}
			
			bool handle_action(uint32_t action_hash) override
			{
				if (!can_handle(action_hash))
				{
					return false;
				}
				
				uint8_t current_x = m_player_piece->get_board_x();
				uint8_t current_y = m_player_piece->get_board_y();
				uint8_t new_x = current_x;
				uint8_t new_y = current_y;
				
				if (action_hash == MOVE_LEFT)
				{
					new_x = current_x - 1;
				}
				else if (action_hash == MOVE_RIGHT)
				{
					new_x = current_x + 1;
				}
				else if (action_hash == MOVE_UP)
				{
					new_y = current_y - 1;
				}
				else if (action_hash == MOVE_DOWN)
				{
					new_y = current_y + 1;
				}
				else if (action_hash == SELECT)
				{
					// For now, just log the current position
					debugln("Player selected at position ({}, {})", current_x, current_y);
					return true;
				}
				
				// Try to move the player piece
				if (m_game_state->move_piece(m_player_piece, new_x, new_y))
				{
					// Update the visualization
					m_visualizer->update_visualization();
					return true;
				}
				
				return false;
			}
			
		private:
			std::shared_ptr<c_bayou_game_state> m_game_state;
			std::shared_ptr<c_game_piece> m_player_piece;
			std::shared_ptr<c_bayou_game_visualizer> m_visualizer;
			std::vector<uint32_t> m_handled_actions;
		};
		
		// Create and connect the action handler
		auto board_action_handler = std::make_shared<c_board_action_handler>(
			game_state, player_piece, game_visualizer);
		input_manager->add_connection<i_action_handler>(board_action_handler);

		// Create the game loop and start
		auto game_loop = std::make_unique<c_game_loop>();
		game_loop->add_child(std::move(window));
		return game_loop->go() ? 0 : 1;
	}
} // namespace atom