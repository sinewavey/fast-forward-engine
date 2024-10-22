#include "virtual_body_3d.h"

bool VirtualBody3D::slide_move(double p_delta) {
	if (Engine::get_singleton()->is_editor_hint() || !is_inside_tree()) {
		return false;
	}
	auto frame = engine->get_physics_frames();

	Vector3 mpos{ get_global_position() };
	Vector3 move_vel{ phys.velocity };
	double	mdist{ move_vel.length() * p_delta };

	bool clipped{ false };

	Vector<Plane> planes{};

	for (int i = 0; i < mv_max_slides; ++i) {
		if (mdist < mv_depen_margin) {
			// print_line(frame, "Broke for small distance");
			break;
		}

		PhysicsServer3D::MotionResult	  trace	 = PhysicsServer3D::MotionResult();
		PhysicsServer3D::MotionParameters params = PhysicsServer3D::MotionParameters{
			get_global_transform(), move_vel.normalized() * mdist, mv_depen_margin
		};

		params.max_collisions		 = 6;
		params.recovery_as_collision = true;

		auto collision = move_and_collide(params, trace, false, false);

		if (!collision) {
			break;
		}

		clipped	 = true;
		mdist	-= (get_global_position() - mpos).length();

		for (int j = 0; j < trace.collision_count; ++j) {
			Vector3 n = trace.collisions[j].normal;
			if (flags & SurfaceControl) {
				if (n[1] >= 0.701f) {
					move_vel = Lux::clip(move_vel, n, 1.001f);
				} else if (n[1] < 0.0f) {
					n[1] = 0.0f;
					n.normalize();
					print_line(frame, "Hit a bad normal");
					move_vel = Lux::clip(move_vel, n, 1.25f);
				} else {
					move_vel = Lux::clip(move_vel, n, 1.001f);
				}
				// } else if (move_vel.dot(n) < 0.0f) {
				// 	move_vel = Lux::clip(move_vel, n, 1.001);
				// }
			} else {
				if (n[1] >= 0.701f) {
					if (phys.surface_flags & Lux::SurfaceFlag::SURFACE_FORCE_AIR) {
						move_vel = Lux::clip(move_vel, n, 1.001f);

					} else {
						// todo: is this correct? maybe should not be used.
						// check_surface_control();
					}
				} else {
					// if (move_vel.dot(n) < 0.0f) {
					move_vel = Lux::clip(move_vel, n, 1.001f);
					// }
				}
			}
		}
		mpos = get_global_position();
	}

	// MAYBE UNUSED
	// check_surface_control();

	phys.velocity = move_vel;

	return clipped;
}

// bool VirtualBody3D::step_slide_move(double p_delta) {
// 	auto frame = engine->get_physics_frames();

// 	// store positions for the double move and slide
// 	auto start = get_global_position();
// 	auto vel   = phys.velocity;

// 	auto start_flags = flags;

// 	// no need to take further action if move was OK
// 	if (!slide_move(p_delta)) {
// 		print_line(frame, "First slide move was OK");
// 		return false;
// 	}

// 	// cast_trace is a combo of DirectSpaceState->cast_motion + rest_info
// 	auto trace = cast_trace(start, start + Vector3(0.0f, -mv_step_height, 0.0f));

// 	// stair stepping does not occur if we are moving upwards off the ground plane
// 	if (vel[1] > 0.0f && (trace.safe == 1.0f || trace.normal[1] < 0.701f)) {
// 		print_line(frame, "First slidemove hit, but second going down was not valid");
// 		return true;
// 	}

// 	// cast up the step height and check for a potential head bonk.
// 	// this ensures stairs are reversible; if you go up a step, you
// 	// should be able to go back down it!
// 	trace = cast_trace(start, start + Vector3(0.0f, mv_step_height, 0.0f));

// 	if (Math::is_zero_approx(trace.unsafe)) {
// 		print_line(frame, "First slidemove hit, but second going up was not valid");
// 		return true;
// 	}

// 	real_t step_size = trace.position[1] - start[1];

// 	// reset and try the slide move again from the step height
// 	set_global_position(trace.position);
// 	phys.velocity = vel;
// 	flags		  = start_flags;

// 	slide_move(p_delta);

// 	// push back down and check final surface
// 	trace =
// 		cast_trace(get_global_position(), get_global_position() - Vector3(0.0f, step_size, 0.0f));

// 	if (!Math::is_zero_approx(trace.unsafe) && trace.normal[1] > 0.701f) {
// 		set_global_position(trace.position);
// 		print_line(frame, step_size, "Adjusting for step slide move");

// 	} else {
// 		print_line(frame, step_size, "Second trace was not safe");
// 		set_global_position(start);
// 		phys.velocity = vel;
// 		return true;
// 	}

// 	if (trace.safe < 1.0f) {
// 		phys.velocity = Lux::clip(phys.velocity, trace.normal, 1.001f);
// 		return true;
// 	}

// 	return false;
// };

bool VirtualBody3D::step_slide_move(double p_delta) {
	auto start = get_global_position();
	auto vel   = phys.velocity;

	if (slide_move(p_delta)) { // note: this is *not* godot's built in move and slide

		return true; // for now disable it all

		auto trace = cast_trace(start, start + Vector3(0.0f, -mv_step_height, 0.0f));

		// stair stepping does not occur if we are moving upwards off the ground plane
		if (phys.velocity[1] > 0 && (trace.safe == 1.0 || trace.normal[1] < 0.7)) {
			return true;
		}

		trace			 = cast_trace(start, start + Vector3(0.0f, mv_step_height, 0.0f));
		real_t step_size = trace.position[1] - start[1];

		// reset and try the slide move again from the step height
		set_global_position(trace.position);
		phys.velocity = vel;
		slide_move(p_delta);

		// push back down and check final surface
		trace = cast_trace(
			get_global_position(), get_global_position() - Vector3(0.0f, step_size, 0.0f));

		if (trace.safe != 0.0f) {
			set_global_position(trace.position);
		}

		if (trace.safe < 1.0f) {
			phys.velocity = Lux::clip(phys.velocity, trace.normal, 1.001f);
		}

		return true;
	};
	return false;
};
