#pragma once

#include "atom.h"
#include "bayou_state.h"
#include <memory>

namespace atom
{
	class c_network_game_manager;

	class c_bayou : public c_atom
	{
	public:
		bool go() override;

		// Set the network manager
		void set_network_manager(std::shared_ptr<c_network_game_manager> network_manager);
		
		// Get the game state
		std::shared_ptr<c_bayou_state> get_game_state() const;

	private:
		std::shared_ptr<c_network_game_manager> m_network_manager;
		std::shared_ptr<c_bayou_state> m_game_state;
	};
} // namespace atom