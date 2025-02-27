#pragma once

#include "atom.h"

#include <SFML/Graphics/Drawable.hpp>

namespace atom
{
	class c_drawable : public t_atom<c_drawable>
	{
	public:
		// Only need to define this constant, everything else comes from the template
		static constexpr uint32_t kind_id() { return "drawable"_h; }

		// Get the underlying drawable object
		virtual sf::Drawable& operator()() = 0;
	};
} // namespace atom