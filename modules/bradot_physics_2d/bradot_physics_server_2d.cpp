/**************************************************************************/
/*  bradot_physics_server_2d.cpp                                           */
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

#include "bradot_physics_server_2d.h"

#include "bradot_body_direct_state_2d.h"
#include "bradot_broad_phase_2d_bvh.h"
#include "bradot_collision_solver_2d.h"

#include "core/config/project_settings.h"
#include "core/debugger/engine_debugger.h"
#include "core/os/os.h"

#define FLUSH_QUERY_CHECK(m_object) \
	ERR_FAIL_COND_MSG(m_object->get_space() && flushing_queries, "Can't change this state while flushing queries. Use call_deferred() or set_deferred() to change monitoring state instead.");

RID BradotPhysicsServer2D::_shape_create(ShapeType p_shape) {
	BradotShape2D *shape = nullptr;
	switch (p_shape) {
		case SHAPE_WORLD_BOUNDARY: {
			shape = memnew(BradotWorldBoundaryShape2D);
		} break;
		case SHAPE_SEPARATION_RAY: {
			shape = memnew(BradotSeparationRayShape2D);
		} break;
		case SHAPE_SEGMENT: {
			shape = memnew(BradotSegmentShape2D);
		} break;
		case SHAPE_CIRCLE: {
			shape = memnew(BradotCircleShape2D);
		} break;
		case SHAPE_RECTANGLE: {
			shape = memnew(BradotRectangleShape2D);
		} break;
		case SHAPE_CAPSULE: {
			shape = memnew(BradotCapsuleShape2D);
		} break;
		case SHAPE_CONVEX_POLYGON: {
			shape = memnew(BradotConvexPolygonShape2D);
		} break;
		case SHAPE_CONCAVE_POLYGON: {
			shape = memnew(BradotConcavePolygonShape2D);
		} break;
		case SHAPE_CUSTOM: {
			ERR_FAIL_V(RID());

		} break;
	}

	RID id = shape_owner.make_rid(shape);
	shape->set_self(id);

	return id;
}

RID BradotPhysicsServer2D::world_boundary_shape_create() {
	return _shape_create(SHAPE_WORLD_BOUNDARY);
}

RID BradotPhysicsServer2D::separation_ray_shape_create() {
	return _shape_create(SHAPE_SEPARATION_RAY);
}

RID BradotPhysicsServer2D::segment_shape_create() {
	return _shape_create(SHAPE_SEGMENT);
}

RID BradotPhysicsServer2D::circle_shape_create() {
	return _shape_create(SHAPE_CIRCLE);
}

RID BradotPhysicsServer2D::rectangle_shape_create() {
	return _shape_create(SHAPE_RECTANGLE);
}

RID BradotPhysicsServer2D::capsule_shape_create() {
	return _shape_create(SHAPE_CAPSULE);
}

RID BradotPhysicsServer2D::convex_polygon_shape_create() {
	return _shape_create(SHAPE_CONVEX_POLYGON);
}

RID BradotPhysicsServer2D::concave_polygon_shape_create() {
	return _shape_create(SHAPE_CONCAVE_POLYGON);
}

void BradotPhysicsServer2D::shape_set_data(RID p_shape, const Variant &p_data) {
	BradotShape2D *shape = shape_owner.get_or_null(p_shape);
	ERR_FAIL_NULL(shape);
	shape->set_data(p_data);
}

void BradotPhysicsServer2D::shape_set_custom_solver_bias(RID p_shape, real_t p_bias) {
	BradotShape2D *shape = shape_owner.get_or_null(p_shape);
	ERR_FAIL_NULL(shape);
	shape->set_custom_bias(p_bias);
}

PhysicsServer2D::ShapeType BradotPhysicsServer2D::shape_get_type(RID p_shape) const {
	const BradotShape2D *shape = shape_owner.get_or_null(p_shape);
	ERR_FAIL_NULL_V(shape, SHAPE_CUSTOM);
	return shape->get_type();
}

Variant BradotPhysicsServer2D::shape_get_data(RID p_shape) const {
	const BradotShape2D *shape = shape_owner.get_or_null(p_shape);
	ERR_FAIL_NULL_V(shape, Variant());
	ERR_FAIL_COND_V(!shape->is_configured(), Variant());
	return shape->get_data();
}

real_t BradotPhysicsServer2D::shape_get_custom_solver_bias(RID p_shape) const {
	const BradotShape2D *shape = shape_owner.get_or_null(p_shape);
	ERR_FAIL_NULL_V(shape, 0);
	return shape->get_custom_bias();
}

void BradotPhysicsServer2D::_shape_col_cbk(const Vector2 &p_point_A, const Vector2 &p_point_B, void *p_userdata) {
	CollCbkData *cbk = static_cast<CollCbkData *>(p_userdata);

	if (cbk->max == 0) {
		return;
	}

	Vector2 rel_dir = (p_point_A - p_point_B);
	real_t rel_length2 = rel_dir.length_squared();
	if (cbk->valid_dir != Vector2()) {
		if (cbk->valid_depth < 10e20) {
			if (rel_length2 > cbk->valid_depth * cbk->valid_depth ||
					(rel_length2 > CMP_EPSILON && cbk->valid_dir.dot(rel_dir.normalized()) < CMP_EPSILON)) {
				cbk->invalid_by_dir++;
				return;
			}
		} else {
			if (rel_length2 > 0 && cbk->valid_dir.dot(rel_dir.normalized()) < CMP_EPSILON) {
				return;
			}
		}
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

		if (rel_length2 < min_depth) {
			return;
		}
		cbk->ptr[min_depth_idx * 2 + 0] = p_point_A;
		cbk->ptr[min_depth_idx * 2 + 1] = p_point_B;
		cbk->passed++;

	} else {
		cbk->ptr[cbk->amount * 2 + 0] = p_point_A;
		cbk->ptr[cbk->amount * 2 + 1] = p_point_B;
		cbk->amount++;
		cbk->passed++;
	}
}

