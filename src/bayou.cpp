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
		auto window = c_window{ "Atomic Engine" };
		auto texture = c_texture{ "assets/texture.png" };
		auto sprite = c_sprite{ texture };
		sprite.set_position(100, 100); // Set position
		sprite.set_scale(0.2f, 0.2f); // Set scale
		window.add_drawable(sprite);
		auto game = c_game_loop{};
		game.add_child(window);
		return game.go();
	}
} // namespace atom