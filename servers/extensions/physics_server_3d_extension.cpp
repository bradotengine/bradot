/**************************************************************************/
/*  physics_server_3d_extension.cpp                                       */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             BRADOT ENGINE                              */
/*                        https://bradotengine.org                        */
/**************************************************************************/
/* Copyright (c) 2024-present Bradot Engine contributors (see AUTHORS.md).*/
/* Copyright (c) 2014-2024 Godot Engine contributors (see AUTHORS.md).    */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "physics_server_3d_extension.h"

bool PhysicsDirectSpaceState3DExtension::is_body_excluded_from_query(const RID &p_body) const {
	return exclude && exclude->has(p_body);
}

thread_local const HashSet<RID> *PhysicsDirectSpaceState3DExtension::exclude = nullptr;

void PhysicsDirectSpaceState3DExtension::_bind_methods() {
	ClassDB::bind_method(D_METHOD("is_body_excluded_from_query", "body"), &PhysicsDirectSpaceState3DExtension::is_body_excluded_from_query);

	BRVIRTUAL_BIND(_intersect_ray, "from", "to", "collision_mask", "collide_with_bodies", "collide_with_areas", "hit_from_inside", "hit_back_faces", "pick_ray", "result");
	BRVIRTUAL_BIND(_intersect_point, "position", "collision_mask", "collide_with_bodies", "collide_with_areas", "results", "max_results");
	BRVIRTUAL_BIND(_intersect_shape, "shape_rid", "transform", "motion", "margin", "collision_mask", "collide_with_bodies", "collide_with_areas", "result_count", "max_results");
	BRVIRTUAL_BIND(_cast_motion, "shape_rid", "transform", "motion", "margin", "collision_mask", "collide_with_bodies", "collide_with_areas", "closest_safe", "closest_unsafe", "info");
	BRVIRTUAL_BIND(_collide_shape, "shape_rid", "transform", "motion", "margin", "collision_mask", "collide_with_bodies", "collide_with_areas", "results", "max_results", "result_count");
	BRVIRTUAL_BIND(_rest_info, "shape_rid", "transform", "motion", "margin", "collision_mask", "collide_with_bodies", "collide_with_areas", "rest_info");
	BRVIRTUAL_BIND(_get_closest_point_to_object_volume, "object", "point");
}

PhysicsDirectSpaceState3DExtension::PhysicsDirectSpaceState3DExtension() {
}