bool BradotPhysicsServer2D::shape_collide(RID p_shape_A, const Transform2D &p_xform_A, const Vector2 &p_motion_A, RID p_shape_B, const Transform2D &p_xform_B, const Vector2 &p_motion_B, Vector2 *r_results, int p_result_max, int &r_result_count) {
	BradotShape2D *shape_A = shape_owner.get_or_null(p_shape_A);
	ERR_FAIL_NULL_V(shape_A, false);
	BradotShape2D *shape_B = shape_owner.get_or_null(p_shape_B);
	ERR_FAIL_NULL_V(shape_B, false);

	if (p_result_max == 0) {
		return BradotCollisionSolver2D::solve(shape_A, p_xform_A, p_motion_A, shape_B, p_xform_B, p_motion_B, nullptr, nullptr);
	}

	CollCbkData cbk;
	cbk.max = p_result_max;
	cbk.amount = 0;
	cbk.passed = 0;
	cbk.ptr = r_results;

	bool res = BradotCollisionSolver2D::solve(shape_A, p_xform_A, p_motion_A, shape_B, p_xform_B, p_motion_B, _shape_col_cbk, &cbk);
	r_result_count = cbk.amount;
	return res;
}

RID BradotPhysicsServer2D::space_create() {
	BradotSpace2D *space = memnew(BradotSpace2D);
	RID id = space_owner.make_rid(space);
	space->set_self(id);
	RID area_id = area_create();
	BradotArea2D *area = area_owner.get_or_null(area_id);
	ERR_FAIL_NULL_V(area, RID());
	space->set_default_area(area);
	area->set_space(space);
	area->set_priority(-1);

	return id;
}

void BradotPhysicsServer2D::space_set_active(RID p_space, bool p_active) {
	BradotSpace2D *space = space_owner.get_or_null(p_space);
	ERR_FAIL_NULL(space);
	if (p_active) {
		active_spaces.insert(space);
	} else {
		active_spaces.erase(space);
	}
}

bool BradotPhysicsServer2D::space_is_active(RID p_space) const {
	const BradotSpace2D *space = space_owner.get_or_null(p_space);
	ERR_FAIL_NULL_V(space, false);

	return active_spaces.has(space);
}

void BradotPhysicsServer2D::space_set_param(RID p_space, SpaceParameter p_param, real_t p_value) {
	BradotSpace2D *space = space_owner.get_or_null(p_space);
	ERR_FAIL_NULL(space);

	space->set_param(p_param, p_value);
}

real_t BradotPhysicsServer2D::space_get_param(RID p_space, SpaceParameter p_param) const {
	const BradotSpace2D *space = space_owner.get_or_null(p_space);
	ERR_FAIL_NULL_V(space, 0);
	return space->get_param(p_param);
}

void BradotPhysicsServer2D::space_set_debug_contacts(RID p_space, int p_max_contacts) {
	BradotSpace2D *space = space_owner.get_or_null(p_space);
	ERR_FAIL_NULL(space);
	space->set_debug_contacts(p_max_contacts);
}

Vector<Vector2> BradotPhysicsServer2D::space_get_contacts(RID p_space) const {
	BradotSpace2D *space = space_owner.get_or_null(p_space);
	ERR_FAIL_NULL_V(space, Vector<Vector2>());
	return space->get_debug_contacts();
}

int BradotPhysicsServer2D::space_get_contact_count(RID p_space) const {
	BradotSpace2D *space = space_owner.get_or_null(p_space);
	ERR_FAIL_NULL_V(space, 0);
	return space->get_debug_contact_count();
}

PhysicsDirectSpaceState2D *BradotPhysicsServer2D::space_get_direct_state(RID p_space) {
	BradotSpace2D *space = space_owner.get_or_null(p_space);
	ERR_FAIL_NULL_V(space, nullptr);
	ERR_FAIL_COND_V_MSG((using_threads && !doing_sync) || space->is_locked(), nullptr, "Space state is inaccessible right now, wait for iteration or physics process notification.");

	return space->get_direct_state();
}

RID BradotPhysicsServer2D::area_create() {
	BradotArea2D *area = memnew(BradotArea2D);
	RID rid = area_owner.make_rid(area);
	area->set_self(rid);
	return rid;
}

void BradotPhysicsServer2D::area_set_space(RID p_area, RID p_space) {
	BradotArea2D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);

	BradotSpace2D *space = nullptr;
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

RID BradotPhysicsServer2D::area_get_space(RID p_area) const {
	BradotArea2D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL_V(area, RID());

	BradotSpace2D *space = area->get_space();
	if (!space) {
		return RID();
	}
	return space->get_self();
}

