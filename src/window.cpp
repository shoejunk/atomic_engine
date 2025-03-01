#include "window.h"
#include "drawable.h"
#include "input_manager_aspect.h"

namespace atom
{
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

			// Forward events to input managers
			auto input_managers = get_connections<i_input_manager>();
			for (auto& atom : input_managers)
			{
				if (auto input_manager = atom->as<i_input_manager>())
				{
					input_manager->process_event(event);
				}
			}
		}

		c_atom::update();

		m_window.clear();

		// Draw all drawables
		auto drawable_connections = get_connections<i_drawable>();
		for (auto& atom : drawable_connections)
		{
			if (auto drawable = atom->as<i_drawable>())
			{
				m_window.draw(drawable->get_drawable());
			}
		}

		m_window.display();
		return true;
	}
}