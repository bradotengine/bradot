/**************************************************************************/
/*  bradot_physics_server_3d.cpp                                           */
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

#include "bradot_physics_server_3d.h"

#include "bradot_body_direct_state_3d.h"
#include "bradot_broad_phase_3d_bvh.h"
#include "joints/bradot_cone_twist_joint_3d.h"
#include "joints/bradot_generic_6dof_joint_3d.h"
#include "joints/bradot_hinge_joint_3d.h"
#include "joints/bradot_pin_joint_3d.h"
#include "joints/bradot_slider_joint_3d.h"

#include "core/debugger/engine_debugger.h"
#include "core/os/os.h"

#define FLUSH_QUERY_CHECK(m_object) \
	ERR_FAIL_COND_MSG(m_object->get_space() && flushing_queries, "Can't change this state while flushing queries. Use call_deferred() or set_deferred() to change monitoring state instead.");

RID BradotPhysicsServer3D::world_boundary_shape_create() {
	BradotShape3D *shape = memnew(BradotWorldBoundaryShape3D);
	RID rid = shape_owner.make_rid(shape);
	shape->set_self(rid);
	return rid;
}
RID BradotPhysicsServer3D::separation_ray_shape_create() {
	BradotShape3D *shape = memnew(BradotSeparationRayShape3D);
	RID rid = shape_owner.make_rid(shape);
	shape->set_self(rid);
	return rid;
}
RID BradotPhysicsServer3D::sphere_shape_create() {
	BradotShape3D *shape = memnew(BradotSphereShape3D);
	RID rid = shape_owner.make_rid(shape);
	shape->set_self(rid);
	return rid;
}
RID BradotPhysicsServer3D::box_shape_create() {
	BradotShape3D *shape = memnew(BradotBoxShape3D);
	RID rid = shape_owner.make_rid(shape);
	shape->set_self(rid);
	return rid;
}
RID BradotPhysicsServer3D::capsule_shape_create() {
	BradotShape3D *shape = memnew(BradotCapsuleShape3D);
	RID rid = shape_owner.make_rid(shape);
	shape->set_self(rid);
	return rid;
}
RID BradotPhysicsServer3D::cylinder_shape_create() {
	BradotShape3D *shape = memnew(BradotCylinderShape3D);
	RID rid = shape_owner.make_rid(shape);
	shape->set_self(rid);
	return rid;
}
RID BradotPhysicsServer3D::convex_polygon_shape_create() {
	BradotShape3D *shape = memnew(BradotConvexPolygonShape3D);
	RID rid = shape_owner.make_rid(shape);
	shape->set_self(rid);
	return rid;
}
RID BradotPhysicsServer3D::concave_polygon_shape_create() {
	BradotShape3D *shape = memnew(BradotConcavePolygonShape3D);
	RID rid = shape_owner.make_rid(shape);
	shape->set_self(rid);
	return rid;
}
RID BradotPhysicsServer3D::heightmap_shape_create() {
	BradotShape3D *shape = memnew(BradotHeightMapShape3D);
	RID rid = shape_owner.make_rid(shape);
	shape->set_self(rid);
	return rid;
}
RID BradotPhysicsServer3D::custom_shape_create() {
	ERR_FAIL_V(RID());
}

void BradotPhysicsServer3D::shape_set_data(RID p_shape, const Variant &p_data) {
	BradotShape3D *shape = shape_owner.get_or_null(p_shape);
	ERR_FAIL_NULL(shape);
	shape->set_data(p_data);
}

void BradotPhysicsServer3D::shape_set_custom_solver_bias(RID p_shape, real_t p_bias) {
	BradotShape3D *shape = shape_owner.get_or_null(p_shape);
	ERR_FAIL_NULL(shape);
	shape->set_custom_bias(p_bias);
}

PhysicsServer3D::ShapeType BradotPhysicsServer3D::shape_get_type(RID p_shape) const {
	const BradotShape3D *shape = shape_owner.get_or_null(p_shape);
	ERR_FAIL_NULL_V(shape, SHAPE_CUSTOM);
	return shape->get_type();
}

Variant BradotPhysicsServer3D::shape_get_data(RID p_shape) const {
	const BradotShape3D *shape = shape_owner.get_or_null(p_shape);
	ERR_FAIL_NULL_V(shape, Variant());
	ERR_FAIL_COND_V(!shape->is_configured(), Variant());
	return shape->get_data();
}

void BradotPhysicsServer3D::shape_set_margin(RID p_shape, real_t p_margin) {
}

real_t BradotPhysicsServer3D::shape_get_margin(RID p_shape) const {
	return 0.0;
}

real_t BradotPhysicsServer3D::shape_get_custom_solver_bias(RID p_shape) const {
	const BradotShape3D *shape = shape_owner.get_or_null(p_shape);
	ERR_FAIL_NULL_V(shape, 0);
	return shape->get_custom_bias();
}

RID BradotPhysicsServer3D::space_create() {
	BradotSpace3D *space = memnew(BradotSpace3D);
	RID id = space_owner.make_rid(space);
	space->set_self(id);
	RID area_id = area_create();
	BradotArea3D *area = area_owner.get_or_null(area_id);
	ERR_FAIL_NULL_V(area, RID());
	space->set_default_area(area);
	area->set_space(space);
	area->set_priority(-1);
	RID sgb = body_create();
	body_set_space(sgb, id);
	body_set_mode(sgb, BODY_MODE_STATIC);
	space->set_static_global_body(sgb);

	return id;
}

void BradotPhysicsServer3D::space_set_active(RID p_space, bool p_active) {
	BradotSpace3D *space = space_owner.get_or_null(p_space);
	ERR_FAIL_NULL(space);
	if (p_active) {
		active_spaces.insert(space);
	} else {
		active_spaces.erase(space);
	}
}

bool BradotPhysicsServer3D::space_is_active(RID p_space) const {
	const BradotSpace3D *space = space_owner.get_or_null(p_space);
	ERR_FAIL_NULL_V(space, false);

	return active_spaces.has(space);
}

void BradotPhysicsServer3D::space_set_param(RID p_space, SpaceParameter p_param, real_t p_value) {
	BradotSpace3D *space = space_owner.get_or_null(p_space);
	ERR_FAIL_NULL(space);

	space->set_param(p_param, p_value);
}

real_t BradotPhysicsServer3D::space_get_param(RID p_space, SpaceParameter p_param) const {
	const BradotSpace3D *space = space_owner.get_or_null(p_space);
	ERR_FAIL_NULL_V(space, 0);
	return space->get_param(p_param);
}

PhysicsDirectSpaceState3D *BradotPhysicsServer3D::space_get_direct_state(RID p_space) {
	BradotSpace3D *space = space_owner.get_or_null(p_space);
	ERR_FAIL_NULL_V(space, nullptr);
	ERR_FAIL_COND_V_MSG((using_threads && !doing_sync) || space->is_locked(), nullptr, "Space state is inaccessible right now, wait for iteration or physics process notification.");

	return space->get_direct_state();
}

