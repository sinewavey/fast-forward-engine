
#include "register_types.h"

#include "core/object/class_db.h"

#include "shared.h"

#include "physics/virtual_body_3d.h"
#include "physics/world_geometry.h"
#include "physics/io_logic.h"

void initialize_lux_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	print_line("Initializing Fast Forward types");

	GDREGISTER_CLASS(VirtualBody3D);
	GDREGISTER_CLASS(WorldGeometry);
	GDREGISTER_CLASS(Trigger);
	GDREGISTER_CLASS(Counter);
}

void uninitialize_lux_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}
