/**************************************************************************/
/*  bradot_area_pair_2d.h                                                  */
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

#ifndef BRADOT_AREA_PAIR_2D_H
#define BRADOT_AREA_PAIR_2D_H

#include "bradot_area_2d.h"
#include "bradot_body_2d.h"
#include "bradot_constraint_2d.h"

class BradotAreaPair2D : public BradotConstraint2D {
	BradotBody2D *body = nullptr;
	BradotArea2D *area = nullptr;
	int body_shape = 0;
	int area_shape = 0;
	bool colliding = false;
	bool has_space_override = false;
	bool process_collision = false;
	bool body_has_attached_area = false;

public:
	virtual bool setup(real_t p_step) override;
	virtual bool pre_solve(real_t p_step) override;
	virtual void solve(real_t p_step) override;

	BradotAreaPair2D(BradotBody2D *p_body, int p_body_shape, BradotArea2D *p_area, int p_area_shape);
	~BradotAreaPair2D();
};

class BradotArea2Pair2D : public BradotConstraint2D {
	BradotArea2D *area_a = nullptr;
	BradotArea2D *area_b = nullptr;
	int shape_a = 0;
	int shape_b = 0;
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

	BradotArea2Pair2D(BradotArea2D *p_area_a, int p_shape_a, BradotArea2D *p_area_b, int p_shape_b);
	~BradotArea2Pair2D();
};

#endif // BRADOT_AREA_PAIR_2D_H