void BradotPhysicsServer3D::space_set_debug_contacts(RID p_space, int p_max_contacts) {
	BradotSpace3D *space = space_owner.get_or_null(p_space);
	ERR_FAIL_NULL(space);
	space->set_debug_contacts(p_max_contacts);
}

Vector<Vector3> BradotPhysicsServer3D::space_get_contacts(RID p_space) const {
	BradotSpace3D *space = space_owner.get_or_null(p_space);
	ERR_FAIL_NULL_V(space, Vector<Vector3>());
	return space->get_debug_contacts();
}

int BradotPhysicsServer3D::space_get_contact_count(RID p_space) const {
	BradotSpace3D *space = space_owner.get_or_null(p_space);
	ERR_FAIL_NULL_V(space, 0);
	return space->get_debug_contact_count();
}

RID BradotPhysicsServer3D::area_create() {
	BradotArea3D *area = memnew(BradotArea3D);
	RID rid = area_owner.make_rid(area);
	area->set_self(rid);
	return rid;
}

void BradotPhysicsServer3D::area_set_space(RID p_area, RID p_space) {
	BradotArea3D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);

	BradotSpace3D *space = nullptr;
	if (p_space.is_valid()) {
		space = space_owner.get_or_null(p_space);
		ERR_FAIL_NULL(space);
	}

	if (area->get_space() == space) {
		return; //pointless
	}

	area->clear_constraints();
	area->set_space(space);
}

RID BradotPhysicsServer3D::area_get_space(RID p_area) const {
	BradotArea3D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL_V(area, RID());

	BradotSpace3D *space = area->get_space();
	if (!space) {
		return RID();
	}
	return space->get_self();
}

void BradotPhysicsServer3D::area_add_shape(RID p_area, RID p_shape, const Transform3D &p_transform, bool p_disabled) {
	BradotArea3D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);

	BradotShape3D *shape = shape_owner.get_or_null(p_shape);
	ERR_FAIL_NULL(shape);

	area->add_shape(shape, p_transform, p_disabled);
}

void BradotPhysicsServer3D::area_set_shape(RID p_area, int p_shape_idx, RID p_shape) {
	BradotArea3D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);

	BradotShape3D *shape = shape_owner.get_or_null(p_shape);
	ERR_FAIL_NULL(shape);
	ERR_FAIL_COND(!shape->is_configured());

	area->set_shape(p_shape_idx, shape);
}

void BradotPhysicsServer3D::area_set_shape_transform(RID p_area, int p_shape_idx, const Transform3D &p_transform) {
	BradotArea3D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);

	area->set_shape_transform(p_shape_idx, p_transform);
}

int BradotPhysicsServer3D::area_get_shape_count(RID p_area) const {
	BradotArea3D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL_V(area, -1);

	return area->get_shape_count();
}

RID BradotPhysicsServer3D::area_get_shape(RID p_area, int p_shape_idx) const {
	BradotArea3D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL_V(area, RID());

	BradotShape3D *shape = area->get_shape(p_shape_idx);
	ERR_FAIL_NULL_V(shape, RID());

	return shape->get_self();
}

Transform3D BradotPhysicsServer3D::area_get_shape_transform(RID p_area, int p_shape_idx) const {
	BradotArea3D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL_V(area, Transform3D());

	return area->get_shape_transform(p_shape_idx);
}

void BradotPhysicsServer3D::area_remove_shape(RID p_area, int p_shape_idx) {
	BradotArea3D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);

	area->remove_shape(p_shape_idx);
}

void BradotPhysicsServer3D::area_clear_shapes(RID p_area) {
	BradotArea3D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);

	while (area->get_shape_count()) {
		area->remove_shape(0);
	}
}

void BradotPhysicsServer3D::area_set_shape_disabled(RID p_area, int p_shape_idx, bool p_disabled) {
	BradotArea3D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);
	ERR_FAIL_INDEX(p_shape_idx, area->get_shape_count());
	FLUSH_QUERY_CHECK(area);
	area->set_shape_disabled(p_shape_idx, p_disabled);
}

void BradotPhysicsServer3D::area_attach_object_instance_id(RID p_area, ObjectID p_id) {
	if (space_owner.owns(p_area)) {
		BradotSpace3D *space = space_owner.get_or_null(p_area);
		p_area = space->get_default_area()->get_self();
	}
	BradotArea3D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);
	area->set_instance_id(p_id);
}

ObjectID BradotPhysicsServer3D::area_get_object_instance_id(RID p_area) const {
	if (space_owner.owns(p_area)) {
		BradotSpace3D *space = space_owner.get_or_null(p_area);
		p_area = space->get_default_area()->get_self();
	}
	BradotArea3D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL_V(area, ObjectID());
	return area->get_instance_id();
}

void BradotPhysicsServer3D::area_set_param(RID p_area, AreaParameter p_param, const Variant &p_value) {
	if (space_owner.owns(p_area)) {
		BradotSpace3D *space = space_owner.get_or_null(p_area);
		p_area = space->get_default_area()->get_self();
	}
	BradotArea3D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);
	area->set_param(p_param, p_value);
}

void BradotPhysicsServer3D::area_set_transform(RID p_area, const Transform3D &p_transform) {
	BradotArea3D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);
	area->set_transform(p_transform);
}

Variant BradotPhysicsServer3D::area_get_param(RID p_area, AreaParameter p_param) const {
	if (space_owner.owns(p_area)) {
		BradotSpace3D *space = space_owner.get_or_null(p_area);
		p_area = space->get_default_area()->get_self();
	}
	BradotArea3D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL_V(area, Variant());

	return area->get_param(p_param);
}

Transform3D BradotPhysicsServer3D::area_get_transform(RID p_area) const {
	BradotArea3D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL_V(area, Transform3D());

	return area->get_transform();
}

void BradotPhysicsServer3D::area_set_collision_layer(RID p_area, uint32_t p_layer) {
	BradotArea3D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);

	area->set_collision_layer(p_layer);
}

uint32_t BradotPhysicsServer3D::area_get_collision_layer(RID p_area) const {
	BradotArea3D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL_V(area, 0);

	return area->get_collision_layer();
}

void BradotPhysicsServer3D::area_set_collision_mask(RID p_area, uint32_t p_mask) {
	BradotArea3D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);

	area->set_collision_mask(p_mask);
}

uint32_t BradotPhysicsServer3D::area_get_collision_mask(RID p_area) const {
	BradotArea3D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL_V(area, 0);

	return area->get_collision_mask();
}

void BradotPhysicsServer3D::area_set_monitorable(RID p_area, bool p_monitorable) {
	BradotArea3D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);
	FLUSH_QUERY_CHECK(area);

	area->set_monitorable(p_monitorable);
}

void BradotPhysicsServer3D::area_set_monitor_callback(RID p_area, const Callable &p_callback) {
	BradotArea3D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);

	area->set_monitor_callback(p_callback.is_valid() ? p_callback : Callable());
}

void BradotPhysicsServer3D::area_set_ray_pickable(RID p_area, bool p_enable) {
	BradotArea3D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);

	area->set_ray_pickable(p_enable);
}

