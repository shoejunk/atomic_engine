#pragma once
#include "atom.h"
#include "action_handler.h"
#include "utility.h"
#include <vector>
#include <algorithm>

namespace atom
{
	class c_menu_action_handler : public c_atom, public i_action_handler
	{
	public:
		c_menu_action_handler()
		{
			register_aspect(this);
			
			m_handled_actions = {
				"menu_up"_h, "menu_down"_h, "menu_select"_h, "menu_back"_h
			};
		}
		
		// Get aspect types
		std::vector<uint32_t> get_aspect_types() const override
		{
			return { i_action_handler::type() };
		}

		bool can_handle(uint32_t action_hash) const override
		{
			return std::find(m_handled_actions.begin(), m_handled_actions.end(), action_hash)
				!= m_handled_actions.end();
		}

		bool handle_action(uint32_t action_hash) override
		{
			if (!can_handle(action_hash))
			{
				return false;
			}

			if (action_hash == "menu_up"_h)
			{
				// Select previous menu item
				atom::debugln("Menu Up");
				return true;
			}
			else if (action_hash == "menu_down"_h)
			{
				// Select next menu item
				atom::debugln("Menu Down");
				return true;
			}
			else if (action_hash == "menu_select"_h)
			{
				// Activate selected item
				atom::debugln("Menu Select");
				return true;
			}
			else if (action_hash == "menu_back"_h)
			{
				// Go back to previous menu
				atom::debugln("Menu Back");
				return true;
			}

			return false;
		}

	private:
		std::vector<uint32_t> m_handled_actions;
	};
} // namespace atom
