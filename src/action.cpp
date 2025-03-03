#include "action.h"

namespace atom
{
	c_action::c_action(uint32_t action_id)
		: m_action_id(action_id)
	{
		register_aspect<c_action>(this);
	}

	uint32_t c_action::get_action_id() const
	{
		return m_action_id;
	}
}
