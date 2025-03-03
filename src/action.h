#pragma once

#include "atom.h"

namespace atom
{
	class c_action : public c_atom
	{
	public:
		static constexpr uint32_t type() { return "action"_h; }
		
	public:
		c_action(uint32_t action_id);

		uint32_t get_action_id() const;

	private:
		uint32_t m_action_id;
	};
}
