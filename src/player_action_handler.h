#pragma once
#include "atom.h"
#include "action_handler.h"
#include "movable.h"
#include "jumper.h"
#include <vector>
#include <algorithm>

namespace atom
{
	// Player action handler with action_handler aspect
	class c_player_action_handler : public c_atom,
		public i_action_handler
	{
	public:
		// Constructor - initialize with actions this handler can process
		c_player_action_handler()
		{
			m_handled_actions = {
				"move_left"_h, "move_right"_h, "move_up"_h, "move_down"_h, "jump"_h
			};

			// Register this as an action handler aspect
			register_aspect<i_action_handler>(this);
		}

		// Override get_aspect_types
		std::vector<uint32_t> get_aspect_types() const override
		{
			return std::vector<uint32_t>{ i_action_handler::type() };
		}

		// Check if we can handle this action (i_action_handler implementation)
		bool can_handle(uint32_t action_hash) const override
		{
			return std::find(m_handled_actions.begin(), m_handled_actions.end(), action_hash)
				!= m_handled_actions.end();
		}

		// Handle an action (i_action_handler implementation)
		bool handle_action(uint32_t action_hash) override
		{
			if (!can_handle(action_hash))
			{
				return false;
			}

			// Get parent
			auto parent = get_parent();
			if (!parent)
			{
				return false;
			}

			// Handle different actions using aspects
			if (action_hash == "move_left"_h)
			{
				if (auto movable = parent->as<i_movable>())
				{
					movable->move(-5.0f, 0.0f);
					return true;
				}
			}
			else if (action_hash == "move_right"_h)
			{
				if (auto movable = parent->as<i_movable>())
				{
					movable->move(5.0f, 0.0f);
					return true;
				}
			}
			else if (action_hash == "move_up"_h)
			{
				if (auto movable = parent->as<i_movable>())
				{
					movable->move(0.0f, -5.0f);
					return true;
				}
			}
			else if (action_hash == "move_down"_h)
			{
				if (auto movable = parent->as<i_movable>())
				{
					movable->move(0.0f, 5.0f);
					return true;
				}
			}
			else if (action_hash == "jump"_h)
			{
				// Try to use the jumpable aspect if available
				if (auto jumpable = parent->as<i_jumpable>())
				{
					jumpable->jump();
					return true;
				}
				
				// Fallback
				debugln("Jump!");
				return true;
			}

			return false;
		}

	private:
		std::vector<uint32_t> m_handled_actions;
	};
}