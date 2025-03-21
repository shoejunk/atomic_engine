# Concise Overview of the Aspect System

## Core Concept
The aspect system is a capability-focused component architecture that replaces inheritance hierarchies with interface-based composition. Components explicitly declare what they can do rather than what they are.

## Key Components

1. **Aspect Interfaces**
   - Pure virtual interfaces that define specific capabilities
   - Each has a unique type identifier (`uint32_t` hash)
   - Examples: `i_movable`, `i_drawable`, `i_action_handler`

2. **Aspect Registration**
   - Components register capabilities in their constructor
   - Each component explicitly reports its aspects via `get_aspect_types()`
   - Aspects are stored in a map keyed by type hash

3. **Capability Flow**
   - When a child is added to a parent, its aspects flow upward
   - Parent gains access to all aspects implemented by its direct children
   - Aspect conflicts (same aspect from multiple sources) are prevented

4. **Capability Access**
   - Components query for capabilities with `as<T>()`
   - Access is direct with no runtime search overhead
   - Example: `if (auto movable = atom->as<i_movable>()) { movable->move(x, y); }`

5. **Connection System**
   - Components can be connected by aspect type
   - Example: Window connects to all components with the drawable aspect
   - Connections are weak references that don't affect ownership

## Usage Patterns

1. **Direct Implementation**
   ```cpp
   class c_sprite : public c_atom, public i_drawable, public i_movable {...}
   ```

2. **Capability Composition**
   ```cpp
   class c_entity : public c_atom
   {
     c_entity()
	 {
       add_child(std::make_unique<c_movement_component>()); // Adds movable aspect
       add_child(std::make_unique<c_renderer_component>()); // Adds drawable aspect
     }
   }
   ```

3. **Capability Delegation**
   ```cpp
   class c_player : public c_atom, public i_movable
   {
     void move(float dx, float dy) override { m_sprite->move(dx, dy); }
   }
   ```

The aspect system combines the clarity of interfaces with the flexibility of composition, creating a predictable, capability-based architecture without the limitations of inheritance hierarchies.