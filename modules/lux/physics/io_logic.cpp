#include "io_logic.h"

void Trigger::apply_properties(const Dictionary& p_properties) {
	Lux::apply_common_properties(this, p_properties);
}

void Trigger::build_complete() {
	if (has_meta("target")) {
		set_target(get_meta("target"));
	}
}

void Trigger::use(Node* p_activator) {
}

void Counter::apply_properties(const Dictionary& p_properties) {
	Lux::apply_common_properties(this, p_properties);
	if (p_properties.has("value")) {
		set_value(p_properties.get("value", int()));
	}
}

void Counter::build_complete() {
	if (has_meta("target")) {
		set_target(get_meta("target"));
	}
}

void Counter::use(Node* p_activator) {
}
