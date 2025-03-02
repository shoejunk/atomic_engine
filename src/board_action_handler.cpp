#include "board_action_handler.h"

namespace atom
{
	c_board_action_handler::c_board_action_handler(
		std::shared_ptr<c_bayou_game_state> game_state,
		std::shared_ptr<c_game_piece> player_piece,
		std::shared_ptr<c_bayou_game_visualizer> visualizer)
		: m_game_state(game_state)
		, m_player_piece(player_piece)
		, m_visualizer(visualizer)
	{
		register_aspect<i_action_handler>(this);
		
		m_handled_actions = {
			MOVE_LEFT, MOVE_RIGHT, MOVE_UP, MOVE_DOWN, SELECT
		};
	}

	bool c_board_action_handler::handle_action(uint32_t action_hash)
	{
		if (!can_handle(action_hash))
		{
			return false;
		}
		
		uint8_t current_x = m_player_piece->get_board_x();
		uint8_t current_y = m_player_piece->get_board_y();
		uint8_t new_x = current_x;
		uint8_t new_y = current_y;
		
		if (action_hash == MOVE_LEFT)
		{
			new_x = current_x - 1;
		}
		else if (action_hash == MOVE_RIGHT)
		{
			new_x = current_x + 1;
		}
		else if (action_hash == MOVE_UP)
		{
			new_y = current_y - 1;
		}
		else if (action_hash == MOVE_DOWN)
		{
			new_y = current_y + 1;
		}
		else if (action_hash == SELECT)
		{
			// For now, just log the current position
			debugln("Player selected at position ({}, {})", current_x, current_y);
			return true;
		}
		
		// Try to move the player piece
		if (m_game_state->move_piece(m_player_piece, new_x, new_y))
		{
			// Update the visualization
			m_visualizer->update_visualization();
			return true;
		}
		
		return false;
	}
}
