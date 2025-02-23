#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include "drawable.h"

namespace atom
{
    class c_texture
    {
    public:
        c_texture(const std::string& file_path);

        sf::Texture& operator()()
        {
            return m_texture;
        }

        const sf::Texture& operator()() const
        {
            return m_texture;
        }

    private:
        sf::Texture m_texture;
    };
} // namespace atom