void BradotPhysicsServer3D::area_set_area_monitor_callback(RID p_area, const Callable &p_callback) {
	BradotArea3D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);

	area->set_area_monitor_callback(p_callback.is_valid() ? p_callback : Callable());
}

/* BODY API */

RID BradotPhysicsServer3D::body_create() {
	BradotBody3D *body = memnew(BradotBody3D);
	RID rid = body_owner.make_rid(body);
	body->set_self(rid);
	return rid;
}

void BradotPhysicsServer3D::body_set_space(RID p_body, RID p_space) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	BradotSpace3D *space = nullptr;
	if (p_space.is_valid()) {
		space = space_owner.get_or_null(p_space);
		ERR_FAIL_NULL(space);
	}

	if (body->get_space() == space) {
		return; //pointless
	}

	body->clear_constraint_map();
	body->set_space(space);
}

RID BradotPhysicsServer3D::body_get_space(RID p_body) const {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, RID());

	BradotSpace3D *space = body->get_space();
	if (!space) {
		return RID();
	}
	return space->get_self();
}

void BradotPhysicsServer3D::body_set_mode(RID p_body, BodyMode p_mode) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->set_mode(p_mode);
}

PhysicsServer3D::BodyMode BradotPhysicsServer3D::body_get_mode(RID p_body) const {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, BODY_MODE_STATIC);

	return body->get_mode();
}

void BradotPhysicsServer3D::body_add_shape(RID p_body, RID p_shape, const Transform3D &p_transform, bool p_disabled) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	BradotShape3D *shape = shape_owner.get_or_null(p_shape);
	ERR_FAIL_NULL(shape);

	body->add_shape(shape, p_transform, p_disabled);
}

void BradotPhysicsServer3D::body_set_shape(RID p_body, int p_shape_idx, RID p_shape) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	BradotShape3D *shape = shape_owner.get_or_null(p_shape);
	ERR_FAIL_NULL(shape);
	ERR_FAIL_COND(!shape->is_configured());

	body->set_shape(p_shape_idx, shape);
}
void BradotPhysicsServer3D::body_set_shape_transform(RID p_body, int p_shape_idx, const Transform3D &p_transform) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->set_shape_transform(p_shape_idx, p_transform);
}

int BradotPhysicsServer3D::body_get_shape_count(RID p_body) const {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, -1);

	return body->get_shape_count();
}

RID BradotPhysicsServer3D::body_get_shape(RID p_body, int p_shape_idx) const {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, RID());

	BradotShape3D *shape = body->get_shape(p_shape_idx);
	ERR_FAIL_NULL_V(shape, RID());

	return shape->get_self();
}

void BradotPhysicsServer3D::body_set_shape_disabled(RID p_body, int p_shape_idx, bool p_disabled) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);
	ERR_FAIL_INDEX(p_shape_idx, body->get_shape_count());
	FLUSH_QUERY_CHECK(body);

	body->set_shape_disabled(p_shape_idx, p_disabled);
}

Transform3D BradotPhysicsServer3D::body_get_shape_transform(RID p_body, int p_shape_idx) const {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, Transform3D());

	return body->get_shape_transform(p_shape_idx);
}

void BradotPhysicsServer3D::body_remove_shape(RID p_body, int p_shape_idx) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->remove_shape(p_shape_idx);
}

void BradotPhysicsServer3D::body_clear_shapes(RID p_body) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	while (body->get_shape_count()) {
		body->remove_shape(0);
	}
}

void BradotPhysicsServer3D::body_set_enable_continuous_collision_detection(RID p_body, bool p_enable) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->set_continuous_collision_detection(p_enable);
}

bool BradotPhysicsServer3D::body_is_continuous_collision_detection_enabled(RID p_body) const {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, false);

	return body->is_continuous_collision_detection_enabled();
}

void BradotPhysicsServer3D::body_set_collision_layer(RID p_body, uint32_t p_layer) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->set_collision_layer(p_layer);
}

uint32_t BradotPhysicsServer3D::body_get_collision_layer(RID p_body) const {
	const BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, 0);

	return body->get_collision_layer();
}

void BradotPhysicsServer3D::body_set_collision_mask(RID p_body, uint32_t p_mask) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->set_collision_mask(p_mask);
}

uint32_t BradotPhysicsServer3D::body_get_collision_mask(RID p_body) const {
	const BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, 0);

	return body->get_collision_mask();
}

void BradotPhysicsServer3D::body_set_collision_priority(RID p_body, real_t p_priority) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->set_collision_priority(p_priority);
}

real_t BradotPhysicsServer3D::body_get_collision_priority(RID p_body) const {
	const BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, 0);

	return body->get_collision_priority();
}

void BradotPhysicsServer3D::body_attach_object_instance_id(RID p_body, ObjectID p_id) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	if (body) {
		body->set_instance_id(p_id);
		return;
	}

	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	if (soft_body) {
		soft_body->set_instance_id(p_id);
		return;
	}

	ERR_FAIL_MSG("Invalid ID.");
}

ObjectID BradotPhysicsServer3D::body_get_object_instance_id(RID p_body) const {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, ObjectID());

	return body->get_instance_id();
}

void BradotPhysicsServer3D::body_set_user_flags(RID p_body, uint32_t p_flags) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);
}

uint32_t BradotPhysicsServer3D::body_get_user_flags(RID p_body) const {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, 0);

	return 0;
}

void BradotPhysicsServer3D::body_set_param(RID p_body, BodyParameter p_param, const Variant &p_value) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->set_param(p_param, p_value);
}

Variant BradotPhysicsServer3D::body_get_param(RID p_body, BodyParameter p_param) const {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, 0);

	return body->get_param(p_param);
}

void BradotPhysicsServer3D::body_reset_mass_properties(RID p_body) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	return body->reset_mass_properties();
}

void BradotPhysicsServer3D::body_set_state(RID p_body, BodyState p_state, const Variant &p_variant) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->set_state(p_state, p_variant);
}

Variant BradotPhysicsServer3D::body_get_state(RID p_body, BodyState p_state) const {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, Variant());

	return body->get_state(p_state);
}

void BradotPhysicsServer3D::body_apply_central_impulse(RID p_body, const Vector3 &p_impulse) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	_update_shapes();

	body->apply_central_impulse(p_impulse);
	body->wakeup();
}

void BradotPhysicsServer3D::body_apply_impulse(RID p_body, const Vector3 &p_impulse, const Vector3 &p_position) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	_update_shapes();

	body->apply_impulse(p_impulse, p_position);
	body->wakeup();
}

void BradotPhysicsServer3D::body_apply_torque_impulse(RID p_body, const Vector3 &p_impulse) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	_update_shapes();

	body->apply_torque_impulse(p_impulse);
	body->wakeup();
}

void BradotPhysicsServer3D::body_apply_central_force(RID p_body, const Vector3 &p_force) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->apply_central_force(p_force);
	body->wakeup();
}

void BradotPhysicsServer3D::body_apply_force(RID p_body, const Vector3 &p_force, const Vector3 &p_position) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->apply_force(p_force, p_position);
	body->wakeup();
}

