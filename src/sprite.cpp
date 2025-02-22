#include "sprite.h"

namespace atom
{
    c_sprite::c_sprite(const c_texture& texture, sf::RenderWindow& window)
        : m_window(window)
    {
        m_sprite.setTexture(texture());
    }

    bool c_sprite::update()
    {
        m_window.draw(m_sprite);
		return true;
    }
} // namespace atom