void BradotPhysicsServer2D::area_add_shape(RID p_area, RID p_shape, const Transform2D &p_transform, bool p_disabled) {
	BradotArea2D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);

	BradotShape2D *shape = shape_owner.get_or_null(p_shape);
	ERR_FAIL_NULL(shape);

	area->add_shape(shape, p_transform, p_disabled);
}

void BradotPhysicsServer2D::area_set_shape(RID p_area, int p_shape_idx, RID p_shape) {
	BradotArea2D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);

	BradotShape2D *shape = shape_owner.get_or_null(p_shape);
	ERR_FAIL_NULL(shape);
	ERR_FAIL_COND(!shape->is_configured());

	area->set_shape(p_shape_idx, shape);
}

void BradotPhysicsServer2D::area_set_shape_transform(RID p_area, int p_shape_idx, const Transform2D &p_transform) {
	BradotArea2D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);

	area->set_shape_transform(p_shape_idx, p_transform);
}

void BradotPhysicsServer2D::area_set_shape_disabled(RID p_area, int p_shape, bool p_disabled) {
	BradotArea2D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);
	ERR_FAIL_INDEX(p_shape, area->get_shape_count());
	FLUSH_QUERY_CHECK(area);

	area->set_shape_disabled(p_shape, p_disabled);
}

int BradotPhysicsServer2D::area_get_shape_count(RID p_area) const {
	BradotArea2D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL_V(area, -1);

	return area->get_shape_count();
}

RID BradotPhysicsServer2D::area_get_shape(RID p_area, int p_shape_idx) const {
	BradotArea2D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL_V(area, RID());

	BradotShape2D *shape = area->get_shape(p_shape_idx);
	ERR_FAIL_NULL_V(shape, RID());

	return shape->get_self();
}

Transform2D BradotPhysicsServer2D::area_get_shape_transform(RID p_area, int p_shape_idx) const {
	BradotArea2D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL_V(area, Transform2D());

	return area->get_shape_transform(p_shape_idx);
}

void BradotPhysicsServer2D::area_remove_shape(RID p_area, int p_shape_idx) {
	BradotArea2D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);

	area->remove_shape(p_shape_idx);
}

void BradotPhysicsServer2D::area_clear_shapes(RID p_area) {
	BradotArea2D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);

	while (area->get_shape_count()) {
		area->remove_shape(0);
	}
}

void BradotPhysicsServer2D::area_attach_object_instance_id(RID p_area, ObjectID p_id) {
	if (space_owner.owns(p_area)) {
		BradotSpace2D *space = space_owner.get_or_null(p_area);
		p_area = space->get_default_area()->get_self();
	}
	BradotArea2D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);
	area->set_instance_id(p_id);
}

ObjectID BradotPhysicsServer2D::area_get_object_instance_id(RID p_area) const {
	if (space_owner.owns(p_area)) {
		BradotSpace2D *space = space_owner.get_or_null(p_area);
		p_area = space->get_default_area()->get_self();
	}
	BradotArea2D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL_V(area, ObjectID());
	return area->get_instance_id();
}

void BradotPhysicsServer2D::area_attach_canvas_instance_id(RID p_area, ObjectID p_id) {
	if (space_owner.owns(p_area)) {
		BradotSpace2D *space = space_owner.get_or_null(p_area);
		p_area = space->get_default_area()->get_self();
	}
	BradotArea2D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);
	area->set_canvas_instance_id(p_id);
}

ObjectID BradotPhysicsServer2D::area_get_canvas_instance_id(RID p_area) const {
	if (space_owner.owns(p_area)) {
		BradotSpace2D *space = space_owner.get_or_null(p_area);
		p_area = space->get_default_area()->get_self();
	}
	BradotArea2D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL_V(area, ObjectID());
	return area->get_canvas_instance_id();
}

void BradotPhysicsServer2D::area_set_param(RID p_area, AreaParameter p_param, const Variant &p_value) {
	if (space_owner.owns(p_area)) {
		BradotSpace2D *space = space_owner.get_or_null(p_area);
		p_area = space->get_default_area()->get_self();
	}
	BradotArea2D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);
	area->set_param(p_param, p_value);
}

void BradotPhysicsServer2D::area_set_transform(RID p_area, const Transform2D &p_transform) {
	BradotArea2D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);
	area->set_transform(p_transform);
}

Variant BradotPhysicsServer2D::area_get_param(RID p_area, AreaParameter p_param) const {
	if (space_owner.owns(p_area)) {
		BradotSpace2D *space = space_owner.get_or_null(p_area);
		p_area = space->get_default_area()->get_self();
	}
	BradotArea2D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL_V(area, Variant());

	return area->get_param(p_param);
}

Transform2D BradotPhysicsServer2D::area_get_transform(RID p_area) const {
	BradotArea2D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL_V(area, Transform2D());

	return area->get_transform();
}

void BradotPhysicsServer2D::area_set_pickable(RID p_area, bool p_pickable) {
	BradotArea2D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);
	area->set_pickable(p_pickable);
}

void BradotPhysicsServer2D::area_set_monitorable(RID p_area, bool p_monitorable) {
	BradotArea2D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);
	FLUSH_QUERY_CHECK(area);

	area->set_monitorable(p_monitorable);
}

void BradotPhysicsServer2D::area_set_collision_layer(RID p_area, uint32_t p_layer) {
	BradotArea2D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);

	area->set_collision_layer(p_layer);
}