void BradotPhysicsServer3D::body_apply_torque(RID p_body, const Vector3 &p_torque) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->apply_torque(p_torque);
	body->wakeup();
}

void BradotPhysicsServer3D::body_add_constant_central_force(RID p_body, const Vector3 &p_force) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->add_constant_central_force(p_force);
	body->wakeup();
}

void BradotPhysicsServer3D::body_add_constant_force(RID p_body, const Vector3 &p_force, const Vector3 &p_position) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->add_constant_force(p_force, p_position);
	body->wakeup();
}

void BradotPhysicsServer3D::body_add_constant_torque(RID p_body, const Vector3 &p_torque) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->add_constant_torque(p_torque);
	body->wakeup();
}

void BradotPhysicsServer3D::body_set_constant_force(RID p_body, const Vector3 &p_force) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->set_constant_force(p_force);
	if (!p_force.is_zero_approx()) {
		body->wakeup();
	}
}

Vector3 BradotPhysicsServer3D::body_get_constant_force(RID p_body) const {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, Vector3());
	return body->get_constant_force();
}

void BradotPhysicsServer3D::body_set_constant_torque(RID p_body, const Vector3 &p_torque) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->set_constant_torque(p_torque);
	if (!p_torque.is_zero_approx()) {
		body->wakeup();
	}
}

Vector3 BradotPhysicsServer3D::body_get_constant_torque(RID p_body) const {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, Vector3());

	return body->get_constant_torque();
}

void BradotPhysicsServer3D::body_set_axis_velocity(RID p_body, const Vector3 &p_axis_velocity) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	_update_shapes();

	Vector3 v = body->get_linear_velocity();
	Vector3 axis = p_axis_velocity.normalized();
	v -= axis * axis.dot(v);
	v += p_axis_velocity;
	body->set_linear_velocity(v);
	body->wakeup();
}

void BradotPhysicsServer3D::body_set_axis_lock(RID p_body, BodyAxis p_axis, bool p_lock) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->set_axis_lock(p_axis, p_lock);
	body->wakeup();
}

bool BradotPhysicsServer3D::body_is_axis_locked(RID p_body, BodyAxis p_axis) const {
	const BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, false);
	return body->is_axis_locked(p_axis);
}

void BradotPhysicsServer3D::body_add_collision_exception(RID p_body, RID p_body_b) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->add_exception(p_body_b);
	body->wakeup();
}

void BradotPhysicsServer3D::body_remove_collision_exception(RID p_body, RID p_body_b) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->remove_exception(p_body_b);
	body->wakeup();
}

void BradotPhysicsServer3D::body_get_collision_exceptions(RID p_body, List<RID> *p_exceptions) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	for (int i = 0; i < body->get_exceptions().size(); i++) {
		p_exceptions->push_back(body->get_exceptions()[i]);
	}
}

void BradotPhysicsServer3D::body_set_contacts_reported_depth_threshold(RID p_body, real_t p_threshold) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);
}

real_t BradotPhysicsServer3D::body_get_contacts_reported_depth_threshold(RID p_body) const {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, 0);
	return 0;
}

void BradotPhysicsServer3D::body_set_omit_force_integration(RID p_body, bool p_omit) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->set_omit_force_integration(p_omit);
}

bool BradotPhysicsServer3D::body_is_omitting_force_integration(RID p_body) const {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, false);
	return body->get_omit_force_integration();
}

void BradotPhysicsServer3D::body_set_max_contacts_reported(RID p_body, int p_contacts) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);
	body->set_max_contacts_reported(p_contacts);
}

int BradotPhysicsServer3D::body_get_max_contacts_reported(RID p_body) const {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, -1);
	return body->get_max_contacts_reported();
}

void BradotPhysicsServer3D::body_set_state_sync_callback(RID p_body, const Callable &p_callable) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);
	body->set_state_sync_callback(p_callable);
}

void BradotPhysicsServer3D::body_set_force_integration_callback(RID p_body, const Callable &p_callable, const Variant &p_udata) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);
	body->set_force_integration_callback(p_callable, p_udata);
}

void BradotPhysicsServer3D::body_set_ray_pickable(RID p_body, bool p_enable) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);
	body->set_ray_pickable(p_enable);
}

bool BradotPhysicsServer3D::body_test_motion(RID p_body, const MotionParameters &p_parameters, MotionResult *r_result) {
	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, false);
	ERR_FAIL_NULL_V(body->get_space(), false);
	ERR_FAIL_COND_V(body->get_space()->is_locked(), false);

	_update_shapes();

	return body->get_space()->test_body_motion(body, p_parameters, r_result);
}

PhysicsDirectBodyState3D *BradotPhysicsServer3D::body_get_direct_state(RID p_body) {
	ERR_FAIL_COND_V_MSG((using_threads && !doing_sync), nullptr, "Body state is inaccessible right now, wait for iteration or physics process notification.");

	if (!body_owner.owns(p_body)) {
		return nullptr;
	}

	BradotBody3D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, nullptr);

	if (!body->get_space()) {
		return nullptr;
	}

	ERR_FAIL_COND_V_MSG(body->get_space()->is_locked(), nullptr, "Body state is inaccessible right now, wait for iteration or physics process notification.");

	return body->get_direct_state();
}

/* SOFT BODY */

RID BradotPhysicsServer3D::soft_body_create() {
	BradotSoftBody3D *soft_body = memnew(BradotSoftBody3D);
	RID rid = soft_body_owner.make_rid(soft_body);
	soft_body->set_self(rid);
	return rid;
}

void BradotPhysicsServer3D::soft_body_update_rendering_server(RID p_body, PhysicsServer3DRenderingServerHandler *p_rendering_server_handler) {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(soft_body);

	soft_body->update_rendering_server(p_rendering_server_handler);
}

void BradotPhysicsServer3D::soft_body_set_space(RID p_body, RID p_space) {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(soft_body);

	BradotSpace3D *space = nullptr;
	if (p_space.is_valid()) {
		space = space_owner.get_or_null(p_space);
		ERR_FAIL_NULL(space);
	}

	if (soft_body->get_space() == space) {
		return;
	}

	soft_body->set_space(space);
}

RID BradotPhysicsServer3D::soft_body_get_space(RID p_body) const {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(soft_body, RID());

	BradotSpace3D *space = soft_body->get_space();
	if (!space) {
		return RID();
	}
	return space->get_self();
}

void BradotPhysicsServer3D::soft_body_set_collision_layer(RID p_body, uint32_t p_layer) {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(soft_body);

	soft_body->set_collision_layer(p_layer);
}

uint32_t BradotPhysicsServer3D::soft_body_get_collision_layer(RID p_body) const {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(soft_body, 0);

	return soft_body->get_collision_layer();
}

void BradotPhysicsServer3D::soft_body_set_collision_mask(RID p_body, uint32_t p_mask) {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(soft_body);

	soft_body->set_collision_mask(p_mask);
}

uint32_t BradotPhysicsServer3D::soft_body_get_collision_mask(RID p_body) const {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(soft_body, 0);

	return soft_body->get_collision_mask();
}

void BradotPhysicsServer3D::soft_body_add_collision_exception(RID p_body, RID p_body_b) {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(soft_body);

	soft_body->add_exception(p_body_b);
}

