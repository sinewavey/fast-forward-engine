#ifndef LUX_TRIGGER_FORCE_H
#define LUX_TRIGGER_FORCE_H

#include "../shared.h"
#include "trigger.h"

class TriggerForce : public Trigger {
	GDCLASS(TriggerForce, Trigger);

public:
	void apply_properties(const Dictionary& p_properties) override;
	void build_complete() override;

	void body_entered(Node3D* p_body);
	void body_exited(Node3D* p_body);

	void use(Node* p_activator);

	TriggerForce();

protected:
	static void _bind_methods();
};

#endif
