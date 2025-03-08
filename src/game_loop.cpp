#include "game_loop.h"

namespace atom
{
	c_game_loop::c_game_loop()
	{
	}

	c_game_loop::~c_game_loop()
	{
	}
	
	void c_game_loop::add_update_callback(std::function<bool()> callback)
	{
		m_update_callbacks.push_back(callback);
	}

	bool c_game_loop::go()
	{
		bool any_true = true;
		while (any_true)
		{
			any_true = false;
			
			// Execute all update callbacks
			for (auto& callback : m_update_callbacks)
			{
				any_true |= callback();
			}
			
			// Update all children
			for (auto& child : m_children)
			{
				any_true |= child->update();
			}
		}
		return true;
	}
} // namespace atom