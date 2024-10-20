#ifndef LUX_IO_TRIGGER_H
#define LUX_IO_TRIGGER_H

#include "../shared.h"
#include "scene/3d/physics/area_3d.h"

class Trigger : public Area3D {
	GDCLASS(Trigger, Area3D);

public:
	void	   set_target(const StringName& p_target);
	StringName get_target() const;

	void	   set_target_func(const StringName& p_target_func);
	StringName get_target_func() const;

	bool is_player_usable() const;

	void					   set_flags(BitField<Lux::TriggerFlag> p_flags);
	BitField<Lux::TriggerFlag> get_flags() const;

	virtual void apply_properties(const Dictionary& p_properties);
	virtual void build_complete();
	virtual void use(Node* p_activator);

	void body_entered(Node3D* p_body);
	void body_exited(Node3D* p_body);

	Trigger();

protected:
	static void _bind_methods();

private:
	StringName target{};
	StringName target_func{ "use" };

	BitField<Lux::TriggerFlag> flags{ 0 };
	Lux::TriggerState		   state{ Lux::TRIGGER_READY };
};

#endif
