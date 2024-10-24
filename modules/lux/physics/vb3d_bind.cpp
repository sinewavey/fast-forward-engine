#include "virtual_body_3d.h"

void VirtualBody3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("slide_move", "delta"), &VirtualBody3D::slide_move);
	ClassDB::bind_method(D_METHOD("step_slide_move", "delta"), &VirtualBody3D::step_slide_move);

	ClassDB::bind_method(
		D_METHOD(LUX_APPLY_PROPERTIES, "properties"), &VirtualBody3D::apply_properties);
	ClassDB::bind_method(D_METHOD(LUX_BUILD_COMPLETE), &VirtualBody3D::build_complete);

	ClassDB::bind_method(D_METHOD("push", "force", "dir", "mass"), &VirtualBody3D::push);

	ClassDB::bind_method(D_METHOD("set_msens", "msens"), &VirtualBody3D::set_msens);
	ClassDB::bind_method(D_METHOD("set_player_controlled", "player_controlled"),
		&VirtualBody3D::set_player_controlled);
	ClassDB::bind_method(D_METHOD("set_camera", "p_camera"), &VirtualBody3D::set_camera);
	ClassDB::bind_method(D_METHOD("set_raycast", "p_raycast"), &VirtualBody3D::set_raycast);
	ClassDB::bind_method(D_METHOD("set_duck_timer", "p_timer"), &VirtualBody3D::set_duck_timer);
	ClassDB::bind_method(
		D_METHOD("set_crouch_collider", "crouch_collider"), &VirtualBody3D::set_crouch_collider);
	ClassDB::bind_method(
		D_METHOD("set_stand_collider", "stand_collider"), &VirtualBody3D::set_stand_collider);

	ClassDB::bind_method(D_METHOD("is_player_controlled"), &VirtualBody3D::is_player_controlled);
	ClassDB::bind_method(D_METHOD("get_msens"), &VirtualBody3D::get_msens);

	ClassDB::bind_method(D_METHOD("get_camera"), &VirtualBody3D::get_camera);
	ClassDB::bind_method(D_METHOD("get_raycast"), &VirtualBody3D::get_raycast);
	ClassDB::bind_method(D_METHOD("get_duck_timer"), &VirtualBody3D::get_duck_timer);

	ClassDB::bind_method(D_METHOD("get_crouch_collider"), &VirtualBody3D::get_crouch_collider);
	ClassDB::bind_method(D_METHOD("get_stand_collider"), &VirtualBody3D::get_stand_collider);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "player_controlled"),
		"set_player_controlled",
		"is_player_controlled");

	ADD_PROPERTY(
		PropertyInfo(Variant::FLOAT, "msens", PROPERTY_HINT_RANGE, "0.0,250.0,0.1,or_greater"),
		"set_msens",
		"get_msens");

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "camera", PROPERTY_HINT_NODE_TYPE, "Camera3D"),
		"set_camera",
		"get_camera");

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "raycast", PROPERTY_HINT_NODE_TYPE, "RayCast3D"),
		"set_raycast",
		"get_raycast");

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "duck_timer", PROPERTY_HINT_NODE_TYPE, "Timer"),
		"set_duck_timer",
		"get_duck_timer");

	ADD_PROPERTY(
		PropertyInfo(
			Variant::OBJECT, "crouch_collider", PROPERTY_HINT_NODE_TYPE, "CollisionShape3D"),
		"set_crouch_collider",
		"get_crouch_collider");

	ADD_PROPERTY(
		PropertyInfo(
			Variant::OBJECT, "stand_collider", PROPERTY_HINT_NODE_TYPE, "CollisionShape3D"),
		"set_stand_collider",
		"get_stand_collider");
}
