#include "board_action_handler.h"
#include "screen_position_action.h"

namespace atom
{
	c_board_action_handler::c_board_action_handler()
	{
		register_aspect<i_action_handler>(this);
		
		m_handled_actions = {
			MOVE_LEFT, MOVE_RIGHT, MOVE_UP, MOVE_DOWN, SELECT
		};
	}

	bool c_board_action_handler::handle_action(c_action const& action)
	{
		auto action_hash = action.get_action_id();

		if (!can_handle(action_hash))
		{
			return false;
		}

		auto game_state = get_parent()->as<c_bayou_state>();
		if (action_hash == SELECT)
		{
			// Use the const version of as<> method
			auto* position_action = action.as<c_screen_position_action>();
			if (position_action != nullptr)
			{
				// Convert screen position to board position
				s_vector2u8 board_pos = m_visualizer->screen_to_board_position(position_action->get_position());
				m_selected_piece = m_game_state->get_piece_at(board_pos.x, board_pos.y);
				return true;
			}
		}

		auto selected_piece = m_selected_piece.lock();
		if (!selected_piece)
		{
			return false;
		}

		auto board_position = selected_piece->as<i_board_position>();
		if (!board_position)
		{
			return false;
		}

		uint8_t current_x = board_position->get_board_x();
		uint8_t current_y = board_position->get_board_y();
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
		
		// Try to move the selected piece
		if (m_game_state->move_piece(selected_piece, new_x, new_y))
		{
			// Update the visualization
			m_visualizer->update_visualization();
			return true;
		}
		
		return false;
	}
}
