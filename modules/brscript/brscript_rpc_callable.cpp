/**************************************************************************/
/*  brscript_rpc_callable.cpp                                             */
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

#include "brscript_rpc_callable.h"

#include "core/object/script_language.h"
#include "core/templates/hashfuncs.h"
#include "scene/main/node.h"

bool BRScriptRPCCallable::compare_equal(const CallableCustom *p_a, const CallableCustom *p_b) {
	return p_a->hash() == p_b->hash();
}

bool BRScriptRPCCallable::compare_less(const CallableCustom *p_a, const CallableCustom *p_b) {
	return p_a->hash() < p_b->hash();
}

uint32_t BRScriptRPCCallable::hash() const {
	return h;
}

String BRScriptRPCCallable::get_as_text() const {
	String class_name = object->get_class();
	Ref<Script> script = object->get_script();
	return class_name + "(" + script->get_path().get_file() + ")::" + String(method) + " (rpc)";
}

CallableCustom::CompareEqualFunc BRScriptRPCCallable::get_compare_equal_func() const {
	return compare_equal;
}

CallableCustom::CompareLessFunc BRScriptRPCCallable::get_compare_less_func() const {
	return compare_less;
}

ObjectID BRScriptRPCCallable::get_object() const {
	return object->get_instance_id();
}

StringName BRScriptRPCCallable::get_method() const {
	return method;
}

int BRScriptRPCCallable::get_argument_count(bool &r_is_valid) const {
	return object->get_method_argument_count(method, &r_is_valid);
}

void BRScriptRPCCallable::call(const Variant **p_arguments, int p_argcount, Variant &r_return_value, Callable::CallError &r_call_error) const {
	r_return_value = object->callp(method, p_arguments, p_argcount, r_call_error);
}

BRScriptRPCCallable::BRScriptRPCCallable(Object *p_object, const StringName &p_method) {
	ERR_FAIL_NULL(p_object);
	object = p_object;
	method = p_method;
	h = method.hash();
	h = hash_murmur3_one_64(object->get_instance_id(), h);
	node = Object::cast_to<Node>(object);
	ERR_FAIL_NULL_MSG(node, "RPC can only be defined on class that extends Node.");
}

Error BRScriptRPCCallable::rpc(int p_peer_id, const Variant **p_arguments, int p_argcount, Callable::CallError &r_call_error) const {
	if (unlikely(!node)) {
		r_call_error.error = Callable::CallError::CALL_ERROR_INSTANCE_IS_NULL;
		return ERR_UNCONFIGURED;
	}
	r_call_error.error = Callable::CallError::CALL_OK;
	return node->rpcp(p_peer_id, method, p_arguments, p_argcount);
}