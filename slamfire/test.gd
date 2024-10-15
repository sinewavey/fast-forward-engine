extends VirtualBody3D

var velocity := Vector3()
var on_ground := false

func _physics_process(delta: float) -> void:
	
	if !on_ground:
		velocity.y -= 25.0 * delta
	else:
		velocity.y  = 0.0
	
	var params := PhysicsShapeQueryParameters3D.new()
	
	params.shape = $StandShape.shape
	params.motion = velocity * delta
	params.transform = global_transform
	
	
	var result := PhysicsTestMotionResult3D.new()
		
	var trace := get_world_3d().direct_space_state.cast_motion(params)
	prints(trace, on_ground)
	
	if trace[0] == 1.0: 
		on_ground = false
		global_position += velocity * delta
		
	else:
		on_ground = true
		global_position += velocity * delta * trace[0]
	
	
	
	return
