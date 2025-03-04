#pragma once

#include "action.h"
#include "action_handler.h"
#include "atom.h"
#include "bayou_state.h"
#include "game_piece.h"
#include "game_state_visualizer.h"

#include "utility.h"

#include <vector>

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

	public:
		c_board_action_handler(
			std::shared_ptr<c_bayou_state> game_state,
			std::shared_ptr<c_bayou_game_visualizer> visualizer);
		
		std::vector<uint32_t> get_aspect_types() const override
		{
			return { i_action_handler::type() };
		}
		
		bool can_handle(uint32_t action_hash) const override
		{
			return std::find(m_handled_actions.begin(), m_handled_actions.end(), action_hash) != m_handled_actions.end();
		}
		
		bool handle_action(c_action const& action) override;
		
	private:
		std::shared_ptr<c_bayou_state> m_game_state;
		std::shared_ptr<c_bayou_game_visualizer> m_visualizer;
		std::vector<uint32_t> m_handled_actions;
		std::weak_ptr<c_atom> m_selected_piece;
	};
}
