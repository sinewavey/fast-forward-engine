#ifndef LUX_SHARED_H
#define LUX_SHARED_H

#include "core/math/math_funcs.h"

#include "core/object/object.h"
#include "core/object/object_id.h"
#include "core/object/ref_counted.h"

#include "core/io/resource.h"
#include "core/io/resource_uid.h"

#include "core/io/resource_loader.h"
#include "core/io/resource_saver.h"

#include "core/math/math_funcs.h"

#include "scene/main/node.h"
#include "scene/3d/node_3d.h"
#include "scene/resources/3d/shape_3d.h"
#include "scene/3d/physics/kinematic_collision_3d.h"
#include "scene/3d/physics/physics_body_3d.h"
#include "scene/3d/physics/collision_shape_3d.h"

namespace Lux {

enum SurfaceFlag {
	SURFACE_FLAG_DEFAULT   = 0,
	SURFACE_FLAG_SLICK	   = 1,
	SURFACE_FLAG_FORCE_AIR = 1 << 1,
	SURFACE_FLAG_BOUNCE	   = 1 << 2,
	SURFACE_FLAG_FLESH	   = 1 << 3,
	SURFACE_FLAG_NODROPS   = 1 << 4,
};

static void apply_common_properties(Node* p_node, const Dictionary& p_properties) {
	if (p_properties.has("targetname")) {
		StringName tn = p_properties["targetname"];
		if (!tn.is_empty()) {
			p_node->add_to_group(p_properties["targetname"], true);
		}
	}
	if (p_properties.has("target")) {
		String target = p_properties["target"];
		if (!target.is_empty()) {
			p_node->set_meta("target", target);
		}
	};
}
} // namespace Lux

#endif
