#include "virtual_body_3d.h"
#include "world_geometry.h"

VirtualBody3D::VirtualBody3D() : PhysicsBody3D(PhysicsServer3D::BODY_MODE_KINEMATIC) {
	engine		 = Engine::get_singleton();
	system_input = Input::get_singleton();
}

VirtualBody3D::~VirtualBody3D() {
}

void VirtualBody3D::enter_tree() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	// Enable base processing, internal physics processing, and transform notifications.
	set_process(true);
	set_physics_process(true);
	set_process_internal(true);
	set_physics_process_internal(true);
	set_process_input(is_player_controlled());
	set_notify_transform(true);
}

void VirtualBody3D::apply_properties(const Dictionary& p_properties) {
}

void VirtualBody3D::build_complete() {
}

void VirtualBody3D::ready() {
}

void VirtualBody3D::tick() {
	uint64_t this_tick = Engine::get_singleton()->get_physics_frames();
	if (last_physics_tick != this_tick) {
		buffer[1]		  = buffer[0];
		last_physics_tick = this_tick;
	}
}

void VirtualBody3D::apply_impulse(const Vector3& p_impulse, const Vector3& p_position) {
	// model.velocity += p_impulse;
}

void VirtualBody3D::push(real_t p_force, Vector3 p_dir, real_t p_mass) {
	for (int i = 0; i < 3; i++) {
		model.velocity[i] += p_force * p_dir[i] / p_mass;
	}
}

void VirtualBody3D::check_duck() {
}

void VirtualBody3D::check_unduck() {
}

void VirtualBody3D::check_platform() {
}

void VirtualBody3D::check_surface_control() {
	flags				&= ~SurfaceControl;
	model.surface_flags	 = 0;
	model.normal		 = Vector3();

	PhysicsServer3D::MotionParameters params{
		get_global_transform(), Vector3(0.0f, -0.1f, 0.0f), 0.004f
	};
	PhysicsServer3D::MotionResult trace;

	auto collision = move_and_collide(params, trace, true);
	print_line(collision);

	if (!collision) {
		return;
	}

	for (int i{ 0 }; i < trace.collision_count; i++) {
		auto cid = trace.collisions[i].collider_id;

		if (auto collider = Object::cast_to<WorldGeometry>(ObjectDB::get_instance(cid));
			collider != nullptr) {
			model.surface_flags |= collider->surface_flags;
		}

		auto n = trace.collisions[i].normal;
		if (n[1] > 0.7) {
			flags		 |= SurfaceControl;
			model.normal  = n;
			return;
		}
	}
}

void VirtualBody3D::check_water_level() {
}

void VirtualBody3D::move_and_slide(double delta) {
	if (Engine::get_singleton()->is_editor_hint() || !is_inside_tree()) {
		return;
	}
}

void VirtualBody3D::update(double p_delta) {
	if (Engine::get_singleton()->is_editor_hint() || !is_inside_tree()) {
		return;
	}

	// var fnc := camera_3d.set_global_position;

	// if Engine.get_frames_per_second() > Engine.physics_ticks_per_second:
	// 	fnc.call(buffer[1].lerp(buffer[0], clampf(Engine.get_physics_interpolation_fraction(),
	// 0.0, 1.0))) else: 	fnc.call(buffer[0])

	if (auto cam = get_camera(); cam != nullptr) {
		auto fnc = [cam](Vector3 p) { cam->set_global_position(p); };

		if (engine->get_frames_per_second() > engine->get_physics_ticks_per_second()) {}
	}
}

void VirtualBody3D::update_timers(double p_delta) {
	model.duck_time = MIN<real_t>(model.duck_time - p_delta, 0.0);
	model.flag_time = MIN<real_t>(model.duck_time - p_delta, 0.0);
}

void VirtualBody3D::update_physics(double p_delta) {
	if (Engine::get_singleton()->is_editor_hint() || !is_inside_tree()) {
		return;
	}

	if (is_player_controlled()) {
		model.input_dir =
			Input::get_singleton()->get_vector("left", "right", "forward", "backward");
	}

	model.origin[1] = model.origin[0];
	update_timers(p_delta);

	check_duck();
	check_unduck();

	check_platform();

	check_surface_control();
	move_and_slide(p_delta);
	model.origin[0] = model.origin[1];
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
				break;
			}
		case NOTIFICATION_READY:
			{
				ready();
				break;
			}
		case NOTIFICATION_PROCESS:
			{
				update(get_process_delta_time());
				break;
			}
		case NOTIFICATION_PHYSICS_PROCESS:
			{
				update_physics(get_physics_process_delta_time());
				break;
			}
		case NOTIFICATION_INTERNAL_PROCESS:
			{

				break;
			}
		case NOTIFICATION_INTERNAL_PHYSICS_PROCESS:
			{
				tick();
				buffer[0] = get_global_position();
				break;
			}
		case NOTIFICATION_TRANSFORM_CHANGED:
			{
				tick();
				buffer[0] = get_global_position();
				break;
			}
		case NOTIFICATION_RESET_PHYSICS_INTERPOLATION:
			{
				break;
			}
	}
}
