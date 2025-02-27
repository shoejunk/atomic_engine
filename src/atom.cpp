#include "atom.h"

namespace atom
{
	bool c_atom::update()
	{
		bool ok = true;
		for (auto& [family, child] : m_children)
		{
			ok &= child->update();
		}
		return ok;
	}

	bool c_atom::go()
	{
		bool ok = true;
		for (auto& [family, child] : m_children)
		{
			ok &= child->go();
		}
		return ok;
	}

	bool c_atom::add_child(std::unique_ptr<c_atom> child)
	{
		auto it = m_children.find(child->get_family());
		if (it != m_children.end())
		{
			return false;
		}
		m_children[child->get_family()] = std::move(child);
		return true;
	}
} // namespace atom