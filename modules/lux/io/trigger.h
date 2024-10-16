#ifndef LUX_IO_TRIGGER_H
#define LUX_IO_TRIGGER_H

#include "../shared.h"
#include "scene/3d/physics/area_3d.h"

class Trigger : public Area3D {
	GDCLASS(Trigger, Area3D);

public:
	void	   set_target(const StringName& p_target) { target = p_target; }
	StringName get_target() const { return target; }

	void set_player_usable(bool p_usable) { player_usable = p_usable; }
	bool is_player_usable() const { return player_usable; }

	void apply_properties(const Dictionary& p_properties);
	void build_complete();

	void body_entered(Node3D* p_body);
	void body_exited(Node3D* p_body);

	void use(Node* p_activator);

	Trigger();

protected:
	static void _bind_methods();

private:
	StringName target{};
	bool	   player_usable{ false };
};

#endif
