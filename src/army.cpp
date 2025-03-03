#include "army.h"

namespace atom
{
	c_army::~c_army()
	{
	}

	void c_army::add_piece(c_atom* piece)
	{
		m_pieces.push_back(piece);
	}

	void c_army::remove_piece(c_atom* piece)
	{
		m_pieces.erase(std::remove(m_pieces.begin(), m_pieces.end(), piece), m_pieces.end());
	}

	const std::vector<c_atom*>& c_army::get_pieces() const
	{
		return m_pieces;
	}
}