void BradotPhysicsServer3D::soft_body_remove_collision_exception(RID p_body, RID p_body_b) {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(soft_body);

	soft_body->remove_exception(p_body_b);
}

void BradotPhysicsServer3D::soft_body_get_collision_exceptions(RID p_body, List<RID> *p_exceptions) {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(soft_body);

	for (int i = 0; i < soft_body->get_exceptions().size(); i++) {
		p_exceptions->push_back(soft_body->get_exceptions()[i]);
	}
}

void BradotPhysicsServer3D::soft_body_set_state(RID p_body, BodyState p_state, const Variant &p_variant) {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(soft_body);

	soft_body->set_state(p_state, p_variant);
}

Variant BradotPhysicsServer3D::soft_body_get_state(RID p_body, BodyState p_state) const {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(soft_body, Variant());

	return soft_body->get_state(p_state);
}

void BradotPhysicsServer3D::soft_body_set_transform(RID p_body, const Transform3D &p_transform) {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(soft_body);

	soft_body->set_state(BODY_STATE_TRANSFORM, p_transform);
}

void BradotPhysicsServer3D::soft_body_set_ray_pickable(RID p_body, bool p_enable) {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(soft_body);

	soft_body->set_ray_pickable(p_enable);
}

void BradotPhysicsServer3D::soft_body_set_simulation_precision(RID p_body, int p_simulation_precision) {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(soft_body);

	soft_body->set_iteration_count(p_simulation_precision);
}

int BradotPhysicsServer3D::soft_body_get_simulation_precision(RID p_body) const {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(soft_body, 0.f);

	return soft_body->get_iteration_count();
}

void BradotPhysicsServer3D::soft_body_set_total_mass(RID p_body, real_t p_total_mass) {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(soft_body);

	soft_body->set_total_mass(p_total_mass);
}

real_t BradotPhysicsServer3D::soft_body_get_total_mass(RID p_body) const {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(soft_body, 0.f);

	return soft_body->get_total_mass();
}

void BradotPhysicsServer3D::soft_body_set_linear_stiffness(RID p_body, real_t p_stiffness) {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(soft_body);

	soft_body->set_linear_stiffness(p_stiffness);
}

real_t BradotPhysicsServer3D::soft_body_get_linear_stiffness(RID p_body) const {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(soft_body, 0.f);

	return soft_body->get_linear_stiffness();
}

void BradotPhysicsServer3D::soft_body_set_pressure_coefficient(RID p_body, real_t p_pressure_coefficient) {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(soft_body);

	soft_body->set_pressure_coefficient(p_pressure_coefficient);
}

real_t BradotPhysicsServer3D::soft_body_get_pressure_coefficient(RID p_body) const {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(soft_body, 0.f);

	return soft_body->get_pressure_coefficient();
}

void BradotPhysicsServer3D::soft_body_set_damping_coefficient(RID p_body, real_t p_damping_coefficient) {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(soft_body);

	soft_body->set_damping_coefficient(p_damping_coefficient);
}

real_t BradotPhysicsServer3D::soft_body_get_damping_coefficient(RID p_body) const {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(soft_body, 0.f);

	return soft_body->get_damping_coefficient();
}

void BradotPhysicsServer3D::soft_body_set_drag_coefficient(RID p_body, real_t p_drag_coefficient) {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(soft_body);

	soft_body->set_drag_coefficient(p_drag_coefficient);
}

real_t BradotPhysicsServer3D::soft_body_get_drag_coefficient(RID p_body) const {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(soft_body, 0.f);

	return soft_body->get_drag_coefficient();
}

void BradotPhysicsServer3D::soft_body_set_mesh(RID p_body, RID p_mesh) {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(soft_body);

	soft_body->set_mesh(p_mesh);
}

AABB BradotPhysicsServer3D::soft_body_get_bounds(RID p_body) const {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(soft_body, AABB());

	return soft_body->get_bounds();
}

void BradotPhysicsServer3D::soft_body_move_point(RID p_body, int p_point_index, const Vector3 &p_global_position) {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(soft_body);

	soft_body->set_vertex_position(p_point_index, p_global_position);
}

Vector3 BradotPhysicsServer3D::soft_body_get_point_global_position(RID p_body, int p_point_index) const {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(soft_body, Vector3());

	return soft_body->get_vertex_position(p_point_index);
}

void BradotPhysicsServer3D::soft_body_remove_all_pinned_points(RID p_body) {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(soft_body);

	soft_body->unpin_all_vertices();
}

void BradotPhysicsServer3D::soft_body_pin_point(RID p_body, int p_point_index, bool p_pin) {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(soft_body);

	if (p_pin) {
		soft_body->pin_vertex(p_point_index);
	} else {
		soft_body->unpin_vertex(p_point_index);
	}
}

bool BradotPhysicsServer3D::soft_body_is_point_pinned(RID p_body, int p_point_index) const {
	BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(soft_body, false);

	return soft_body->is_vertex_pinned(p_point_index);
}

/* JOINT API */

RID BradotPhysicsServer3D::joint_create() {
	BradotJoint3D *joint = memnew(BradotJoint3D);
	RID rid = joint_owner.make_rid(joint);
	joint->set_self(rid);
	return rid;
}

void BradotPhysicsServer3D::joint_clear(RID p_joint) {
	BradotJoint3D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL(joint);
	if (joint->get_type() != JOINT_TYPE_MAX) {
		BradotJoint3D *empty_joint = memnew(BradotJoint3D);
		empty_joint->copy_settings_from(joint);

		joint_owner.replace(p_joint, empty_joint);
		memdelete(joint);
	}
}

void BradotPhysicsServer3D::joint_make_pin(RID p_joint, RID p_body_A, const Vector3 &p_local_A, RID p_body_B, const Vector3 &p_local_B) {
	BradotBody3D *body_A = body_owner.get_or_null(p_body_A);
	ERR_FAIL_NULL(body_A);

	if (!p_body_B.is_valid()) {
		ERR_FAIL_NULL(body_A->get_space());
		p_body_B = body_A->get_space()->get_static_global_body();
	}

	BradotBody3D *body_B = body_owner.get_or_null(p_body_B);
	ERR_FAIL_NULL(body_B);

	ERR_FAIL_COND(body_A == body_B);

	BradotJoint3D *prev_joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL(prev_joint);

	BradotJoint3D *joint = memnew(BradotPinJoint3D(body_A, p_local_A, body_B, p_local_B));

	joint->copy_settings_from(prev_joint);
	joint_owner.replace(p_joint, joint);
	memdelete(prev_joint);
}

void BradotPhysicsServer3D::pin_joint_set_param(RID p_joint, PinJointParam p_param, real_t p_value) {
	BradotJoint3D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL(joint);
	ERR_FAIL_COND(joint->get_type() != JOINT_TYPE_PIN);
	BradotPinJoint3D *pin_joint = static_cast<BradotPinJoint3D *>(joint);
	pin_joint->set_param(p_param, p_value);
}

