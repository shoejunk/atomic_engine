#include "game_loop.h"

namespace atom
{
	c_game_loop::c_game_loop()
	{
	}

	c_game_loop::~c_game_loop()
	{
	}

	bool c_game_loop::go()
	{
		bool any_true = true;
		while (any_true)
		{
			any_true = false;
			for (auto& [family, child] : m_children)
			{
				any_true |= child->update();
			}
		}
		return true;
	}
} // namespace atom