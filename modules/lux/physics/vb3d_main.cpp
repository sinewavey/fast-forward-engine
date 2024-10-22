#include "virtual_body_3d.h"
#include "world_geometry.h"

// ===========================
// Initialization

VirtualBody3D::VirtualBody3D() : PhysicsBody3D(PhysicsServer3D::BODY_MODE_KINEMATIC) {
	engine		 = Engine::get_singleton();
	system_input = Input::get_singleton();
	set_process_mode(PROCESS_MODE_PAUSABLE);

	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	set_process(true);
	set_physics_process(true);
	set_process_internal(true);
	set_physics_process_internal(true);
	set_notify_transform(true);
}

VirtualBody3D::~VirtualBody3D() {
}

void VirtualBody3D::enter_tree() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	set_process_input(flags & PlayerControlled);
}

void VirtualBody3D::ready() {
}

// ===========================
// Building

void VirtualBody3D::apply_properties(const Dictionary& p_properties) {
	Lux::FGD::apply_properties(this, p_properties);
	view.angles = p_properties.get("angles", Vector3());
}

void VirtualBody3D::build_complete() {
	Lux::FGD::finalize_entity(this);
	if (get_camera() != nullptr) {
		get_camera()->set_quaternion(Quaternion::from_euler(view.angles));
	}
}

// ===========================
// Force application

// clang-format off
void VirtualBody3D::apply_acceleration(Vector3 p_dir, real_t p_speed, real_t p_accel, double p_delta) {
	// allow strafe acceleration and intentionally miscalculate p_speed - velocity.length()
	real_t cap = p_speed - phys.velocity.dot(p_dir);
	if (p_accel <= 0.0 || cap <= 0.0) { return; }

	real_t mod	   = MIN(p_speed * p_accel * p_delta, cap);
	phys.velocity += p_dir * mod;
} // clang-format on

void VirtualBody3D::apply_friction(double p_delta) {
	auto& vel = phys.velocity;
	auto  vec = Vector3(vel);

	// ignore slope movement without overriding real velocity value
	if (flags & SurfaceControl) {
		vec[1] = 0.0f;
	}

	auto speed = vec.length();

	if (speed < 0.03125) {
		vel[0] = 0.0f;
		vel[2] = 0.0f;
		return;
	}

	auto   fric{ mv_friction };
	real_t drop{}, control{};

	if (flags & Ducked) {
		fric *= 1.1;
	}

	// non-submerged ground friction applied if the player is
	// not walking on slick/force air surfaces, not about to jump or is in knockback time.
	if (phys.water_level < 2) {
		if (flags & SurfaceControl &&
			!(phys.surface_flags & (Lux::SURFACE_SLICK | Lux::SURFACE_FORCE_AIR))) {
			if (!(flags & (QueueJump | Knockback))) {
				control	 = MAX(mv_stop_speed, speed);
				drop	+= control * fric * p_delta;
			}
		}
	}

	// calculate water friction here

	real_t mod{ MAX(speed - drop, 0.0f) };
	mod /= speed;

	vel[0] *= mod;
	vel[2] *= mod;
}

void VirtualBody3D::apply_impulse(const Vector3& p_impulse, const Vector3& p_position) {
}

void VirtualBody3D::add_knockback_time(double p_time) {
	auto aux		= CLAMP<double>(p_time * 2.0, 0.0, 0.3);
	phys.flag_time += aux;
	flags		   |= Knockback;
}

void VirtualBody3D::push(real_t p_force, Vector3 p_dir, real_t p_mass) {
	add_knockback_time(p_force / 150.0f);
	for (int i = 0; i < 3; i++) {
		phys.velocity[i] += p_force * p_dir[i] / p_mass;
	}
}

// ===============================
// Physics queries

void VirtualBody3D::check_duck() {
	if ((flags & Ladder) && (flags & Ducked)) {
		set_duck(false);
		return;
	}

	// ducking desired
	if (input_cmd.upmove < 0.0f) {
		if (!(flags & (Ducked | Ducking))) {
			flags |= Ducking;
			get_duck_timer()->start(0.4);
		}

		double time = 0.4 - get_duck_timer()->get_time_left();

		if (time <= 0.0) {
			time = 0.0;
		}

		if (flags & Ducking) {
			if (Math::is_zero_approx(get_duck_timer()->get_time_left()) || !(flags & GroundPlane)) {
				set_duck(true);
			} else {
				double frac			= Lux::spline_fraction(time, 2.5);
				view.base_offset[1] = ((crouch_view_height - crouch_aabb.size[1]) * frac) +
					(stand_view_height * (1.0 - frac));
			}
		}
	} else if (flags & (Ducking | Ducked)) {
		// cast trace up to test we can unduck without Getting Fucking Stuck
		if (!test_move(get_global_transform(),
				Vector3{ 0.0f, crouch_aabb.size[1] * 0.5f, 0.0f },
				nullptr,
				mv_depen_margin)) {
			set_duck(false);
		}
	}
	return;
}

bool VirtualBody3D::check_jump() {
	if (flags & SurfaceControl && input_cmd.upmove > 0.0f) {
		flags |= QueueJump;
		return true;
	}
	return false;
}