real_t BradotPhysicsServer3D::pin_joint_get_param(RID p_joint, PinJointParam p_param) const {
	BradotJoint3D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL_V(joint, 0);
	ERR_FAIL_COND_V(joint->get_type() != JOINT_TYPE_PIN, 0);
	BradotPinJoint3D *pin_joint = static_cast<BradotPinJoint3D *>(joint);
	return pin_joint->get_param(p_param);
}

void BradotPhysicsServer3D::pin_joint_set_local_a(RID p_joint, const Vector3 &p_A) {
	BradotJoint3D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL(joint);
	ERR_FAIL_COND(joint->get_type() != JOINT_TYPE_PIN);
	BradotPinJoint3D *pin_joint = static_cast<BradotPinJoint3D *>(joint);
	pin_joint->set_pos_a(p_A);
}

Vector3 BradotPhysicsServer3D::pin_joint_get_local_a(RID p_joint) const {
	BradotJoint3D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL_V(joint, Vector3());
	ERR_FAIL_COND_V(joint->get_type() != JOINT_TYPE_PIN, Vector3());
	BradotPinJoint3D *pin_joint = static_cast<BradotPinJoint3D *>(joint);
	return pin_joint->get_position_a();
}

void BradotPhysicsServer3D::pin_joint_set_local_b(RID p_joint, const Vector3 &p_B) {
	BradotJoint3D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL(joint);
	ERR_FAIL_COND(joint->get_type() != JOINT_TYPE_PIN);
	BradotPinJoint3D *pin_joint = static_cast<BradotPinJoint3D *>(joint);
	pin_joint->set_pos_b(p_B);
}

Vector3 BradotPhysicsServer3D::pin_joint_get_local_b(RID p_joint) const {
	BradotJoint3D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL_V(joint, Vector3());
	ERR_FAIL_COND_V(joint->get_type() != JOINT_TYPE_PIN, Vector3());
	BradotPinJoint3D *pin_joint = static_cast<BradotPinJoint3D *>(joint);
	return pin_joint->get_position_b();
}

void BradotPhysicsServer3D::joint_make_hinge(RID p_joint, RID p_body_A, const Transform3D &p_frame_A, RID p_body_B, const Transform3D &p_frame_B) {
	BradotBody3D *body_A = body_owner.get_or_null(p_body_A);
	ERR_FAIL_NULL(body_A);

	if (!p_body_B.is_valid()) {
		ERR_FAIL_NULL(body_A->get_space());
		p_body_B = body_A->get_space()->get_static_global_body();
	}

	BradotBody3D *body_B = body_owner.get_or_null(p_body_B);
	ERR_FAIL_NULL(body_B);

	ERR_FAIL_COND(body_A == body_B);

	BradotJoint3D *prev_joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL(prev_joint);

	BradotJoint3D *joint = memnew(BradotHingeJoint3D(body_A, body_B, p_frame_A, p_frame_B));

	joint->copy_settings_from(prev_joint);
	joint_owner.replace(p_joint, joint);
	memdelete(prev_joint);
}

void BradotPhysicsServer3D::joint_make_hinge_simple(RID p_joint, RID p_body_A, const Vector3 &p_pivot_A, const Vector3 &p_axis_A, RID p_body_B, const Vector3 &p_pivot_B, const Vector3 &p_axis_B) {
	BradotBody3D *body_A = body_owner.get_or_null(p_body_A);
	ERR_FAIL_NULL(body_A);

	if (!p_body_B.is_valid()) {
		ERR_FAIL_NULL(body_A->get_space());
		p_body_B = body_A->get_space()->get_static_global_body();
	}

	BradotBody3D *body_B = body_owner.get_or_null(p_body_B);
	ERR_FAIL_NULL(body_B);

	ERR_FAIL_COND(body_A == body_B);

	BradotJoint3D *prev_joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL(prev_joint);

	BradotJoint3D *joint = memnew(BradotHingeJoint3D(body_A, body_B, p_pivot_A, p_pivot_B, p_axis_A, p_axis_B));

	joint->copy_settings_from(prev_joint);
	joint_owner.replace(p_joint, joint);
	memdelete(prev_joint);
}

void BradotPhysicsServer3D::hinge_joint_set_param(RID p_joint, HingeJointParam p_param, real_t p_value) {
	BradotJoint3D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL(joint);
	ERR_FAIL_COND(joint->get_type() != JOINT_TYPE_HINGE);
	BradotHingeJoint3D *hinge_joint = static_cast<BradotHingeJoint3D *>(joint);
	hinge_joint->set_param(p_param, p_value);
}

real_t BradotPhysicsServer3D::hinge_joint_get_param(RID p_joint, HingeJointParam p_param) const {
	BradotJoint3D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL_V(joint, 0);
	ERR_FAIL_COND_V(joint->get_type() != JOINT_TYPE_HINGE, 0);
	BradotHingeJoint3D *hinge_joint = static_cast<BradotHingeJoint3D *>(joint);
	return hinge_joint->get_param(p_param);
}

void BradotPhysicsServer3D::hinge_joint_set_flag(RID p_joint, HingeJointFlag p_flag, bool p_enabled) {
	BradotJoint3D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL(joint);
	ERR_FAIL_COND(joint->get_type() != JOINT_TYPE_HINGE);
	BradotHingeJoint3D *hinge_joint = static_cast<BradotHingeJoint3D *>(joint);
	hinge_joint->set_flag(p_flag, p_enabled);
}

bool BradotPhysicsServer3D::hinge_joint_get_flag(RID p_joint, HingeJointFlag p_flag) const {
	BradotJoint3D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL_V(joint, false);
	ERR_FAIL_COND_V(joint->get_type() != JOINT_TYPE_HINGE, false);
	BradotHingeJoint3D *hinge_joint = static_cast<BradotHingeJoint3D *>(joint);
	return hinge_joint->get_flag(p_flag);
}

void BradotPhysicsServer3D::joint_set_solver_priority(RID p_joint, int p_priority) {
	BradotJoint3D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL(joint);
	joint->set_priority(p_priority);
}

int BradotPhysicsServer3D::joint_get_solver_priority(RID p_joint) const {
	BradotJoint3D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL_V(joint, 0);
	return joint->get_priority();
}

void BradotPhysicsServer3D::joint_disable_collisions_between_bodies(RID p_joint, bool p_disable) {
	BradotJoint3D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL(joint);

	joint->disable_collisions_between_bodies(p_disable);

	if (2 == joint->get_body_count()) {
		BradotBody3D *body_a = *joint->get_body_ptr();
		BradotBody3D *body_b = *(joint->get_body_ptr() + 1);

		if (p_disable) {
			body_add_collision_exception(body_a->get_self(), body_b->get_self());
			body_add_collision_exception(body_b->get_self(), body_a->get_self());
		} else {
			body_remove_collision_exception(body_a->get_self(), body_b->get_self());
			body_remove_collision_exception(body_b->get_self(), body_a->get_self());
		}
	}
}

bool BradotPhysicsServer3D::joint_is_disabled_collisions_between_bodies(RID p_joint) const {
	BradotJoint3D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL_V(joint, true);

	return joint->is_disabled_collisions_between_bodies();
}

