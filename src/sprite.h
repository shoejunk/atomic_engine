#pragma once

#include "atom.h"
#include "texture.h"
#include <SFML/Graphics.hpp>

namespace atom
{
	class c_sprite : public c_atom, public c_drawable
    {
    public:
        c_sprite(const c_texture& texture);
		sf::Drawable& operator()() { return m_sprite; }

		void set_position(float x, float y)
		{
			m_sprite.setPosition(x, y);
		}

		void set_scale(float x, float y)
		{
			m_sprite.setScale(x, y);
		}

    private:
        sf::Sprite m_sprite;
    };
} // namespace atom
