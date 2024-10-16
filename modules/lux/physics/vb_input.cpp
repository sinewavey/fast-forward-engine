#include "virtual_body_3d.h"



void VirtualBody3D::input(const Ref<InputEvent>& p_event) {
	if (Engine::get_singleton()->is_editor_hint() || !is_inside_tree()) {
		return;
	}

	if (!is_player_controlled()) {
		return;
	}


	if (p_event->get_class() == "InputEventMouseMotion") {
		Vector2 aux = p_event->get("screen_relative");
		return;
	}

    if (p_event->get_class() == "InputEventKey") {
        
        
    }
}
