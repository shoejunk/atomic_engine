#pragma once

#include "utility.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>

namespace atom
{
	// Base atom class
	class c_atom
	{
	public:
		c_atom() = default;
		virtual ~c_atom() = default;

		// Basic atom methods
		virtual bool update();
		virtual bool go();

		// Get the vector of aspect types this atom implements directly
		virtual std::vector<uint32_t> get_aspect_types() const { return {}; }

		// Child management
		bool add_child(std::unique_ptr<c_atom> child);

		// Connection management - based on aspects
		// Add connection by atom with specific aspect
		template<typename AspectType>
		void add_connection(std::shared_ptr<c_atom> atom)
		{
			if (!atom || !atom->as<AspectType>())
			{
				return;
			}
			
			m_connections[AspectType::type()].push_back(std::weak_ptr<c_atom>(atom));
		}

		template<typename AspectType>
		std::vector<std::weak_ptr<c_atom>>& get_connections()
		{
			return m_connections[AspectType::type()];
		}

		// Parent management
		void set_parent(c_atom* parent) { m_parent = parent; }
		c_atom* get_parent() const { return m_parent; }

		template<typename AspectType>
		AspectType* as()
		{
			if (m_parent == nullptr)
			{
				return internal_as<AspectType>();
			}
			else if (auto res = m_parent->internal_as<AspectType>())
			{
				return res;
			}
			else
			{
				return internal_as<AspectType>();;
			}
		}

		template<typename AspectType>
		const AspectType* as() const
		{
			if (m_parent == nullptr)
			{
				return internal_as<AspectType>();
			}
			else if (auto res = m_parent->internal_as<AspectType>())
			{
				return res;
			}
			else
			{
				return internal_as<AspectType>();;
			}
		}

		// Aspect management
		template<typename AspectType>
		void register_aspect(c_atom* atom)
		{
			m_aspects[AspectType::type()] = atom;
		}

		void unregister_aspect(uint32_t aspect_type)
		{
			m_aspects.erase(aspect_type);
		}

		// Check if the atom has a specific aspect
		bool has(uint32_t aspect_type) const
		{
			return m_aspects.find(aspect_type) != m_aspects.end();
		}

	protected:
		template<typename AspectType>
		const AspectType* internal_as() const
		{
			auto it = m_aspects.find(AspectType::type());
			if (it != m_aspects.end())
			{
				return dynamic_cast<const AspectType*>(it->second);
			}

			return nullptr;
		}

		template<typename AspectType>
		AspectType* internal_as()
		{
			auto it = m_aspects.find(AspectType::type());
			if (it != m_aspects.end())
			{
				return dynamic_cast<AspectType*>(it->second);
			}

			return nullptr;
		}

	protected:
		std::vector<std::unique_ptr<c_atom>> m_children;
		std::unordered_map<uint32_t, std::vector<std::weak_ptr<c_atom>>> m_connections;

	private:
		std::unordered_map<uint32_t, c_atom*> m_aspects;
		c_atom* m_parent = nullptr;
	};
} // namespace atom