void PhysicsDirectBodyState3DExtension::_bind_methods() {
	BRVIRTUAL_BIND(_get_total_gravity);
	BRVIRTUAL_BIND(_get_total_linear_damp);
	BRVIRTUAL_BIND(_get_total_angular_damp);

	BRVIRTUAL_BIND(_get_center_of_mass);
	BRVIRTUAL_BIND(_get_center_of_mass_local);
	BRVIRTUAL_BIND(_get_principal_inertia_axes);

	BRVIRTUAL_BIND(_get_inverse_mass);
	BRVIRTUAL_BIND(_get_inverse_inertia);
	BRVIRTUAL_BIND(_get_inverse_inertia_tensor);

	BRVIRTUAL_BIND(_set_linear_velocity, "velocity");
	BRVIRTUAL_BIND(_get_linear_velocity);

	BRVIRTUAL_BIND(_set_angular_velocity, "velocity");
	BRVIRTUAL_BIND(_get_angular_velocity);

	BRVIRTUAL_BIND(_set_transform, "transform");
	BRVIRTUAL_BIND(_get_transform);

	BRVIRTUAL_BIND(_get_velocity_at_local_position, "local_position");

	BRVIRTUAL_BIND(_apply_central_impulse, "impulse");
	BRVIRTUAL_BIND(_apply_impulse, "impulse", "position");
	BRVIRTUAL_BIND(_apply_torque_impulse, "impulse");

	BRVIRTUAL_BIND(_apply_central_force, "force");
	BRVIRTUAL_BIND(_apply_force, "force", "position");
	BRVIRTUAL_BIND(_apply_torque, "torque");

	BRVIRTUAL_BIND(_add_constant_central_force, "force");
	BRVIRTUAL_BIND(_add_constant_force, "force", "position");
	BRVIRTUAL_BIND(_add_constant_torque, "torque");

	BRVIRTUAL_BIND(_set_constant_force, "force");
	BRVIRTUAL_BIND(_get_constant_force);

	BRVIRTUAL_BIND(_set_constant_torque, "torque");
	BRVIRTUAL_BIND(_get_constant_torque);

	BRVIRTUAL_BIND(_set_sleep_state, "enabled");
	BRVIRTUAL_BIND(_is_sleeping);

	BRVIRTUAL_BIND(_get_contact_count);

	BRVIRTUAL_BIND(_get_contact_local_position, "contact_idx");
	BRVIRTUAL_BIND(_get_contact_local_normal, "contact_idx");
	BRVIRTUAL_BIND(_get_contact_impulse, "contact_idx");
	BRVIRTUAL_BIND(_get_contact_local_shape, "contact_idx");
	BRVIRTUAL_BIND(_get_contact_local_velocity_at_position, "contact_idx");
	BRVIRTUAL_BIND(_get_contact_collider, "contact_idx");
	BRVIRTUAL_BIND(_get_contact_collider_position, "contact_idx");
	BRVIRTUAL_BIND(_get_contact_collider_id, "contact_idx");
	BRVIRTUAL_BIND(_get_contact_collider_object, "contact_idx");
	BRVIRTUAL_BIND(_get_contact_collider_shape, "contact_idx");
	BRVIRTUAL_BIND(_get_contact_collider_velocity_at_position, "contact_idx");
	BRVIRTUAL_BIND(_get_step);
	BRVIRTUAL_BIND(_integrate_forces);
	BRVIRTUAL_BIND(_get_space_state);
}

PhysicsDirectBodyState3DExtension::PhysicsDirectBodyState3DExtension() {
}

thread_local const HashSet<RID> *PhysicsServer3DExtension::exclude_bodies = nullptr;
thread_local const HashSet<ObjectID> *PhysicsServer3DExtension::exclude_objects = nullptr;

bool PhysicsServer3DExtension::body_test_motion_is_excluding_body(RID p_body) const {
	return exclude_bodies && exclude_bodies->has(p_body);
}

bool PhysicsServer3DExtension::body_test_motion_is_excluding_object(ObjectID p_object) const {
	return exclude_objects && exclude_objects->has(p_object);
}

