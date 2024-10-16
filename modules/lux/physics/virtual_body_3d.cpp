#include "virtual_body_3d.h"

VirtualBody3D::VirtualBody3D() : PhysicsBody3D(PhysicsServer3D::BODY_MODE_KINEMATIC) {
}

VirtualBody3D::~VirtualBody3D() {
}

void VirtualBody3D::enter_tree() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	print_line("Enter Tree");
	set_process(true);
	set_physics_process(true);
	set_process_input(is_player_controlled());
}

void VirtualBody3D::apply_properties(const Dictionary& p_properties) {
	
}

void VirtualBody3D::build_complete() {
	
}


void VirtualBody3D::ready() {
}

void VirtualBody3D::apply_impulse(const Vector3& p_impulse, const Vector3& p_position) {
	// model.velocity += p_impulse;
}

void VirtualBody3D::push(real_t p_force, Vector3 p_dir, real_t p_mass) {
	for (int i = 0; i < 3; i++) {
		model.velocity[i] += p_force * p_dir[i] / p_mass;
	}
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
}

void VirtualBody3D::update_timers(double p_delta) {
	model.duck_time = MIN<real_t>(model.duck_time - p_delta, 0.0);
	model.flag_time = MIN<real_t>(model.duck_time - p_delta, 0.0);
}

void VirtualBody3D::update_physics(double p_delta) {
	if (Engine::get_singleton()->is_editor_hint() || !is_inside_tree()) {
		return;
	}

	update_timers(p_delta);
	move_and_slide(p_delta);
}

void VirtualBody3D::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE:
			{
				enter_tree();
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
	}
}
