#include "q_compat.h"

void Worldspawn::_bind_methods() {
	ClassDB::bind_method(
		D_METHOD("_func_godot_apply_properties", "properties"), &Worldspawn::apply_properties);
	ClassDB::bind_method(D_METHOD("_func_godot_build_complete"), &Worldspawn::build_complete);
}

void Worldspawn::apply_properties(const Dictionary& p_properties) {
	Lux::FGD::apply_properties(this, p_properties);
}

void Worldspawn::build_complete() {
	Lux::FGD::finalize_entity(this);
}
