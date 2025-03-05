#pragma once

#include "utility.h"

#include <cstdint>

namespace atom
{
	class i_board_state
	{
	public:
		static constexpr uint32_t type() { return "board_state"_h; }

	public:
		virtual std::shared_ptr<c_atom> get_piece_at(uint8_t x, uint8_t y) = 0;
		virtual bool add_piece(std::shared_ptr<c_atom> piece) = 0;
		virtual bool remove_piece(std::shared_ptr<c_atom> piece) = 0;
		virtual bool move_piece(std::shared_ptr<c_atom> piece, uint8_t new_x, uint8_t new_y) = 0;
		virtual uint8_t get_board_width() const = 0;
		virtual uint8_t get_board_height() const = 0;
	};
}
