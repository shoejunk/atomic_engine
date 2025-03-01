#pragma once
#include "utility.h"
#include <cstdint>

namespace atom
{
	// Board position aspect - represents position on a game board
	class i_board_position
	{
	public:
		static constexpr uint32_t type() { return "board_position"_h; }

		// Get current board coordinates
		virtual uint8_t get_board_x() const = 0;
		virtual uint8_t get_board_y() const = 0;

		// Set board coordinates
		virtual void set_board_position(uint8_t x, uint8_t y) = 0;
	};
}