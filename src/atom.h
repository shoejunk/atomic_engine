#pragma once

#include "utility.h"
#include <vector>
#include <unordered_map>
#include <memory>

namespace atom
{
	// Forward declaration of the atom base
	class c_atom;

	// Base template for all atoms
	template<typename Derived, typename Parent = c_atom>
	class t_atom : public Parent
	{
	public:
		static constexpr uint32_t kind() {
			// This should be specialized in derived classes
			return Derived::kind_id();
		}

		// For classes directly inheriting from c_atom, family is the same as kind
		// For others, it's the family of the first ancestor above c_atom
		static constexpr uint32_t family() {
			if constexpr (std::is_same_v<Parent, c_atom>) {
				return kind();
			}
			else {
				return Parent::family();
			}
		}

		// Virtual function implementations
		virtual uint32_t get_kind() const override { return kind(); }

		virtual bool kind_of(uint32_t in_kind) override {
			return in_kind == kind() || Parent::kind_of(in_kind);
		}

		virtual uint32_t parent_kind() override { return Parent::kind(); }

		virtual uint32_t get_family() override { return family(); }
	};

	// Base atom class
	class c_atom
	{
	public:
		static constexpr uint32_t kind() { return "atom"_h; }

	public:
		c_atom() = default;
		virtual ~c_atom() = default;

		// Pure virtual functions that derived classes will implement via CRTP
		virtual uint32_t get_kind() const = 0;
		virtual bool kind_of(uint32_t in_kind) { return in_kind == kind(); }
		virtual uint32_t parent_kind() = 0;
		virtual uint32_t get_family() = 0;

		virtual bool update();
		virtual bool go();
		bool add_child(std::unique_ptr<c_atom> child);

		void add_connection(std::shared_ptr<c_atom> connection)
		{
			m_connections[connection->get_family()].push_back(connection);
		}

		template<class T>
		std::vector<std::shared_ptr<T>> get_connections()
		{
			auto& connections = m_connections[T::kind()];
			std::vector<std::shared_ptr<T>> result;
			for (auto& connection : connections)
			{
				if (auto ptr = connection.lock()) {
					result.push_back(std::static_pointer_cast<T>(ptr));
				}
			}
			return result;
		}

	protected:
		std::unordered_map<uint32_t, std::unique_ptr<c_atom>> m_children;
		std::unordered_map<uint32_t, std::vector<std::weak_ptr<c_atom>>> m_connections;
	};
} // namespace atom