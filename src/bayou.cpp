#include "bayou.h"
#include "window.h"
#include "texture.h"
#include "sprite.h"
#include "utility.h"
#include "game_loop.h"

namespace atom
{
	bool c_bayou::go()
	{
		auto window = std::make_unique<c_window>("Atomic Engine");
		auto texture = std::make_unique<c_texture>("assets/texture.png");
		auto sprite = std::make_shared<c_sprite>(*texture);
		sprite->set_position(100, 100); // Set position
		sprite->set_scale(0.2f, 0.2f); // Set scale
		window->add_connection(sprite); // Pass shared_ptr
		auto game = std::make_unique<c_game_loop>();
		game->add_child(std::move(window));
		return game->go();
	}
} // namespace atom