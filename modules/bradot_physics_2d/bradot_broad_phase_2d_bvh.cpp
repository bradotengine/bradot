/**************************************************************************/
/*  bradot_broad_phase_2d_bvh.cpp                                          */
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

#include "bradot_broad_phase_2d_bvh.h"
#include "bradot_collision_object_2d.h"

BradotBroadPhase2D::ID BradotBroadPhase2DBVH::create(BradotCollisionObject2D *p_object, int p_subindex, const Rect2 &p_aabb, bool p_static) {
	uint32_t tree_id = p_static ? TREE_STATIC : TREE_DYNAMIC;
	uint32_t tree_collision_mask = p_static ? TREE_FLAG_DYNAMIC : (TREE_FLAG_STATIC | TREE_FLAG_DYNAMIC);
	ID oid = bvh.create(p_object, true, tree_id, tree_collision_mask, p_aabb, p_subindex); // Pair everything, don't care?
	return oid + 1;
}

void BradotBroadPhase2DBVH::move(ID p_id, const Rect2 &p_aabb) {
	ERR_FAIL_COND(!p_id);
	bvh.move(p_id - 1, p_aabb);
}

void BradotBroadPhase2DBVH::set_static(ID p_id, bool p_static) {
	ERR_FAIL_COND(!p_id);
	uint32_t tree_id = p_static ? TREE_STATIC : TREE_DYNAMIC;
	uint32_t tree_collision_mask = p_static ? TREE_FLAG_DYNAMIC : (TREE_FLAG_STATIC | TREE_FLAG_DYNAMIC);
	bvh.set_tree(p_id - 1, tree_id, tree_collision_mask, false);
}

void BradotBroadPhase2DBVH::remove(ID p_id) {
	ERR_FAIL_COND(!p_id);
	bvh.erase(p_id - 1);
}

BradotCollisionObject2D *BradotBroadPhase2DBVH::get_object(ID p_id) const {
	ERR_FAIL_COND_V(!p_id, nullptr);
	BradotCollisionObject2D *it = bvh.get(p_id - 1);
	ERR_FAIL_NULL_V(it, nullptr);
	return it;
}

bool BradotBroadPhase2DBVH::is_static(ID p_id) const {
	ERR_FAIL_COND_V(!p_id, false);
	uint32_t tree_id = bvh.get_tree_id(p_id - 1);
	return tree_id == 0;
}

int BradotBroadPhase2DBVH::get_subindex(ID p_id) const {
	ERR_FAIL_COND_V(!p_id, 0);
	return bvh.get_subindex(p_id - 1);
}

int BradotBroadPhase2DBVH::cull_segment(const Vector2 &p_from, const Vector2 &p_to, BradotCollisionObject2D **p_results, int p_max_results, int *p_result_indices) {
	return bvh.cull_segment(p_from, p_to, p_results, p_max_results, nullptr, 0xFFFFFFFF, p_result_indices);
}

int BradotBroadPhase2DBVH::cull_aabb(const Rect2 &p_aabb, BradotCollisionObject2D **p_results, int p_max_results, int *p_result_indices) {
	return bvh.cull_aabb(p_aabb, p_results, p_max_results, nullptr, 0xFFFFFFFF, p_result_indices);
}

void *BradotBroadPhase2DBVH::_pair_callback(void *self, uint32_t p_A, BradotCollisionObject2D *p_object_A, int subindex_A, uint32_t p_B, BradotCollisionObject2D *p_object_B, int subindex_B) {
	BradotBroadPhase2DBVH *bpo = static_cast<BradotBroadPhase2DBVH *>(self);
	if (!bpo->pair_callback) {
		return nullptr;
	}

	return bpo->pair_callback(p_object_A, subindex_A, p_object_B, subindex_B, bpo->pair_userdata);
}

void BradotBroadPhase2DBVH::_unpair_callback(void *self, uint32_t p_A, BradotCollisionObject2D *p_object_A, int subindex_A, uint32_t p_B, BradotCollisionObject2D *p_object_B, int subindex_B, void *pairdata) {
	BradotBroadPhase2DBVH *bpo = static_cast<BradotBroadPhase2DBVH *>(self);
	if (!bpo->unpair_callback) {
		return;
	}

	bpo->unpair_callback(p_object_A, subindex_A, p_object_B, subindex_B, pairdata, bpo->unpair_userdata);
}

void BradotBroadPhase2DBVH::set_pair_callback(PairCallback p_pair_callback, void *p_userdata) {
	pair_callback = p_pair_callback;
	pair_userdata = p_userdata;
}

void BradotBroadPhase2DBVH::set_unpair_callback(UnpairCallback p_unpair_callback, void *p_userdata) {
	unpair_callback = p_unpair_callback;
	unpair_userdata = p_userdata;
}

void BradotBroadPhase2DBVH::update() {
	bvh.update();
}

BradotBroadPhase2D *BradotBroadPhase2DBVH::_create() {
	return memnew(BradotBroadPhase2DBVH);
}

BradotBroadPhase2DBVH::BradotBroadPhase2DBVH() {
	bvh.set_pair_callback(_pair_callback, this);
	bvh.set_unpair_callback(_unpair_callback, this);
}