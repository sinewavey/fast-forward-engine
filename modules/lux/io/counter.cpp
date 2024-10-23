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

	ClassDB::bind_method(D_METHOD("set_initial_value", "p_value"), &Counter::set_value);
	ClassDB::bind_method(D_METHOD("get_initial_value"), &Counter::get_value);
	ADD_PROPERTY(
		PropertyInfo(Variant::INT, "initial_value"), "set_initial_value", "get_initial_value");

	ClassDB::bind_method(D_METHOD(LUX_APPLY_PROPERTIES, "properties"), &Counter::apply_properties);
	ClassDB::bind_method(D_METHOD(LUX_BUILD_COMPLETE), &Counter::build_complete);
	ClassDB::bind_method(D_METHOD("use", "activator"), &Counter::use);
}

void Counter::apply_properties(const Dictionary& p_properties) {
	Lux::FGD::apply_properties(this, p_properties);
}

void Counter::build_complete() {
	Lux::FGD::finalize_entity(this);
	set_initial_value(value);
}

void Counter::use(Node* p_activator) {
	value -= 1;
	if (value <= 0) {
		SceneTree::get_singleton()->call_group(target, target_func, this);
	}
}
