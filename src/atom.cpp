#include "atom.h"

namespace atom
{
	bool c_atom::update()
	{
		bool ok = true;
		for (auto* child : m_children)
		{
			ok &= child->update();
		}
		return ok;
	}

	bool c_atom::go()
	{
		bool ok = true;
		for (auto* child : m_children)
		{
			ok &= child->go();
		}
		return ok;
	}

	void c_atom::add_child(c_atom& child)
	{
		m_children.push_back(&child);
	}
} // namespace atom
