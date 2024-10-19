#include "counter.h"

void Counter::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_target", "p_target"), &Counter::set_target);
	ClassDB::bind_method(D_METHOD("get_target"), &Counter::get_target);
	ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "target"), "set_target", "get_target");

	ClassDB::bind_method(D_METHOD("set_target_func", "p_target_func"), &Counter::set_target_func);
	ClassDB::bind_method(D_METHOD("get_target_func"), &Counter::get_target_func);
	ADD_PROPERTY(
		PropertyInfo(Variant::STRING_NAME, "target_func"), "set_target_func", "get_target_func");

	ClassDB::bind_method(D_METHOD("set_value", "p_value"), &Counter::set_value);
	ClassDB::bind_method(D_METHOD("get_value"), &Counter::get_value);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "value"), "set_value", "get_value");

	ClassDB::bind_method(
		D_METHOD("_func_godot_apply_properties", "properties"), &Counter::apply_properties);
	ClassDB::bind_method(D_METHOD("_func_godot_build_complete"), &Counter::build_complete);
	ClassDB::bind_method(D_METHOD("use", "activator"), &Counter::use);
}

void Counter::apply_properties(const Dictionary& p_properties) {
	Lux::FGD::apply_properties(this, p_properties);
	if (p_properties.has("value")) {
		set_value(p_properties.get("value", int()));
	}
}

void Counter::build_complete() {
	Lux::FGD::finalize_entity(this);
}

void Counter::use(Node* p_activator) {
	Lux::IO::use_target(target, target_func, this);
}
