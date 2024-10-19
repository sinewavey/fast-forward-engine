#include "virtual_body_3d.h"

void VirtualBody3D::update_frame_input(double p_delta) {
	if (get_tree()->is_paused()) {
		return;
	}

	input_cmd.dir = system_input->get_vector("left", "right", "forward", "backward");

	for (auto& it : input_cmd.actions) {
		if (system_input->is_action_pressed(it.key)) {
			it.value += p_delta;
		} else {
			it.value = 0.0;
		}
		print_line(it.key, it.value);
	}
}

void VirtualBody3D::input(const Ref<InputEvent>& p_event) {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	if (!is_player_controlled() || get_tree()->is_paused()) {
		return;
	}

	if (p_event->get_class() == "InputEventMouseMotion") {
		auto& vec = cam_data.view_angles;

		Vector2 aux	 = p_event->get("screen_relative");
		aux			*= input_motion_mod * msens;

		vec[0] = CLAMP(vec[0] - aux[1], -Lux::m_eta, Lux::m_eta);
		vec[1] = Math::wrapf(vec[1] - aux[0], -Lux::m_pi, Lux::m_pi);

		if (auto cam = get_camera(); cam != nullptr) {
			cam->set_quaternion(Quaternion::from_euler(vec));
		}
		return;
	}
}
