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
		register_aspect<i_board_state>(this);
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
		
		// Add to pieces list for network synchronization
		auto game_piece = std::dynamic_pointer_cast<c_game_piece>(piece);
		if (game_piece)
		{
			m_pieces.push_back(game_piece);
		}

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
		
		// Remove from pieces list
		auto game_piece = std::dynamic_pointer_cast<c_game_piece>(piece);
		if (game_piece)
		{
			auto it = std::find(m_pieces.begin(), m_pieces.end(), game_piece);
			if (it != m_pieces.end())
			{
				m_pieces.erase(it);
			}
		}

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
	
	void c_bayou_state::clear_pieces()
	{
		// Clear the board
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			for (int y = 0; y < BOARD_HEIGHT; y++)
			{
				m_board[x][y].reset();
			}
		}
		
		// Clear pieces list
		m_pieces.clear();
	}
	
	std::vector<std::shared_ptr<c_game_piece>> c_bayou_state::get_pieces() const
	{
		return m_pieces;
	}
	
	std::shared_ptr<c_game_piece> c_bayou_state::get_piece_by_id(uint32_t id) const
	{
		for (const auto& piece : m_pieces)
		{
			if (piece->get_id() == id)
			{
				return piece;
			}
		}
		return nullptr;
	}
	
	void c_bayou_state::set_texture_for_piece_type(uint32_t piece_type, std::shared_ptr<c_texture> texture)
	{
		m_piece_textures[piece_type] = texture;
	}
	
	std::shared_ptr<c_texture> c_bayou_state::get_texture_for_piece_type(uint32_t piece_type) const
	{
		auto it = m_piece_textures.find(piece_type);
		if (it != m_piece_textures.end())
		{
			return it->second;
		}
		return nullptr;
	}
}
