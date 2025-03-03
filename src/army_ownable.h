#pragma once

#include "utility.h"

#include <cstdint>

namespace atom
{
	class i_army_ownable
	{
	public:
		static constexpr uint32_t type() { return "army_ownable"_h; }
		virtual uint8_t get_army_idx() const = 0;
	};
} // namespace atom
