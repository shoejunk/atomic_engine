#pragma once

#include "army.h"
#include "atom.h"
#include "board_state.h"
#include "game_board_position.h"
#include "game_piece.h"
#include "texture.h"

#include <array>
#include <memory>
#include <cstdint>
#include <vector>
#include <unordered_map>

namespace atom
{
	// Bayou game state implementation
	class c_bayou_state : public c_atom, public i_board_state
	{
	public:
		static constexpr uint32_t type() { return "bayou_state"_h; }

		static constexpr uint8_t BOARD_WIDTH = 8;
		static constexpr uint8_t BOARD_HEIGHT = 8;

	public:
		c_bayou_state();

		// Get aspect types
		std::vector<uint32_t> get_aspect_types() const override
		{
			return { c_bayou_state::type(), i_board_state::type()};
		}

		// i_board_state implementation
		std::shared_ptr<c_atom> get_piece_at(uint8_t x, uint8_t y) override
		{
			if (x >= BOARD_WIDTH || y >= BOARD_HEIGHT)
			{
				return nullptr;
			}
			return m_board[x][y];
		}

		bool add_piece(std::shared_ptr<c_atom> piece) override;
		bool remove_piece(std::shared_ptr<c_atom> piece) override;
		bool move_piece(std::shared_ptr<c_atom> piece, uint8_t new_x, uint8_t new_y) override;
		uint8_t get_board_width() const override { return BOARD_WIDTH; }
		uint8_t get_board_height() const override { return BOARD_HEIGHT; }

		// Network-related methods
		void clear_pieces();
		std::vector<std::shared_ptr<c_game_piece>> get_pieces() const;
		std::shared_ptr<c_game_piece> get_piece_by_id(uint32_t id) const;
		void set_texture_for_piece_type(uint32_t piece_type, std::shared_ptr<c_texture> texture);
		std::shared_ptr<c_texture> get_texture_for_piece_type(uint32_t piece_type) const;

	private:
		std::array<std::array<std::shared_ptr<c_atom>, BOARD_HEIGHT>, BOARD_WIDTH> m_board;
		std::array<c_army, 2> m_armies;
		std::vector<std::shared_ptr<c_game_piece>> m_pieces;
		std::unordered_map<uint32_t, std::shared_ptr<c_texture>> m_piece_textures;
	};
}