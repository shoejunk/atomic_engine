#pragma once
#include "atom.h"
#include "input_context.h"
#include "action_handler.h"
#include <stack>
#include <memory>
#include <unordered_map>
#include <SFML/Window/Event.hpp>

namespace atom
{
	class c_input_context;
	class i_action_handler;

	// Input manager aspect
	class i_input_manager
	{
	public:
		static constexpr uint32_t type() { return "input_manager"_h; }

		// Create a new context
		virtual std::shared_ptr<c_input_context> create_context(uint32_t context_hash) = 0;

		// Get a context by hash
		virtual std::shared_ptr<c_input_context> get_context(uint32_t context_hash) = 0;

		// Push a context onto the stack
		virtual void push_context(uint32_t context_hash) = 0;

		// Pop the top context from the stack
		virtual void pop_context() = 0;

		// Clear all contexts
		virtual void clear_contexts() = 0;

		// Process an event through the active context
		virtual void process_event(const sf::Event& event) = 0;
	};

	class c_input_manager : public c_atom, public i_input_manager
	{
	public:
		// Initialize the input manager
		c_input_manager()
		{
			// Register this as an input manager aspect
			register_aspect<i_input_manager>(this);
		}

		// Get aspect types
		std::vector<uint32_t> get_aspect_types() const override
		{
			return { i_input_manager::type() };
		}

		// Update function (called per frame)
		bool update() override;

		// i_input_manager implementation
		// Create a new context
		std::shared_ptr<c_input_context> create_context(uint32_t context_hash) override;

		// Get a context by hash
		std::shared_ptr<c_input_context> get_context(uint32_t context_hash) override;

		// Push a context onto the stack
		void push_context(uint32_t context_hash) override;

		// Pop the top context from the stack
		void pop_context() override;

		// Clear all contexts
		void clear_contexts() override;

		// Process an event through the active context
		void process_event(const sf::Event& event) override;

	private:
		std::unordered_map<uint32_t, std::shared_ptr<c_input_context>> m_contexts;
		std::stack<std::shared_ptr<c_input_context>> m_context_stack;
	};
}