#include "input_binding.h"
#include <algorithm>

namespace atom
{
	c_input_binding::c_input_binding(sf::Keyboard::Key key, uint32_t action_hash,
		e_activation_type activation, e_modifiers modifiers)
		: m_key(key)
		, m_is_keyboard(true)
		, m_is_combination(false)
		, m_activation_type(activation)
		, m_modifiers(modifiers)
		, m_action_hash(action_hash)
	{
	}

	c_input_binding::c_input_binding(sf::Mouse::Button button, uint32_t action_hash,
		e_activation_type activation, e_modifiers modifiers)
		: m_button(button)
		, m_is_keyboard(false)
		, m_is_combination(false)
		, m_activation_type(activation)
		, m_modifiers(modifiers)
		, m_action_hash(action_hash)
	{
	}

	c_input_binding::c_input_binding(const std::vector<sf::Keyboard::Key>& keys, uint32_t action_hash,
		e_activation_type activation)
		: m_keys(keys)
		, m_is_keyboard(true)
		, m_is_combination(true)
		, m_activation_type(activation)
		, m_modifiers(e_modifiers::none)
		, m_action_hash(action_hash)
	{
	}

	bool c_input_binding::check_modifiers() const
	{
		bool ctrl_required = (m_modifiers & e_modifiers::ctrl) != e_modifiers::none;
		bool shift_required = (m_modifiers & e_modifiers::shift) != e_modifiers::none;
		bool alt_required = (m_modifiers & e_modifiers::alt) != e_modifiers::none;

		bool ctrl_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);
		bool shift_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
		bool alt_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt);

		return (ctrl_required == ctrl_pressed) &&
			(shift_required == shift_pressed) &&
			(alt_required == alt_pressed);
	}

	bool c_input_binding::is_activated(const sf::Event& event) const
	{
		if (m_is_combination)
		{
			// For combinations, we need a key event for the last key and all others must be held
			if ((m_activation_type == e_activation_type::pressed && event.type == sf::Event::KeyPressed) ||
				(m_activation_type == e_activation_type::released && event.type == sf::Event::KeyReleased))
			{
				// Check if the event is for one of our keys
				auto it = std::find(m_keys.begin(), m_keys.end(), event.key.code);
				if (it != m_keys.end())
				{
					// Check if all other keys are pressed
					for (auto& key : m_keys)
					{
						if (key != event.key.code && !sf::Keyboard::isKeyPressed(key))
						{
							return false;
						}
					}
					return true;
				}
			}
			return false;
		}
		else if (m_is_keyboard)
		{
			if (m_activation_type == e_activation_type::pressed &&
				event.type == sf::Event::KeyPressed &&
				event.key.code == m_key &&
				check_modifiers())
			{
				return true;
			}
			else if (m_activation_type == e_activation_type::released &&
				event.type == sf::Event::KeyReleased &&
				event.key.code == m_key &&
				check_modifiers())
			{
				return true;
			}
		}
		else
		{
			if (m_activation_type == e_activation_type::pressed &&
				event.type == sf::Event::MouseButtonPressed &&
				event.mouseButton.button == m_button &&
				check_modifiers())
			{
				return true;
			}
			else if (m_activation_type == e_activation_type::released &&
				event.type == sf::Event::MouseButtonReleased &&
				event.mouseButton.button == m_button &&
				check_modifiers())
			{
				return true;
			}
		}
		return false;
	}

	bool c_input_binding::is_held() const
	{
		if (m_activation_type != e_activation_type::held)
		{
			return false;
		}

		if (!check_modifiers())
		{
			return false;
		}

		if (m_is_combination)
		{
			// For combinations, all keys must be held
			for (auto& key : m_keys)
			{
				if (!sf::Keyboard::isKeyPressed(key))
				{
					return false;
				}
			}
			return true;
		}
		else if (m_is_keyboard)
		{
			return sf::Keyboard::isKeyPressed(m_key);
		}
		else
		{
			return sf::Mouse::isButtonPressed(m_button);
		}
	}

	uint32_t c_input_binding::get_action_hash() const
	{
		return m_action_hash;
	}
}