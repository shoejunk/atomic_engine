#pragma once

#include "atom.h"
#include "texture.h"
#include <SFML/Graphics.hpp>

namespace atom
{
    class c_sprite : public c_atom
    {
    public:
        c_sprite(const c_texture& texture, sf::RenderWindow& window);
        bool update() override;

    private:
        sf::Sprite m_sprite;
        sf::RenderWindow& m_window;
    };
} // namespace atom
