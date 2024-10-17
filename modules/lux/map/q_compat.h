#ifndef LUX_Q_COMPAT_H
#define LUX_Q_COMPAT_H

#include "../shared.h"

#include "scene/resources/environment.h"
#include "scene/3d/lightmap_gi.h"

#include "scene/3d/camera_3d.h"
#include "scene/3d/world_environment.h"
#include "scene/main/viewport.h"

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

	void build_skybox();

protected:
	static void _bind_methods();

private:
	LightmapGI* lightmap_gi{ nullptr };
    Skybox* skybox{ nullptr };
};

#endif
