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
	Lux::apply_fgd_properties(this, p_properties);

	if (p_properties.has("surface_flags")) {
		set_surface_flags(p_properties.get("surface_flags", uint32_t()));
	}
}

void WorldGeometry::build_complete() {
	List<StringName> metadata{};
	get_meta_list(&metadata);

	for (int i = 0; i < get_child_count(); i++) {
		auto child = get_child(i);

		// mesh properties
		if (auto geometry_instance = Object::cast_to<GeometryInstance3D>(child);
			geometry_instance != nullptr) {
			for (auto meta : metadata) {
				if (meta == "_cs") {
					int value = get_meta("_cs", 1);
					geometry_instance->set_cast_shadows_setting(
						GeometryInstance3D::ShadowCastingSetting(value));
				}
				if (meta == "_rs") {
					auto mi = Object::cast_to<MeshInstance3D>(child);

					if (mi != nullptr) {
						int value = get_meta("_rs", true);
					}
				}
			}
		}
	}
}

void WorldGeometry::set_surface_flags(uint32_t p_flags) {
	surface_flags = p_flags;
}

uint32_t WorldGeometry::get_surface_flags() const {
	return surface_flags;
}
