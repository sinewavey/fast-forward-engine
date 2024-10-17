#ifndef VIRTUAL_BODY_3D_H
#define VIRTUAL_BODY_3D_H

#include "../shared.h"

#include "core/input/input.h"
#include "core/input/input_map.h"
#include "core/input/input_event.h"
#include "scene/3d/camera_3d.h"

class VirtualBody3D : public PhysicsBody3D {
	GDCLASS(VirtualBody3D, PhysicsBody3D);

	struct BodyModel {
		ObjectID stand_collider{};
		ObjectID crouch_collider{};

		ObjectID ground_entity{};
		ObjectID platform{};

		RID ground_rid{};
		RID platform_rid{};

		Ref<Shape3D> stand_shape{ nullptr };
		Ref<Shape3D> crouch_shape{ nullptr };

		Vector2 input_dir{};
		Vector2 sway{};

		Vector3 origin[2];
		Vector3 view_angles{};
		Vector3 view_offset{ 0.0f, 0.4375f, 0.0f };

		Vector3 normal{};

		Vector3 velocity{};
		Vector3 angular_velocity{};
		Vector3 platform_velocity{};

		real_t flag_time{ 0.0 };
		real_t duck_time{ 0.0 };

		uint32_t surface_flags{ 0 };
		uint8_t	 water_level{ 0 };
	};


public:
	static constexpr real_t		 input_motion_mod{ 0.001f * (Math_PI / 180.0f) };
	static constexpr int32_t	 action_count{ 18 };
	static constexpr const char* action_list[action_count]{ // clang-format off
		"forward", "backward", "left", "right", "jump", "duck",
		"cam_up", "cam_down", "cam_left", "cam_right",
		"attack", "attack2", "arm", "leg", "grapple", "pda", "use", "hkit"
	}; // clang-format on

	enum Flags {
		PlayerControlled = 1,
		Ducking			 = 1 << 1,
		Ducked			 = 1 << 2,
		SurfaceControl	 = 1 << 3,
		Knockback		 = 1 << 4,
	};

	VirtualBody3D();
	~VirtualBody3D();

	void set_player_controlled(bool p_player_controlled);
	bool is_player_controlled() const;

	void	  set_camera(Camera3D* p_camera);
	Camera3D* get_camera() const;

	void			  set_crouch_collider(CollisionShape3D* p_shape);
	CollisionShape3D* get_crouch_collider() const;
	void			  set_stand_collider(CollisionShape3D* p_shape);
	CollisionShape3D* get_stand_collider() const;

	void		 set_crouch_shape(Ref<Shape3D> p_shape);
	Ref<Shape3D> get_crouch_shape() const;
	void		 set_stand_shape(Ref<Shape3D> p_shape);
	Ref<Shape3D> get_stand_shape() const;

	real_t get_duck_time() const;
	real_t get_flag_time() const;

	void	set_view_angles(const Vector3& p_angles);
	Vector3 get_view_angles() const;

	void apply_properties(const Dictionary& p_properties);
	void build_complete();

	void apply_impulse(const Vector3& p_impulse, const Vector3& p_position = Vector3());
	void push(real_t p_force, Vector3 p_dir, real_t p_mass = 1.0);

	void check_duck();
	void check_unduck();
	void check_platform();
	void check_surface_control();
	void check_water_level();

	void move_and_slide(double p_delta);

	void enter_tree();
	void ready();
	void input(const Ref<InputEvent>& p_event);

	void update(double p_delta);
	void update_physics(double p_delta);
	void update_timers(double p_delta);

	void tick();

protected:
	static void _bind_methods();
	void		_notification(int p_what);

private:
	Engine* engine{ nullptr };
	Input*	system_input{ nullptr };

	ObjectID  camera{};
	BodyModel model{};
	
	HashMap<StringName, real_t> actions{};

	Vector3 buffer[2]{};

	real_t msens{ 75.0f };

	uint64_t flags{ 0 };

	uint32_t last_physics_tick = 0;
	uint32_t last_update_tick  = UINT32_MAX;
	
};

#endif
