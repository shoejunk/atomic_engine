#include "bayou_texture_bank.h"

namespace atom
{
	c_bayou_texture_bank::c_bayou_texture_bank()
	{
		register_aspect<c_bayou_texture_bank>(this);
		m_textures["tinkeringTom_blue"_h] = std::make_shared<c_texture>("assets/tinkeringTom_blue.png");
		m_textures["terrain"_h] = std::make_shared<c_texture>("assets/terrain.png");
	}
} // namespace atom