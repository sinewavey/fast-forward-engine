#include "trigger_force.h"

void TriggerForce::_bind_methods() {
}

TriggerForce::TriggerForce() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	connect("body_entered", callable_mp(this, &TriggerForce::body_entered), CONNECT_PERSIST);
	connect("body_exited", callable_mp(this, &TriggerForce::body_exited));
}

void TriggerForce::apply_properties(const Dictionary& p_properties) {
	Trigger::apply_properties(p_properties);
}

void TriggerForce::body_entered(Node3D* p_body) {
	print_line(get_name(), "body enter:", p_body->get_name());
}

void TriggerForce::body_exited(Node3D* p_body) {
	print_line(get_name(), "body exit:", p_body->get_name());
}

void TriggerForce::build_complete() {
	Trigger::build_complete();
}

void TriggerForce::use(Node* p_activator) {
	Trigger::use(p_activator);
}
