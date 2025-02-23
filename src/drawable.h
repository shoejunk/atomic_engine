#pragma once

#include "atom.h"
#include "utility.h"
#include <SFML/Graphics/Drawable.hpp>

namespace atom
{
	class c_drawable
	{
	public:
		static constexpr uint32_t get_type() { return "drawable"_h; }

		virtual sf::Drawable& operator()() = 0;
	};
} // namespace atom
