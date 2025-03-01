#pragma once
#include "atom.h"
#include <SFML/System/Vector2.hpp>

namespace atom
{
	// Scale aspect - anything that can be scaled
	class i_scalable
	{
	public:
		static constexpr uint32_t type() { return "scalable"_h; }

		// Get scale
		virtual sf::Vector2f get_scale() const = 0;

		// Set scale
		virtual void set_scale(float x, float y) = 0;
	};
}
