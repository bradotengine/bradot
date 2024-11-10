/**************************************************************************/
/*  interop_types.h                                                       */
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

#ifndef INTEROP_TYPES_H
#define INTEROP_TYPES_H

#include "core/math/math_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

// This is taken from the old BRNative, which was removed.

#define BRADOT_VARIANT_SIZE (sizeof(real_t) * 4 + sizeof(int64_t))

typedef struct {
	uint8_t _dont_touch_that[BRADOT_VARIANT_SIZE];
} bradot_variant;

#define BRADOT_ARRAY_SIZE sizeof(void *)

typedef struct {
	uint8_t _dont_touch_that[BRADOT_ARRAY_SIZE];
} bradot_array;

#define BRADOT_DICTIONARY_SIZE sizeof(void *)

typedef struct {
	uint8_t _dont_touch_that[BRADOT_DICTIONARY_SIZE];
} bradot_dictionary;

#define BRADOT_STRING_SIZE sizeof(void *)

typedef struct {
	uint8_t _dont_touch_that[BRADOT_STRING_SIZE];
} bradot_string;

#define BRADOT_STRING_NAME_SIZE sizeof(void *)

typedef struct {
	uint8_t _dont_touch_that[BRADOT_STRING_NAME_SIZE];
} bradot_string_name;

#define BRADOT_PACKED_ARRAY_SIZE (2 * sizeof(void *))

typedef struct {
	uint8_t _dont_touch_that[BRADOT_PACKED_ARRAY_SIZE];
} bradot_packed_array;

#define BRADOT_VECTOR2_SIZE (sizeof(real_t) * 2)

typedef struct {
	uint8_t _dont_touch_that[BRADOT_VECTOR2_SIZE];
} bradot_vector2;

#define BRADOT_VECTOR2I_SIZE (sizeof(int32_t) * 2)

typedef struct {
	uint8_t _dont_touch_that[BRADOT_VECTOR2I_SIZE];
} bradot_vector2i;

#define BRADOT_RECT2_SIZE (sizeof(real_t) * 4)

typedef struct bradot_rect2 {
	uint8_t _dont_touch_that[BRADOT_RECT2_SIZE];
} bradot_rect2;

#define BRADOT_RECT2I_SIZE (sizeof(int32_t) * 4)

typedef struct bradot_rect2i {
	uint8_t _dont_touch_that[BRADOT_RECT2I_SIZE];
} bradot_rect2i;

#define BRADOT_VECTOR3_SIZE (sizeof(real_t) * 3)

typedef struct {
	uint8_t _dont_touch_that[BRADOT_VECTOR3_SIZE];
} bradot_vector3;

#define BRADOT_VECTOR3I_SIZE (sizeof(int32_t) * 3)

typedef struct {
	uint8_t _dont_touch_that[BRADOT_VECTOR3I_SIZE];
} bradot_vector3i;

#define BRADOT_TRANSFORM2D_SIZE (sizeof(real_t) * 6)

typedef struct {
	uint8_t _dont_touch_that[BRADOT_TRANSFORM2D_SIZE];
} bradot_transform2d;

#define BRADOT_VECTOR4_SIZE (sizeof(real_t) * 4)

typedef struct {
	uint8_t _dont_touch_that[BRADOT_VECTOR4_SIZE];
} bradot_vector4;

#define BRADOT_VECTOR4I_SIZE (sizeof(int32_t) * 4)

typedef struct {
	uint8_t _dont_touch_that[BRADOT_VECTOR4I_SIZE];
} bradot_vector4i;

#define BRADOT_PLANE_SIZE (sizeof(real_t) * 4)

typedef struct {
	uint8_t _dont_touch_that[BRADOT_PLANE_SIZE];
} bradot_plane;

#define BRADOT_QUATERNION_SIZE (sizeof(real_t) * 4)

typedef struct {
	uint8_t _dont_touch_that[BRADOT_QUATERNION_SIZE];
} bradot_quaternion;

#define BRADOT_AABB_SIZE (sizeof(real_t) * 6)

typedef struct {
	uint8_t _dont_touch_that[BRADOT_AABB_SIZE];
} bradot_aabb;

#define BRADOT_BASIS_SIZE (sizeof(real_t) * 9)

typedef struct {
	uint8_t _dont_touch_that[BRADOT_BASIS_SIZE];
} bradot_basis;

#define BRADOT_TRANSFORM3D_SIZE (sizeof(real_t) * 12)

typedef struct {
	uint8_t _dont_touch_that[BRADOT_TRANSFORM3D_SIZE];
} bradot_transform3d;

#define BRADOT_PROJECTION_SIZE (sizeof(real_t) * 4 * 4)

typedef struct {
	uint8_t _dont_touch_that[BRADOT_PROJECTION_SIZE];
} bradot_projection;

// Colors should always use 32-bit floats, so don't use real_t here.
#define BRADOT_COLOR_SIZE (sizeof(float) * 4)

typedef struct {
	uint8_t _dont_touch_that[BRADOT_COLOR_SIZE];
} bradot_color;

#define BRADOT_NODE_PATH_SIZE sizeof(void *)

typedef struct {
	uint8_t _dont_touch_that[BRADOT_NODE_PATH_SIZE];
} bradot_node_path;

#define BRADOT_RID_SIZE sizeof(uint64_t)

typedef struct {
	uint8_t _dont_touch_that[BRADOT_RID_SIZE];
} bradot_rid;

// Alignment hardcoded in `core/variant/callable.h`.
#define BRADOT_CALLABLE_SIZE (16)

typedef struct {
	uint8_t _dont_touch_that[BRADOT_CALLABLE_SIZE];
} bradot_callable;

// Alignment hardcoded in `core/variant/callable.h`.
#define BRADOT_SIGNAL_SIZE (16)

typedef struct {
	uint8_t _dont_touch_that[BRADOT_SIGNAL_SIZE];
} bradot_signal;

#ifdef __cplusplus
}
#endif

#endif // INTEROP_TYPES_H
