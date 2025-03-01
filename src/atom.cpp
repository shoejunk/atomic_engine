#include "atom.h"

namespace atom
{
	bool c_atom::update()
	{
		bool ok = true;
		for (auto& child : m_children)
		{
			ok &= child->update();
		}
		return ok;
	}

	bool c_atom::go()
	{
		bool ok = true;
		for (auto& child : m_children)
		{
			ok &= child->go();
		}
		return ok;
	}

	bool c_atom::add_child(std::unique_ptr<c_atom> child)
	{
		if (!child)
		{
			return false;
		}

		// Check for aspect conflicts
		auto child_aspects = child->get_aspect_types();
		for (auto aspect_type : child_aspects)
		{
			if (has(aspect_type))
			{
				// Conflict found - this atom already has this aspect
				return false;
			}
		}

		// Set parent pointer
		child->set_parent(this);

		// Register all child aspects in the parent
		for (auto aspect_type : child_aspects)
		{
			// Because aspect_type is in child_aspects, we know that child implements an aspect
			i_aspect* aspect_ptr = reinterpret_cast<i_aspect*>(child.get());
			if (aspect_ptr)
			{
				m_aspects[aspect_type] = aspect_ptr;
			}
		}

		// Add child to children vector
		m_children.push_back(std::move(child));

		return true;
	}
} // namespace atom