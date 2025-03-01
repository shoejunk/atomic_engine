#include "input_manager.h"
#include <algorithm>

namespace atom
{
	c_input_manager::c_input_manager()
	{
	}

	bool c_input_manager::update()
	{
		// First update the base class
		if (!c_atom::update())
		{
			return false;
		}

		// If there's no active context, nothing to do
		if (m_context_stack.empty())
		{
			return true;
		}

		// Update the active context for held inputs
		m_context_stack.top()->update(m_handlers);

		return true;
	}

	std::shared_ptr<c_input_context> c_input_manager::create_context(uint32_t context_hash)
	{
		auto context = std::make_shared<c_input_context>(context_hash);
		m_contexts[context_hash] = context;
		return context;
	}

	std::shared_ptr<c_input_context> c_input_manager::get_context(uint32_t context_hash)
	{
		auto it = m_contexts.find(context_hash);
		if (it != m_contexts.end())
		{
			return it->second;
		}
		return nullptr;
	}

	void c_input_manager::push_context(uint32_t context_hash)
	{
		auto context = get_context(context_hash);
		if (context)
		{
			m_context_stack.push(context);
		}
	}

	void c_input_manager::pop_context()
	{
		if (!m_context_stack.empty())
		{
			m_context_stack.pop();
		}
	}

	void c_input_manager::clear_contexts()
	{
		while (!m_context_stack.empty())
		{
			m_context_stack.pop();
		}
	}

	void c_input_manager::add_handler(std::shared_ptr<c_action_handler> handler)
	{
		if (handler)
		{
			m_handlers.push_back(handler);
		}
	}

	void c_input_manager::remove_handler(std::shared_ptr<c_action_handler> handler)
	{
		auto it = std::find(m_handlers.begin(), m_handlers.end(), handler);
		if (it != m_handlers.end())
		{
			m_handlers.erase(it);
		}
	}

	void c_input_manager::process_event(const sf::Event& event)
	{
		// If there's no active context, nothing to do
		if (m_context_stack.empty())
		{
			return;
		}

		// Process the event through the active context
		m_context_stack.top()->process_event(event, m_handlers);
	}
}