#include "world_geometry.h"

#include "scene/3d/mesh_instance_3d.h"

WorldGeometry::WorldGeometry() {
}

void WorldGeometry::_bind_methods() {
	ClassDB::bind_method(
		D_METHOD("_func_godot_apply_properties", "properties"), &WorldGeometry::apply_properties);
	ClassDB::bind_method(D_METHOD("_func_godot_build_complete"), &WorldGeometry::build_complete);

	ClassDB::bind_method(
		D_METHOD("set_surface_flags", "p_surface_flags"), &WorldGeometry::set_surface_flags);
	ClassDB::bind_method(D_METHOD("get_surface_flags"), &WorldGeometry::get_surface_flags);
	ADD_PROPERTY(
		PropertyInfo(Variant::INT, "surface_flags"), "set_surface_flags", "get_surface_flags");
}

void WorldGeometry::apply_properties(const Dictionary& p_properties) {
	Lux::FGD::apply_properties(this, p_properties);
}

void WorldGeometry::build_complete() {
	Lux::FGD::finalize_entity(this);
}

void WorldGeometry::set_surface_flags(uint32_t p_flags) {
	surface_flags = p_flags;
}

uint32_t WorldGeometry::get_surface_flags() const {
	return surface_flags;
}
