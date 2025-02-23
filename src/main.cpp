#include "game_loop.h"
#include "sprite.h"
#include "texture.h"
#include "utility.h"
#include "window.h"

#include <iostream>

using namespace atom;

int main()
{
	auto window = c_window{"Atomic Engine"};
	auto texture = c_texture{ "assets/texture.png" };
	auto sprite = c_sprite{texture};
	sprite.set_position(100, 100); // Set position
	sprite.set_scale(0.2f, 0.2f); // Set scale
	window.add_drawable(sprite);
	auto game = c_game_loop{};
	game.add_child(window);
	game.go();

	return 0;
}
