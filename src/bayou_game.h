#pragma once

#include "atom.h"
#include "battle_grid.h"

namespace atom
{
	class c_bayou_game : public c_atom
	{
	private:
		c_battle_grid m_battle_grid{ {8, 8}, 2 };
	};
}