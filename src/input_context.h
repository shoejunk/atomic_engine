#pragma once
#include "atom.h"
#include "input_binding.h"
#include "action_handler.h"
#include <vector>
#include <memory>
#include <SFML/Window/Event.hpp>

namespace atom
{
	class c_input_binding;
	class i_action_handler;

	// Input context aspect
	class i_input_context : public i_aspect
	{
	public:
		static constexpr uint32_t type() { return "input_context"_h; }

		virtual uint32_t get_aspect_type() const override { return type(); }

		// Add a binding to this context
		virtual void add_binding(std::shared_ptr<c_input_binding> binding) = 0;

		// Remove a binding from this context
		virtual void remove_binding(std::shared_ptr<c_input_binding> binding) = 0;

		// Remove all bindings for a specific action
		virtual void remove_bindings_for_action(uint32_t action_hash) = 0;

		// Process an input event and distribute actions to handlers
		virtual void process_event(const sf::Event& event, const std::vector<std::shared_ptr<i_action_handler>>& handlers) = 0;

		// Update held inputs
		virtual void update(const std::vector<std::shared_ptr<i_action_handler>>& handlers) = 0;

		// Get the context hash
		virtual uint32_t get_context_hash() const = 0;
	};

	class c_input_context : public c_atom, public i_input_context
	{
	public:
		static constexpr uint32_t kind_id() { return "input_context"_h; }

		// Constructor with context hash
		c_input_context(uint32_t context_hash);

		// Get aspect types
		std::vector<uint32_t> get_aspect_types() const override
		{
			return { i_input_context::type() };
		}

		// Add a binding to this context
		void add_binding(std::shared_ptr<c_input_binding> binding) override;

		// Remove a binding from this context
		void remove_binding(std::shared_ptr<c_input_binding> binding) override;

		// Remove all bindings for a specific action
		void remove_bindings_for_action(uint32_t action_hash) override;

		// Process an input event and distribute actions to handlers
		void process_event(const sf::Event& event, const std::vector<std::shared_ptr<i_action_handler>>& handlers) override;

		// Update held inputs
		void update(const std::vector<std::shared_ptr<i_action_handler>>& handlers) override;

		// Get the context hash
		uint32_t get_context_hash() const override;

	private:
		uint32_t m_context_hash;
		std::vector<std::shared_ptr<c_input_binding>> m_bindings;

		// Helper to trigger action on handlers
		void trigger_action(uint32_t action_hash, const std::vector<std::shared_ptr<i_action_handler>>& handlers) const;
	};
}