uint32_t BradotPhysicsServer2D::area_get_collision_layer(RID p_area) const {
	BradotArea2D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL_V(area, 0);

	return area->get_collision_layer();
}

void BradotPhysicsServer2D::area_set_collision_mask(RID p_area, uint32_t p_mask) {
	BradotArea2D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);

	area->set_collision_mask(p_mask);
}

uint32_t BradotPhysicsServer2D::area_get_collision_mask(RID p_area) const {
	BradotArea2D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL_V(area, 0);

	return area->get_collision_mask();
}

void BradotPhysicsServer2D::area_set_monitor_callback(RID p_area, const Callable &p_callback) {
	BradotArea2D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);

	area->set_monitor_callback(p_callback.is_valid() ? p_callback : Callable());
}

void BradotPhysicsServer2D::area_set_area_monitor_callback(RID p_area, const Callable &p_callback) {
	BradotArea2D *area = area_owner.get_or_null(p_area);
	ERR_FAIL_NULL(area);

	area->set_area_monitor_callback(p_callback.is_valid() ? p_callback : Callable());
}

/* BODY API */

RID BradotPhysicsServer2D::body_create() {
	BradotBody2D *body = memnew(BradotBody2D);
	RID rid = body_owner.make_rid(body);
	body->set_self(rid);
	return rid;
}

void BradotPhysicsServer2D::body_set_space(RID p_body, RID p_space) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);
	BradotSpace2D *space = nullptr;
	if (p_space.is_valid()) {
		space = space_owner.get_or_null(p_space);
		ERR_FAIL_NULL(space);
	}

	if (body->get_space() == space) {
		return; //pointless
	}

	body->clear_constraint_list();
	body->set_space(space);
}

RID BradotPhysicsServer2D::body_get_space(RID p_body) const {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, RID());

	BradotSpace2D *space = body->get_space();
	if (!space) {
		return RID();
	}
	return space->get_self();
}

void BradotPhysicsServer2D::body_set_mode(RID p_body, BodyMode p_mode) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);
	FLUSH_QUERY_CHECK(body);

	body->set_mode(p_mode);
}

PhysicsServer2D::BodyMode BradotPhysicsServer2D::body_get_mode(RID p_body) const {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, BODY_MODE_STATIC);

	return body->get_mode();
}

void BradotPhysicsServer2D::body_add_shape(RID p_body, RID p_shape, const Transform2D &p_transform, bool p_disabled) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	BradotShape2D *shape = shape_owner.get_or_null(p_shape);
	ERR_FAIL_NULL(shape);

	body->add_shape(shape, p_transform, p_disabled);
}

void BradotPhysicsServer2D::body_set_shape(RID p_body, int p_shape_idx, RID p_shape) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	BradotShape2D *shape = shape_owner.get_or_null(p_shape);
	ERR_FAIL_NULL(shape);
	ERR_FAIL_COND(!shape->is_configured());

	body->set_shape(p_shape_idx, shape);
}

void BradotPhysicsServer2D::body_set_shape_transform(RID p_body, int p_shape_idx, const Transform2D &p_transform) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->set_shape_transform(p_shape_idx, p_transform);
}

int BradotPhysicsServer2D::body_get_shape_count(RID p_body) const {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, -1);

	return body->get_shape_count();
}

RID BradotPhysicsServer2D::body_get_shape(RID p_body, int p_shape_idx) const {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, RID());

	BradotShape2D *shape = body->get_shape(p_shape_idx);
	ERR_FAIL_NULL_V(shape, RID());

	return shape->get_self();
}

Transform2D BradotPhysicsServer2D::body_get_shape_transform(RID p_body, int p_shape_idx) const {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, Transform2D());

	return body->get_shape_transform(p_shape_idx);
}

void BradotPhysicsServer2D::body_remove_shape(RID p_body, int p_shape_idx) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->remove_shape(p_shape_idx);
}

void BradotPhysicsServer2D::body_clear_shapes(RID p_body) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	while (body->get_shape_count()) {
		body->remove_shape(0);
	}
}

void BradotPhysicsServer2D::body_set_shape_disabled(RID p_body, int p_shape_idx, bool p_disabled) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);
	ERR_FAIL_INDEX(p_shape_idx, body->get_shape_count());
	FLUSH_QUERY_CHECK(body);

	body->set_shape_disabled(p_shape_idx, p_disabled);
}

void BradotPhysicsServer2D::body_set_shape_as_one_way_collision(RID p_body, int p_shape_idx, bool p_enable, real_t p_margin) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);
	ERR_FAIL_INDEX(p_shape_idx, body->get_shape_count());
	FLUSH_QUERY_CHECK(body);

	body->set_shape_as_one_way_collision(p_shape_idx, p_enable, p_margin);
}

void BradotPhysicsServer2D::body_set_continuous_collision_detection_mode(RID p_body, CCDMode p_mode) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);
	body->set_continuous_collision_detection_mode(p_mode);
}

BradotPhysicsServer2D::CCDMode BradotPhysicsServer2D::body_get_continuous_collision_detection_mode(RID p_body) const {
	const BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, CCD_MODE_DISABLED);

	return body->get_continuous_collision_detection_mode();
}

void BradotPhysicsServer2D::body_attach_object_instance_id(RID p_body, ObjectID p_id) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->set_instance_id(p_id);
}

ObjectID BradotPhysicsServer2D::body_get_object_instance_id(RID p_body) const {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, ObjectID());

	return body->get_instance_id();
}

