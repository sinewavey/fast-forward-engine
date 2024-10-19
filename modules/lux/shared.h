#ifndef LUX_SHARED_H
#define LUX_SHARED_H

#include <type_traits>
#include <ranges>

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

#include "scene/resources/3d/box_shape_3d.h"
#include "scene/resources/3d/sphere_shape_3d.h"
#include "scene/resources/3d/convex_polygon_shape_3d.h"
#include "scene/resources/3d/concave_polygon_shape_3d.h"

#include "scene/3d/physics/kinematic_collision_3d.h"
#include "scene/3d/physics/physics_body_3d.h"
#include "scene/3d/physics/collision_shape_3d.h"

#include "scene/3d/visual_instance_3d.h"

namespace Lux {

enum SurfaceType : uint8_t {
	SURFACE_SLICK	  = 1,
	SURFACE_FORCE_AIR = 1 << 1,
	SURFACE_BOUNCE	  = 1 << 2,
	SURFACE_FLESH	  = 1 << 3,
	SURFACE_NODROPS	  = 1 << 4,
};

enum TriggerFlag : uint8_t {
	TRIGGER_ONCE		= 1,
	TRIGGER_TOGGLE		= 1 << 1,
	TRIGGER_ON_ENTRY	= 1 << 2,
	TRIGGER_ON_EXIT		= 1 << 3,
	TRIGGER_USABLE		= 1 << 4,
	TRIGGER_PLAYER_ONLY = 1 << 5,
};

enum TriggerState {
	TRIGGER_READY,
	TRIGGER_USED,
	TRIGGER_WAIT,
};

enum LockFlag : uint8_t {
	LOCK_OPEN	= 0,
	LOCK_GREEN	= 1,
	LOCK_ORANGE = 1 << 1,
	LOCK_PURPLE = 1 << 2,
};

inline static const Vector3 vec3_right{ 1.0f, 0.0f, 0.0f };
inline static const Vector3 vec3_up{ 0.0f, 1.0f, 0.0f };
inline static const Vector3 vec3_fwd{ 0.0f, 0.0f, -1.0f };

static constexpr real_t m_pi{ Math_PI };
static constexpr real_t m_eta{ Math_PI * 0.5 };

namespace Concepts {

template <typename T>
concept node_derived = std::is_base_of_v<Node, T>;

} // namespace Concepts

template <Concepts::node_derived T>
static auto child_view(T* p_node) {
	return std::views::iota(0, p_node->get_child_count()) |
		std::views::transform([p_node](int i) -> Node* { return p_node->get_child(i); });
}

static Vector3 clip(const Vector3& p_in, const Vector3& p_normal, real_t p_adj = 1.0f) {
	real_t bump = p_in.dot(p_normal);
	if (bump < 0.0) {
		bump *= p_adj;
	} else {
		bump /= p_adj;
	}
	return (p_in - (p_normal * bump));
}

static ObjectID get_id_or_null(Node* p_node) {
	return p_node ? p_node->get_instance_id() : ObjectID();
}

static NodePath get_path_or_null(Node* p_node) {
	return p_node ? p_node->get_path() : NodePath();
}

template <typename T>
static T* instance_from_id(ObjectID p_id) {
	return Object::cast_to<T>(ObjectDB::get_instance(p_id));
}

template <typename T>
static T* instance_from_path(Node* p_from, NodePath p_path) {
	return Object::cast_to<T>(p_from->get_node_or_null(p_path));
}

namespace IO {

static void
use_target(StringName p_target, StringName p_func = "use", Node* p_activator = nullptr) {
	List<Node*> nodes;
	SceneTree::get_singleton()->get_nodes_in_group(p_target, &nodes);
	for (auto node : nodes) {
		if (node->has_method(p_func)) {
			node->call(p_func, p_activator);
		}
	}
}

static void use_target(StringName p_target, StringName p_func, Array p_args = {}) {
	List<Node*> nodes;
	SceneTree::get_singleton()->get_nodes_in_group(p_target, &nodes);
	for (auto node : nodes) {
		if (node->has_method(p_func)) {
			node->callv(p_func, p_args);
		}
	}
}

} // namespace IO

namespace FGD {

static void apply_properties(Node* p_node, const Dictionary& p_properties) {
	for (Variant key : p_properties.keys()) {
		if (!key.is_string()) {
			continue;
		}

		if (key.operator==("classname") || key.operator==("origin")) {
			continue;
		}

		auto value	 = p_properties[key];
		bool success = false;

		p_node->set(key, value, &success);

		// if (success) {
		// print_line(p_node->get_name(), "set:", key, value);
		// } else {
		if (!success) {
			p_node->set("metadata/" + key.stringify(), value, &success);
			// if (success) {
			// print_line(p_node->get_name(), "set key metadata:", key, value);
			// } else
			// print_line(p_node->get_name(), "could not set key:", key);
		}

		if (key.operator==("targetname")) {
			StringName target_name{ p_properties.get("targetname", "") };
			if (!target_name.is_empty()) {
				p_node->add_to_group(target_name, true);
			}
			continue;
		}
	}
}

static void finalize_mesh_children(Node* p_parent) {
	int cs_value = p_parent->get_meta("_cs", 1);

	for (int i = 0; i < p_parent->get_child_count(); i++) {
		auto child = p_parent->get_child(i);
		if (Object::cast_to<GeometryInstance3D>(child) != nullptr) {
			child->call(
				"set_cast_shadows_setting", GeometryInstance3D::ShadowCastingSetting(cs_value));
		}
	}
}

static void finalize_entity(Node* p_node) {
	List<StringName> md{};
	p_node->get_meta_list(&md);

	finalize_mesh_children(p_node);

	// for (auto meta : md) {
	// 	p_node->remove_meta(meta);
	// }
}

} // namespace FGD

} // namespace Lux

#endif
