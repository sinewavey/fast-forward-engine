#include "virtual_body_3d.h"

real_t VirtualBody3D::get_flag_time() const {
	return body_data.flag_time;
}

real_t VirtualBody3D::get_duck_time() const {
	return body_data.duck_time;
}

void VirtualBody3D::set_player_controlled(bool p_player_controlled) {
	if (p_player_controlled) {
		flags |= PlayerControlled;
	} else {
		flags &= ~PlayerControlled;
	}
}

bool VirtualBody3D::is_player_controlled() const {
	return flags & PlayerControlled;
}

void VirtualBody3D::set_camera(Camera3D* p_camera) {
	camera = Lux::get_id_or_null(p_camera);
}

Camera3D* VirtualBody3D::get_camera() const {
	return Object::cast_to<Camera3D>(ObjectDB::get_instance(camera));
}

void VirtualBody3D::set_view_angles(const Vector3& p_angles) {
	cam_data.view_angles = p_angles;
}

Vector3 VirtualBody3D::get_view_angles() const {
	return cam_data.view_angles;
}

void VirtualBody3D::set_crouch_collider(CollisionShape3D* p_shape) {
	crouch_collider = Lux::get_id_or_null(p_shape);
}

CollisionShape3D* VirtualBody3D::get_crouch_collider() const {
	return Object::cast_to<CollisionShape3D>(ObjectDB::get_instance(crouch_collider));
}

void VirtualBody3D::set_stand_collider(CollisionShape3D* p_shape) {
	stand_collider = Lux::get_id_or_null(p_shape);
}

void VirtualBody3D::set_crouch_shape(Ref<Shape3D> p_shape) {
	crouch_shape = p_shape;
}

Ref<Shape3D> VirtualBody3D::get_crouch_shape() const {
	return crouch_shape;
}

CollisionShape3D* VirtualBody3D::get_stand_collider() const {
	return Object::cast_to<CollisionShape3D>(ObjectDB::get_instance(stand_collider));
}

void VirtualBody3D::set_stand_shape(Ref<Shape3D> p_shape) {
	stand_shape = p_shape;
}

Ref<Shape3D> VirtualBody3D::get_stand_shape() const {
	return stand_shape;
}