void PhysicsServer3DExtension::_bind_methods() {
	/* SHAPE API */

	BRVIRTUAL_BIND(_world_boundary_shape_create);
	BRVIRTUAL_BIND(_separation_ray_shape_create);
	BRVIRTUAL_BIND(_sphere_shape_create);
	BRVIRTUAL_BIND(_box_shape_create);
	BRVIRTUAL_BIND(_capsule_shape_create);
	BRVIRTUAL_BIND(_cylinder_shape_create);
	BRVIRTUAL_BIND(_convex_polygon_shape_create);
	BRVIRTUAL_BIND(_concave_polygon_shape_create);
	BRVIRTUAL_BIND(_heightmap_shape_create);
	BRVIRTUAL_BIND(_custom_shape_create);

	BRVIRTUAL_BIND(_shape_set_data, "shape", "data");
	BRVIRTUAL_BIND(_shape_set_custom_solver_bias, "shape", "bias");

	BRVIRTUAL_BIND(_shape_set_margin, "shape", "margin");
	BRVIRTUAL_BIND(_shape_get_margin, "shape");

	BRVIRTUAL_BIND(_shape_get_type, "shape");
	BRVIRTUAL_BIND(_shape_get_data, "shape");
	BRVIRTUAL_BIND(_shape_get_custom_solver_bias, "shape");

	/* SPACE API */

	BRVIRTUAL_BIND(_space_create);
	BRVIRTUAL_BIND(_space_set_active, "space", "active");
	BRVIRTUAL_BIND(_space_is_active, "space");

	BRVIRTUAL_BIND(_space_set_param, "space", "param", "value");
	BRVIRTUAL_BIND(_space_get_param, "space", "param");

	BRVIRTUAL_BIND(_space_get_direct_state, "space");

	BRVIRTUAL_BIND(_space_set_debug_contacts, "space", "max_contacts");
	BRVIRTUAL_BIND(_space_get_contacts, "space");
	BRVIRTUAL_BIND(_space_get_contact_count, "space");

	/* AREA API */

	BRVIRTUAL_BIND(_area_create);

	BRVIRTUAL_BIND(_area_set_space, "area", "space");
	BRVIRTUAL_BIND(_area_get_space, "area");

	BRVIRTUAL_BIND(_area_add_shape, "area", "shape", "transform", "disabled");
	BRVIRTUAL_BIND(_area_set_shape, "area", "shape_idx", "shape");
	BRVIRTUAL_BIND(_area_set_shape_transform, "area", "shape_idx", "transform");
	BRVIRTUAL_BIND(_area_set_shape_disabled, "area", "shape_idx", "disabled");

	BRVIRTUAL_BIND(_area_get_shape_count, "area");
	BRVIRTUAL_BIND(_area_get_shape, "area", "shape_idx");
	BRVIRTUAL_BIND(_area_get_shape_transform, "area", "shape_idx");

	BRVIRTUAL_BIND(_area_remove_shape, "area", "shape_idx");
	BRVIRTUAL_BIND(_area_clear_shapes, "area");

	BRVIRTUAL_BIND(_area_attach_object_instance_id, "area", "id");
	BRVIRTUAL_BIND(_area_get_object_instance_id, "area");

	BRVIRTUAL_BIND(_area_set_param, "area", "param", "value");
	BRVIRTUAL_BIND(_area_set_transform, "area", "transform");

	BRVIRTUAL_BIND(_area_get_param, "area", "param");
	BRVIRTUAL_BIND(_area_get_transform, "area");

	BRVIRTUAL_BIND(_area_set_collision_layer, "area", "layer");
	BRVIRTUAL_BIND(_area_get_collision_layer, "area");

	BRVIRTUAL_BIND(_area_set_collision_mask, "area", "mask");
	BRVIRTUAL_BIND(_area_get_collision_mask, "area");

	BRVIRTUAL_BIND(_area_set_monitorable, "area", "monitorable");
	BRVIRTUAL_BIND(_area_set_ray_pickable, "area", "enable");

	BRVIRTUAL_BIND(_area_set_monitor_callback, "area", "callback");
	BRVIRTUAL_BIND(_area_set_area_monitor_callback, "area", "callback");

	/* BODY API */

	ClassDB::bind_method(D_METHOD("body_test_motion_is_excluding_body", "body"), &PhysicsServer3DExtension::body_test_motion_is_excluding_body);
	ClassDB::bind_method(D_METHOD("body_test_motion_is_excluding_object", "object"), &PhysicsServer3DExtension::body_test_motion_is_excluding_object);

	BRVIRTUAL_BIND(_body_create);

	BRVIRTUAL_BIND(_body_set_space, "body", "space");
	BRVIRTUAL_BIND(_body_get_space, "body");

	BRVIRTUAL_BIND(_body_set_mode, "body", "mode");
	BRVIRTUAL_BIND(_body_get_mode, "body");

	BRVIRTUAL_BIND(_body_add_shape, "body", "shape", "transform", "disabled");
	BRVIRTUAL_BIND(_body_set_shape, "body", "shape_idx", "shape");
	BRVIRTUAL_BIND(_body_set_shape_transform, "body", "shape_idx", "transform");
	BRVIRTUAL_BIND(_body_set_shape_disabled, "body", "shape_idx", "disabled");

	BRVIRTUAL_BIND(_body_get_shape_count, "body");
	BRVIRTUAL_BIND(_body_get_shape, "body", "shape_idx");
	BRVIRTUAL_BIND(_body_get_shape_transform, "body", "shape_idx");

	BRVIRTUAL_BIND(_body_remove_shape, "body", "shape_idx");
	BRVIRTUAL_BIND(_body_clear_shapes, "body");

	BRVIRTUAL_BIND(_body_attach_object_instance_id, "body", "id");
	BRVIRTUAL_BIND(_body_get_object_instance_id, "body");

	BRVIRTUAL_BIND(_body_set_enable_continuous_collision_detection, "body", "enable");
	BRVIRTUAL_BIND(_body_is_continuous_collision_detection_enabled, "body");

	BRVIRTUAL_BIND(_body_set_collision_layer, "body", "layer");
	BRVIRTUAL_BIND(_body_get_collision_layer, "body");

	BRVIRTUAL_BIND(_body_set_collision_mask, "body", "mask");
	BRVIRTUAL_BIND(_body_get_collision_mask, "body");

	BRVIRTUAL_BIND(_body_set_collision_priority, "body", "priority");
	BRVIRTUAL_BIND(_body_get_collision_priority, "body");

	BRVIRTUAL_BIND(_body_set_user_flags, "body", "flags");
	BRVIRTUAL_BIND(_body_get_user_flags, "body");

	BRVIRTUAL_BIND(_body_set_param, "body", "param", "value");
	BRVIRTUAL_BIND(_body_get_param, "body", "param");

	BRVIRTUAL_BIND(_body_reset_mass_properties, "body");

	BRVIRTUAL_BIND(_body_set_state, "body", "state", "value");
	BRVIRTUAL_BIND(_body_get_state, "body", "state");

	BRVIRTUAL_BIND(_body_apply_central_impulse, "body", "impulse");
	BRVIRTUAL_BIND(_body_apply_impulse, "body", "impulse", "position");
	BRVIRTUAL_BIND(_body_apply_torque_impulse, "body", "impulse");

	BRVIRTUAL_BIND(_body_apply_central_force, "body", "force");
	BRVIRTUAL_BIND(_body_apply_force, "body", "force", "position");
	BRVIRTUAL_BIND(_body_apply_torque, "body", "torque");

	BRVIRTUAL_BIND(_body_add_constant_central_force, "body", "force");
	BRVIRTUAL_BIND(_body_add_constant_force, "body", "force", "position");
	BRVIRTUAL_BIND(_body_add_constant_torque, "body", "torque");

	BRVIRTUAL_BIND(_body_set_constant_force, "body", "force");
	BRVIRTUAL_BIND(_body_get_constant_force, "body");

	BRVIRTUAL_BIND(_body_set_constant_torque, "body", "torque");
	BRVIRTUAL_BIND(_body_get_constant_torque, "body");

	BRVIRTUAL_BIND(_body_set_axis_velocity, "body", "axis_velocity");

	BRVIRTUAL_BIND(_body_set_axis_lock, "body", "axis", "lock");
	BRVIRTUAL_BIND(_body_is_axis_locked, "body", "axis");

	BRVIRTUAL_BIND(_body_add_collision_exception, "body", "excepted_body");
	BRVIRTUAL_BIND(_body_remove_collision_exception, "body", "excepted_body");
	BRVIRTUAL_BIND(_body_get_collision_exceptions, "body");

	BRVIRTUAL_BIND(_body_set_max_contacts_reported, "body", "amount");
	BRVIRTUAL_BIND(_body_get_max_contacts_reported, "body");

	BRVIRTUAL_BIND(_body_set_contacts_reported_depth_threshold, "body", "threshold");
	BRVIRTUAL_BIND(_body_get_contacts_reported_depth_threshold, "body");

	BRVIRTUAL_BIND(_body_set_omit_force_integration, "body", "enable");
	BRVIRTUAL_BIND(_body_is_omitting_force_integration, "body");

	BRVIRTUAL_BIND(_body_set_state_sync_callback, "body", "callable");
	BRVIRTUAL_BIND(_body_set_force_integration_callback, "body", "callable", "userdata");

	BRVIRTUAL_BIND(_body_set_ray_pickable, "body", "enable");

	BRVIRTUAL_BIND(_body_test_motion, "body", "from", "motion", "margin", "max_collisions", "collide_separation_ray", "recovery_as_collision", "result");

	BRVIRTUAL_BIND(_body_get_direct_state, "body");

	/* SOFT BODY API */

	BRVIRTUAL_BIND(_soft_body_create);

	BRVIRTUAL_BIND(_soft_body_update_rendering_server, "body", "rendering_server_handler");

	BRVIRTUAL_BIND(_soft_body_set_space, "body", "space");
	BRVIRTUAL_BIND(_soft_body_get_space, "body");

	BRVIRTUAL_BIND(_soft_body_set_ray_pickable, "body", "enable");

	BRVIRTUAL_BIND(_soft_body_set_collision_layer, "body", "layer");
	BRVIRTUAL_BIND(_soft_body_get_collision_layer, "body");

	BRVIRTUAL_BIND(_soft_body_set_collision_mask, "body", "mask");
	BRVIRTUAL_BIND(_soft_body_get_collision_mask, "body");

	BRVIRTUAL_BIND(_soft_body_add_collision_exception, "body", "body_b");
	BRVIRTUAL_BIND(_soft_body_remove_collision_exception, "body", "body_b");
	BRVIRTUAL_BIND(_soft_body_get_collision_exceptions, "body");

	BRVIRTUAL_BIND(_soft_body_set_state, "body", "state", "variant");
	BRVIRTUAL_BIND(_soft_body_get_state, "body", "state");

	BRVIRTUAL_BIND(_soft_body_set_transform, "body", "transform");

	BRVIRTUAL_BIND(_soft_body_set_simulation_precision, "body", "simulation_precision");
	BRVIRTUAL_BIND(_soft_body_get_simulation_precision, "body");

	BRVIRTUAL_BIND(_soft_body_set_total_mass, "body", "total_mass");
	BRVIRTUAL_BIND(_soft_body_get_total_mass, "body");

	BRVIRTUAL_BIND(_soft_body_set_linear_stiffness, "body", "linear_stiffness");
	BRVIRTUAL_BIND(_soft_body_get_linear_stiffness, "body");

	BRVIRTUAL_BIND(_soft_body_set_pressure_coefficient, "body", "pressure_coefficient");
	BRVIRTUAL_BIND(_soft_body_get_pressure_coefficient, "body");

	BRVIRTUAL_BIND(_soft_body_set_damping_coefficient, "body", "damping_coefficient");
	BRVIRTUAL_BIND(_soft_body_get_damping_coefficient, "body");

	BRVIRTUAL_BIND(_soft_body_set_drag_coefficient, "body", "drag_coefficient");
	BRVIRTUAL_BIND(_soft_body_get_drag_coefficient, "body");

	BRVIRTUAL_BIND(_soft_body_set_mesh, "body", "mesh");

	BRVIRTUAL_BIND(_soft_body_get_bounds, "body");

	BRVIRTUAL_BIND(_soft_body_move_point, "body", "point_index", "global_position");
	BRVIRTUAL_BIND(_soft_body_get_point_global_position, "body", "point_index");

	BRVIRTUAL_BIND(_soft_body_remove_all_pinned_points, "body");
	BRVIRTUAL_BIND(_soft_body_pin_point, "body", "point_index", "pin");
	BRVIRTUAL_BIND(_soft_body_is_point_pinned, "body", "point_index");

	/* JOINT API */

	BRVIRTUAL_BIND(_joint_create);
	BRVIRTUAL_BIND(_joint_clear, "joint");

	BRVIRTUAL_BIND(_joint_make_pin, "joint", "body_A", "local_A", "body_B", "local_B");

	BRVIRTUAL_BIND(_pin_joint_set_param, "joint", "param", "value");
	BRVIRTUAL_BIND(_pin_joint_get_param, "joint", "param");

	BRVIRTUAL_BIND(_pin_joint_set_local_a, "joint", "local_A");
	BRVIRTUAL_BIND(_pin_joint_get_local_a, "joint");

	BRVIRTUAL_BIND(_pin_joint_set_local_b, "joint", "local_B");
	BRVIRTUAL_BIND(_pin_joint_get_local_b, "joint");

	BRVIRTUAL_BIND(_joint_make_hinge, "joint", "body_A", "hinge_A", "body_B", "hinge_B");
	BRVIRTUAL_BIND(_joint_make_hinge_simple, "joint", "body_A", "pivot_A", "axis_A", "body_B", "pivot_B", "axis_B");

	BRVIRTUAL_BIND(_hinge_joint_set_param, "joint", "param", "value");
	BRVIRTUAL_BIND(_hinge_joint_get_param, "joint", "param");

	BRVIRTUAL_BIND(_hinge_joint_set_flag, "joint", "flag", "enabled");
	BRVIRTUAL_BIND(_hinge_joint_get_flag, "joint", "flag");

	BRVIRTUAL_BIND(_joint_make_slider, "joint", "body_A", "local_ref_A", "body_B", "local_ref_B");

	BRVIRTUAL_BIND(_slider_joint_set_param, "joint", "param", "value");
	BRVIRTUAL_BIND(_slider_joint_get_param, "joint", "param");

	BRVIRTUAL_BIND(_joint_make_cone_twist, "joint", "body_A", "local_ref_A", "body_B", "local_ref_B");

	BRVIRTUAL_BIND(_cone_twist_joint_set_param, "joint", "param", "value");
	BRVIRTUAL_BIND(_cone_twist_joint_get_param, "joint", "param");

	BRVIRTUAL_BIND(_joint_make_generic_6dof, "joint", "body_A", "local_ref_A", "body_B", "local_ref_B");

	BRVIRTUAL_BIND(_generic_6dof_joint_set_param, "joint", "axis", "param", "value");
	BRVIRTUAL_BIND(_generic_6dof_joint_get_param, "joint", "axis", "param");

	BRVIRTUAL_BIND(_generic_6dof_joint_set_flag, "joint", "axis", "flag", "enable");
	BRVIRTUAL_BIND(_generic_6dof_joint_get_flag, "joint", "axis", "flag");

	BRVIRTUAL_BIND(_joint_get_type, "joint");

	BRVIRTUAL_BIND(_joint_set_solver_priority, "joint", "priority");
	BRVIRTUAL_BIND(_joint_get_solver_priority, "joint");

	BRVIRTUAL_BIND(_joint_disable_collisions_between_bodies, "joint", "disable");
	BRVIRTUAL_BIND(_joint_is_disabled_collisions_between_bodies, "joint");

	BRVIRTUAL_BIND(_free_rid, "rid");

	BRVIRTUAL_BIND(_set_active, "active");

	BRVIRTUAL_BIND(_init);
	BRVIRTUAL_BIND(_step, "step");
	BRVIRTUAL_BIND(_sync);
	BRVIRTUAL_BIND(_flush_queries);
	BRVIRTUAL_BIND(_end_sync);
	BRVIRTUAL_BIND(_finish);

	BRVIRTUAL_BIND(_is_flushing_queries);
	BRVIRTUAL_BIND(_get_process_info, "process_info");
}

PhysicsServer3DExtension::PhysicsServer3DExtension() {
}

PhysicsServer3DExtension::~PhysicsServer3DExtension() {
}