void BradotPhysicsServer2D::body_attach_canvas_instance_id(RID p_body, ObjectID p_id) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->set_canvas_instance_id(p_id);
}

ObjectID BradotPhysicsServer2D::body_get_canvas_instance_id(RID p_body) const {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, ObjectID());

	return body->get_canvas_instance_id();
}

void BradotPhysicsServer2D::body_set_collision_layer(RID p_body, uint32_t p_layer) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);
	body->set_collision_layer(p_layer);
}

uint32_t BradotPhysicsServer2D::body_get_collision_layer(RID p_body) const {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, 0);

	return body->get_collision_layer();
}

void BradotPhysicsServer2D::body_set_collision_mask(RID p_body, uint32_t p_mask) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);
	body->set_collision_mask(p_mask);
}

uint32_t BradotPhysicsServer2D::body_get_collision_mask(RID p_body) const {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, 0);

	return body->get_collision_mask();
}

void BradotPhysicsServer2D::body_set_collision_priority(RID p_body, real_t p_priority) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->set_collision_priority(p_priority);
}

real_t BradotPhysicsServer2D::body_get_collision_priority(RID p_body) const {
	const BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, 0);

	return body->get_collision_priority();
}

void BradotPhysicsServer2D::body_set_param(RID p_body, BodyParameter p_param, const Variant &p_value) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->set_param(p_param, p_value);
}

Variant BradotPhysicsServer2D::body_get_param(RID p_body, BodyParameter p_param) const {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, 0);

	return body->get_param(p_param);
}

void BradotPhysicsServer2D::body_reset_mass_properties(RID p_body) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	return body->reset_mass_properties();
}

void BradotPhysicsServer2D::body_set_state(RID p_body, BodyState p_state, const Variant &p_variant) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->set_state(p_state, p_variant);
}

Variant BradotPhysicsServer2D::body_get_state(RID p_body, BodyState p_state) const {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, Variant());

	return body->get_state(p_state);
}

void BradotPhysicsServer2D::body_apply_central_impulse(RID p_body, const Vector2 &p_impulse) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->apply_central_impulse(p_impulse);
	body->wakeup();
}

void BradotPhysicsServer2D::body_apply_torque_impulse(RID p_body, real_t p_torque) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	_update_shapes();

	body->apply_torque_impulse(p_torque);
	body->wakeup();
}

void BradotPhysicsServer2D::body_apply_impulse(RID p_body, const Vector2 &p_impulse, const Vector2 &p_position) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	_update_shapes();

	body->apply_impulse(p_impulse, p_position);
	body->wakeup();
}

void BradotPhysicsServer2D::body_apply_central_force(RID p_body, const Vector2 &p_force) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->apply_central_force(p_force);
	body->wakeup();
}

void BradotPhysicsServer2D::body_apply_force(RID p_body, const Vector2 &p_force, const Vector2 &p_position) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->apply_force(p_force, p_position);
	body->wakeup();
}

void BradotPhysicsServer2D::body_apply_torque(RID p_body, real_t p_torque) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->apply_torque(p_torque);
	body->wakeup();
}

void BradotPhysicsServer2D::body_add_constant_central_force(RID p_body, const Vector2 &p_force) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->add_constant_central_force(p_force);
	body->wakeup();
}

void BradotPhysicsServer2D::body_add_constant_force(RID p_body, const Vector2 &p_force, const Vector2 &p_position) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->add_constant_force(p_force, p_position);
	body->wakeup();
}

void BradotPhysicsServer2D::body_add_constant_torque(RID p_body, real_t p_torque) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->add_constant_torque(p_torque);
	body->wakeup();
}

void BradotPhysicsServer2D::body_set_constant_force(RID p_body, const Vector2 &p_force) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->set_constant_force(p_force);
	if (!p_force.is_zero_approx()) {
		body->wakeup();
	}
}

Vector2 BradotPhysicsServer2D::body_get_constant_force(RID p_body) const {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, Vector2());
	return body->get_constant_force();
}

void BradotPhysicsServer2D::body_set_constant_torque(RID p_body, real_t p_torque) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->set_constant_torque(p_torque);
	if (!Math::is_zero_approx(p_torque)) {
		body->wakeup();
	}
}

real_t BradotPhysicsServer2D::body_get_constant_torque(RID p_body) const {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, 0);

	return body->get_constant_torque();
}

void BradotPhysicsServer2D::body_set_axis_velocity(RID p_body, const Vector2 &p_axis_velocity) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	_update_shapes();

	Vector2 v = body->get_linear_velocity();
	Vector2 axis = p_axis_velocity.normalized();
	v -= axis * axis.dot(v);
	v += p_axis_velocity;
	body->set_linear_velocity(v);
	body->wakeup();
}

void BradotPhysicsServer2D::body_add_collision_exception(RID p_body, RID p_body_b) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->add_exception(p_body_b);
	body->wakeup();
}

void BradotPhysicsServer2D::body_remove_collision_exception(RID p_body, RID p_body_b) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->remove_exception(p_body_b);
	body->wakeup();
}

void BradotPhysicsServer2D::body_get_collision_exceptions(RID p_body, List<RID> *p_exceptions) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	for (int i = 0; i < body->get_exceptions().size(); i++) {
		p_exceptions->push_back(body->get_exceptions()[i]);
	}
}

