#include "window.h"

namespace atom
{
	c_window::c_window(
		const std::string& title,
		unsigned int width,
		unsigned int height,
		unsigned int position_x,
		unsigned int position_y,
		unsigned int framerate_limit
	)
	{
		m_window.create(sf::VideoMode(width, height), title);
		m_window.setPosition(sf::Vector2i(position_x, position_y));
		m_window.setFramerateLimit(framerate_limit);
	}

	c_window::~c_window()
	{
		m_window.close();
	}

	bool c_window::update()
	{
		if (!m_window.isOpen())
		{
			return false;
		}

		for (auto event = sf::Event{}; m_window.pollEvent(event);)
		{
			if (event.type == sf::Event::Closed)
			{
				m_window.close();
				return false;
			}
		}

		c_atom::update();

		m_window.clear();
		for (auto& drawable : m_drawables)
		{
			m_window.draw((*drawable)());
		}
		m_window.display();
		return true;
	}
} // namespace atom
