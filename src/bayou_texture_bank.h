#pragma once

#include "atom.h"
#include "texture.h"

namespace atom
{
	class c_bayou_texture_bank : public c_atom
	{
	public:
		static constexpr uint32_t type() { return "bayou_texture_bank"_h; }

	public:
		c_bayou_texture_bank();

		// Get aspect types
		std::vector<uint32_t> get_aspect_types() const override
		{
			return { c_bayou_texture_bank::type() };
		}

		std::shared_ptr<c_texture> get_texture(uint32_t texture_hash)
		{
			auto it = m_textures.find(texture_hash);
			if (it == m_textures.end())
			{
				return nullptr;
			}
			return it->second;
		}

	private:
		std::unordered_map<uint32_t, std::shared_ptr<c_texture>> m_textures;
	};
} // namespace atom