void BradotPhysicsServer2D::body_set_contacts_reported_depth_threshold(RID p_body, real_t p_threshold) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);
}

real_t BradotPhysicsServer2D::body_get_contacts_reported_depth_threshold(RID p_body) const {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, 0);
	return 0;
}

void BradotPhysicsServer2D::body_set_omit_force_integration(RID p_body, bool p_omit) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);

	body->set_omit_force_integration(p_omit);
}

bool BradotPhysicsServer2D::body_is_omitting_force_integration(RID p_body) const {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, false);
	return body->get_omit_force_integration();
}

void BradotPhysicsServer2D::body_set_max_contacts_reported(RID p_body, int p_contacts) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);
	body->set_max_contacts_reported(p_contacts);
}

int BradotPhysicsServer2D::body_get_max_contacts_reported(RID p_body) const {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, -1);
	return body->get_max_contacts_reported();
}

void BradotPhysicsServer2D::body_set_state_sync_callback(RID p_body, const Callable &p_callable) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);
	body->set_state_sync_callback(p_callable);
}

void BradotPhysicsServer2D::body_set_force_integration_callback(RID p_body, const Callable &p_callable, const Variant &p_udata) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);
	body->set_force_integration_callback(p_callable, p_udata);
}

bool BradotPhysicsServer2D::body_collide_shape(RID p_body, int p_body_shape, RID p_shape, const Transform2D &p_shape_xform, const Vector2 &p_motion, Vector2 *r_results, int p_result_max, int &r_result_count) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, false);
	ERR_FAIL_INDEX_V(p_body_shape, body->get_shape_count(), false);

	return shape_collide(body->get_shape(p_body_shape)->get_self(), body->get_transform() * body->get_shape_transform(p_body_shape), Vector2(), p_shape, p_shape_xform, p_motion, r_results, p_result_max, r_result_count);
}

void BradotPhysicsServer2D::body_set_pickable(RID p_body, bool p_pickable) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL(body);
	body->set_pickable(p_pickable);
}

bool BradotPhysicsServer2D::body_test_motion(RID p_body, const MotionParameters &p_parameters, MotionResult *r_result) {
	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, false);
	ERR_FAIL_NULL_V(body->get_space(), false);
	ERR_FAIL_COND_V(body->get_space()->is_locked(), false);

	_update_shapes();

	return body->get_space()->test_body_motion(body, p_parameters, r_result);
}

PhysicsDirectBodyState2D *BradotPhysicsServer2D::body_get_direct_state(RID p_body) {
	ERR_FAIL_COND_V_MSG((using_threads && !doing_sync), nullptr, "Body state is inaccessible right now, wait for iteration or physics process notification.");

	if (!body_owner.owns(p_body)) {
		return nullptr;
	}

	BradotBody2D *body = body_owner.get_or_null(p_body);
	ERR_FAIL_NULL_V(body, nullptr);

	if (!body->get_space()) {
		return nullptr;
	}

	ERR_FAIL_COND_V_MSG(body->get_space()->is_locked(), nullptr, "Body state is inaccessible right now, wait for iteration or physics process notification.");

	return body->get_direct_state();
}

/* JOINT API */

RID BradotPhysicsServer2D::joint_create() {
	BradotJoint2D *joint = memnew(BradotJoint2D);
	RID joint_rid = joint_owner.make_rid(joint);
	joint->set_self(joint_rid);
	return joint_rid;
}

void BradotPhysicsServer2D::joint_clear(RID p_joint) {
	BradotJoint2D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL(joint);
	if (joint->get_type() != JOINT_TYPE_MAX) {
		BradotJoint2D *empty_joint = memnew(BradotJoint2D);
		empty_joint->copy_settings_from(joint);

		joint_owner.replace(p_joint, empty_joint);
		memdelete(joint);
	}
}

void BradotPhysicsServer2D::joint_set_param(RID p_joint, JointParam p_param, real_t p_value) {
	BradotJoint2D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL(joint);

	switch (p_param) {
		case JOINT_PARAM_BIAS:
			joint->set_bias(p_value);
			break;
		case JOINT_PARAM_MAX_BIAS:
			joint->set_max_bias(p_value);
			break;
		case JOINT_PARAM_MAX_FORCE:
			joint->set_max_force(p_value);
			break;
	}
}

real_t BradotPhysicsServer2D::joint_get_param(RID p_joint, JointParam p_param) const {
	const BradotJoint2D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL_V(joint, -1);

	switch (p_param) {
		case JOINT_PARAM_BIAS:
			return joint->get_bias();
			break;
		case JOINT_PARAM_MAX_BIAS:
			return joint->get_max_bias();
			break;
		case JOINT_PARAM_MAX_FORCE:
			return joint->get_max_force();
			break;
	}

	return 0;
}

void BradotPhysicsServer2D::joint_disable_collisions_between_bodies(RID p_joint, const bool p_disable) {
	BradotJoint2D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL(joint);

	joint->disable_collisions_between_bodies(p_disable);

	if (2 == joint->get_body_count()) {
		BradotBody2D *body_a = *joint->get_body_ptr();
		BradotBody2D *body_b = *(joint->get_body_ptr() + 1);

		if (p_disable) {
			body_add_collision_exception(body_a->get_self(), body_b->get_self());
			body_add_collision_exception(body_b->get_self(), body_a->get_self());
		} else {
			body_remove_collision_exception(body_a->get_self(), body_b->get_self());
			body_remove_collision_exception(body_b->get_self(), body_a->get_self());
		}
	}
}

