#include "q_compat.h"

void MapInstance::_bind_methods() {
	ClassDB::bind_method(
		D_METHOD("set_execute_build", "p_execute_build"), &MapInstance::set_execute_build);
	ClassDB::bind_method(D_METHOD("get_execute_build"), &MapInstance::get_execute_build);
	ADD_PROPERTY(
		PropertyInfo(Variant::CALLABLE, "execute_build", PROPERTY_HINT_TOOL_BUTTON, "Build"),
		"set_execute_build",
		"get_execute_build");

	ClassDB::bind_method(D_METHOD("set_skybox", "p_skybox"), &MapInstance::set_skybox);
	ClassDB::bind_method(D_METHOD("get_skybox"), &MapInstance::get_skybox);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "skybox", PROPERTY_HINT_NODE_TYPE, "Skybox"),
		"set_skybox",
		"get_skybox");

	ClassDB::bind_method(D_METHOD("set_worldspawn", "p_worldspawn"), &MapInstance::set_worldspawn);
	ClassDB::bind_method(D_METHOD("get_worldspawn"), &MapInstance::get_worldspawn);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "worldspawn", PROPERTY_HINT_NODE_TYPE, "worldspawn"),
		"set_worldspawn",
		"get_worldspawn");

	ClassDB::bind_method(D_METHOD("set_lightmap", "p_lightmap"), &MapInstance::set_lightmap);
	ClassDB::bind_method(D_METHOD("get_lightmap"), &MapInstance::get_lightmap);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "lightmap", PROPERTY_HINT_NODE_TYPE, "LightmapGI"),
		"set_lightmap",
		"get_lightmap");

	ClassDB::bind_method(D_METHOD("set_map_builder", "p_map_builder"), &MapInstance::set_lightmap);
	ClassDB::bind_method(D_METHOD("get_map_builder"), &MapInstance::get_lightmap);
	ADD_PROPERTY(
		PropertyInfo(Variant::OBJECT, "map_builder", PROPERTY_HINT_NODE_TYPE, "MapBuilder"),
		"set_map_builder",
		"get_map_builder");
}

void MapInstance::set_skybox(Skybox* p_skybox) {
	skybox = Lux::get_id_or_null(p_skybox);
}

Skybox* MapInstance::get_skybox() const {
	return Object::cast_to<Skybox>(ObjectDB::get_instance(skybox));
}

void MapInstance::set_worldspawn(Worldspawn* p_worldspawn) {
	worldspawn = Lux::get_id_or_null(p_worldspawn);
}

Worldspawn* MapInstance::get_worldspawn() const {
	return Object::cast_to<Worldspawn>(ObjectDB::get_instance(worldspawn));
}

void MapInstance::set_lightmap(LightmapGI* p_lightmap) {
	lightmap = Lux::get_id_or_null(p_lightmap);
}

LightmapGI* MapInstance::get_lightmap() const {
	return Object::cast_to<LightmapGI>(ObjectDB::get_instance(lightmap));
}

void MapInstance::set_map_builder(MapBuilder* p_builder) {
	map_builder = Lux::get_path_or_null(p_builder);
}

MapBuilder* MapInstance::get_map_builder() const {
	return Object::cast_to<MapBuilder>(get_node_or_null(map_builder));
}

void MapInstance::execute_build() {
	auto mb{ get_map_builder() };
	if (mb == nullptr) {
		return;
	}
	mb->call("verify_and_build");
}
