#pragma once

#include "atom.h"
#include <vector>

namespace atom
{

	class i_army
	{
	public:
		static constexpr uint32_t type() { return "army"_h; }
		virtual void add_piece(c_atom* piece) = 0;
		virtual void remove_piece(c_atom* piece) = 0;
		virtual const std::vector<c_atom*>& get_pieces() const = 0;
		virtual void set_army_idx(uint8_t army_idx) = 0;
		virtual uint8_t get_army_idx() const = 0;
	};

class c_army : public c_atom, public i_army
{
public:
	c_army() : m_army_idx(static_cast<uint8_t>(-1)) {}
	c_army(uint8_t army_idx) : m_army_idx(army_idx) {}
    ~c_army();

	// i_army
    void add_piece(c_atom* piece) override;
    void remove_piece(c_atom* piece) override;
    const std::vector<c_atom*>& get_pieces() const override;
	void set_army_idx(uint8_t army_idx) override { m_army_idx = army_idx; }
	uint8_t get_army_idx() const override { return m_army_idx; }

private:
    std::vector<c_atom*> m_pieces;
	uint8_t m_army_idx;
};

}
