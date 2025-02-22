#include "game_loop.h"
#include "window.h"

#include <iostream>

using namespace atom;

int main()
{
	auto window = c_window{"Window 1"};
	auto window2 = c_window{"Window 2"};
	auto game = c_game_loop{};
	game.add_child(window);
	game.add_child(window2);
	game.go();
	std::cout << "Program is ending.\n";
	return 0;
}
