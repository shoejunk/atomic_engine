#include "bayou_client.h"

#include "bayou_game.h"
#include "bayou_visualizer.h"

#include "window.h"

namespace atom
{
	bool c_bayou_client::go()
	{
		c_bayou_visualizer visualizer;
		c_bayou_game game;
		return visualizer.go();
	}
}