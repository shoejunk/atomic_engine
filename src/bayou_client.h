#pragma once

#include "atom.h"

namespace atom
{
	class c_bayou_client : public c_atom
	{
	public:
		bool go() override;
	};
}