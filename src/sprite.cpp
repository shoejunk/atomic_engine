#include "sprite.h"

namespace atom
{
	c_sprite::c_sprite(const c_texture& texture)
	{
		m_sprite.setTexture(texture());
		m_sprite.setPosition(0, 0);
		m_sprite.setScale(1, 1);

		// Register aspects
		register_aspect(static_cast<i_drawable*>(this));
		register_aspect(static_cast<i_movable*>(this));
		register_aspect(static_cast<i_scalable*>(this));
	}
} // namespace atom
