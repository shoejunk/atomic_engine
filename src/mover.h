#pragma once
#include "atom.h"
#include "sprite.h"
#include "movable.h"

namespace atom
{
	// Movement component
	class c_mover : public c_atom, public i_movable
	{
	public:
		c_mover()
		{
			register_aspect<i_movable>(this);
		}

		// Get aspect types
		std::vector<uint32_t> get_aspect_types() const override
		{
			return { i_movable::type() };
		}

		// Move by a given delta
		void move(float dx, float dy) override
		{
			// Get the sprite from the parent
			if (auto parent = get_parent())
			{
				if (auto sprite = parent->as<c_sprite>())
				{
					sf::Vector2f pos = sprite->get_position();
					sprite->set_position(pos.x + dx, pos.y + dy);
				}
			}
		}
	};
}