#include "screen_position_action.h"

namespace atom
{
	c_screen_position_action::c_screen_position_action(uint32_t action_type, s_vector2i32 position)
		: c_action(action_type)
		, m_position(position)
	{
		register_aspect<c_screen_position_action>(this);
	}

	s_vector2i32 c_screen_position_action::get_position() const
	{
		return m_position;
	}
}
