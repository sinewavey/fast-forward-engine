#ifndef LUX_VIRTUAL_BODY_3D_H
#define LUX_VIRTUAL_BODY_3D_H

#include "../shared.h"

#include "core/input/input.h"
#include "core/input/input_map.h"
#include "core/input/input_event.h"

#include "scene/main/timer.h"
#include "scene/3d/camera_3d.h"
#include "scene/3d/physics/ray_cast_3d.h"

class VirtualBody3D : public PhysicsBody3D {
	GDCLASS(VirtualBody3D, PhysicsBody3D);

public: // clang-format off
	static constexpr real_t input_motion_mod{ 0.001f * (Lux::M_PI / 180.0f) }; // applied to screen relative mouse motion to scale
	static constexpr real_t stand_view_mod{ 0.821f }; // view height as percentage of AABB height
	static constexpr real_t crouch_view_mod{ 0.777f }; // view height as percentage of AABB height

	static inline const AABB stand_aabb{ {0.0f, 0.0f, 0.0f}, { 0.8f, 1.75, 0.8f } };
	static inline const AABB crouch_aabb{ {0.0f, 0.0f, 0.0f}, { 0.8f, 0.875f, 0.8f } };

	// static constexpr real_t get_view_height(real_t p_height, real_t p_mod) { return (-p_height * 0.5f) + (p_height * p_mod); };
	static constexpr real_t stand_view_height{ 0.56175 };
	static constexpr real_t crouch_view_height{ 0.242375 };

	static constexpr real_t mv_speed{ 10.0f }, mv_stop_speed{ 3.125f }; // move speeds
	static constexpr real_t mv_duck_mod{ 0.35f }, mv_swim_mod{ 0.5f }, mv_wade_mod{ 0.7f }; // move speed modifiers
	static constexpr real_t mv_accel{ 10.0f }, mv_air_accel{ 1.0f }, mv_water_accel{ 4.0f }; // move acceleration constants
	static constexpr real_t mv_friction{ 6.0f }; // ground friction
	static constexpr real_t mv_gravity{ 25.0f };
	static constexpr real_t mv_jump{ 8.4375f };

	static constexpr real_t mv_ground_dist{ 0.1f }, mv_depen_margin{ 0.004f };
	static constexpr real_t mv_step_height{ 0.5625 };

	static constexpr int mv_max_slides{ 4 };

	// clang-format on

	enum Flags {
		PlayerControlled = 1,
		Ducking			 = 1 << 1,
		Ducked			 = 1 << 2,
		GroundPlane		 = 1 << 3,
		SurfaceControl	 = 1 << 4,
		Ladder			 = 1 << 5,
		Knockback		 = 1 << 6,
		WaterJump		 = 1 << 7,
		LadderJump		 = 1 << 8,
		QueueJump		 = 1 << 9,
		Grappled		 = 1 << 10,
	};

	struct InputCmd {
		Vector2 dir{};	  // move dir
		Vector2 motion{}; // camera motion
		real_t	upmove{ 0.0f };
		bool	grapple{ false };
	};

	struct ViewModel {
		Vector3	 adj_origin{};								   // intended interpolated position
		Vector3	 angles{};									   // desired rotation euler angles
		Vector3	 base_offset{ 0.0f, stand_view_height, 0.0f }; // offset from body origin
		Vector3	 buffer[2]{};			// this and last physics tick position
		uint32_t last_physics_tick = 0; // needed to determine when to push buffer into new frame
		uint32_t last_update_frame = UINT32_MAX;
		bool	 skip{};
	};

	struct PhysicsModel {
		ObjectID ground_entity{};
		ObjectID platform{};
		RID		 ground_rid{};
		RID		 platform_rid{};

		Vector3 origin[2];
		Vector3 normal{};
		Vector3 velocity{};
		Vector3 angular_velocity{};
		Vector3 platform_velocity{};

		real_t flag_time{ 0.0f };
		real_t duck_time{ 0.0f };

		uint32_t surface_flags{ 0 };
		uint8_t	 water_level{ 0 };
	};

	struct Trace {
		Vector3 position;
		Vector3 normal{ 0.0f, 1.0f, 0.0f };
		real_t	safe{ 1.0f };
		real_t	unsafe{ 1.0f };
		Trace(Vector3 p_pos) : position(p_pos) {};
		Trace(Vector3 p_pos, Vector3 p_normal, real_t p_safe_fraction, real_t p_unsafe_fraction) :
			position(p_pos),
			normal(p_normal),
			safe{ p_safe_fraction },
			unsafe{ p_unsafe_fraction } {};
	};

	VirtualBody3D();
	~VirtualBody3D();

	// Properties

	void set_player_controlled(bool p_player_controlled);
	bool is_player_controlled() const;

	void	  set_camera(Camera3D* p_camera);
	Camera3D* get_camera() const;

	void	   set_raycast(RayCast3D* p_raycast);
	RayCast3D* get_raycast() const;

	void   set_duck_timer(Timer* p_timer);
	Timer* get_duck_timer() const;

	void			  set_crouch_collider(CollisionShape3D* p_shape);
	CollisionShape3D* get_crouch_collider() const;

	void			  set_stand_collider(CollisionShape3D* p_shape);
	CollisionShape3D* get_stand_collider() const;

	real_t get_duck_time() const;
	real_t get_flag_time() const;

	void   set_msens(real_t p_msens);
	real_t get_msens() const;

	void	set_view_angles(const Vector3& p_angles);
	Vector3 get_view_angles() const;

	void apply_properties(const Dictionary& p_properties);
	void build_complete();

	// External forces

	void add_knockback_time(double p_time);
	void apply_impulse(const Vector3& p_impulse, const Vector3& p_position = Vector3());
	void push(real_t p_force, Vector3 p_dir, real_t p_mass = 1.0);
	// void grapple(GrappleMode p_mode); // light, heavy, or projectile

	// Movement

	void apply_acceleration(Vector3 p_dir, real_t p_speed, real_t p_accel, double p_delta);
	void apply_friction(double p_delta);

	void check_duck();
	void set_duck(bool p_ducked);

	bool check_jump();
	void jump();

	void check_platform();
	void check_surface_control();
	void check_water_level();

	bool slide_move(double p_delta);
	bool step_slide_move(double p_delta);

	// Internal

	void ensure_interpolation_update();

	void enter_tree();
	void ready();
	void input(const Ref<InputEvent>& p_event);

	void update(double p_delta);
	void update_physics(double p_delta);
	void update_frame_input();

protected:
	static void _bind_methods();
	void		_notification(int p_what);

private:
	Trace cast_trace(Vector3 p_from, Vector3 p_to);

	Engine* engine{ nullptr };
	Input*	system_input{ nullptr };

	Ref<BoxShape3D> stand_shape{};
	Ref<BoxShape3D> crouch_shape{};

	ObjectID camera{};
	ObjectID duck_timer{};
	ObjectID crouch_collider{};
	ObjectID stand_collider{};
	ObjectID raycast{};

	InputCmd	 input_cmd{};
	PhysicsModel phys{};
	ViewModel	 view{};

	uint32_t flags{ 0 };

	real_t msens{ 75.0f };
};

#endif
