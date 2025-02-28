#pragma once

#include "utility.h"
#include "aspect.h"
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
		void add_connection(std::shared_ptr<i_aspect> connection);

		template<typename AspectType>
		std::vector<std::shared_ptr<c_atom>> get_connections()
		{
			auto& connections = m_connections[AspectType::type()];
			std::vector<std::shared_ptr<c_atom>> result;
			for (auto& connection : connections)
			{
				if (auto ptr = connection.lock()) {
					result.push_back(ptr);
				}
			}
			return result;
		}

		// Parent management
		void set_parent(c_atom* parent) { m_parent = parent; }
		c_atom* get_parent() const { return m_parent; }

		template<typename AspectType>
		AspectType* as()
		{
			auto it = m_aspects.find(AspectType::type());
			if (it != m_aspects.end())
			{
				return static_cast<AspectType*>(it->second);
			}

			return nullptr;
		}

		// Aspect management
		void register_aspect(i_aspect* aspect)
		{
			if (aspect) {
				m_aspects[aspect->get_aspect_type()] = aspect;
			}
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
		c_atom* m_parent = nullptr;
		std::vector<std::unique_ptr<c_atom>> m_children;
		std::unordered_map<uint32_t, std::vector<std::weak_ptr<i_aspect>>> m_connections;
		std::unordered_map<uint32_t, i_aspect*> m_aspects;
	};
} // namespace atom