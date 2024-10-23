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
#include "scene/3d/mesh_instance_3d.h"

#define LUX_APPLY_PROPERTIES "_func_godot_apply_properties"
#define LUX_BUILD_COMPLETE	 "_func_godot_build_complete"

namespace Lux {

enum SurfaceFlag : uint8_t {
	SURFACE_SLICK	  = 1,
	SURFACE_FORCE_AIR = 1 << 1,
	SURFACE_BOUNCE	  = 1 << 2,
	SURFACE_FLESH	  = 1 << 3,
	SURFACE_NODROPS	  = 1 << 4,
	SURFACE_CLIMBABLE = 1 << 5,
};

enum TriggerFlag : uint8_t {
	TRIGGER_ONCE	  = 1,
	TRIGGER_TOGGLE	  = 1 << 1,
	TRIGGER_USABLE	  = 1 << 2,
	TRIGGER_ON_ENTRY  = 1 << 3,
	TRIGGER_ON_EXIT	  = 1 << 4,
	TRIGGER_ON_PLAYER = 1 << 5,
	TRIGGER_ON_ENEMY  = 1 << 6,
	TRIGGER_ON_ACTOR  = TRIGGER_ON_PLAYER | TRIGGER_ON_ENEMY
};

enum TriggerState {
	TRIGGER_READY,
	TRIGGER_USED,
	TRIGGER_WAIT,
};

enum MoverState {
	MOVER_IDLE,
	MOVER_ACTIVE,
	MOVER_WAIT,
};

enum LockFlag : uint8_t {
	LOCK_OPEN	= 0,
	LOCK_GREEN	= 1,
	LOCK_ORANGE = 1 << 1,
	LOCK_PURPLE = 1 << 2,
};

enum AddChildOwner {
	OWNER_ROOT,
	OWNER_PARENT_OWNER,
	OWNER_PARENT,
};

inline static const Vector3 VEC3_RIGHT{ 1.0f, 0.0f, 0.0f };
inline static const Vector3 VEC3_UP{ 0.0f, 1.0f, 0.0f };
inline static const Vector3 VEC3_FWD{ 0.0f, 0.0f, -1.0f };

static constexpr real_t M_PI{ Math_PI };
static constexpr real_t M_ETA{ Math_PI * 0.5 };

namespace Concepts {

template <typename T>
concept obj_derived = std::is_base_of_v<Object, T>;

template <typename T>
concept node_derived = std::is_base_of_v<Node, T>;

} // namespace Concepts

// Math

_FORCE_INLINE_ Vector3 clip(const Vector3& p_in, const Vector3& p_normal, real_t p_adj = 1.0f) {
	real_t bump = p_in.dot(p_normal);
	if (bump < 0.0) {
		bump *= p_adj;
	} else {
		bump /= p_adj;
	}
	return (p_in - (p_normal * bump));
}

_FORCE_INLINE_ Vector3 vec_xz(const Vector3& p_vec) {
	return Vector3{ p_vec.x, 0.0f, p_vec.z };
}

_FORCE_INLINE_ Vector3 vec_y(const Vector3& p_vec) {
	return Vector3{ 0.0f, p_vec.y, 0.0f };
}

_FORCE_INLINE_ real_t spline_fraction(real_t value, real_t scale) {
	value *= scale;
	return (3.0f) * (value * value) - (2.0f) * (value * value * value);
}

_FORCE_INLINE_ double spline_fraction(double value, double scale) {
	value *= scale;
	return (3.0) * (value * value) - (2.0) * (value * value * value);
}

// Tools

template <Concepts::obj_derived T>
T* instance_from_id(ObjectID p_id) {
	return Object::cast_to<T>(ObjectDB::get_instance(p_id));
}

template <Concepts::node_derived T>
T* instance_from_path(Node* p_from, NodePath p_path) {
	return Object::cast_to<T>(p_from->get_node_or_null(p_path));
}

template <Concepts::node_derived T>
auto child_view(T* p_node) {
	return std::views::iota(0, p_node->get_child_count()) |
		std::views::transform([p_node](int i) -> Node* { return p_node->get_child(i); });
}

ObjectID get_id_or_null(Node* p_node);
NodePath get_path_or_null(Node* p_node);
void	 add_child_persist(Node* p_parent,
		Node*					 p_child,
		bool					 p_readable_name = false,
		Lux::AddChildOwner p_owner				 = Lux::OWNER_ROOT);

namespace FGD {

void apply_properties(Node* p_node, const Dictionary& p_properties);
void set_targetname(Node* p_node, const StringName& p_name);
void finalize_entity(Node* p_node);
void finalize_mesh_children(Node* p_parent);

} // namespace FGD

} // namespace Lux

#endif
