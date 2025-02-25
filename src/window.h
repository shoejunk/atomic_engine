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
		);
		~c_window();

		bool update() override;

		sf::RenderWindow& operator()()
		{
			return m_window;
		}

		void add_drawable(c_drawable& drawable)
		{
			m_drawables.push_back(&drawable);
		}

	private:
		sf::RenderWindow m_window;
		std::vector<c_drawable*> m_drawables;
	};
} // namespace atom
