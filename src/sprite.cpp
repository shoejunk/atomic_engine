#include "sprite.h"

namespace atom
{
    c_sprite::c_sprite(const c_texture& texture)
    {
        m_sprite.setTexture(texture());
        m_sprite.setPosition(0, 0); // Default position
        m_sprite.setScale(1, 1); // Default scale
    }
} // namespace atom