BradotPhysicsServer3D::JointType BradotPhysicsServer3D::joint_get_type(RID p_joint) const {
	BradotJoint3D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL_V(joint, JOINT_TYPE_PIN);
	return joint->get_type();
}

void BradotPhysicsServer3D::joint_make_slider(RID p_joint, RID p_body_A, const Transform3D &p_local_frame_A, RID p_body_B, const Transform3D &p_local_frame_B) {
	BradotBody3D *body_A = body_owner.get_or_null(p_body_A);
	ERR_FAIL_NULL(body_A);

	if (!p_body_B.is_valid()) {
		ERR_FAIL_NULL(body_A->get_space());
		p_body_B = body_A->get_space()->get_static_global_body();
	}

	BradotBody3D *body_B = body_owner.get_or_null(p_body_B);
	ERR_FAIL_NULL(body_B);

	ERR_FAIL_COND(body_A == body_B);

	BradotJoint3D *prev_joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL(prev_joint);

	BradotJoint3D *joint = memnew(BradotSliderJoint3D(body_A, body_B, p_local_frame_A, p_local_frame_B));

	joint->copy_settings_from(prev_joint);
	joint_owner.replace(p_joint, joint);
	memdelete(prev_joint);
}

void BradotPhysicsServer3D::slider_joint_set_param(RID p_joint, SliderJointParam p_param, real_t p_value) {
	BradotJoint3D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL(joint);
	ERR_FAIL_COND(joint->get_type() != JOINT_TYPE_SLIDER);
	BradotSliderJoint3D *slider_joint = static_cast<BradotSliderJoint3D *>(joint);
	slider_joint->set_param(p_param, p_value);
}

real_t BradotPhysicsServer3D::slider_joint_get_param(RID p_joint, SliderJointParam p_param) const {
	BradotJoint3D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL_V(joint, 0);
	ERR_FAIL_COND_V(joint->get_type() != JOINT_TYPE_CONE_TWIST, 0);
	BradotSliderJoint3D *slider_joint = static_cast<BradotSliderJoint3D *>(joint);
	return slider_joint->get_param(p_param);
}

void BradotPhysicsServer3D::joint_make_cone_twist(RID p_joint, RID p_body_A, const Transform3D &p_local_frame_A, RID p_body_B, const Transform3D &p_local_frame_B) {
	BradotBody3D *body_A = body_owner.get_or_null(p_body_A);
	ERR_FAIL_NULL(body_A);

	if (!p_body_B.is_valid()) {
		ERR_FAIL_NULL(body_A->get_space());
		p_body_B = body_A->get_space()->get_static_global_body();
	}

	BradotBody3D *body_B = body_owner.get_or_null(p_body_B);
	ERR_FAIL_NULL(body_B);

	ERR_FAIL_COND(body_A == body_B);

	BradotJoint3D *prev_joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL(prev_joint);

	BradotJoint3D *joint = memnew(BradotConeTwistJoint3D(body_A, body_B, p_local_frame_A, p_local_frame_B));

	joint->copy_settings_from(prev_joint);
	joint_owner.replace(p_joint, joint);
	memdelete(prev_joint);
}

void BradotPhysicsServer3D::cone_twist_joint_set_param(RID p_joint, ConeTwistJointParam p_param, real_t p_value) {
	BradotJoint3D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL(joint);
	ERR_FAIL_COND(joint->get_type() != JOINT_TYPE_CONE_TWIST);
	BradotConeTwistJoint3D *cone_twist_joint = static_cast<BradotConeTwistJoint3D *>(joint);
	cone_twist_joint->set_param(p_param, p_value);
}

real_t BradotPhysicsServer3D::cone_twist_joint_get_param(RID p_joint, ConeTwistJointParam p_param) const {
	BradotJoint3D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL_V(joint, 0);
	ERR_FAIL_COND_V(joint->get_type() != JOINT_TYPE_CONE_TWIST, 0);
	BradotConeTwistJoint3D *cone_twist_joint = static_cast<BradotConeTwistJoint3D *>(joint);
	return cone_twist_joint->get_param(p_param);
}

void BradotPhysicsServer3D::joint_make_generic_6dof(RID p_joint, RID p_body_A, const Transform3D &p_local_frame_A, RID p_body_B, const Transform3D &p_local_frame_B) {
	BradotBody3D *body_A = body_owner.get_or_null(p_body_A);
	ERR_FAIL_NULL(body_A);

	if (!p_body_B.is_valid()) {
		ERR_FAIL_NULL(body_A->get_space());
		p_body_B = body_A->get_space()->get_static_global_body();
	}

	BradotBody3D *body_B = body_owner.get_or_null(p_body_B);
	ERR_FAIL_NULL(body_B);

	ERR_FAIL_COND(body_A == body_B);

	BradotJoint3D *prev_joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL(prev_joint);

	BradotJoint3D *joint = memnew(BradotGeneric6DOFJoint3D(body_A, body_B, p_local_frame_A, p_local_frame_B, true));

	joint->copy_settings_from(prev_joint);
	joint_owner.replace(p_joint, joint);
	memdelete(prev_joint);
}

void BradotPhysicsServer3D::generic_6dof_joint_set_param(RID p_joint, Vector3::Axis p_axis, G6DOFJointAxisParam p_param, real_t p_value) {
	BradotJoint3D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL(joint);
	ERR_FAIL_COND(joint->get_type() != JOINT_TYPE_6DOF);
	BradotGeneric6DOFJoint3D *generic_6dof_joint = static_cast<BradotGeneric6DOFJoint3D *>(joint);
	generic_6dof_joint->set_param(p_axis, p_param, p_value);
}

real_t BradotPhysicsServer3D::generic_6dof_joint_get_param(RID p_joint, Vector3::Axis p_axis, G6DOFJointAxisParam p_param) const {
	BradotJoint3D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL_V(joint, 0);
	ERR_FAIL_COND_V(joint->get_type() != JOINT_TYPE_6DOF, 0);
	BradotGeneric6DOFJoint3D *generic_6dof_joint = static_cast<BradotGeneric6DOFJoint3D *>(joint);
	return generic_6dof_joint->get_param(p_axis, p_param);
}

void BradotPhysicsServer3D::generic_6dof_joint_set_flag(RID p_joint, Vector3::Axis p_axis, G6DOFJointAxisFlag p_flag, bool p_enable) {
	BradotJoint3D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL(joint);
	ERR_FAIL_COND(joint->get_type() != JOINT_TYPE_6DOF);
	BradotGeneric6DOFJoint3D *generic_6dof_joint = static_cast<BradotGeneric6DOFJoint3D *>(joint);
	generic_6dof_joint->set_flag(p_axis, p_flag, p_enable);
}

bool BradotPhysicsServer3D::generic_6dof_joint_get_flag(RID p_joint, Vector3::Axis p_axis, G6DOFJointAxisFlag p_flag) const {
	BradotJoint3D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL_V(joint, false);
	ERR_FAIL_COND_V(joint->get_type() != JOINT_TYPE_6DOF, false);
	BradotGeneric6DOFJoint3D *generic_6dof_joint = static_cast<BradotGeneric6DOFJoint3D *>(joint);
	return generic_6dof_joint->get_flag(p_axis, p_flag);
}

