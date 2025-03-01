#pragma once
#include "atom.h"
#include "drawable.h"
#include <SFML/Graphics.hpp>

namespace atom
{
	class c_window : public c_atom
	{
	public:
		c_window(
			const std::string& title = "Atomic Engine",
			unsigned int width = 800,
			unsigned int height = 600,
			unsigned int position_x = 100,
			unsigned int position_y = 100,
			unsigned int framerate_limit = 60
		)
		{
			m_window.create(sf::VideoMode(width, height), title);
			m_window.setPosition(sf::Vector2i(position_x, position_y));
			m_window.setFramerateLimit(framerate_limit);
		}

		~c_window()
		{
			m_window.close();
		}

		bool update() override;

		sf::RenderWindow& operator()()
		{
			return m_window;
		}

	private:
		sf::RenderWindow m_window;
	};
}