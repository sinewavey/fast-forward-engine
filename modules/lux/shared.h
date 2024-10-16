#ifndef LUX_SHARED_H
#define LUX_SHARED_H

#include <type_traits>

#include "core/math/math_funcs.h"

#include "core/object/object.h"
#include "core/object/object_id.h"
#include "core/object/ref_counted.h"
#include "core/io/resource.h"
#include "core/io/resource_uid.h"

#include "core/io/resource_loader.h"
#include "core/io/resource_saver.h"

#include "scene/main/node.h"
#include "scene/3d/node_3d.h"

#include "scene/resources/3d/shape_3d.h"

#include "scene/3d/physics/kinematic_collision_3d.h"
#include "scene/3d/physics/physics_body_3d.h"
#include "scene/3d/physics/collision_shape_3d.h"

#include "scene/3d/visual_instance_3d.h"

namespace Lux {

enum SurfaceFlag {
	SURFACE_FLAG_DEFAULT   = 0,
	SURFACE_FLAG_SLICK	   = 1,
	SURFACE_FLAG_FORCE_AIR = 1 << 1,
	SURFACE_FLAG_BOUNCE	   = 1 << 2,
	SURFACE_FLAG_FLESH	   = 1 << 3,
	SURFACE_FLAG_NODROPS   = 1 << 4,
};

_FORCE_INLINE_ static Vector3
clip_velocity(const Vector3& p_velocity, const Vector3& p_normal, real_t backoff = 1.0) {
	real_t bump = p_velocity.dot(p_normal);
	bump < 0.0 ? bump *= backoff : bump /= backoff;
	return (p_velocity - p_normal * bump);
}

static void use_target(Node* p_activator, StringName p_target) {
	List<Node*> nodes;
	(p_activator->get_tree())->get_nodes_in_group(p_target, &nodes);
	for (auto node : nodes) {
		if (node->has_method("use")) {
			node->call("use", p_activator);
		}
	}
}

static void apply_fgd_properties(Node* p_node, const Dictionary& p_properties) {
	for (Variant key : p_properties.keys()) {
		auto kstr = StringName(key);

		p_node->set(key, p_properties[key]);

		if (kstr == "targetname") {
			StringName tn = p_properties["targetname"];
			if (!tn.is_empty()) {
				p_node->add_to_group(p_properties["targetname"], true);
			}
			continue;
		}
		if (kstr == "target") {
			String target = p_properties["target"];
			if (!target.is_empty()) {
				p_node->set_meta("target", target);
			}
			continue;
		};

		if (kstr == "_cs") {
			p_node->set_meta("_cs",
				p_properties.get(
					"_cs", GeometryInstance3D::ShadowCastingSetting::SHADOW_CASTING_SETTING_ON));
			continue;
		}

		if (kstr == "_rs") {
			p_node->set_meta("_rs", p_properties.get("_rs", true));
			continue;
		}

		if (kstr == "ignore_occlusion_culling") {
			p_node->set_meta(
				"ignore_occlusion_culling", p_properties.get("ignore_occlusion_culling", false));
			continue;
		}
	}
}

} // namespace Lux

#endif
