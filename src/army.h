#pragma once

#include "atom.h"
#include <vector>

namespace atom
{
	class c_army : public c_atom
	{
	public:
		static constexpr uint32_t type() { return "army"_h; }

	public:
		c_army() : m_army_idx(static_cast<uint8_t>(-1)) { register_aspect<c_army>(this); }
		c_army(uint8_t army_idx) : m_army_idx(army_idx) {}
		~c_army();

		// Get aspect types
		std::vector<uint32_t> get_aspect_types() const override
		{
			return { c_army::type() };
		}

		void add_piece(c_atom* piece);
		void remove_piece(c_atom* piece);
		const std::vector<c_atom*>& get_pieces() const;
		void set_army_idx(uint8_t army_idx) { m_army_idx = army_idx; }
		uint8_t get_army_idx() const { return m_army_idx; }

	private:
		std::vector<c_atom*> m_pieces;
		uint8_t m_army_idx;
	};
}
