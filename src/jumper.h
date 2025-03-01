#pragma once
#include "atom.h"
#include "utility.h"
#include "aspect.h"

namespace atom
{
	// Jumpable aspect - anything that can jump
	class i_jumpable : public i_aspect
	{
	public:
		static constexpr uint32_t type() { return "jumpable"_h; }

		virtual uint32_t get_aspect_type() const override { return type(); }

		// Jump with a given force
		virtual void jump() = 0;
	};

	// Jump component
	class c_jumper : public c_atom, public i_jumpable
	{
	public:
		c_jumper()
		{
			register_aspect(this);
		}
		
		// Get aspect types
		std::vector<uint32_t> get_aspect_types() const override
		{
			return { i_jumpable::type() };
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