void VirtualBody3D::check_platform() {
}

void VirtualBody3D::check_surface_control() {
	flags &= ~(SurfaceControl | GroundPlane);

	phys.surface_flags = 0;
	phys.normal		   = Vector3(0.0, 1.0, 0.0);

	auto trace = _move(Vector3(0.0f, -mv_ground_dist, 0.0f), true, mv_depen_margin);

	if (trace.is_null()) {
		return;
	}

	flags |= GroundPlane;

	for (int i{ 0 }; i < trace->get_collision_count(); i++) {
		auto collider = trace->get_collider(i);

		if (auto wg = Object::cast_to<WorldGeometry>(collider); wg != nullptr) {
			phys.surface_flags |= wg->surface_flags;
		}

		auto n = trace->get_normal(i);

		if (!i) {
			phys.normal = n;
		}
		
		if (phys.velocity.dot(n) > 3.2f) {
			continue;	
		}

		if (n[1] >= 0.701) {
			flags		|= SurfaceControl;
			phys.normal	 = n;
			return;
		}
	}
}

void VirtualBody3D::check_water_level() {
	phys.water_level = 0;
}

VirtualBody3D::Trace VirtualBody3D::cast_trace(Vector3 p_from, Vector3 p_to) {
	real_t frac[2] = { 1.0f, 1.0f };

	Transform3D	 tr{ get_global_basis(), p_from };
	Vector3		 motion{ p_to - p_from };
	Ref<Shape3D> active_shape;

	if (flags & Ducked) {
		active_shape = get_crouch_collider()->get_shape();
	} else {
		active_shape = get_stand_collider()->get_shape();
	}

	HashSet<RID> exclude{};
	exclude.insert(get_rid());

	PhysicsDirectSpaceState3D::ShapeParameters params{};
	PhysicsDirectSpaceState3D::ShapeRestInfo   rest_info{};

	params.shape_rid		   = active_shape->get_rid();
	params.margin			   = mv_depen_margin;
	params.transform		   = tr;
	params.motion			   = motion;
	params.exclude			   = exclude;
	params.collide_with_bodies = true;
	params.collide_with_areas  = false;

	auto dss	= get_world_3d()->get_direct_space_state();
	auto result = dss->cast_motion(params, frac[0], frac[1]);

	if (!result) {
		return Trace{ p_to };
	}

	Vector3 p_end{ p_from + motion * frac[1] };
	params.transform.origin = p_end;

	dss->rest_info(params, &rest_info);

	return Trace{ p_end, rest_info.normal, frac[0], frac[1] };
};

// ===========================
// Actions

void VirtualBody3D::set_duck(bool p_ducked) {
	auto crouch_col = get_crouch_collider();
	auto stand_col	= get_stand_collider();

	if (crouch_col == nullptr || stand_col == nullptr) {
		return;
	}

	stand_col->set_disabled(p_ducked);
	crouch_col->set_disabled(!p_ducked);

	auto pos = get_global_position();

	if (p_ducked) {
		flags &= ~Ducking;
		flags |= Ducked;
		if (flags & GroundPlane) {
			// this is what we call " Fucked In the Head, Right Here"
			pos[1] -= (stand_aabb.size[1] * 0.5) - mv_depen_margin;
			set_global_position(pos);
		}
		view.base_offset[1] = crouch_view_height;
	} else {
		if ((flags & Ducked) && (flags & SurfaceControl)) {
			pos[1] += (stand_aabb.size[1] * 0.5) + mv_depen_margin;
			set_global_position(pos);
		}
		flags				&= ~(Ducking | Ducked);
		view.base_offset[1]	 = stand_view_height;
	}
};

void VirtualBody3D::jump() {
	if (!(flags & QueueJump)) {
		return;
	}
	flags &= ~(QueueJump | GroundPlane | SurfaceControl);
	set_global_position(get_global_position() + Vector3{ 0.0f, mv_ground_dist, 0.0f });

	phys.velocity	 = phys.velocity.slide(phys.normal);
	phys.velocity[1] = MAX(phys.velocity[1] + mv_jump, mv_jump);
};

// Internal processing

void VirtualBody3D::update(double p_delta) {
	if (Engine::get_singleton()->is_editor_hint() || !is_inside_tree()) {
		return;
	};
	input_cmd.motion = Vector2();
}

