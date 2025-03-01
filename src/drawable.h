#pragma once
#include "aspect.h"
#include <SFML/Graphics/Drawable.hpp>

namespace atom
{
	// Drawable aspect - anything that can be drawn
	class i_drawable : public i_aspect
	{
	public:
		static constexpr uint32_t type() { return "drawable"_h; }

		virtual uint32_t get_aspect_type() const override { return type(); }

		// Get the underlying drawable
		virtual sf::Drawable& get_drawable() = 0;
	};
}
