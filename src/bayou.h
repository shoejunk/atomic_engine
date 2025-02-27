#pragma once

#include "atom.h"

namespace atom
{
	class c_bayou : public c_atom
	{
	public:
		// Kind is the hash of "bayou"
		static consteval uint32_t kind() { return "bayou"_h; }

		// Since this is a direct descendant of c_atom, family is the same as kind
		static consteval uint32_t family() { return kind(); }

	public:
		// Return this class's kind
		virtual uint32_t get_kind() const override { return kind(); }

		// Return true if in_kind matches this class or any parent class
		virtual bool kind_of(uint32_t in_kind) override {
			return in_kind == kind() || c_atom::kind_of(in_kind);
		}

		// Return parent's kind
		virtual uint32_t parent_kind() override { return c_atom::kind(); }

		// Return family (same as kind since it's a direct descendant of c_atom)
		virtual uint32_t get_family() override { return family(); }

		bool go() override;
	};
} // namespace atom