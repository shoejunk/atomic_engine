#pragma once
#include "atom.h"
#include "game_board_position.h"
#include "drawable.h"
#include "sprite.h"
#include "texture.h"
#include <string>

namespace atom
{
	// Game piece class - represents an entity that can be placed on the board
	class c_game_piece : public c_atom, public i_board_position, public i_drawable
	{
	public:
		c_game_piece(const c_texture& texture, const std::string& piece_type, uint8_t board_x = 0, uint8_t board_y = 0)
			: m_board_x(board_x)
			, m_board_y(board_y)
			, m_piece_type(piece_type)
		{
			// Create sprite
			m_sprite = std::make_shared<c_sprite>(texture);
			m_sprite->set_scale(0.2f, 0.2f);
			
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

		// Get piece type
		const std::string& get_piece_type() const { return m_piece_type; }

	private:
		uint8_t m_board_x;
		uint8_t m_board_y;
		std::shared_ptr<c_sprite> m_sprite;
		std::string m_piece_type;
	};
}