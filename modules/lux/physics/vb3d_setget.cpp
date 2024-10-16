#include "virtual_body_3d.h"

real_t VirtualBody3D::get_flag_time() const {
	return model.flag_time;
}

real_t VirtualBody3D::get_duck_time() const {
	return model.duck_time;
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
	if (p_camera == nullptr) {
		camera = ObjectID();
		return;
	}
	camera = p_camera->get_instance_id();
}

Camera3D* VirtualBody3D::get_camera() const {
	return Object::cast_to<Camera3D>(ObjectDB::get_instance(camera));
}

void VirtualBody3D::set_view_angles(const Vector3& p_angles) {
	model.view_angles = p_angles;
}

Vector3 VirtualBody3D::get_view_angles() const {
	return model.view_angles;
}

void VirtualBody3D::set_crouch_collider(CollisionShape3D* p_shape) {
	if (p_shape == nullptr) {
		model.crouch_collider = ObjectID();
		return;
	}
	model.crouch_collider = p_shape->get_instance_id();
}

CollisionShape3D* VirtualBody3D::get_crouch_collider() const {
	return Object::cast_to<CollisionShape3D>(ObjectDB::get_instance(model.crouch_collider));
}

void VirtualBody3D::set_stand_collider(CollisionShape3D* p_shape) {
	if (p_shape == nullptr) {
		model.stand_collider = ObjectID();
		return;
	}
	model.stand_collider = p_shape->get_instance_id();
}

void VirtualBody3D::set_crouch_shape(Ref<Shape3D> p_shape) {
	if (model.crouch_shape == p_shape) {
		return;
	}
	model.crouch_shape = p_shape;
}

Ref<Shape3D> VirtualBody3D::get_crouch_shape() const {
	return model.crouch_shape;
}

CollisionShape3D* VirtualBody3D::get_stand_collider() const {
	return Object::cast_to<CollisionShape3D>(ObjectDB::get_instance(model.stand_collider));
}

void VirtualBody3D::set_stand_shape(Ref<Shape3D> p_shape) {
	if (model.stand_shape == p_shape) {
		return;
	}
	model.stand_shape = p_shape;
}

Ref<Shape3D> VirtualBody3D::get_stand_shape() const {
	return model.stand_shape;
}
