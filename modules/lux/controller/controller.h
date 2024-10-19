#ifndef LUX_CONTROLLER_H
#define LUX_CONTROLLER_H

#include "../shared.h"

class Controller : public Node3D {
    GDCLASS(Controller, Node3D);

    protected:
		static void _bind_methods() {};
};

#endif