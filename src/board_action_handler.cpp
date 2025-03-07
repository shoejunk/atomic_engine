#include "board_action_handler.h"
#include "board_state.h"
#include "game_state_visualizer.h"
#include "screen_position_action.h"
#include "utility.h"
#include "movable.h"

namespace atom
{
	c_board_action_handler::c_board_action_handler()
	{
		register_aspect<i_action_handler>(this);
		
		m_handled_actions = {
			MOVE_LEFT, MOVE_RIGHT, MOVE_UP, MOVE_DOWN, SELECT,
			DRAG_START, DRAG, DROP
		};
	}

	bool c_board_action_handler::handle_action(c_action const& action)
	{
		auto action_hash = action.get_action_id();

		if (!can_handle(action_hash))
		{
			return false;
		}

		auto board_state = as<i_board_state>();
		if (!board_state)
		{
			return false;
		}

		auto visualizer = as<c_bayou_game_visualizer>();
		if (!visualizer)
		{
			return false;
		}

		auto* position_action = action.as<c_screen_position_action>();
		if (position_action == nullptr)
		{
			return false;
		}

		if (action_hash == SELECT)
		{
			// Use the const version of as<> method
			auto* position_action = action.as<c_screen_position_action>();
			if (position_action != nullptr)
			{
				// Convert screen position to board position
				s_vector2u8 board_pos = visualizer->screen_to_board_position(position_action->get_position());
				m_selected_piece = board_state->get_piece_at(board_pos.x, board_pos.y);
				return true;
			}
		}
		else if (action_hash == DRAG_START)
		{
			// Convert screen position to board position
			s_vector2u8 board_pos = visualizer->screen_to_board_position(position_action->get_position());
			m_dragged_piece = board_state->get_piece_at(board_pos.x, board_pos.y);
			
			if (auto piece = m_dragged_piece.lock())
			{
				if (auto board_position = piece->as<i_board_position>())
				{
					// Calculate screen position for the piece
					float screen_x = visualizer->get_board_x() + board_position->get_board_x() * visualizer->get_cell_size() + visualizer->get_cell_size() / 2.0f;
					float screen_y = visualizer->get_board_y() + board_position->get_board_y() * visualizer->get_cell_size() + visualizer->get_cell_size() / 2.0f;
					
					// Store the offset between piece position and click position
					auto click_pos = position_action->get_position();
					m_drag_offset = {
						static_cast<uint8_t>(click_pos.x - screen_x),
						static_cast<uint8_t>(click_pos.y - screen_y)
					};
					return true;
				}
			}
		}
		else if (action_hash == DRAG)
		{
			if (auto piece = m_dragged_piece.lock())
			{
				if (auto movable = piece->as<i_movable>())
				{
					// Update sprite position for visual feedback during drag
					auto pos = position_action->get_position();
					movable->set_position(
						static_cast<float>(pos.x - m_drag_offset.x),
						static_cast<float>(pos.y - m_drag_offset.y)
					);
					return true;
				}
			}
		}
		else if (action_hash == DROP)
		{
			if (auto piece = m_dragged_piece.lock())
			{
				if (auto board_position = piece->as<i_board_position>())
				{
					// Convert drop position to board coordinates
					s_vector2u8 new_pos = visualizer->screen_to_board_position(position_action->get_position());
					
					// Try to move the piece
					if (board_state->move_piece(piece, new_pos.x, new_pos.y))
					{
						visualizer->update_visualization();
					}
					else
					{
						// Move failed, reset piece position
						visualizer->update_visualization();
					}
					
					m_dragged_piece.reset();
					return true;
				}
			}
		}
		else if (action_hash == MOVE_LEFT)
		{
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
			uint8_t new_x = current_x - 1;
			uint8_t new_y = current_y;
			
			// Try to move the selected piece
			if (board_state->move_piece(selected_piece, new_x, new_y))
			{
				// Update the visualization
				visualizer->update_visualization();
				return true;
			}
			
			return false;
		}
		else if (action_hash == MOVE_RIGHT)
		{
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
			uint8_t new_x = current_x + 1;
			uint8_t new_y = current_y;
			
			// Try to move the selected piece
			if (board_state->move_piece(selected_piece, new_x, new_y))
			{
				// Update the visualization
				visualizer->update_visualization();
				return true;
			}
			
			return false;
		}
		else if (action_hash == MOVE_UP)
		{
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
			uint8_t new_y = current_y - 1;
			
			// Try to move the selected piece
			if (board_state->move_piece(selected_piece, new_x, new_y))
			{
				// Update the visualization
				visualizer->update_visualization();
				return true;
			}
			
			return false;
		}
		else if (action_hash == MOVE_DOWN)
		{
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
			uint8_t new_y = current_y + 1;
			
			// Try to move the selected piece
			if (board_state->move_piece(selected_piece, new_x, new_y))
			{
				// Update the visualization
				visualizer->update_visualization();
				return true;
			}
			
			return false;
		}

		return false;
	}
}
