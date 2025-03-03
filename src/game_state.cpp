#include "game_state.h"

namespace atom
{
	c_bayou_game_state::c_bayou_game_state()
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
		register_aspect<i_game_state>(this);
	}
}
