/**************************************************************************/
/*  bradot_area_pair_3d.h                                                  */
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

#ifndef BRADOT_AREA_PAIR_3D_H
#define BRADOT_AREA_PAIR_3D_H

#include "bradot_area_3d.h"
#include "bradot_body_3d.h"
#include "bradot_constraint_3d.h"
#include "bradot_soft_body_3d.h"

class BradotAreaPair3D : public BradotConstraint3D {
	BradotBody3D *body = nullptr;
	BradotArea3D *area = nullptr;
	int body_shape;
	int area_shape;
	bool colliding = false;
	bool process_collision = false;
	bool has_space_override = false;
	bool body_has_attached_area = false;

public:
	virtual bool setup(real_t p_step) override;
	virtual bool pre_solve(real_t p_step) override;
	virtual void solve(real_t p_step) override;

	BradotAreaPair3D(BradotBody3D *p_body, int p_body_shape, BradotArea3D *p_area, int p_area_shape);
	~BradotAreaPair3D();
};

class BradotArea2Pair3D : public BradotConstraint3D {
	BradotArea3D *area_a = nullptr;
	BradotArea3D *area_b = nullptr;
	int shape_a;
	int shape_b;
	bool colliding_a = false;
	bool colliding_b = false;
	bool process_collision_a = false;
	bool process_collision_b = false;
	bool area_a_monitorable;
	bool area_b_monitorable;

public:
	virtual bool setup(real_t p_step) override;
	virtual bool pre_solve(real_t p_step) override;
	virtual void solve(real_t p_step) override;

	BradotArea2Pair3D(BradotArea3D *p_area_a, int p_shape_a, BradotArea3D *p_area_b, int p_shape_b);
	~BradotArea2Pair3D();
};

class BradotAreaSoftBodyPair3D : public BradotConstraint3D {
	BradotSoftBody3D *soft_body = nullptr;
	BradotArea3D *area = nullptr;
	int soft_body_shape;
	int area_shape;
	bool colliding = false;
	bool process_collision = false;
	bool has_space_override = false;
	bool body_has_attached_area = false;

public:
	virtual bool setup(real_t p_step) override;
	virtual bool pre_solve(real_t p_step) override;
	virtual void solve(real_t p_step) override;

	BradotAreaSoftBodyPair3D(BradotSoftBody3D *p_sof_body, int p_soft_body_shape, BradotArea3D *p_area, int p_area_shape);
	~BradotAreaSoftBodyPair3D();
};

#endif // BRADOT_AREA_PAIR_3D_H