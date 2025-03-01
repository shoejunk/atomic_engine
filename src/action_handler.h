#pragma once
#include "atom.h"
#include <vector>

namespace atom
{
	// Base class for action handlers
	class i_action_handler : public i_aspect
	{
	public:
		static constexpr uint32_t type() { return "action_handler"_h; }

		virtual uint32_t get_aspect_type() const override { return type(); }

		// Handle an action by its hash
		virtual bool handle_action(uint32_t action_hash) = 0;

		// Check if this handler can handle a specific action
		virtual bool can_handle(uint32_t action_hash) const = 0;
	};
}