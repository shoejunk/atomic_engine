#pragma once
#include "atom.h"
#include "utility.h"

namespace atom
{
	// Jumpable aspect - anything that can jump
	class i_jumpable
	{
	public:
		static constexpr uint32_t type() { return "jumpable"_h; }

		// Jump with a given force
		virtual void jump() = 0;
	};

	// Jump component
	class c_jumper : public c_atom, public i_jumpable
	{
	public:
		c_jumper()
		{
			register_aspect<i_jumpable>(this);
		}
		
		// Jump with a given force
		void jump() override
		{
			// Jump logic would go here
			// For simplicity, just log the jump
			debugln("Jump!");
		}
	};
}