void VirtualBody3D::update_physics(double p_delta) {
	if (Engine::get_singleton()->is_editor_hint() || !is_inside_tree()) {
		return;
	}

	// cache data and update pre-move inputs
	phys.origin[1] = phys.origin[0];
	phys.origin[0] = get_global_position();

	phys.flag_time -= p_delta;

	if (phys.flag_time <= 0.0) {
		phys.flag_time	= 0.0;
		flags		   &= ~(Knockback | WaterJump | LadderJump);
	}

	if (flags & PlayerControlled) {
		update_frame_input();
	}

	// before we do moves, check for changes in the physics shape size and its results.
	check_duck();
	check_water_level();

	// check if (still) standing on a platform and "silently" add the velocity.
	// performs an internal grounded check that is not taken into account for determining
	// surface control.
	check_platform();

	check_surface_control();
	check_jump(); // check now, but will execute later

	Vector3 wish_dir;
	if (flags & Ladder) {
		// Move along ladders first, even when under water.
		wish_dir = Quaternion::from_euler(view.angles)
					   .xform(Vector3(input_cmd.dir[0], 0.0f, input_cmd.dir[1]));
	} else if (phys.water_level > 2) {
		// slide along ground else go in wish direction to swim
		wish_dir = Quaternion::from_euler(view.angles)
					   .xform(Vector3(input_cmd.dir[0], 0.0f, input_cmd.dir[1]));
	} else {
		// create a flat input direction and project onto movement plane
		wish_dir = Basis{ Lux::VEC3_UP, view.angles[1] + Lux::M_PI }
					   .xform(Vector3{ input_cmd.dir[0], 0.0f, input_cmd.dir[1] })
					   .slide(phys.normal);
	}

	auto   wish_speed{ mv_speed };
	real_t accel;

	// if airborne or on a slick surface, apply air acceleration and gravity
	if (phys.water_level > 0) {
		accel		= mv_water_accel;
		wish_speed *= (phys.water_level == 1 ? mv_wade_mod : mv_swim_mod);
	} else if (!(flags & SurfaceControl) || flags & Knockback ||
		(phys.surface_flags & Lux::SurfaceFlag::SURFACE_SLICK)) {
		phys.velocity[1] -= mv_gravity * p_delta;
		accel			  = mv_air_accel;
	} else {
		accel = mv_accel;
	}

	if (flags & Ducked) {
		wish_speed *= mv_duck_mod;
	}

	// apply forces for frame and move body.
	apply_friction(p_delta);
	apply_acceleration(wish_dir, wish_speed, accel, p_delta);

	// slide along the ground
	if (flags & GroundPlane) {
		phys.velocity = phys.velocity.slide(phys.normal);
	}

	step_slide_move(p_delta);

	check_surface_control();

	jump(); // mama i am sorry this is Here

	// Platform velocity applies here
	print_line(
		"Ended frame", flags & SurfaceControl ? "walk" : "air", phys.velocity.length() * 32.0f);
}

void VirtualBody3D::ensure_interpolation_update() {
	uint64_t this_tick = Engine::get_singleton()->get_physics_frames();
	if (view.last_physics_tick != this_tick) {
		view.buffer[1]		   = view.buffer[0];
		view.last_physics_tick = this_tick;
	}
}

void VirtualBody3D::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE:
			{
				enter_tree();
				break;
			}
		case NOTIFICATION_ENTER_WORLD:
			{
				if (get_camera() != nullptr) {
					get_camera()->set_physics_interpolation_mode(PHYSICS_INTERPOLATION_MODE_OFF);
				}
				view.buffer[0] = get_global_position() + view.base_offset;
				view.buffer[1] = view.buffer[0];
				break;
			}
		case NOTIFICATION_READY:
			{
				ready();
				break;
			}
		case NOTIFICATION_PROCESS:
			{
				if (!get_tree()->is_paused()) {
					update(get_process_delta_time());
				}
				break;
			}
		case NOTIFICATION_PHYSICS_PROCESS:
			{
				if (!get_tree()->is_paused()) {
					update_physics(get_physics_process_delta_time());
				}
				break;
			}
		case NOTIFICATION_INTERNAL_PROCESS:
			{
				if (Engine::get_singleton()->is_editor_hint() || !is_inside_tree() ||
					get_tree()->is_paused()) {
					break;
				}
				if (auto cam = get_camera(); cam != nullptr) {
					uint64_t frame = Engine::get_singleton()->get_frames_drawn();
					if (view.last_update_frame != frame || view.skip) {
						view.skip			   = false;
						view.last_update_frame = frame;
						view.adj_origin		   = view.buffer[1].lerp(view.buffer[0],
							   CLAMP(engine->get_physics_interpolation_fraction(), 0.0, 1.0));
						cam->set_global_position(view.adj_origin);
					}
				}
				break;
			}
		case NOTIFICATION_INTERNAL_PHYSICS_PROCESS:
			{
				ensure_interpolation_update();
				view.buffer[0] = get_global_position() + view.base_offset;
				break;
			}
		case NOTIFICATION_TRANSFORM_CHANGED:
			{
				ensure_interpolation_update();
				view.buffer[0] = get_global_position() + view.base_offset;
				break;
			}
		case NOTIFICATION_RESET_PHYSICS_INTERPOLATION:
			{
				if (is_inside_tree()) {
					view.buffer[0] = get_global_position() + view.base_offset;
					view.buffer[1] = view.buffer[0];
				}
				break;
			}
		case NOTIFICATION_PAUSED:
			{
				if (auto cam = get_camera(); cam != nullptr) {
					view.last_update_frame = Engine::get_singleton()->get_frames_drawn();
					view.adj_origin		   = view.buffer[1].lerp(view.buffer[0],
						   CLAMP(engine->get_physics_interpolation_fraction(), 0.0, 1.0));
					cam->set_global_position(view.adj_origin);
				}
				break;
			}
	}
}
