#include "shared.h"

ObjectID Lux::get_id_or_null(Node* p_node) {
	return p_node ? p_node->get_instance_id() : ObjectID();
}

NodePath Lux::get_path_or_null(Node* p_node) {
	return p_node ? p_node->get_path() : NodePath();
}

void Lux::add_child_persist(Node* p_parent,
	Node*						  p_child,
	bool						  p_readable_name,
	Lux::AddChildOwner			  p_owner) {
	// clang-format off

	ERR_FAIL_COND_MSG(p_parent == nullptr, "Cannot add child to null parent node.");
	ERR_FAIL_COND_MSG(p_child == nullptr, "Cannot add null child node.");
	ERR_FAIL_COND_MSG(!p_parent->is_inside_tree() && p_owner == Lux::OWNER_ROOT, "Cannot add child to root node outside tree.");

	p_parent->add_child(p_child, p_readable_name);

	auto fnc = [&](Node* n) { p_child->set_owner(n); };

	switch (p_owner) { 
		case Lux::OWNER_ROOT: 			{ fnc(SceneTree::get_singleton()->get_edited_scene_root()); break; 	}
		case Lux::OWNER_PARENT_OWNER: 	{ fnc(p_parent->get_owner()); break; 								}
		case Lux::OWNER_PARENT: 		{ fnc(p_parent); break; 											}
	} // clang-format on
}

void Lux::add_sibling_persist(Node* p_caller, Node* p_new, bool p_readable_name) {
	ERR_FAIL_COND_MSG(p_caller == nullptr, "Cannot add sibling to null node.");
	ERR_FAIL_COND_MSG(p_new == nullptr, "Cannot add null sibling node.");
	p_caller->add_sibling(p_new, p_readable_name);
	p_new->set_owner(SceneTree::get_singleton()->get_edited_scene_root());
}

void Lux::FGD::set_targetname(Node* p_node, const StringName& p_name) {
	if (p_name.is_empty()) {
		StringName val = p_node->get_meta("targetname", StringName());
		if (!val.is_empty() && p_node->is_in_group(val)) {
			p_node->remove_from_group(val);
		}
		p_node->remove_meta("targetname");
	} else {
		p_node->add_to_group(p_name, true);
		p_node->set_meta("targetname", p_name);
	}
}

void Lux::FGD::apply_properties(Node* p_node, const Dictionary& p_properties) {
	ERR_FAIL_COND_MSG(p_node == nullptr, "Cannot apply properties to null node.");

	for (Variant key : p_properties.keys()) {
		if (!key.is_string()) {
			WARN_PRINT("Non-string property key ignored: " + key.stringify());
			continue;
		}

		// classname and origin might cause problems in godot with set so they are skipped
		// flags are skipped to avoid clashing with spawnflags
		if (key.operator==("classname") || key.operator==("origin") || key.operator==("flags")) {
			continue;
		}

		Variant value = p_properties[key];

		if (key.operator==("targetname")) {
			FGD::set_targetname(p_node, value);
			continue;
		}

		if (key.operator==("spawnflags")) {
			key = StringName{ "flags" };
		}

		bool success{};
		p_node->set(key, value, &success);

		if (success) {
			continue;
		}

		p_node->set("metadata/" + key.stringify(), value, &success);

		if (!success) {
			WARN_PRINT("Failed to apply property: " + key.stringify());
		}
	}
}

void Lux::FGD::finalize_mesh_children(Node* p_parent) {
	int cs_value = p_parent->get_meta("_cs", 1);
	int rs_value = p_parent->get_meta("_rs", true);

	for (int i = 0; i < p_parent->get_child_count(); i++) {
		auto child = p_parent->get_child(i);
		if (child->is_class("GeometryInstance3D")) {
			child->call(
				"set_cast_shadows_setting", GeometryInstance3D::ShadowCastingSetting(cs_value));
		}

		// if (child->is_class("MeshInstance3D")) {
		// handle recieve shadow override though likely will be material based
		// }
	}
}

void Lux::FGD::finalize_entity(Node* p_node) {
	List<StringName> md{};
	p_node->get_meta_list(&md);

	Lux::FGD::finalize_mesh_children(p_node);

	// for (auto meta : md) {
	// 	p_node->remove_meta(meta);
	// }
}
