#pragma once

#include "atom.h"
#include "drawable.h"
#include "texture.h"
#include <SFML/Graphics.hpp>

namespace atom
{
    class c_sprite : public t_atom<c_sprite, c_drawable>
    {
    public:
        // Only need this constant, all the virtual methods are handled by t_atom
        static constexpr uint32_t kind_id() { return "sprite"_h; }
        
        c_sprite(const c_texture& texture);
        
        sf::Drawable& operator()() override { return m_sprite; }
        
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