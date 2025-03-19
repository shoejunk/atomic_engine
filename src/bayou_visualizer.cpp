#include "bayou_visualizer.h"

#include "window.h"

namespace atom
{
	bool c_bayou_visualizer::go()
	{
		c_window window{ "Bayou Bonanza", 1280, 1024, 100, 50 };
		while (window.update());
		return true;
	}
} // namespace atom