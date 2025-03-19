#pragma once

#include "atom.h"

#include "utility.h"

#include <cstdint>

namespace atom
{
	class c_battle_grid : public c_atom
	{
	public:
		c_battle_grid(s_vector2u32 dimensions, uint32_t army_count)
		{
			m_grid.resize(dimensions.x);
			for (auto& row : m_grid)
			{
				row.resize(dimensions.y);
			}
			m_armies.resize(army_count);
		}

		s_vector2u32 get_dimensions() const;

	private:
		std::vector<std::vector<std::weak_ptr<c_atom>>> m_grid;
		std::vector<std::vector<std::shared_ptr<c_atom>>> m_armies;
	};
} // namespace atom
