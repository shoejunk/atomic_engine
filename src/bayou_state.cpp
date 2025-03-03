#include "bayou_state.h"

namespace atom
{
	c_bayou_state::c_bayou_state()
	{
		// Set up armies
		m_armies[0].set_army_idx(0);
		m_armies[1].set_army_idx(1);

		// Clear the board
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			for (int y = 0; y < BOARD_HEIGHT; y++)
			{
				m_board[x][y].reset();
			}
		}

		// Register aspects
		register_aspect<c_bayou_state>(this);
	}

	bool c_bayou_state::add_piece(std::shared_ptr<c_atom> piece)
	{
		auto board_position = piece->as<i_board_position>();
		if (!board_position)
		{
			return false;
		}

		uint8_t x = board_position->get_board_x();
		uint8_t y = board_position->get_board_y();

		// Validate position
		if (x >= BOARD_WIDTH || y >= BOARD_HEIGHT)
		{
			return false;
		}

		// Check if position is already occupied
		if (m_board[x][y])
		{
			return false;
		}

		// Add piece to the board
		m_board[x][y] = piece;

		return true;
	}

	bool c_bayou_state::remove_piece(std::shared_ptr<c_atom> piece)
	{
		auto board_position = piece->as<i_board_position>();
		if (!board_position)
		{
			return false;
		}

		uint8_t x = board_position->get_board_x();
		uint8_t y = board_position->get_board_y();

		// Validate position
		if (x >= BOARD_WIDTH || y >= BOARD_HEIGHT)
		{
			return false;
		}

		// Check if the piece is on the board at this position
		if (m_board[x][y] != piece)
		{
			return false;
		}

		// Remove piece from the board
		m_board[x][y].reset();

		return true;
	}

	bool c_bayou_state::move_piece(std::shared_ptr<c_atom> piece, uint8_t new_x, uint8_t new_y)
	{
		auto board_position = piece->as<i_board_position>();
		if (!board_position)
		{
			return false;
		}

		uint8_t old_x = board_position->get_board_x();
		uint8_t old_y = board_position->get_board_y();

		// Validate old position
		if (old_x >= BOARD_WIDTH || old_y >= BOARD_HEIGHT)
		{
			return false;
		}

		// Validate new position
		if (new_x >= BOARD_WIDTH || new_y >= BOARD_HEIGHT)
		{
			return false;
		}

		// Check if the piece is on the board at the old position
		if (m_board[old_x][old_y] != piece)
		{
			return false;
		}

		// Check if new position is free
		if (m_board[new_x][new_y])
		{
			return false;
		}

		// Move the piece
		m_board[old_x][old_y].reset();
		m_board[new_x][new_y] = piece;
		board_position->set_board_position(new_x, new_y);

		return true;
	}
}
