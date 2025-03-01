#pragma once
#include <SFML/System/Vector2.hpp>

namespace atom
{
	// Movable aspect - anything that can be positioned
	class i_movable
	{
	public:
		static constexpr uint32_t type() { return "movable"_h; }

		// Get position
		virtual sf::Vector2f get_position() const = 0;

		// Set position
		virtual void set_position(float x, float y) = 0;

		// Relative movement
		virtual void move(float dx, float dy) = 0;
	};
}
