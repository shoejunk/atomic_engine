#include "input_context.h"
#include <algorithm>

namespace atom
{
	c_input_context::c_input_context(uint32_t context_hash)
		: m_context_hash(context_hash)
	{
	}

	void c_input_context::add_binding(std::shared_ptr<c_input_binding> binding)
	{
		m_bindings.push_back(binding);
	}

	void c_input_context::remove_binding(std::shared_ptr<c_input_binding> binding)
	{
		auto it = std::find(m_bindings.begin(), m_bindings.end(), binding);
		if (it != m_bindings.end())
		{
			m_bindings.erase(it);
		}
	}

	void c_input_context::remove_bindings_for_action(uint32_t action_hash)
	{
		// Create a copy of the bindings vector
		auto bindings_copy = m_bindings;

		// Clear the original bindings
		m_bindings.clear();

		// Add back only the bindings that don't match the action hash
		for (auto& binding : bindings_copy)
		{
			if (binding->get_action_hash() != action_hash)
			{
				m_bindings.push_back(binding);
			}
		}
	}

	void c_input_context::process_event(const sf::Event& event, const std::vector<std::shared_ptr<c_action_handler>>& handlers)
	{
		for (auto& binding : m_bindings)
		{
			if (binding->is_activated(event))
			{
				trigger_action(binding->get_action_hash(), handlers);
			}
		}
	}

	void c_input_context::update(const std::vector<std::shared_ptr<c_action_handler>>& handlers)
	{
		for (auto& binding : m_bindings)
		{
			if (binding->is_held())
			{
				trigger_action(binding->get_action_hash(), handlers);
			}
		}
	}

	void c_input_context::trigger_action(uint32_t action_hash, const std::vector<std::shared_ptr<c_action_handler>>& handlers) const
	{
		// Send the action to all handlers that can process it
		for (auto& handler : handlers)
		{
			if (handler->can_handle(action_hash))
			{
				handler->handle_action(action_hash);
			}
		}
	}

	uint32_t c_input_context::get_context_hash() const
	{
		return m_context_hash;
	}
}