#pragma once

#include <vector>

namespace atom
{
	class c_atom
	{
	public:
		c_atom() {};
		~c_atom() {};
		virtual bool update();
		virtual bool go();
		void add_child(c_atom& child);

	protected:
		std::vector<c_atom*> m_children;
	};
} // namespace atom
