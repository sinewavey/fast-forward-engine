#include "virtual_body_3d.h"

void VirtualBody3D::input(const Ref<InputEvent>& p_event) {
	if (Engine::get_singleton()->is_editor_hint() || !is_inside_tree()) {
		return;
	}

	if (!is_player_controlled()) {
		return;
	}

	if (p_event->get_class() == "InputEventMouseMotion") {
		Vector2 aux			  = p_event->get("screen_relative");
		model.sway			 += aux;
		aux					 *= input_motion_mod * msens;
		model.view_angles[0] -= aux[1];
		model.view_angles[1] -= aux[0];
		if (auto cam = get_camera(); cam != nullptr) {
			cam->set_quaternion(Quaternion::from_euler(model.view_angles));
		}
		return;
	}

	if (p_event->get_class() == "InputEventKey") {}
}
