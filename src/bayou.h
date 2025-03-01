#pragma once

#include "atom.h"

namespace atom
{
	class c_bayou : public c_atom
	{
	public:
		bool go() override;
	};
} // namespace atom