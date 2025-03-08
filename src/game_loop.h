#pragma once

#include "atom.h"
#include <functional>
#include <vector>

namespace atom
{
	class c_game_loop : public c_atom
	{
	public:
		c_game_loop();
		~c_game_loop();

		bool go() override;
		
		// Add an update callback that will be called each frame
		// Returns true if the game should continue, false to exit
		void add_update_callback(std::function<bool()> callback);

	private:
		std::vector<std::function<bool()>> m_update_callbacks;
	};
} // namespace atom