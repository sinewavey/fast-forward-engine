#include "virtual_body_3d.h"

// Input

void VirtualBody3D::set_msens(real_t p_msens) {
	msens = p_msens;
};

real_t VirtualBody3D::get_msens() const {
	return msens;
};

void VirtualBody3D::set_player_controlled(bool p_player_controlled) {
	if (p_player_controlled) {
		flags |= PlayerControlled;
	} else {
		flags &= ~PlayerControlled;
	}
}

// clang-format off
bool VirtualBody3D::is_player_controlled() const { return flags & PlayerControlled; }
void VirtualBody3D::set_view_angles(const Vector3& p_angles) { view.angles = p_angles; }
Vector3 VirtualBody3D::get_view_angles() const { return view.angles; }
// clang-format on
// Nodes

void VirtualBody3D::set_camera(Camera3D* p_camera) {
	camera = Lux::get_id_or_null(p_camera);
}

Camera3D* VirtualBody3D::get_camera() const {
	return Object::cast_to<Camera3D>(ObjectDB::get_instance(camera));
}

void VirtualBody3D::set_duck_timer(Timer* p_timer) {
	duck_timer = Lux::get_id_or_null(p_timer);
}

Timer* VirtualBody3D::get_duck_timer() const {
	return Object::cast_to<Timer>(ObjectDB::get_instance(duck_timer));
}

void VirtualBody3D::set_crouch_collider(CollisionShape3D* p_shape) {
	crouch_collider = Lux::get_id_or_null(p_shape);
	crouch_shape	= (p_shape != nullptr ? p_shape->get_shape() : nullptr);
}

CollisionShape3D* VirtualBody3D::get_crouch_collider() const {
	return Object::cast_to<CollisionShape3D>(ObjectDB::get_instance(crouch_collider));
}

void VirtualBody3D::set_stand_collider(CollisionShape3D* p_shape) {
	stand_collider = Lux::get_id_or_null(p_shape);
	stand_shape	   = (p_shape != nullptr ? p_shape->get_shape() : nullptr);
}

CollisionShape3D* VirtualBody3D::get_stand_collider() const {
	return Object::cast_to<CollisionShape3D>(ObjectDB::get_instance(stand_collider));
}

// Misc

real_t VirtualBody3D::get_flag_time() const {
	return phys.flag_time;
}

real_t VirtualBody3D::get_duck_time() const {
	return phys.duck_time;
}
