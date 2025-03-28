#pragma once

#include "action.h"
#include "action_handler.h"
#include "atom.h"
#include "network_game_manager.h"

#include "utility.h"

#include <vector>
#include <memory>

namespace atom
{
	class c_board_action_handler : public c_atom, public i_action_handler
	{
	public:
		// Define action and context hash constants
		static constexpr uint32_t MOVE_LEFT = "move_left"_h;
		static constexpr uint32_t MOVE_RIGHT = "move_right"_h;
		static constexpr uint32_t MOVE_UP = "move_up"_h;
		static constexpr uint32_t MOVE_DOWN = "move_down"_h;
		static constexpr uint32_t SELECT = "select"_h;
		static constexpr uint32_t DRAG_START = "drag_start"_h;
		static constexpr uint32_t DRAG = "drag"_h;
		static constexpr uint32_t DROP = "drop"_h;

	public:
		c_board_action_handler();
		
		std::vector<uint32_t> get_aspect_types() const override
		{
			return { i_action_handler::type() };
		}
		
		bool can_handle(uint32_t action_hash) const override
		{
			return std::find(m_handled_actions.begin(), m_handled_actions.end(), action_hash) != m_handled_actions.end();
		}
		
		bool handle_action(c_action const& action) override;
		
		// Set the network game manager
		void set_network_manager(std::shared_ptr<c_network_game_manager> network_manager)
		{
			m_network_manager = network_manager;
		}
		
	private:
		std::vector<uint32_t> m_handled_actions;
		std::weak_ptr<c_atom> m_selected_piece;
		std::weak_ptr<c_atom> m_dragged_piece;
		s_vector2i32 m_drag_offset;
		s_vector2u8 m_original_board_pos; // Store the original board position for drag-and-drop operations
		std::shared_ptr<c_network_game_manager> m_network_manager;
	};
}
