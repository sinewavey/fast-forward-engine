#ifndef LUX_PLATFORM_BODY_3D
#define LUX_PLATFORM_BODY_3D

#include "../shared.h"

#include "scene/3d/physics/animatable_body_3d.h"
#include "scene/3d/path_3d.h"

class PlatformBody3D : public AnimatableBody3D {
	GDCLASS(PlatformBody3D, AnimatableBody3D);

public:
	void		  set_path_follower(PathFollow3D* p_node);
	PathFollow3D* get_path_follower() const;

	void	set_path_3d(Path3D* p_node);
	Path3D* get_path_3d() const;

	void	 set_surface_flags(uint32_t p_flags);
	uint32_t get_surface_flags() const;

	void apply_properties(const Dictionary& p_properties);
	void build_complete();

	void use(Node* p_activator);

	void move_forward();
	void move_reverse();

	void toggle();

	void move_to_progress(float p_progress);

	PlatformBody3D();

protected:
	static void _bind_methods();
	void		_notification(int p_what);

private:
	ObjectID path_follower{};
	ObjectID path_3d{};

	real_t	 move_progress{ 0.0 };
	uint32_t surface_flags{ 0 };

	Lux::MoverState state{ Lux::MOVER_IDLE };
};

#endif
