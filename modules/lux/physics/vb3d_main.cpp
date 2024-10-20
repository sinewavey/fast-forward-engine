#include "virtual_body_3d.h"
#include "world_geometry.h"

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

	set_process_input(is_player_controlled());
}

void VirtualBody3D::apply_properties(const Dictionary& p_properties) {
	Lux::FGD::apply_properties(this, p_properties);
	cam_data.view_angles = p_properties.get("angles", Vector3());
}

void VirtualBody3D::build_complete() {
	Lux::FGD::finalize_entity(this);
	if (get_camera() != nullptr) {
		get_camera()->set_quaternion(Quaternion::from_euler(cam_data.view_angles));
	}
}

void VirtualBody3D::ready() {
}

void VirtualBody3D::ensure_interpolation_update() {
	uint64_t this_tick = Engine::get_singleton()->get_physics_frames();
	if (cam_data.last_physics_tick != this_tick) {
		cam_data.buffer[1]		   = cam_data.buffer[0];
		cam_data.last_physics_tick = this_tick;
	}
}

void VirtualBody3D::apply_acceleration(Vector3 p_dir,
	real_t									   p_speed,
	real_t									   p_accel,
	double									   p_delta) {

	real_t add = p_speed - body_data.velocity.dot(p_dir);

	if (p_accel <= 0.0 || add <= 0.0) {
		return;
	}

	real_t acc			= MIN(p_speed * p_accel * p_delta, add);
	body_data.velocity += p_dir * acc;
}

void VirtualBody3D::apply_friction(double p_delta) {
	auto& vel = body_data.velocity;
	auto  vec = Vector3(vel);

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

	// Non submerged ground friction; applied if the player is
	// not walking on slick surfaces, not about to jump or is in knockback time.
	if (body_data.water_level < 2) {
		if (flags & SurfaceControl &&
			!(body_data.surface_flags & Lux::SurfaceFlag::SURFACE_SLICK)) {
			if (!(flags & QueueJump || flags & Knockback)) {
				control	 = MAX(mv_stopspeed, speed);
				drop	+= control * fric * p_delta;
			}
		}
	}

	// Water friction goes here

	real_t mod{ MAX(speed - drop, 0.0f) };
	mod /= speed;

	vel[0] *= mod;
	vel[2] *= mod;
}

void VirtualBody3D::apply_gravity(double p_delta) {
}

void VirtualBody3D::apply_impulse(const Vector3& p_impulse, const Vector3& p_position) {
}

void VirtualBody3D::push(real_t p_force, Vector3 p_dir, real_t p_mass) {
	for (int i = 0; i < 3; i++) {
		body_data.velocity[i] += p_force * p_dir[i] / p_mass;
	}
}

void VirtualBody3D::duck() {

};
void VirtualBody3D::unduck() {

};

void VirtualBody3D::jump() {
	if (flags & QueueJump) {
		flags &= ~QueueJump;
		flags &= ~SurfaceControl;

		auto pos  = get_global_position();
		pos[1]	 += 0.1;
		set_global_position(pos);

		body_data.velocity	  = body_data.velocity.slide(body_data.normal);
		body_data.velocity[1] = MAX(body_data.velocity[1] + mv_jump, mv_jump);
	}
};

bool VirtualBody3D::check_duck() {
	return false;
}

bool VirtualBody3D::check_unduck() {
	return false;
}

bool VirtualBody3D::check_jump() {
	if (flags & SurfaceControl && input_cmd.upmove > 0.0f) {
		flags |= QueueJump;
	}

	return flags & QueueJump;
}

void VirtualBody3D::check_platform() {
}

void VirtualBody3D::check_surface_control() {
	flags					&= ~SurfaceControl;
	body_data.surface_flags	 = 0;
	body_data.normal		 = Vector3(0.0, 1.0, 0.0);

	auto trace = _move(Vector3(0.0f, -0.1f, 0.0f), true, 0.004);

	if (trace.is_null()) {
		return;
	}

	for (int i{ 0 }; i < trace->get_collision_count(); i++) {
		auto collider = trace->get_collider(i);

		if (auto wg = Object::cast_to<WorldGeometry>(collider); wg != nullptr) {
			body_data.surface_flags |= wg->surface_flags;
		}

		auto n = trace->get_normal(i);
		if (n[1] > 0.7) {
			flags			 |= SurfaceControl;
			body_data.normal  = n;
			return;
		}
	}
}

void VirtualBody3D::check_water_level() {
	body_data.water_level = 0;
}

