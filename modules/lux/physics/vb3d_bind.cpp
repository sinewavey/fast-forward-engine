#include "virtual_body_3d.h"

void VirtualBody3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("move_and_slide", "delta"), &VirtualBody3D::move_and_slide);
    
    ClassDB::bind_method(D_METHOD("_func_godot_apply_properties", "properties"), &VirtualBody3D::apply_properties);
    ClassDB::bind_method(D_METHOD("_func_godot_build_complete"), &VirtualBody3D::build_complete);

	ClassDB::bind_method(D_METHOD("set_player_controlled", "player_controlled"),
		&VirtualBody3D::set_player_controlled);
	ClassDB::bind_method(D_METHOD("is_player_controlled"), &VirtualBody3D::is_player_controlled);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "player_controlled"),
		"set_player_controlled",
		"is_player_controlled");

	ClassDB::bind_method(D_METHOD("set_camera", "p_camera"), &VirtualBody3D::set_camera);
	ClassDB::bind_method(D_METHOD("get_camera"), &VirtualBody3D::get_camera);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "camera", PROPERTY_HINT_NODE_TYPE, "Camera3D"),
		"set_camera",
		"get_camera");

	ClassDB::bind_method(
		D_METHOD("set_crouch_collider", "crouch_collider"), &VirtualBody3D::set_crouch_collider);
	ClassDB::bind_method(D_METHOD("get_crouch_collider"), &VirtualBody3D::get_crouch_collider);
	ADD_PROPERTY(
		PropertyInfo(
			Variant::OBJECT, "crouch_collider", PROPERTY_HINT_NODE_TYPE, "CollisionShape3D"),
		"set_crouch_collider",
		"get_crouch_collider");

	ClassDB::bind_method(
		D_METHOD("set_crouch_shape", "crouch_shape"), &VirtualBody3D::set_crouch_shape);
	ClassDB::bind_method(D_METHOD("get_crouch_shape"), &VirtualBody3D::get_crouch_shape);
	ADD_PROPERTY(
		PropertyInfo(Variant::OBJECT, "crouch_shape", PROPERTY_HINT_RESOURCE_TYPE, "Shape3D"),
		"set_crouch_shape",
		"get_crouch_shape");

	ClassDB::bind_method(
		D_METHOD("set_stand_collider", "stand_collider"), &VirtualBody3D::set_stand_collider);
	ClassDB::bind_method(D_METHOD("get_stand_collider"), &VirtualBody3D::get_stand_collider);
	ADD_PROPERTY(
		PropertyInfo(
			Variant::OBJECT, "stand_collider", PROPERTY_HINT_NODE_TYPE, "CollisionShape3D"),
		"set_stand_collider",
		"get_stand_collider");

	ClassDB::bind_method(
		D_METHOD("set_stand_shape", "stand_shape"), &VirtualBody3D::set_stand_shape);
	ClassDB::bind_method(D_METHOD("get_stand_shape"), &VirtualBody3D::get_stand_shape);
	ADD_PROPERTY(
		PropertyInfo(Variant::OBJECT, "stand_shape", PROPERTY_HINT_RESOURCE_TYPE, "Shape3D"),
		"set_stand_shape",
		"get_stand_shape");
}
