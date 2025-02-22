#pragma once

#include "atom.h"

namespace atom
{
	class c_game_loop : public c_atom
	{
	public:
		c_game_loop();
		~c_game_loop();

		bool go() override;
	};
} // namespace atom
