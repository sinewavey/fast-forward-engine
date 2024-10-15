#ifndef VIRTUAL_BODY_3D_H
#define VIRTUAL_BODY_3D_H

#include "scene/3d/physics/kinematic_collision_3d.h"
#include "scene/3d/physics/physics_body_3d.h"

class VirtualBody3D : public PhysicsBody3D {
	GDCLASS(VirtualBody3D, PhysicsBody3D);

public:
	VirtualBody3D();

	void _physics_process(real_t p_delta) {
		if (Engine::get_singleton()->is_editor_hint()) {
			return;
		}
		print_line("test");
	}

protected:
	static void _bind_methods();
};

#endif