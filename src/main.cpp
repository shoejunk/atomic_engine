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
	auto sprite = c_sprite{c_texture{"assets/texture.png"}, window()};
	auto game = c_game_loop{};
	game.add_child(window);
	game.go();

	return 0;
}