bool BradotPhysicsServer2D::joint_is_disabled_collisions_between_bodies(RID p_joint) const {
	const BradotJoint2D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL_V(joint, true);

	return joint->is_disabled_collisions_between_bodies();
}

void BradotPhysicsServer2D::joint_make_pin(RID p_joint, const Vector2 &p_pos, RID p_body_a, RID p_body_b) {
	BradotBody2D *A = body_owner.get_or_null(p_body_a);
	ERR_FAIL_NULL(A);
	BradotBody2D *B = nullptr;
	if (body_owner.owns(p_body_b)) {
		B = body_owner.get_or_null(p_body_b);
		ERR_FAIL_NULL(B);
	}

	BradotJoint2D *prev_joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL(prev_joint);

	BradotJoint2D *joint = memnew(BradotPinJoint2D(p_pos, A, B));

	joint_owner.replace(p_joint, joint);
	joint->copy_settings_from(prev_joint);
	memdelete(prev_joint);
}

void BradotPhysicsServer2D::joint_make_groove(RID p_joint, const Vector2 &p_a_groove1, const Vector2 &p_a_groove2, const Vector2 &p_b_anchor, RID p_body_a, RID p_body_b) {
	BradotBody2D *A = body_owner.get_or_null(p_body_a);
	ERR_FAIL_NULL(A);

	BradotBody2D *B = body_owner.get_or_null(p_body_b);
	ERR_FAIL_NULL(B);

	BradotJoint2D *prev_joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL(prev_joint);

	BradotJoint2D *joint = memnew(BradotGrooveJoint2D(p_a_groove1, p_a_groove2, p_b_anchor, A, B));

	joint_owner.replace(p_joint, joint);
	joint->copy_settings_from(prev_joint);
	memdelete(prev_joint);
}

void BradotPhysicsServer2D::joint_make_damped_spring(RID p_joint, const Vector2 &p_anchor_a, const Vector2 &p_anchor_b, RID p_body_a, RID p_body_b) {
	BradotBody2D *A = body_owner.get_or_null(p_body_a);
	ERR_FAIL_NULL(A);

	BradotBody2D *B = body_owner.get_or_null(p_body_b);
	ERR_FAIL_NULL(B);

	BradotJoint2D *prev_joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL(prev_joint);

	BradotJoint2D *joint = memnew(BradotDampedSpringJoint2D(p_anchor_a, p_anchor_b, A, B));

	joint_owner.replace(p_joint, joint);
	joint->copy_settings_from(prev_joint);
	memdelete(prev_joint);
}

void BradotPhysicsServer2D::pin_joint_set_flag(RID p_joint, PinJointFlag p_flag, bool p_enabled) {
	BradotJoint2D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL(joint);
	ERR_FAIL_COND(joint->get_type() != JOINT_TYPE_PIN);

	BradotPinJoint2D *pin_joint = static_cast<BradotPinJoint2D *>(joint);
	pin_joint->set_flag(p_flag, p_enabled);
}

bool BradotPhysicsServer2D::pin_joint_get_flag(RID p_joint, PinJointFlag p_flag) const {
	BradotJoint2D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL_V(joint, false);
	ERR_FAIL_COND_V(joint->get_type() != JOINT_TYPE_PIN, false);

	BradotPinJoint2D *pin_joint = static_cast<BradotPinJoint2D *>(joint);
	return pin_joint->get_flag(p_flag);
}

void BradotPhysicsServer2D::pin_joint_set_param(RID p_joint, PinJointParam p_param, real_t p_value) {
	BradotJoint2D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL(joint);
	ERR_FAIL_COND(joint->get_type() != JOINT_TYPE_PIN);

	BradotPinJoint2D *pin_joint = static_cast<BradotPinJoint2D *>(joint);
	pin_joint->set_param(p_param, p_value);
}

real_t BradotPhysicsServer2D::pin_joint_get_param(RID p_joint, PinJointParam p_param) const {
	BradotJoint2D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL_V(joint, 0);
	ERR_FAIL_COND_V(joint->get_type() != JOINT_TYPE_PIN, 0);

	BradotPinJoint2D *pin_joint = static_cast<BradotPinJoint2D *>(joint);
	return pin_joint->get_param(p_param);
}

void BradotPhysicsServer2D::damped_spring_joint_set_param(RID p_joint, DampedSpringParam p_param, real_t p_value) {
	BradotJoint2D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL(joint);
	ERR_FAIL_COND(joint->get_type() != JOINT_TYPE_DAMPED_SPRING);

	BradotDampedSpringJoint2D *dsj = static_cast<BradotDampedSpringJoint2D *>(joint);
	dsj->set_param(p_param, p_value);
}

real_t BradotPhysicsServer2D::damped_spring_joint_get_param(RID p_joint, DampedSpringParam p_param) const {
	BradotJoint2D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL_V(joint, 0);
	ERR_FAIL_COND_V(joint->get_type() != JOINT_TYPE_DAMPED_SPRING, 0);

	BradotDampedSpringJoint2D *dsj = static_cast<BradotDampedSpringJoint2D *>(joint);
	return dsj->get_param(p_param);
}

