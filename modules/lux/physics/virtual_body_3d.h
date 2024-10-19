#ifndef VIRTUAL_BODY_3D_H
#define VIRTUAL_BODY_3D_H

#include "../shared.h"

#include "core/input/input.h"
#include "core/input/input_map.h"
#include "core/input/input_event.h"
#include "scene/3d/camera_3d.h"

class VirtualBody3D : public PhysicsBody3D {
	GDCLASS(VirtualBody3D, PhysicsBody3D);

public: // clang-format off
	static constexpr int32_t	 action_count{ 10 };
	static constexpr const char* action_list[action_count]{ 
		"forward", "backward", "left", "right", "jump", "duck",
		"cam_up", "cam_down", "cam_left", "cam_right"}; // clang-format on
	static constexpr real_t input_motion_mod{ 0.001f * (Math_PI / 180.0f) };

	static inline const Vector3 default_size{ 0.8f, 1.75f, 0.8f };
	static inline const Vector3 default_crouch_size{ 0.8f, 0.875, 0.8f };
	static inline const Vector3 default_camera_offset{ 0.0f, 0.4375f, 0.0f };

	static constexpr real_t mv_speed{ 10.0f };
	static constexpr real_t mv_stopspeed{ 3.125f };
	static constexpr real_t mv_accel{ 10.0f };
	static constexpr real_t mv_air_accel{ 1.0f };
	static constexpr real_t mv_friction{ 6.0f };
	static constexpr real_t mv_gravity{ 25.0f };
	static constexpr real_t mv_jump{ 8.4375f };

	enum Flags {
		PlayerControlled = 1,
		Ducking			 = 1 << 1,
		Ducked			 = 1 << 2,
		SurfaceControl	 = 1 << 3,
		Ladder			 = 1 << 4,
		Knockback		 = 1 << 5,
		WaterJump		 = 1 << 6,
		LadderJump		 = 1 << 7,
		QueueJump		 = 1 << 8,
	};

	struct InputCmd {
		Vector2 dir{};	  // move dir
		Vector2 motion{}; // camera motion
		// HashMap<StringName, double> actions{};
		Array actions{};
	};

	struct CameraModel {
		Vector3	 buffer[2]{};						 // for interpolated position
		Vector3	 adj_origin{};						 // intended interpolated position
		Vector3	 view_angles{};						 // desired rotation euler angles
		Vector3	 base_offset{ 0.0f, 0.4375f, 0.0f }; // offset from body origin
		uint32_t last_physics_tick = 0;
		uint32_t last_update_frame = UINT32_MAX;
		bool	 skip{};
	};

	struct BodyModel {
		ObjectID ground_entity{};
		ObjectID platform{};

		RID ground_rid{};
		RID platform_rid{};

		Vector3 origin[2];

		Vector3 normal{};

		Vector3 velocity{};
		Vector3 angular_velocity{};
		Vector3 platform_velocity{};

		real_t flag_time{ 0.0f };
		real_t duck_time{ 0.0f };
		real_t cl_time{ 0.0f };

		uint32_t surface_flags{ 0 };
		uint8_t	 water_level{ 0 };
	};

	VirtualBody3D();
	~VirtualBody3D();

	// Properties

	void set_player_controlled(bool p_player_controlled);
	bool is_player_controlled() const;

	void	  set_camera(Camera3D* p_camera);
	Camera3D* get_camera() const;

	void			  set_crouch_collider(CollisionShape3D* p_shape);
	CollisionShape3D* get_crouch_collider() const;
	void			  set_crouch_shape(Ref<Shape3D> p_shape);
	Ref<Shape3D>	  get_crouch_shape() const;
	void			  set_stand_collider(CollisionShape3D* p_shape);
	CollisionShape3D* get_stand_collider() const;
	void			  set_stand_shape(Ref<Shape3D> p_shape);
	Ref<Shape3D>	  get_stand_shape() const;

	real_t get_duck_time() const;
	real_t get_flag_time() const;

	void	set_view_angles(const Vector3& p_angles);
	Vector3 get_view_angles() const;

	void apply_properties(const Dictionary& p_properties);
	void build_complete();

	// External forces

	void apply_impulse(const Vector3& p_impulse, const Vector3& p_position = Vector3());
	void push(real_t p_force, Vector3 p_dir, real_t p_mass = 1.0);
	// void grapple(GrappleMode p_mode); // light, heavy, or projectile

	// Movement

	void apply_acceleration(Vector3 p_dir, real_t p_speed, real_t p_accel, double p_delta);
	void apply_friction(double p_delta);
	void apply_gravity(double p_delta);

	bool check_duck();
	bool check_unduck();
	bool check_jump();

	void duck();
	void unduck();
	void jump();

	void check_platform();
	void check_surface_control();
	void check_water_level();

	void move_and_slide(double p_delta);

	// Internal

	void ensure_interpolation_update();

	void enter_tree();
	void ready();
	void input(const Ref<InputEvent>& p_event);

	void update(double p_delta);
	void update_physics(double p_delta);
	void update_timers(double p_delta);
	void update_frame_input(double p_delta);

protected:
	static void _bind_methods();
	void		_notification(int p_what);

private:
	Engine* engine{ nullptr };
	Input*	system_input{ nullptr };

	Ref<BoxShape3D> stand_shape{};
	Ref<BoxShape3D> crouch_shape{};

	ObjectID stand_collider{};
	ObjectID crouch_collider{};
	ObjectID camera{};

	InputCmd	input_cmd{};
	BodyModel	body_data{};
	CameraModel cam_data{};

	uint64_t flags{ 0 };

	real_t msens{ 75.0f };
};

#endif
