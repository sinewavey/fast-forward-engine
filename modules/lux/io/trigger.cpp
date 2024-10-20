#include "trigger.h"

void Trigger::_bind_methods() {
	ADD_SIGNAL(MethodInfo("used", PropertyInfo(Variant::OBJECT, "activator")));

	ClassDB::bind_method(
		D_METHOD("_func_godot_apply_properties", "properties"), &Trigger::apply_properties);
	ClassDB::bind_method(D_METHOD("_func_godot_build_complete"), &Trigger::build_complete);

	ClassDB::bind_method(D_METHOD("set_target", "p_target"), &Trigger::set_target);
	ClassDB::bind_method(D_METHOD("set_target_func", "p_target_func"), &Trigger::set_target_func);
	ClassDB::bind_method(D_METHOD("set_flags", "p_flags"), &Trigger::set_flags);

	ClassDB::bind_method(D_METHOD("get_target_func"), &Trigger::get_target_func);
	ClassDB::bind_method(D_METHOD("get_target"), &Trigger::get_target);
	ClassDB::bind_method(D_METHOD("get_flags"), &Trigger::get_flags);

	ClassDB::bind_method(D_METHOD("is_player_usable"), &Trigger::is_player_usable);

	ClassDB::bind_method(D_METHOD("use", "activator"), &Trigger::use);

	BIND_BITFIELD_FLAG(Lux::TRIGGER_ONCE);
	BIND_BITFIELD_FLAG(Lux::TRIGGER_TOGGLE);
	BIND_BITFIELD_FLAG(Lux::TRIGGER_USABLE);
	BIND_BITFIELD_FLAG(Lux::TRIGGER_ON_ENTRY);
	BIND_BITFIELD_FLAG(Lux::TRIGGER_ON_EXIT);
	BIND_BITFIELD_FLAG(Lux::TRIGGER_ON_PLAYER);
	BIND_BITFIELD_FLAG(Lux::TRIGGER_ON_ENEMY);

	BIND_ENUM_CONSTANT(Lux::TRIGGER_READY);
	BIND_ENUM_CONSTANT(Lux::TRIGGER_USED);
	BIND_ENUM_CONSTANT(Lux::TRIGGER_WAIT);

	ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "target"), "set_target", "get_target");
	ADD_PROPERTY(
		PropertyInfo(Variant::STRING_NAME, "target_func"), "set_target_func", "get_target_func");
	ADD_PROPERTY(PropertyInfo(Variant::INT,
					 "flags",
					 PROPERTY_HINT_FLAGS,
					 "Once,Toggle,Usable,On Entry,On Exit,On Player,On Enemies"),
		"set_flags",
		"get_flags");
}

Trigger::Trigger() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	connect("body_entered", callable_mp(this, &Trigger::body_entered), CONNECT_PERSIST);
	connect("body_exited", callable_mp(this, &Trigger::body_exited));
}

void Trigger::set_target(const StringName& p_target) {
	target = p_target;
}

StringName Trigger::get_target() const {
	return target;
}

void Trigger::set_target_func(const StringName& p_target_func) {
	target_func = p_target_func;
}

StringName Trigger::get_target_func() const {
	return target_func;
}

bool Trigger::is_player_usable() const {
	return flags & Lux::TRIGGER_USABLE;
}

void Trigger::set_flags(BitField<Lux::TriggerFlag> p_flags) {
	flags = p_flags;
}

BitField<Lux::TriggerFlag> Trigger::get_flags() const {
	return flags;
}

void Trigger::body_entered(Node3D* p_body) {
	print_line(get_name(), "body enter:", p_body->get_name());
	if (flags & Lux::TRIGGER_ON_ENTRY) {
		use(p_body);
	}
}

void Trigger::body_exited(Node3D* p_body) {
	print_line(get_name(), "body exit:", p_body->get_name());
	if (flags & Lux::TRIGGER_ON_EXIT) {
		use(p_body);
	}
}

void Trigger::apply_properties(const Dictionary& p_properties) {
	Lux::FGD::apply_properties(this, p_properties);
}

void Trigger::build_complete() {
	Lux::FGD::finalize_entity(this);
}

void Trigger::use(Node* p_activator) {
	Lux::IO::use_target(target, target_func, this);
}

VARIANT_BITFIELD_CAST(Lux::TriggerFlag);
VARIANT_ENUM_CAST(Lux::TriggerState);
