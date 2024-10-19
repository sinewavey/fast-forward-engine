#ifndef LUX_Q_COMPAT_H
#define LUX_Q_COMPAT_H

#include "../shared.h"

#include "scene/3d/camera_3d.h"

#include "scene/resources/environment.h"
#include "scene/3d/world_environment.h"
#include "scene/3d/lightmap_gi.h"

#include "scene/main/viewport.h"

class MapBuilder : public Node3D {
	GDCLASS(MapBuilder, Node3D);

protected:
	static void _bind_methods() {};
};

class Skybox : public SubViewport {
	GDCLASS(Skybox, SubViewport);

public:
	void	  set_camera(Camera3D* p_camera);
	Camera3D* get_camera() const;

	void clear_visuals();

protected:
	static void _bind_methods();

private:
	Camera3D* camera{ nullptr };
};

class Worldspawn : public WorldEnvironment {
	GDCLASS(Worldspawn, WorldEnvironment);

public:
	void apply_properties(const Dictionary& p_properties);
	void build_complete();

protected:
	static void _bind_methods();
};

class MapInstance : public Node {
	GDCLASS(MapInstance, Node);

public:
	void	set_skybox(Skybox* p_skybox);
	Skybox* get_skybox() const;

	void		set_worldspawn(Worldspawn* p_worldspawn);
	Worldspawn* get_worldspawn() const;

	void		set_lightmap(LightmapGI* p_lightmap);
	LightmapGI* get_lightmap() const;

	void set_map_builder(MapBuilder* p_map_builder);
	MapBuilder* get_map_builder() const;

	void	 set_execute_build(const Callable& p_callable) {};
	Callable get_execute_build() const { return Callable(this, "execute_build"); };

	void execute_build();

protected:
	static void _bind_methods();

private:
	NodePath map_builder{};
	ObjectID skybox{};
	ObjectID worldspawn{};
	ObjectID lightmap{};
};

#endif
