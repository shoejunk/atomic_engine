#pragma once
#include "atom.h"
#include "drawable.h"
#include "bayou_state.h"
#include "game_board_position.h"
#include "drawable_group.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <algorithm>

namespace atom
{
	// Bayou game state visualizer
	class c_bayou_game_visualizer : public c_atom, public i_drawable
	{
	public:
		static constexpr uint32_t type() { return "bayou_game_visualizer"_h; }

	public:
		c_bayou_game_visualizer(std::shared_ptr<c_bayou_state> game_state)
			: m_game_state(game_state)
			, m_cell_size(96)
			, m_board_x(50)
			, m_board_y(50)
		{
			// Create the grid background
			initialize_grid();

			// Register aspects
			register_aspect<i_drawable>(this);
			register_aspect<c_bayou_game_visualizer>(this);
		}

		// Get aspect types
		std::vector<uint32_t> get_aspect_types() const override
		{
			return {
				i_drawable::type(),
				c_bayou_game_visualizer::type()
			};
		}

		// Convert screen position to board position
		s_vector2u8 screen_to_board_position(const s_vector2i32& screen_pos) const
		{
			// Calculate board coordinates from screen coordinates
			int32_t board_x = (screen_pos.x - m_board_x) / m_cell_size;
			int32_t board_y = (screen_pos.y - m_board_y) / m_cell_size;
			
			// Clamp to valid board coordinates
			board_x = std::max(0, std::min(board_x, static_cast<int32_t>(m_game_state->get_board_width() - 1)));
			board_y = std::max(0, std::min(board_y, static_cast<int32_t>(m_game_state->get_board_height() - 1)));
			
			return {static_cast<uint8_t>(board_x), static_cast<uint8_t>(board_y)};
		}

		// Implement i_drawable
		sf::Drawable& get_drawable() override
		{
			return m_drawable_group;
		}

		void update_visualization()
		{
			// Clear pieces (but keep grid)
			// The previous approach caused an infinite loop because popTransform() affects m_transform_stack
			// but size() looks at m_drawables, which are separate data structures
			m_drawable_group.clear_except_first();

			// Add all pieces from game state
			for (uint8_t x = 0; x < m_game_state->get_board_width(); x++)
			{
				for (uint8_t y = 0; y < m_game_state->get_board_height(); y++)
				{
					auto piece = m_game_state->get_piece_at(x, y);
					if (!piece)
					{
						continue;
					}

					auto drawable = piece->as<i_drawable>();
					if (!drawable)
					{
						continue;
					}

					// Calculate screen position
					float screen_x = m_board_x + x * m_cell_size + m_cell_size / 2.0f;
					float screen_y = m_board_y + y * m_cell_size + m_cell_size / 2.0f;

					// Add transform for this piece
					sf::Transform transform;
					transform.translate(screen_x, screen_y);
					m_drawable_group.pushTransform(transform);
					m_drawable_group.draw(drawable->get_drawable());
					m_drawable_group.popTransform();
				}
			}
		}

		void update_visualization_with_dragged_piece(std::shared_ptr<c_atom> dragged_piece, float x, float y)
		{
			// First update the normal visualization (without the dragged piece)
			update_visualization();
			
			// Then add the dragged piece at its current position
			if (auto drawable = dragged_piece->as<i_drawable>())
			{
				add_dragged_piece(drawable, x, y);
			}
		}

		void set_cell_size(uint32_t size)
		{
			m_cell_size = static_cast<uint32_t>(size);
			initialize_grid();
		}

		void set_board_position(uint32_t x, uint32_t y)
		{
			m_board_x = x;
			m_board_y = y;
			initialize_grid();
		}

		// Getters for board position and cell size
		uint32_t get_board_x() const { return m_board_x; }
		uint32_t get_board_y() const { return m_board_y; }
		uint32_t get_cell_size() const { return m_cell_size; }

		// Add a piece that's being dragged to the drawable group
		void add_dragged_piece(i_drawable* drawable, float x, float y)
		{
			// Add transform for this piece
			sf::Transform transform;
			transform.translate(x, y);
			m_drawable_group.pushTransform(transform);
			m_drawable_group.draw(drawable->get_drawable());
			m_drawable_group.popTransform();
		}

	private:
		std::shared_ptr<c_bayou_state> m_game_state;
		uint32_t m_cell_size;
		uint32_t m_board_x;
		uint32_t m_board_y;
		sf::RenderTexture m_grid_texture;
		sf::Sprite m_grid_sprite;
		sf::RenderStates m_render_states;
		c_drawable_group m_drawable_group;

		void initialize_grid()
		{
			// Calculate grid dimensions
			uint32_t grid_width = m_game_state->get_board_width() * m_cell_size;
			uint32_t grid_height = m_game_state->get_board_height() * m_cell_size;

			// Create render texture for grid
			m_grid_texture.create(grid_width, grid_height);
			m_grid_texture.clear(sf::Color(50, 50, 50));

			// Draw grid lines
			sf::RectangleShape cell(sf::Vector2f(static_cast<float>(m_cell_size), static_cast<float>(m_cell_size)));
			cell.setFillColor(sf::Color::Transparent);
			cell.setOutlineColor(sf::Color(100, 100, 100));
			cell.setOutlineThickness(1.0f);

			// Alternating cell colors
			sf::RectangleShape dark_cell(sf::Vector2f(static_cast<float>(m_cell_size), static_cast<float>(m_cell_size)));
			dark_cell.setFillColor(sf::Color(30, 30, 30));
			dark_cell.setOutlineColor(sf::Color(100, 100, 100));
			dark_cell.setOutlineThickness(1.0f);

			// Draw cells
			for (uint8_t x = 0; x < m_game_state->get_board_width(); x++)
			{
				for (uint8_t y = 0; y < m_game_state->get_board_height(); y++)
				{
					// Set position
					cell.setPosition(static_cast<float>(x * m_cell_size), static_cast<float>(y * m_cell_size));
					dark_cell.setPosition(static_cast<float>(x * m_cell_size), static_cast<float>(y * m_cell_size));

					// Draw alternating colors
					if ((x + y) % 2 == 0)
					{
						m_grid_texture.draw(dark_cell);
					}
					else
					{
						m_grid_texture.draw(cell);
					}
				}
			}

			m_grid_texture.display();
			m_grid_sprite.setTexture(m_grid_texture.getTexture());
			m_grid_sprite.setPosition(static_cast<float>(m_board_x), static_cast<float>(m_board_y));

			// Set up drawable group
			m_drawable_group.clear();
			m_drawable_group.draw(m_grid_sprite);
		}
	};
}