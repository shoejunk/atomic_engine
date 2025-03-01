#pragma once
#include <cstdint>

namespace atom
{
	// Base class for all aspects
	class i_aspect
	{
	public:
		virtual ~i_aspect() = default;

		// Get the type hash of this aspect
		virtual uint32_t get_aspect_type() const = 0;
	};
}