VirtualBody3D::Trace VirtualBody3D::cast_trace(Vector3 p_from, Vector3 p_to) {
	real_t frac[2] = { 1.0f, 1.0f };

	Transform3D	 tr{ get_global_basis(), p_from };
	Vector3		 motion{ p_to - p_from };
	auto		 active_shape{ stand_shape };
	HashSet<RID> exclude{};
	exclude.insert(get_rid());

	PhysicsDirectSpaceState3D::ShapeParameters params{};
	PhysicsDirectSpaceState3D::ShapeRestInfo   rest_info{};

	params.shape_rid = active_shape->get_rid();
	// params.margin			   = active_shape->get_margin();
	params.margin			   = 0.002;
	params.transform		   = tr;
	params.motion			   = motion;
	params.exclude			   = exclude;
	params.collide_with_bodies = true;
	params.collide_with_areas  = false;

	auto dss = get_world_3d()->get_direct_space_state();

	auto result = dss->cast_motion(params, frac[0], frac[1]);

	if (!result) {
		return Trace{ p_to };
	}

	Vector3 p_end{ p_from + motion * frac[0] };
	params.transform.origin = p_end;

	dss->rest_info(params, &rest_info);

	return Trace{ p_end, rest_info.normal, frac[0], frac[1] };
};

bool VirtualBody3D::trace_step(Vector3 p_in_vel, Vector3& p_out_vel, double p_delta) {

	if (p_in_vel.length_squared() < 1.0) {
		p_in_vel = p_in_vel.normalized() * mv_speed;
	}

	Vector3 start{ get_global_position() };
	Vector3 step_pos{ start };

	step_pos[1] += mv_step_height;

	auto trace_up = cast_trace(start, step_pos);

	// Ensure stairs are reversible: cap up move height if head bonk.
	if (trace_up.position[1] < step_pos[1]) {
		step_pos[1] = trace_up.position[1];
	}

	step_pos += Vector3(p_in_vel[0], 0.0, p_in_vel[2]) * p_delta;
	// FIXME: Probably should be a real slide move
	auto trace_fwd = cast_trace(trace_up.position, step_pos);

	// Push back down
	auto trace_down = cast_trace(
		trace_fwd.position, Vector3(trace_fwd.position[0], start[1], trace_fwd.position[2]));

	if (trace_down.position.distance_squared_to(start) < 0.01) {
		return false;
	}

	if (trace_down.position[1] >= start[1] && trace_down.normal.y > 0.7) {
		set_global_position(trace_down.position);
		// p_out_vel  = Lux::clip(p_in_vel, trace_down.normal, 1.001);
		flags |= SurfaceControl;
		return true;
	}
	return false;
}

bool VirtualBody3D::move_and_slide(double p_delta) {
	if (Engine::get_singleton()->is_editor_hint() || !is_inside_tree()) {
		return false;
	}

	bool clipped{ false };

	Vector3 mpos{ get_global_position() };
	Vector3 move_vel{ body_data.velocity };
	double	mdist{ move_vel.length() * p_delta };

	for (int i = 0; i < 6; ++i) {
		if (mdist < 0.002) {
			break;
		}

		PhysicsServer3D::MotionResult	  trace	 = PhysicsServer3D::MotionResult();
		PhysicsServer3D::MotionParameters params = PhysicsServer3D::MotionParameters();

		params.from			  = get_global_transform();
		params.motion		  = move_vel.normalized() * mdist;
		params.margin		  = 0.002;
		params.max_collisions = 6;

		auto collision = move_and_collide(params, trace);

		if (!collision) {
			break;
		}

		clipped = true;

		mdist -= (get_global_position() - mpos).length();

		for (int j = 0; j < trace.collision_count; ++j) {
			Vector3 n = trace.collisions[j].normal;
			if (flags & SurfaceControl) {
				if (n[1] >= 0.7) {
					move_vel = Lux::clip(move_vel, n, 1.001);
					// } else if (move_vel.dot(n) < 0.0f && !trace_step(move_vel, move_vel,
					// p_delta)) {
				} else if (move_vel.dot(n) < 0.0f) {
					move_vel = Lux::clip(move_vel, n, 1.001);
				}
			} else {
				if (n[1] >= 0.7) {
					// check_surface_control();
					if (body_data.surface_flags & Lux::SurfaceFlag::SURFACE_FORCE_AIR) {
						move_vel = Lux::clip(move_vel, n, 1.001);
					}
				} else {
					// if (move_vel.dot(n) < 0.0f && !trace_step(move_vel, move_vel, p_delta)) {
					if (move_vel.dot(n) < 0.0f) {
						move_vel = Lux::clip(move_vel, n, 1.001);
					}
				}
			}
		}
		mpos = get_global_position();
	}

	// check_surface_control();

	body_data.velocity	= move_vel;
	body_data.origin[0] = mpos;

	return clipped;
}

void VirtualBody3D::update(double p_delta) {
	if (Engine::get_singleton()->is_editor_hint() || !is_inside_tree()) {
		return;
	};
	input_cmd.motion = Vector2();
}

void VirtualBody3D::update_timers(double p_delta) {
	body_data.duck_time = MIN<real_t>(body_data.duck_time - p_delta, 0.0);
	body_data.flag_time = MIN<real_t>(body_data.duck_time - p_delta, 0.0);
}

