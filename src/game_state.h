#pragma once
#include "army.h"
#include "atom.h"
#include "game_board_position.h"
#include "game_piece.h"

#include <array>
#include <memory>
#include <cstdint>

namespace atom
{
	// Game state aspect - represents the state of the game
	class i_game_state
	{
	public:
		static constexpr uint32_t type() { return "game_state"_h; }

		// Get a piece at a specific position
		virtual std::shared_ptr<c_atom> get_piece_at(uint8_t x, uint8_t y) const = 0;

		// Add a piece to the board
		virtual bool add_piece(std::shared_ptr<c_atom> piece) = 0;

		// Remove a piece from the board
		virtual bool remove_piece(std::shared_ptr<c_atom> piece) = 0;

		// Move a piece on the board
		virtual bool move_piece(std::shared_ptr<c_atom> piece, uint8_t x, uint8_t y) = 0;

		// Get board dimensions
		virtual uint8_t get_board_width() const = 0;
		virtual uint8_t get_board_height() const = 0;
	};

	// Bayou game state implementation
	class c_bayou_game_state : public c_atom, public i_game_state
	{
	public:
		static constexpr uint8_t BOARD_WIDTH = 8;
		static constexpr uint8_t BOARD_HEIGHT = 8;

		c_bayou_game_state();

		// Get aspect types
		std::vector<uint32_t> get_aspect_types() const override
		{
			return { i_game_state::type() };
		}

		// Implement i_game_state
		std::shared_ptr<c_atom> get_piece_at(uint8_t x, uint8_t y) const override
		{
			if (x >= BOARD_WIDTH || y >= BOARD_HEIGHT)
			{
				return nullptr;
			}
			return m_board[x][y];
		}

		bool add_piece(std::shared_ptr<c_atom> piece) override
		{
			// Check if piece has board position aspect
			auto board_pos = piece->as<i_board_position>();
			if (!board_pos)
			{
				return false;
			}

			uint8_t x = board_pos->get_board_x();
			uint8_t y = board_pos->get_board_y();

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
			
			// Add connection for tracking
			add_connection<i_board_position>(piece);
			
			return true;
		}

		bool remove_piece(std::shared_ptr<c_atom> piece) override
		{
			// Check if piece has board position aspect
			auto board_pos = piece->as<i_board_position>();
			if (!board_pos)
			{
				return false;
			}

			uint8_t x = board_pos->get_board_x();
			uint8_t y = board_pos->get_board_y();

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

		bool move_piece(std::shared_ptr<c_atom> piece, uint8_t new_x, uint8_t new_y) override
		{
			// Check if piece has board position aspect
			auto board_pos = piece->as<i_board_position>();
			if (!board_pos)
			{
				return false;
			}

			uint8_t old_x = board_pos->get_board_x();
			uint8_t old_y = board_pos->get_board_y();

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
			board_pos->set_board_position(new_x, new_y);

			return true;
		}

		uint8_t get_board_width() const override { return BOARD_WIDTH; }
		uint8_t get_board_height() const override { return BOARD_HEIGHT; }

	private:
		std::array<std::array<std::shared_ptr<c_atom>, BOARD_HEIGHT>, BOARD_WIDTH> m_board;
		std::array<c_army, 2> m_armies;
	};
}