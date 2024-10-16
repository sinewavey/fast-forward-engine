#include "trigger.h"

void Trigger::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_target", "p_target"), &Trigger::set_target);
	ClassDB::bind_method(D_METHOD("get_target"), &Trigger::get_target);
	ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "target"), "set_target", "get_target");

	ClassDB::bind_method(
		D_METHOD("set_player_usable", "p_player_usable"), &Trigger::set_player_usable);
	ClassDB::bind_method(D_METHOD("is_player_usable"), &Trigger::is_player_usable);
	ADD_PROPERTY(
		PropertyInfo(Variant::BOOL, "player_usable"), "set_player_usable", "is_player_usable");

	ClassDB::bind_method(D_METHOD("apply_properties", "properties"), &Trigger::apply_properties);
	ClassDB::bind_method(D_METHOD("build_complete"), &Trigger::build_complete);
	ClassDB::bind_method(D_METHOD("use", "activator"), &Trigger::use);
}

Trigger::Trigger() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	connect("body_entered", callable_mp(this, &Trigger::body_entered), CONNECT_PERSIST);
	connect("body_exited", callable_mp(this, &Trigger::body_exited));
}

void Trigger::body_entered(Node3D* p_body) {
	print_line(get_name(), "body enter:", p_body->get_name());
}

void Trigger::body_exited(Node3D* p_body) {
	print_line(get_name(), "body exit:", p_body->get_name());
}

void Trigger::apply_properties(const Dictionary& p_properties) {
	Lux::apply_fgd_properties(this, p_properties);
}

void Trigger::build_complete() {
	if (has_meta("target")) {
		set_target(get_meta("target"));
	}
}

void Trigger::use(Node* p_activator) {
}