void BradotPhysicsServer3D::free(RID p_rid) {
	_update_shapes(); //just in case

	if (shape_owner.owns(p_rid)) {
		BradotShape3D *shape = shape_owner.get_or_null(p_rid);

		while (shape->get_owners().size()) {
			BradotShapeOwner3D *so = shape->get_owners().begin()->key;
			so->remove_shape(shape);
		}

		shape_owner.free(p_rid);
		memdelete(shape);
	} else if (body_owner.owns(p_rid)) {
		BradotBody3D *body = body_owner.get_or_null(p_rid);

		body->set_space(nullptr);

		while (body->get_shape_count()) {
			body->remove_shape(0);
		}

		body_owner.free(p_rid);
		memdelete(body);
	} else if (soft_body_owner.owns(p_rid)) {
		BradotSoftBody3D *soft_body = soft_body_owner.get_or_null(p_rid);

		soft_body->set_space(nullptr);

		soft_body_owner.free(p_rid);
		memdelete(soft_body);
	} else if (area_owner.owns(p_rid)) {
		BradotArea3D *area = area_owner.get_or_null(p_rid);

		area->set_space(nullptr);

		while (area->get_shape_count()) {
			area->remove_shape(0);
		}

		area_owner.free(p_rid);
		memdelete(area);
	} else if (space_owner.owns(p_rid)) {
		BradotSpace3D *space = space_owner.get_or_null(p_rid);

		while (space->get_objects().size()) {
			BradotCollisionObject3D *co = static_cast<BradotCollisionObject3D *>(*space->get_objects().begin());
			co->set_space(nullptr);
		}

		active_spaces.erase(space);
		free(space->get_default_area()->get_self());
		free(space->get_static_global_body());

		space_owner.free(p_rid);
		memdelete(space);
	} else if (joint_owner.owns(p_rid)) {
		BradotJoint3D *joint = joint_owner.get_or_null(p_rid);

		joint_owner.free(p_rid);
		memdelete(joint);

	} else {
		ERR_FAIL_MSG("Invalid ID.");
	}
}

void BradotPhysicsServer3D::set_active(bool p_active) {
	active = p_active;
}

void BradotPhysicsServer3D::init() {
	stepper = memnew(BradotStep3D);
}

void BradotPhysicsServer3D::step(real_t p_step) {
	if (!active) {
		return;
	}

	_update_shapes();

	island_count = 0;
	active_objects = 0;
	collision_pairs = 0;
	for (const BradotSpace3D *E : active_spaces) {
		stepper->step(const_cast<BradotSpace3D *>(E), p_step);
		island_count += E->get_island_count();
		active_objects += E->get_active_objects();
		collision_pairs += E->get_collision_pairs();
	}
}

void BradotPhysicsServer3D::sync() {
	doing_sync = true;
}

void BradotPhysicsServer3D::flush_queries() {
	if (!active) {
		return;
	}

	flushing_queries = true;

	uint64_t time_beg = OS::get_singleton()->get_ticks_usec();

	for (const BradotSpace3D *E : active_spaces) {
		BradotSpace3D *space = const_cast<BradotSpace3D *>(E);
		space->call_queries();
	}

	flushing_queries = false;

	if (EngineDebugger::is_profiling("servers")) {
		uint64_t total_time[BradotSpace3D::ELAPSED_TIME_MAX];
		static const char *time_name[BradotSpace3D::ELAPSED_TIME_MAX] = {
			"integrate_forces",
			"generate_islands",
			"setup_constraints",
			"solve_constraints",
			"integrate_velocities"
		};

		for (int i = 0; i < BradotSpace3D::ELAPSED_TIME_MAX; i++) {
			total_time[i] = 0;
		}

		for (const BradotSpace3D *E : active_spaces) {
			for (int i = 0; i < BradotSpace3D::ELAPSED_TIME_MAX; i++) {
				total_time[i] += E->get_elapsed_time(BradotSpace3D::ElapsedTime(i));
			}
		}

		Array values;
		values.resize(BradotSpace3D::ELAPSED_TIME_MAX * 2);
		for (int i = 0; i < BradotSpace3D::ELAPSED_TIME_MAX; i++) {
			values[i * 2 + 0] = time_name[i];
			values[i * 2 + 1] = USEC_TO_SEC(total_time[i]);
		}
		values.push_back("flush_queries");
		values.push_back(USEC_TO_SEC(OS::get_singleton()->get_ticks_usec() - time_beg));

		values.push_front("physics_3d");
		EngineDebugger::profiler_add_frame_data("servers", values);
	}
}

void BradotPhysicsServer3D::end_sync() {
	doing_sync = false;
}

void BradotPhysicsServer3D::finish() {
	memdelete(stepper);
}

int BradotPhysicsServer3D::get_process_info(ProcessInfo p_info) {
	switch (p_info) {
		case INFO_ACTIVE_OBJECTS: {
			return active_objects;
		} break;
		case INFO_COLLISION_PAIRS: {
			return collision_pairs;
		} break;
		case INFO_ISLAND_COUNT: {
			return island_count;
		} break;
	}

	return 0;
}

void BradotPhysicsServer3D::_update_shapes() {
	while (pending_shape_update_list.first()) {
		pending_shape_update_list.first()->self()->_shape_changed();
		pending_shape_update_list.remove(pending_shape_update_list.first());
	}
}

void BradotPhysicsServer3D::_shape_col_cbk(const Vector3 &p_point_A, int p_index_A, const Vector3 &p_point_B, int p_index_B, const Vector3 &normal, void *p_userdata) {
	CollCbkData *cbk = static_cast<CollCbkData *>(p_userdata);

	if (cbk->max == 0) {
		return;
	}

	if (cbk->amount == cbk->max) {
		//find least deep
		real_t min_depth = 1e20;
		int min_depth_idx = 0;
		for (int i = 0; i < cbk->amount; i++) {
			real_t d = cbk->ptr[i * 2 + 0].distance_squared_to(cbk->ptr[i * 2 + 1]);
			if (d < min_depth) {
				min_depth = d;
				min_depth_idx = i;
			}
		}

		real_t d = p_point_A.distance_squared_to(p_point_B);
		if (d < min_depth) {
			return;
		}
		cbk->ptr[min_depth_idx * 2 + 0] = p_point_A;
		cbk->ptr[min_depth_idx * 2 + 1] = p_point_B;

	} else {
		cbk->ptr[cbk->amount * 2 + 0] = p_point_A;
		cbk->ptr[cbk->amount * 2 + 1] = p_point_B;
		cbk->amount++;
	}
}

BradotPhysicsServer3D *BradotPhysicsServer3D::bradot_singleton = nullptr;
BradotPhysicsServer3D::BradotPhysicsServer3D(bool p_using_threads) {
	bradot_singleton = this;
	BradotBroadPhase3D::create_func = BradotBroadPhase3DBVH::_create;

	using_threads = p_using_threads;
}
