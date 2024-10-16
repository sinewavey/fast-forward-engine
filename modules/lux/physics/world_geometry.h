#ifndef LUX_WORLD_GEOMETRY_H
#define LUX_WORLD_GEOMETRY_H

#include "../shared.h"
#include "scene/3d/physics/static_body_3d.h"

class WorldGeometry : public StaticBody3D {
	GDCLASS(WorldGeometry, StaticBody3D);

public:
	WorldGeometry();

	void	 set_surface_flags(uint32_t p_flags);
	uint32_t get_surface_flags() const;

	void apply_properties(const Dictionary& p_properties);
	void build_complete();

	uint32_t surface_flags{};

protected:
	static void _bind_methods();
};

#endif
