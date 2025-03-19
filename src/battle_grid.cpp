#include "battle_grid.h"

namespace atom
{
	s_vector2u32 c_battle_grid::get_dimensions() const
	{
		if (m_grid.empty())
		{
			return { 0, 0 };
		}

		return { static_cast<uint32_t>(m_grid.size()), static_cast<uint32_t>(m_grid[0].size()) };
	}
} // namespace atom
