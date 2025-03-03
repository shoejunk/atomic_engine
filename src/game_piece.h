#pragma once
#include "army_ownable.h"
#include "atom.h"
#include "game_board_position.h"
#include "drawable.h"
#include "sprite.h"
#include "texture.h"

#include <string>

namespace atom
{
	// Game piece class - represents an entity that can be placed on the board
	class c_game_piece : public c_atom, public i_board_position, public i_drawable, public i_army_ownable
	{
	public:
		c_game_piece(const c_texture& texture, uint32_t piece_type, uint8_t army_idx, uint8_t board_x = 0, uint8_t board_y = 0)
			: m_piece_type(piece_type)
			, m_army_idx(army_idx)
			, m_board_x(board_x)
			, m_board_y(board_y)
		{
			// Create sprite
			m_sprite = std::make_shared<c_sprite>(texture);
			m_sprite->set_origin_center();
			m_sprite->set_scale(0.35f, 0.35f);
			
			// Register aspects
			register_aspect<i_board_position>(this);
			register_aspect<i_drawable>(this);
		}

		// Override get_aspect_types
		std::vector<uint32_t> get_aspect_types() const override
		{
			return {
				i_board_position::type(),
				i_drawable::type()
			};
		}

		// Implement i_board_position
		uint8_t get_board_x() const override { return m_board_x; }
		uint8_t get_board_y() const override { return m_board_y; }

		void set_board_position(uint8_t x, uint8_t y) override
		{
			m_board_x = x;
			m_board_y = y;
		}

		// Implement i_drawable (delegate to sprite)
		sf::Drawable& get_drawable() override { return m_sprite->get_drawable(); }

		// Implement i_army_ownable
		uint8_t get_army_idx() const override { return m_army_idx; }

		// Get piece type
		const uint32_t get_piece_type() const { return m_piece_type; }

	private:
		std::shared_ptr<c_sprite> m_sprite;
		uint32_t m_piece_type;
		uint8_t m_army_idx;
		uint8_t m_board_x;
		uint8_t m_board_y;
	};
}