PhysicsServer2D::JointType BradotPhysicsServer2D::joint_get_type(RID p_joint) const {
	BradotJoint2D *joint = joint_owner.get_or_null(p_joint);
	ERR_FAIL_NULL_V(joint, JOINT_TYPE_PIN);

	return joint->get_type();
}

void BradotPhysicsServer2D::free(RID p_rid) {
	_update_shapes(); // just in case

	if (shape_owner.owns(p_rid)) {
		BradotShape2D *shape = shape_owner.get_or_null(p_rid);

		while (shape->get_owners().size()) {
			BradotShapeOwner2D *so = shape->get_owners().begin()->key;
			so->remove_shape(shape);
		}

		shape_owner.free(p_rid);
		memdelete(shape);
	} else if (body_owner.owns(p_rid)) {
		BradotBody2D *body = body_owner.get_or_null(p_rid);

		body_set_space(p_rid, RID());

		while (body->get_shape_count()) {
			body->remove_shape(0);
		}

		body_owner.free(p_rid);
		memdelete(body);

	} else if (area_owner.owns(p_rid)) {
		BradotArea2D *area = area_owner.get_or_null(p_rid);

		area->set_space(nullptr);

		while (area->get_shape_count()) {
			area->remove_shape(0);
		}

		area_owner.free(p_rid);
		memdelete(area);
	} else if (space_owner.owns(p_rid)) {
		BradotSpace2D *space = space_owner.get_or_null(p_rid);

		while (space->get_objects().size()) {
			BradotCollisionObject2D *co = static_cast<BradotCollisionObject2D *>(*space->get_objects().begin());
			co->set_space(nullptr);
		}

		active_spaces.erase(space);
		free(space->get_default_area()->get_self());
		space_owner.free(p_rid);
		memdelete(space);
	} else if (joint_owner.owns(p_rid)) {
		BradotJoint2D *joint = joint_owner.get_or_null(p_rid);

		joint_owner.free(p_rid);
		memdelete(joint);

	} else {
		ERR_FAIL_MSG("Invalid ID.");
	}
}

void BradotPhysicsServer2D::set_active(bool p_active) {
	active = p_active;
}

void BradotPhysicsServer2D::init() {
	doing_sync = false;
	stepper = memnew(BradotStep2D);
}

void BradotPhysicsServer2D::step(real_t p_step) {
	if (!active) {
		return;
	}

	_update_shapes();

	island_count = 0;
	active_objects = 0;
	collision_pairs = 0;
	for (const BradotSpace2D *E : active_spaces) {
		stepper->step(const_cast<BradotSpace2D *>(E), p_step);
		island_count += E->get_island_count();
		active_objects += E->get_active_objects();
		collision_pairs += E->get_collision_pairs();
	}
}

void BradotPhysicsServer2D::sync() {
	doing_sync = true;
}

void BradotPhysicsServer2D::flush_queries() {
	if (!active) {
		return;
	}

	flushing_queries = true;

	uint64_t time_beg = OS::get_singleton()->get_ticks_usec();

	for (const BradotSpace2D *E : active_spaces) {
		BradotSpace2D *space = const_cast<BradotSpace2D *>(E);
		space->call_queries();
	}

	flushing_queries = false;

	if (EngineDebugger::is_profiling("servers")) {
		uint64_t total_time[BradotSpace2D::ELAPSED_TIME_MAX];
		static const char *time_name[BradotSpace2D::ELAPSED_TIME_MAX] = {
			"integrate_forces",
			"generate_islands",
			"setup_constraints",
			"solve_constraints",
			"integrate_velocities"
		};

		for (int i = 0; i < BradotSpace2D::ELAPSED_TIME_MAX; i++) {
			total_time[i] = 0;
		}

		for (const BradotSpace2D *E : active_spaces) {
			for (int i = 0; i < BradotSpace2D::ELAPSED_TIME_MAX; i++) {
				total_time[i] += E->get_elapsed_time(BradotSpace2D::ElapsedTime(i));
			}
		}

		Array values;
		values.resize(BradotSpace2D::ELAPSED_TIME_MAX * 2);
		for (int i = 0; i < BradotSpace2D::ELAPSED_TIME_MAX; i++) {
			values[i * 2 + 0] = time_name[i];
			values[i * 2 + 1] = USEC_TO_SEC(total_time[i]);
		}
		values.push_back("flush_queries");
		values.push_back(USEC_TO_SEC(OS::get_singleton()->get_ticks_usec() - time_beg));

		values.push_front("physics_2d");
		EngineDebugger::profiler_add_frame_data("servers", values);
	}
}

void BradotPhysicsServer2D::end_sync() {
	doing_sync = false;
}

void BradotPhysicsServer2D::finish() {
	memdelete(stepper);
}

void BradotPhysicsServer2D::_update_shapes() {
	while (pending_shape_update_list.first()) {
		pending_shape_update_list.first()->self()->_shape_changed();
		pending_shape_update_list.remove(pending_shape_update_list.first());
	}
}

int BradotPhysicsServer2D::get_process_info(ProcessInfo p_info) {
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

BradotPhysicsServer2D *BradotPhysicsServer2D::bradot_singleton = nullptr;

BradotPhysicsServer2D::BradotPhysicsServer2D(bool p_using_threads) {
	bradot_singleton = this;
	BradotBroadPhase2D::create_func = BradotBroadPhase2DBVH::_create;

	using_threads = p_using_threads;
}
