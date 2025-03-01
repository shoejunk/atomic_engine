#pragma once
#include "atom.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <vector>

namespace atom
{
	// Enum for activation types
	enum class e_activation_type
	{
		pressed,
		released,
		held
	};

	// Bitflags for modifiers
	enum class e_modifiers : uint8_t
	{
		none = 0,
		ctrl = 1 << 0,
		shift = 1 << 1,
		alt = 1 << 2
	};

	inline e_modifiers operator|(e_modifiers a, e_modifiers b)
	{
		return static_cast<e_modifiers>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}

	inline e_modifiers operator&(e_modifiers a, e_modifiers b)
	{
		return static_cast<e_modifiers>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
	}

	// Input binding aspect
	class i_input_binding
	{
	public:
		static constexpr uint32_t type() { return "input_binding"_h; }

		// Check if this binding is activated by the given event
		virtual bool is_activated(const sf::Event& event) const = 0;

		// Check if this binding is being held
		virtual bool is_held() const = 0;

		// Get the action hash
		virtual uint32_t get_action_hash() const = 0;
	};

	class c_input_binding : public c_atom, public i_input_binding
	{
	public:
		// Constructor for keyboard binding
		c_input_binding(sf::Keyboard::Key key, uint32_t action_hash,
			e_activation_type activation = e_activation_type::pressed,
			e_modifiers modifiers = e_modifiers::none);

		// Constructor for mouse button binding
		c_input_binding(sf::Mouse::Button button, uint32_t action_hash,
			e_activation_type activation = e_activation_type::pressed,
			e_modifiers modifiers = e_modifiers::none);

		// Constructor for key combination (all keys must be pressed)
		c_input_binding(const std::vector<sf::Keyboard::Key>& keys, uint32_t action_hash,
			e_activation_type activation = e_activation_type::pressed);

		// Get aspect types
		std::vector<uint32_t> get_aspect_types() const override
		{
			return { i_input_binding::type() };
		}

		// Check if this binding is activated by the given event
		bool is_activated(const sf::Event& event) const override;

		// Check if this binding is being held
		bool is_held() const override;

		// Get the action hash
		uint32_t get_action_hash() const override;

	private:
		// For single input
		union {
			sf::Keyboard::Key m_key;
			sf::Mouse::Button m_button;
		};

		// For key combinations
		std::vector<sf::Keyboard::Key> m_keys;

		bool m_is_keyboard;
		bool m_is_combination;
		e_activation_type m_activation_type;
		e_modifiers m_modifiers;
		uint32_t m_action_hash;

		// Helper to check if modifiers match current keyboard state
		bool check_modifiers() const;
	};
}