#include "board_action_handler.h"
#include "board_state.h"
#include "game_state_visualizer.h"
#include "screen_position_action.h"
#include "utility.h"
#include "movable.h"
#include "game_piece.h"
#include "network_interface.h"

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
			auto piece = board_state->get_piece_at(board_pos.x, board_pos.y);
			
			if (piece)
			{
				// Get the piece's board position
				auto board_position = piece->as<i_board_position>();
				if (!board_position)
				{
					return false;
				}
				
				// Calculate the screen position for the center of the cell
				float center_x = visualizer->get_board_x() + board_pos.x * visualizer->get_cell_size() + visualizer->get_cell_size() / 2.0f;
				float center_y = visualizer->get_board_y() + board_pos.y * visualizer->get_cell_size() + visualizer->get_cell_size() / 2.0f;
				
				// Get the click position
				auto click_pos = position_action->get_position();
				
				// Calculate the offset from the cell center to the click position
				m_drag_offset = {
					static_cast<int32_t>(click_pos.x - center_x),
					static_cast<int32_t>(click_pos.y - center_y)
				};
				
				// Store the piece being dragged and its original position
				m_dragged_piece = piece;
				m_original_board_pos = board_pos;
				
				return true;
			}
			return false;
		}
		else if (action_hash == DRAG)
		{
			if (auto piece = m_dragged_piece.lock())
			{
				// Get the current mouse position
				auto mouse_position = position_action->get_position();
				
				// Calculate the new position by accounting for the drag offset
				float new_x = static_cast<float>(mouse_position.x - m_drag_offset.x);
				float new_y = static_cast<float>(mouse_position.y - m_drag_offset.y);
				
				// Update visualization with the dragged piece at the new position
				visualizer->update_visualization_with_dragged_piece(piece, new_x, new_y);
				
				return true;
			}
		}
		else if (action_hash == DROP)
		{
			if (auto piece = m_dragged_piece.lock())
			{
				// Get the current mouse position
				auto mouse_position = position_action->get_position();
				
				// Calculate the position with offset
				float piece_x = static_cast<float>(mouse_position.x - m_drag_offset.x);
				float piece_y = static_cast<float>(mouse_position.y - m_drag_offset.y);
				
				// Calculate the relative position within the board grid
				float rel_x = piece_x - visualizer->get_board_x();
				float rel_y = piece_y - visualizer->get_board_y();
				
				// Calculate the cell coordinates (floored to get the cell index)
				int32_t cell_x = static_cast<int32_t>(rel_x / visualizer->get_cell_size());
				int32_t cell_y = static_cast<int32_t>(rel_y / visualizer->get_cell_size());
				
				// Clamp to valid board coordinates
				cell_x = std::max(0, std::min(cell_x, static_cast<int32_t>(board_state->get_board_width() - 1)));
				cell_y = std::max(0, std::min(cell_y, static_cast<int32_t>(board_state->get_board_height() - 1)));
				
				// Try to move the piece to the new position
				if (board_state->move_piece(piece, static_cast<uint8_t>(cell_x), static_cast<uint8_t>(cell_y)))
				{
					// Move succeeded, send network message if we're in a networked game
					if (m_network_manager && m_network_manager->is_connected())
					{
						// Get the game piece
						auto game_piece = std::dynamic_pointer_cast<c_game_piece>(piece);
						if (game_piece)
						{
							// Create network message
							s_network_message message;
							message.type = e_message_type::MOVE_PIECE;
							message.sender_id = m_network_manager->get_client_id();
							
							// Pack piece ID and new position
							message.data.resize(sizeof(uint32_t) + sizeof(uint8_t) * 2);
							*reinterpret_cast<uint32_t*>(message.data.data()) = game_piece->get_id();
							*(message.data.data() + sizeof(uint32_t)) = static_cast<uint8_t>(cell_x);
							*(message.data.data() + sizeof(uint32_t) + sizeof(uint8_t)) = static_cast<uint8_t>(cell_y);
							
							// Send message
							m_network_manager->send_message(message);
						}
					}
				}
				else
				{
					// Move failed, put the piece back at its original position
					board_state->move_piece(piece, m_original_board_pos.x, m_original_board_pos.y);
				}
				
				// Update the visualization
				visualizer->update_visualization();
				
				// Reset the dragged piece
				m_dragged_piece.reset();
				return true;
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
				// Move succeeded, send network message if we're in a networked game
				if (m_network_manager && m_network_manager->is_connected())
				{
					// Get the game piece
					auto game_piece = std::dynamic_pointer_cast<c_game_piece>(selected_piece);
					if (game_piece)
					{
						// Create network message
						s_network_message message;
						message.type = e_message_type::MOVE_PIECE;
						message.sender_id = m_network_manager->get_client_id();
						
						// Pack piece ID and new position
						message.data.resize(sizeof(uint32_t) + sizeof(uint8_t) * 2);
						*reinterpret_cast<uint32_t*>(message.data.data()) = game_piece->get_id();
						*(message.data.data() + sizeof(uint32_t)) = new_x;
						*(message.data.data() + sizeof(uint32_t) + sizeof(uint8_t)) = new_y;
						
						// Send message
						m_network_manager->send_message(message);
					}
				}
				
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
				// Move succeeded, send network message if we're in a networked game
				if (m_network_manager && m_network_manager->is_connected())
				{
					// Get the game piece
					auto game_piece = std::dynamic_pointer_cast<c_game_piece>(selected_piece);
					if (game_piece)
					{
						// Create network message
						s_network_message message;
						message.type = e_message_type::MOVE_PIECE;
						message.sender_id = m_network_manager->get_client_id();
						
						// Pack piece ID and new position
						message.data.resize(sizeof(uint32_t) + sizeof(uint8_t) * 2);
						*reinterpret_cast<uint32_t*>(message.data.data()) = game_piece->get_id();
						*(message.data.data() + sizeof(uint32_t)) = new_x;
						*(message.data.data() + sizeof(uint32_t) + sizeof(uint8_t)) = new_y;
						
						// Send message
						m_network_manager->send_message(message);
					}
				}
				
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
				// Move succeeded, send network message if we're in a networked game
				if (m_network_manager && m_network_manager->is_connected())
				{
					// Get the game piece
					auto game_piece = std::dynamic_pointer_cast<c_game_piece>(selected_piece);
					if (game_piece)
					{
						// Create network message
						s_network_message message;
						message.type = e_message_type::MOVE_PIECE;
						message.sender_id = m_network_manager->get_client_id();
						
						// Pack piece ID and new position
						message.data.resize(sizeof(uint32_t) + sizeof(uint8_t) * 2);
						*reinterpret_cast<uint32_t*>(message.data.data()) = game_piece->get_id();
						*(message.data.data() + sizeof(uint32_t)) = new_x;
						*(message.data.data() + sizeof(uint32_t) + sizeof(uint8_t)) = new_y;
						
						// Send message
						m_network_manager->send_message(message);
					}
				}
				
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
				// Move succeeded, send network message if we're in a networked game
				if (m_network_manager && m_network_manager->is_connected())
				{
					// Get the game piece
					auto game_piece = std::dynamic_pointer_cast<c_game_piece>(selected_piece);
					if (game_piece)
					{
						// Create network message
						s_network_message message;
						message.type = e_message_type::MOVE_PIECE;
						message.sender_id = m_network_manager->get_client_id();
						
						// Pack piece ID and new position
						message.data.resize(sizeof(uint32_t) + sizeof(uint8_t) * 2);
						*reinterpret_cast<uint32_t*>(message.data.data()) = game_piece->get_id();
						*(message.data.data() + sizeof(uint32_t)) = new_x;
						*(message.data.data() + sizeof(uint32_t) + sizeof(uint8_t)) = new_y;
						
						// Send message
						m_network_manager->send_message(message);
					}
				}
				
				// Update the visualization
				visualizer->update_visualization();
				return true;
			}
			
			return false;
		}

		return false;
	}
}
