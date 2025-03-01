#pragma once
#include "atom.h"
#include "movable.h"
#include "drawable.h"
#include "player_action_handler.h"
#include "sprite.h"
#include <memory>

namespace atom
{
	class c_player : public c_atom,
		public i_movable,
		public i_drawable
	{
	public:
		// Constructor
		c_player(c_texture& texture)
		{
			// Create a sprite (we don't add it as a child to avoid aspect conflicts)
			m_sprite = std::make_shared<c_sprite>(texture);
			m_sprite->set_position(100, 100);
			m_sprite->set_scale(0.2f, 0.2f);

			// Create action handler
			auto action_handler = std::make_unique<c_player_action_handler>();

			// Register aspects on this player
			register_aspect<i_movable>(this);
			register_aspect<i_drawable>(this);

			// Add action handler as child (will register its aspects)
			add_child(std::move(action_handler));
		}

		// Override get_aspect_types
		std::vector<uint32_t> get_aspect_types() const override
		{
			return { i_movable::type(), i_drawable::type() };
		}

		// Implementation of i_movable
		sf::Vector2f get_position() const override
		{
			return m_sprite->get_position();
		}

		void set_position(float x, float y) override
		{
			m_sprite->set_position(x, y);
		}

		void move(float dx, float dy) override
		{
			m_sprite->move(dx, dy);
		}

		// Implementation of i_drawable
		sf::Drawable& get_drawable() override
		{
			return m_sprite->get_drawable();
		}

	private:
		std::shared_ptr<c_sprite> m_sprite;
	};
}
