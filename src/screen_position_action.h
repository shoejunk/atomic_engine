#pragma once

#include "utility.h"

#include "action.h"

#include <SFML/System/Vector2.hpp>

namespace atom
{
	class c_screen_position_action : public c_action
	{
	public:
		c_screen_position_action(uint32_t action_type, s_vector2i position);

		s_vector2i get_position() const;

	private:
		s_vector2i m_position;
	};
}
