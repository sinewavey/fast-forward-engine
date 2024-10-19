#include "shared.h"

#include "register_types.h"
#include "core/object/class_db.h"

#include "map/q_compat.h"

#include "controller/controller.h"

#include "physics/virtual_body_3d.h"
#include "physics/world_geometry.h"

#include "io/trigger.h"
#include "io/counter.h"
#include "io/trigger_force.h"

void initialize_lux_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	print_line("Initializing Fast Forward types");

	GDREGISTER_CLASS(MapBuilder);
	GDREGISTER_CLASS(MapInstance);

	GDREGISTER_CLASS(Worldspawn);
	GDREGISTER_CLASS(WorldGeometry);
	GDREGISTER_CLASS(Skybox);

	GDREGISTER_CLASS(Counter);
	GDREGISTER_CLASS(Trigger);
	GDREGISTER_CLASS(TriggerForce);

	GDREGISTER_CLASS(VirtualBody3D);

	GDREGISTER_CLASS(Controller);
}

void uninitialize_lux_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}
