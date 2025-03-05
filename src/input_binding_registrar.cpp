#include "input_binding_registrar.h"

namespace atom
{
	void c_input_binding_registrar::register_binding(
		uint32_t context_hash,
		uint32_t action_hash,
		sf::Keyboard::Key key,
		e_activation_type activation,
		e_modifiers modifiers
	)
	{
		// Get the input manager
		auto* input_manager = get_parent()->as<c_input_manager>();
		if (!input_manager)
		{
			return;
		}

		// Get or create the context
		auto context = input_manager->get_context(context_hash);
		if (!context)
		{
			context = input_manager->create_context(context_hash);
		}

		// Create and register the binding
		auto binding = std::make_shared<c_input_binding>(key, action_hash, activation, modifiers);
		context->add_binding(binding);
	}

	void c_input_binding_registrar::register_binding(
		uint32_t context_hash,
		uint32_t action_hash,
		sf::Mouse::Button button,
		e_activation_type activation,
		e_modifiers modifiers
	)
	{
		// Get the input manager
		auto* input_manager = get_parent()->as<c_input_manager>();
		if (!input_manager)
		{
			return;
		}

		// Get or create the context
		auto context = input_manager->get_context(context_hash);
		if (!context)
		{
			context = input_manager->create_context(context_hash);
		}

		// Create and register the binding
		auto binding = std::make_shared<c_input_binding>(button, action_hash, activation, modifiers);
		context->add_binding(binding);
	}

	void c_input_binding_registrar::register_binding(
		uint32_t context_hash,
		uint32_t action_hash,
		const std::vector<sf::Keyboard::Key>& keys,
		e_activation_type activation
	)
	{
		// Get the input manager
		auto* input_manager = get_parent()->as<c_input_manager>();
		if (!input_manager)
		{
			return;
		}

		// Get or create the context
		auto context = input_manager->get_context(context_hash);
		if (!context)
		{
			context = input_manager->create_context(context_hash);
		}

		// Create and register the binding
		auto binding = std::make_shared<c_input_binding>(keys, action_hash, activation);
		context->add_binding(binding);
	}

	void c_input_binding_registrar::rebind(
		uint32_t context_hash,
		uint32_t action_hash,
		sf::Keyboard::Key key,
		e_activation_type activation,
		e_modifiers modifiers
	)
	{
		// Get the input manager
		auto* input_manager = get_parent()->as<c_input_manager>();
		if (!input_manager)
		{
			return;
		}

		// Get the context
		auto context = input_manager->get_context(context_hash);
		if (!context)
		{
			return;
		}

		// Remove any existing bindings for this action
		context->remove_bindings_for_action(action_hash);

		// Add the new binding
		register_binding(context_hash, action_hash, key, activation, modifiers);
	}

	void c_input_binding_registrar::rebind(
		uint32_t context_hash,
		uint32_t action_hash,
		sf::Mouse::Button button,
		e_activation_type activation,
		e_modifiers modifiers
	)
	{
		// Get the input manager
		auto* input_manager = get_parent()->as<c_input_manager>();
		if (!input_manager)
		{
			return;
		}

		// Get the context
		auto context = input_manager->get_context(context_hash);
		if (!context)
		{
			return;
		}

		// Remove any existing bindings for this action
		context->remove_bindings_for_action(action_hash);

		// Add the new binding
		register_binding(context_hash, action_hash, button, activation, modifiers);
	}
}