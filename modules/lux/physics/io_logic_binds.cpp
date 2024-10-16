#include "io_logic.h"

void Trigger::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_target", "p_target"), &Trigger::set_target);
	ClassDB::bind_method(D_METHOD("get_target"), &Trigger::get_target);
	ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "target"), "set_target", "get_target");

	ClassDB::bind_method(D_METHOD("apply_properties", "properties"), &Trigger::apply_properties);
	ClassDB::bind_method(D_METHOD("build_complete"), &Trigger::build_complete);
	ClassDB::bind_method(D_METHOD("use", "activator"), &Trigger::use);
}

void Counter::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_target", "p_target"), &Counter::set_target);
	ClassDB::bind_method(D_METHOD("get_target"), &Counter::get_target);
	ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "target"), "set_target", "get_target");

	ClassDB::bind_method(D_METHOD("set_value", "p_value"), &Counter::set_value);
	ClassDB::bind_method(D_METHOD("get_value"), &Counter::get_value);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "value"), "set_value", "get_value");

	ClassDB::bind_method(D_METHOD("apply_properties", "properties"), &Counter::apply_properties);
	ClassDB::bind_method(D_METHOD("build_complete"), &Counter::build_complete);
	ClassDB::bind_method(D_METHOD("use", "activator"), &Counter::use);
}
