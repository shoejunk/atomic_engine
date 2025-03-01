#pragma once
#include "atom.h"
#include "drawable.h"
#include "movable.h"
#include "scalable.h"
#include "texture.h"
#include <SFML/Graphics.hpp>

namespace atom
{
	class c_sprite : public c_atom,
		public i_drawable,
		public i_movable,
		public i_scalable
	{
	public:
		c_sprite(const c_texture& texture);

		// Override get_aspect_types to return all implemented aspect types
		std::vector<uint32_t> get_aspect_types() const override
		{
			return {
				i_drawable::type(),
				i_movable::type(),
				i_scalable::type()
			};
		}

		// i_drawable implementation
		sf::Drawable& get_drawable() override { return m_sprite; }

		// i_movable implementation
		sf::Vector2f get_position() const override { return m_sprite.getPosition(); }
		void set_position(float x, float y) override { m_sprite.setPosition(x, y); }
		void move(float dx, float dy) override { m_sprite.move(dx, dy); }

		// i_scalable implementation
		sf::Vector2f get_scale() const override { return m_sprite.getScale(); }
		void set_scale(float x, float y) override { m_sprite.setScale(x, y); }

	private:
		sf::Sprite m_sprite;
	};
}