#ifndef LUX_WORLD_GEOMETRY_H
#define LUX_WORLD_GEOMETRY_H

#include "../shared.h"
#include "scene/3d/physics/static_body_3d.h"

class WorldGeometry : public StaticBody3D {
	GDCLASS(WorldGeometry, StaticBody3D);

public:
	WorldGeometry();

	void						set_surface_flags(BitField<Lux::SurfaceFlag> p_flags);
	BitField<Lux::SurfaceFlag> get_surface_flags() const;

	void apply_properties(const Dictionary& p_properties);
	void build_complete();

	BitField<Lux::SurfaceFlag> surface_flags{};

protected:
	static void _bind_methods();
};

#endif
