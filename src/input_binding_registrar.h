#pragma once
#include "atom.h"
#include "input_binding.h"
#include "input_manager.h"
#include <vector>

namespace atom
{
	// Input binding registrar aspect
	class i_input_binding_registrar : public i_aspect
	{
	public:
		static constexpr uint32_t type() { return "input_binding_registrar"_h; }

		virtual uint32_t get_aspect_type() const override { return type(); }

		// Register a binding to an action in a context
		virtual void register_binding(
			uint32_t context_hash,
			uint32_t action_hash,
			sf::Keyboard::Key key,
			e_activation_type activation = e_activation_type::pressed,
			e_modifiers modifiers = e_modifiers::none
		) = 0;

		// Register a binding with a mouse button
		virtual void register_binding(
			uint32_t context_hash,
			uint32_t action_hash,
			sf::Mouse::Button button,
			e_activation_type activation = e_activation_type::pressed,
			e_modifiers modifiers = e_modifiers::none
		) = 0;

		// Register a binding with a key combination
		virtual void register_binding(
			uint32_t context_hash,
			uint32_t action_hash,
			const std::vector<sf::Keyboard::Key>& keys,
			e_activation_type activation = e_activation_type::pressed
		) = 0;

		// Rebind a keyboard key
		virtual void rebind(
			uint32_t context_hash,
			uint32_t action_hash,
			sf::Keyboard::Key key,
			e_activation_type activation = e_activation_type::pressed,
			e_modifiers modifiers = e_modifiers::none
		) = 0;

		// Rebind a mouse button
		virtual void rebind(
			uint32_t context_hash,
			uint32_t action_hash,
			sf::Mouse::Button button,
			e_activation_type activation = e_activation_type::pressed,
			e_modifiers modifiers = e_modifiers::none
		) = 0;
	};

	class c_input_binding_registrar : public c_atom, public i_input_binding_registrar
	{
	public:
		c_input_binding_registrar()
		{
			register_aspect<i_input_binding_registrar>(this);
		}

		// Get aspect types
		std::vector<uint32_t> get_aspect_types() const override
		{
			return { i_input_binding_registrar::type() };
		}

		// Register a binding to an action in a context
		void register_binding(
			uint32_t context_hash,
			uint32_t action_hash,
			sf::Keyboard::Key key,
			e_activation_type activation = e_activation_type::pressed,
			e_modifiers modifiers = e_modifiers::none
		) override;

		// Register a binding with a mouse button
		void register_binding(
			uint32_t context_hash,
			uint32_t action_hash,
			sf::Mouse::Button button,
			e_activation_type activation = e_activation_type::pressed,
			e_modifiers modifiers = e_modifiers::none
		) override;

		// Register a binding with a key combination
		void register_binding(
			uint32_t context_hash,
			uint32_t action_hash,
			const std::vector<sf::Keyboard::Key>& keys,
			e_activation_type activation = e_activation_type::pressed
		) override;

		// Rebind a keyboard key
		void rebind(
			uint32_t context_hash,
			uint32_t action_hash,
			sf::Keyboard::Key key,
			e_activation_type activation = e_activation_type::pressed,
			e_modifiers modifiers = e_modifiers::none
		) override;

		// Rebind a mouse button
		void rebind(
			uint32_t context_hash,
			uint32_t action_hash,
			sf::Mouse::Button button,
			e_activation_type activation = e_activation_type::pressed,
			e_modifiers modifiers = e_modifiers::none
		) override;
	};
}