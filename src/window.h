#pragma once

#include "atom.h"
#include <SFML/Graphics.hpp>

namespace atom
{
	class c_window : public t_atom<c_window>
	{
	public:
		// Only need to define this constant
		static constexpr uint32_t kind_id() { return "window"_h; }

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

	private:
		sf::RenderWindow m_window;
	};
} // namespace atom