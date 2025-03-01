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
#include "menu.h"

namespace atom
{
	bool c_bayou::go()
	{
		// Create main game objects
		auto window = std::make_unique<c_window>("Atomic Engine");
		auto texture = std::make_unique<c_texture>("assets/texture.png");

		// Set up input system
		auto input_manager = std::make_shared<c_input_manager>();
		window->add_connection(input_manager); // Connect by aspect type

		// Create a registrar for binding
		auto registrar = std::make_unique<c_input_binding_registrar>();
		auto registrar_ptr = registrar.get();

		// Add the registrar as a child to the input manager
		// This works because they don't share any aspects
		input_manager->add_child(std::move(registrar));

		// Create player and menu as shared_ptrs
		auto player = std::make_shared<c_player>(*texture);
		auto menu = std::make_shared<c_menu>();

		// Connect player to window for rendering (window will use player's drawable aspect)
		window->add_connection(player);

		// Register player and menu with input manager
		input_manager->register_atom(player);
		input_manager->register_atom(menu);

		// Define action and context hash constants
		constexpr uint32_t GAMEPLAY_CONTEXT = "gameplay"_h;
		constexpr uint32_t MENU_CONTEXT = "menu"_h;
		constexpr uint32_t MOVE_LEFT = "move_left"_h;
		constexpr uint32_t MOVE_RIGHT = "move_right"_h;
		constexpr uint32_t MOVE_UP = "move_up"_h;
		constexpr uint32_t MOVE_DOWN = "move_down"_h;
		constexpr uint32_t JUMP = "jump"_h;
		constexpr uint32_t MENU_UP = "menu_up"_h;
		constexpr uint32_t MENU_DOWN = "menu_down"_h;
		constexpr uint32_t MENU_SELECT = "menu_select"_h;
		constexpr uint32_t MENU_BACK = "menu_back"_h;

		// Register bindings for gameplay context
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, MOVE_LEFT, sf::Keyboard::A, e_activation_type::held);
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, MOVE_RIGHT, sf::Keyboard::D, e_activation_type::held);
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, MOVE_UP, sf::Keyboard::W, e_activation_type::held);
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, MOVE_DOWN, sf::Keyboard::S, e_activation_type::held);
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, JUMP, sf::Keyboard::Space, e_activation_type::pressed);

		// Arrow key alternatives
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, MOVE_LEFT, sf::Keyboard::Left, e_activation_type::held);
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, MOVE_RIGHT, sf::Keyboard::Right, e_activation_type::held);
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, MOVE_UP, sf::Keyboard::Up, e_activation_type::held);
		registrar_ptr->register_binding(GAMEPLAY_CONTEXT, MOVE_DOWN, sf::Keyboard::Down, e_activation_type::held);

		// Register bindings for menu context
		registrar_ptr->register_binding(MENU_CONTEXT, MENU_UP, sf::Keyboard::Up, e_activation_type::pressed);
		registrar_ptr->register_binding(MENU_CONTEXT, MENU_DOWN, sf::Keyboard::Down, e_activation_type::pressed);
		registrar_ptr->register_binding(MENU_CONTEXT, MENU_SELECT, sf::Keyboard::Return, e_activation_type::pressed);
		registrar_ptr->register_binding(MENU_CONTEXT, MENU_BACK, sf::Keyboard::Escape, e_activation_type::pressed);

		// Start with gameplay context active
		input_manager->push_context(GAMEPLAY_CONTEXT);

		// Create the game loop and start
		auto game_loop = std::make_unique<c_game_loop>();
		game_loop->add_child(std::move(window));
		return game_loop->go() ? 0 : 1;
	}
} // namespace atom