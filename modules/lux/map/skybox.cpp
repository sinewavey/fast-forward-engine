#include "q_compat.h"

void Skybox::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_camera", "p_camera"), &Skybox::set_camera);
	ClassDB::bind_method(D_METHOD("get_camera"), &Skybox::get_camera);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "camera", PROPERTY_HINT_NODE_TYPE, "Camera3D"),
		"set_camera",
		"get_camera");
}

void Skybox::set_camera(Camera3D* p_camera) {
	camera = p_camera;
}

Camera3D* Skybox::get_camera() const {
	return camera;
}

void Skybox::clear_visuals() {
	Vector<Node*> freed{};

	for (auto node : Lux::child_view(this)) {
		if (node == camera) {
			continue;
		}
		freed.append(node);
	}

	for (auto node : freed) {
		node->queue_free();
	}
}
