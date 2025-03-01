#pragma once

class c_action : public t_atom<c_action, c_input>
{
public:
	static constexpr uint32_t kind_id() { return "action"_h; }

	// Constructor with action name
	c_action(const std::string& name);

	// Add a handler for this action
	void add_handler(std::shared_ptr<c_action_handler> handler);

	// Trigger this action
	void trigger();

	// Get the action name
	const std::string& get_name() const;

private:
	std::string m_name;
	std::vector<std::weak_ptr<c_action_handler>> m_handlers;
};
