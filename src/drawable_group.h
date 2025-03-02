#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <stack>

namespace atom
{
	// A drawable that can contain multiple other drawables with transforms
	class c_drawable_group : public sf::Drawable
	{
	public:
		c_drawable_group() {}

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override
		{
			for (const auto& item : m_drawables)
			{
				sf::RenderStates combinedStates = states;
				combinedStates.transform.combine(item.transform);
				target.draw(*item.drawable, combinedStates);
			}
		}

		void draw(const sf::Drawable& drawable)
		{
			DrawableItem item;
			item.drawable = &drawable;
			item.transform = m_transform_stack.empty() ? sf::Transform() : m_transform_stack.top();
			m_drawables.push_back(item);
		}

		void pushTransform(const sf::Transform& transform)
		{
			if (m_transform_stack.empty())
			{
				m_transform_stack.push(transform);
			}
			else
			{
				sf::Transform combined = m_transform_stack.top();
				combined.combine(transform);
				m_transform_stack.push(combined);
			}
		}

		void popTransform()
		{
			if (!m_transform_stack.empty())
			{
				m_transform_stack.pop();
			}
		}

		void clear()
		{
			m_drawables.clear();
			while (!m_transform_stack.empty())
			{
				m_transform_stack.pop();
			}
		}

		// Clears all drawables except the first one
		void clear_except_first()
		{
			if (!m_drawables.empty())
			{
				// Save the first drawable
				DrawableItem first = m_drawables[0];
				
				// Clear everything
				clear();
				
				// Add back the first drawable
				m_drawables.push_back(first);
			}
		}

		size_t size() const
		{
			return m_drawables.size();
		}

	private:
		struct DrawableItem
		{
			const sf::Drawable* drawable;
			sf::Transform transform;
		};

		std::vector<DrawableItem> m_drawables;
		std::stack<sf::Transform> m_transform_stack;
	};
}