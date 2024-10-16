#ifndef LUX_IO_COUNTER_H
#define LUX_IO_COUNTER_H

#include "../shared.h"

class Counter : public Node3D {
	GDCLASS(Counter, Node3D);

public:
	void	   set_target(const StringName& p_target) { target = p_target; }
	StringName get_target() const { return target; }

	void set_value(int p_value) { value = p_value; }
	int	 get_value() const { return value; }

	void apply_properties(const Dictionary& p_properties);
	void build_complete();

	void use(Node* p_activator);

protected:
	static void _bind_methods();

private:
	StringName target{};
	int		   value = 0;
};

#endif
