#include "platform_body_3d.h"
#include "scene/3d/path_3d.h"
#include "scene/3d/remote_transform_3d.h"

void PlatformBody3D::_bind_methods() {
	GDVIRTUAL_BIND(_use, "activator");

	ClassDB::bind_method(D_METHOD("set_remote_transform", "p_remote_transform"),
		&PlatformBody3D::set_remote_transform);
	ClassDB::bind_method(D_METHOD("get_remote_transform"), &PlatformBody3D::get_remote_transform);
	ADD_PROPERTY(
		PropertyInfo(
			Variant::OBJECT, "remote_transform", PROPERTY_HINT_NODE_TYPE, "RemoteTransform3D"),
		"set_remote_transform",
		"get_remote_transform");

	ClassDB::bind_method(D_METHOD("set_path_3d", "p_path_3d"), &PlatformBody3D::set_path_3d);
	ClassDB::bind_method(D_METHOD("get_path_3d"), &PlatformBody3D::get_path_3d);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "path_3d", PROPERTY_HINT_NODE_TYPE, "Path3D"),
		"set_path_3d",
		"get_path_3d");

	ClassDB::bind_method(
		D_METHOD("set_path_follower", "p_path_follower"), &PlatformBody3D::set_path_follower);
	ClassDB::bind_method(D_METHOD("get_path_follower"), &PlatformBody3D::get_path_follower);
	ADD_PROPERTY(
		PropertyInfo(Variant::OBJECT, "path_follower", PROPERTY_HINT_NODE_TYPE, "PathFollow3D"),
		"set_path_follower",
		"get_path_follower");

	ClassDB::bind_method(
		D_METHOD("set_surface_flags", "p_flags"), &PlatformBody3D::set_surface_flags);
	ClassDB::bind_method(D_METHOD("get_surface_flags"), &PlatformBody3D::get_surface_flags);

	ClassDB::bind_method(
		D_METHOD(LUX_APPLY_PROPERTIES, "properties"), &PlatformBody3D::apply_properties);
	ClassDB::bind_method(D_METHOD(LUX_BUILD_COMPLETE), &PlatformBody3D::build_complete);

	ClassDB::bind_method(D_METHOD("use", "activator"), &PlatformBody3D::use);
	ClassDB::bind_method(D_METHOD("toggle"), &PlatformBody3D::toggle);

	ClassDB::bind_method(D_METHOD("move_forward"), &PlatformBody3D::move_forward);
	ClassDB::bind_method(D_METHOD("move_reverse"), &PlatformBody3D::move_reverse);

	ClassDB::bind_method(D_METHOD("move_to_move_to_progress", "move_to_progress"),
		&PlatformBody3D::move_to_progress);
}

PlatformBody3D::PlatformBody3D() {
}

void PlatformBody3D::set_path_3d(Path3D* p_node) {
	path_3d = Lux::get_id_or_null(p_node);
}

Path3D* PlatformBody3D::get_path_3d() const {
	return Lux::instance_from_id<Path3D>(path_3d);
}

void PlatformBody3D::set_path_follower(PathFollow3D* p_node) {
	path_follower = Lux::get_id_or_null(p_node);
}

PathFollow3D* PlatformBody3D::get_path_follower() const {
	return Lux::instance_from_id<PathFollow3D>(path_follower);
}

void PlatformBody3D::set_remote_transform(RemoteTransform3D* p_node) {
	remote_transform = Lux::get_id_or_null(p_node);
}

RemoteTransform3D* PlatformBody3D::get_remote_transform() const {
	return Lux::instance_from_id<RemoteTransform3D>(remote_transform);
}

void PlatformBody3D::set_surface_flags(uint32_t p_flags) {
	surface_flags = p_flags;
}

uint32_t PlatformBody3D::get_surface_flags() const {
	return surface_flags;
}

void PlatformBody3D::apply_properties(const Dictionary& p_properties) {
	Lux::FGD::apply_properties(this, p_properties);


	auto p				  = memnew(Path3D);
	auto pf				  = memnew(PathFollow3D);
	auto remote_transform = memnew(RemoteTransform3D);

	set_process_mode(PROCESS_MODE_PAUSABLE);
	pf->set_process_mode(PROCESS_MODE_PAUSABLE);

	p->set_position(get_position());

	// TODO: set according to spawnflags
	pf->set_rotation_mode(PathFollow3D::RotationMode::ROTATION_NONE);
	pf->set_tilt_enabled(false);
	pf->set_loop(false);

	Lux::add_sibling_persist(get_parent(), p, true);
	Lux::add_child_persist(p, pf, true);
	Lux::add_child_persist(pf, remote_transform, true);

	remote_transform->set_remote_node(get_path());

	set_path_3d(p);
	set_path_follower(pf);
}

void PlatformBody3D::build_complete() {
	Lux::FGD::finalize_entity(this);
}

void PlatformBody3D::use(Node* p_activator) {
	print_line(GDVIRTUAL_IS_OVERRIDDEN(_use) ? "PlatformBody3D::use (VIRTUAL CALL)"
											 : "PlatformBody3D::use");
	if (GDVIRTUAL_IS_OVERRIDDEN(_use)) {
		GDVIRTUAL_CALL(_use, p_activator);
	} else {
		toggle();
	}
}

void PlatformBody3D::move_forward() {
}
void PlatformBody3D::move_reverse() {
}

void PlatformBody3D::toggle() {
}

void PlatformBody3D::move_to_progress(float p_progress) {
}

void PlatformBody3D::_notification(int p_what) {
}