void VirtualBody3D::update_physics(double p_delta) {
	if (Engine::get_singleton()->is_editor_hint() || !is_inside_tree()) {
		return;
	}

	// cache data and update pre-move inputs
	body_data.origin[1] = body_data.origin[0];

	if (is_player_controlled()) {
		update_frame_input();
	}

	update_timers(p_delta);

	// Before we do moves, check for changes in the physics shape size and its results.
	check_duck();
	check_unduck();
	check_water_level();

	// Check if (still) standing on a platform and "silently" add the velocity.
	// Performs an internal grounded check that is not taken into account for determining
	// surface control.
	check_platform();

	check_surface_control();
	check_jump();

	// Wish direction depends on the contacted surface
	Vector3 wish_dir;
	// if (flags & Ladder) { // Move along ladders first, even when under water.
	// wish_dir =
	// 	Quaternion::from_euler(cam_data.view_angles).xform(Vector3(input_cmd.dir[0], 0.0f,
	// input_cmd.dir[1]));
	// } // else if (water_level > 2) {
	// slide along ground or otherwise wish dir is true transform (may move in all 3 axes)
	// } else {
	// create a flat input direction otherwise

	wish_dir = Vector3(input_cmd.dir[0], 0.0f, input_cmd.dir[1]);
	wish_dir.rotate(Vector3(0.0f, 1.0f, 0.0f), cam_data.view_angles[1] + Lux::m_pi);

	// wish_dir = Basis(Vector3{ 0.0f, 1.0f, 0.0f }, cam_data.view_angles[1])
	// 			   .xform(Vector3{ input_cmd.dir[0], 0.0f, input_cmd.dir[1] });
	// }

	// Project onto movement plane
	wish_dir = wish_dir.slide(body_data.normal);

	real_t accel;
	// If airborne or on a slick surface, apply air acceleration and gravity
	if (!(flags & SurfaceControl) || (body_data.surface_flags & Lux::SurfaceFlag::SURFACE_SLICK)) {
		body_data.velocity[1] -= mv_gravity * p_delta;
		accel				   = mv_air_accel;
	} else {
		accel = mv_accel;
	}
	// Calculate water friction here

	// Apply forces for frame and move body.
	apply_friction(p_delta);
	apply_acceleration(wish_dir, mv_speed, accel, p_delta);

	// Slide along the ground
	if (flags & SurfaceControl) {
		body_data.velocity = body_data.velocity.slide(body_data.normal);
	}

	// Store positions for the double move and slide
	auto start = get_global_position();
	auto vel   = body_data.velocity;

	if (move_and_slide(p_delta)) { // note: this is *not* godot's built in move and slide
		auto trace = cast_trace(start, start + Vector3(0.0f, -mv_step_height, 0.0f));

		// stair stepping does not occur if we are moving upwards off the ground plane
		if (body_data.velocity[1] > 0 && (trace.fraction[0] == 1.0 || trace.normal[1] < 0.7)) {
			return;
		}

		trace			 = cast_trace(start, start + Vector3(0.0f, mv_step_height, 0.0f));
		real_t step_size = trace.position[1] - start[1];

		// reset and try the slide move again from the step height
		set_global_position(trace.position);
		body_data.velocity = vel;
		move_and_slide(p_delta);

		// push back down and check final surface
		trace = cast_trace(
			get_global_position(), get_global_position() - Vector3(0.0f, step_size, 0.0f));

		if (trace.fraction[0] != 0.0f) {
			set_global_position(trace.position);
		}
		if (trace.fraction[0] < 1.0f) {
			body_data.velocity = Lux::clip(body_data.velocity, trace.normal, 1.001f);
		}
	};

	check_surface_control();
	jump(); // mama i am sorry this is Here
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
				cam_data.buffer[0] = get_global_position() + cam_data.base_offset;
				cam_data.buffer[1] = cam_data.buffer[1];
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
					return;
				}
				if (auto cam = get_camera(); cam != nullptr) {
					uint64_t frame = Engine::get_singleton()->get_frames_drawn();
					if (cam_data.last_update_frame != frame || cam_data.skip) {
						cam_data.skip			   = false;
						cam_data.last_update_frame = frame;
						cam_data.adj_origin		   = cam_data.buffer[1].lerp(cam_data.buffer[0],
							   CLAMP(engine->get_physics_interpolation_fraction(), 0.0, 1.0));
						cam->set_global_position(cam_data.adj_origin);
					}
				}
				break;
			}
		case NOTIFICATION_INTERNAL_PHYSICS_PROCESS:
			{
				ensure_interpolation_update();
				cam_data.buffer[0] = get_global_position() + cam_data.base_offset;
				break;
			}
		case NOTIFICATION_TRANSFORM_CHANGED:
			{
				ensure_interpolation_update();
				cam_data.buffer[0] = get_global_position() + cam_data.base_offset;
				break;
			}
		case NOTIFICATION_RESET_PHYSICS_INTERPOLATION:
			{
				if (is_inside_tree()) {
					cam_data.buffer[0] = get_global_position();
					cam_data.buffer[1] = cam_data.buffer[0];
				}
			}
	}
}
