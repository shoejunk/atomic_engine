#pragma once
#include "utility.h"
#include <SFML/Graphics/Drawable.hpp>

namespace atom
{
	// Drawable aspect - anything that can be drawn
	class i_drawable
	{
	public:
		virtual ~i_drawable() = default;

		static constexpr uint32_t type() { return "drawable"_h; }

		// Get the underlying drawable
		virtual sf::Drawable& get_drawable() = 0;
	};
}
