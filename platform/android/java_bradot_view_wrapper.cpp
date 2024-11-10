/**************************************************************************/
/*  java_bradot_view_wrapper.cpp                                           */
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

#include "java_bradot_view_wrapper.h"

#include "thread_jandroid.h"

BradotJavaViewWrapper::BradotJavaViewWrapper(jobject bradot_view) {
	JNIEnv *env = get_jni_env();
	ERR_FAIL_NULL(env);

	_bradot_view = env->NewGlobalRef(bradot_view);

	_cls = (jclass)env->NewGlobalRef(env->GetObjectClass(bradot_view));

	int android_device_api_level = android_get_device_api_level();
	if (android_device_api_level >= __ANDROID_API_N__) {
		_configure_pointer_icon = env->GetMethodID(_cls, "configurePointerIcon", "(ILjava/lang/String;FF)V");
		_set_pointer_icon = env->GetMethodID(_cls, "setPointerIcon", "(I)V");
	}
	if (android_device_api_level >= __ANDROID_API_O__) {
		_request_pointer_capture = env->GetMethodID(_cls, "requestPointerCapture", "()V");
		_release_pointer_capture = env->GetMethodID(_cls, "releasePointerCapture", "()V");
	}

	_can_capture_pointer = env->GetMethodID(_cls, "canCapturePointer", "()Z");
}

bool BradotJavaViewWrapper::can_update_pointer_icon() const {
	return _configure_pointer_icon != nullptr && _set_pointer_icon != nullptr;
}

bool BradotJavaViewWrapper::can_capture_pointer() const {
	// We can capture the pointer if the other jni capture method ids are initialized,
	// and BradotView#canCapturePointer() returns true.
	if (_request_pointer_capture != nullptr && _release_pointer_capture != nullptr && _can_capture_pointer != nullptr) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL_V(env, false);

		return env->CallBooleanMethod(_bradot_view, _can_capture_pointer);
	}

	return false;
}

void BradotJavaViewWrapper::request_pointer_capture() {
	if (_request_pointer_capture != nullptr) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL(env);

		env->CallVoidMethod(_bradot_view, _request_pointer_capture);
	}
}

void BradotJavaViewWrapper::release_pointer_capture() {
	if (_release_pointer_capture != nullptr) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL(env);

		env->CallVoidMethod(_bradot_view, _release_pointer_capture);
	}
}

void BradotJavaViewWrapper::configure_pointer_icon(int pointer_type, const String &image_path, const Vector2 &p_hotspot) {
	if (_configure_pointer_icon != nullptr) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL(env);

		jstring jImagePath = env->NewStringUTF(image_path.utf8().get_data());
		env->CallVoidMethod(_bradot_view, _configure_pointer_icon, pointer_type, jImagePath, p_hotspot.x, p_hotspot.y);
		env->DeleteLocalRef(jImagePath);
	}
}

void BradotJavaViewWrapper::set_pointer_icon(int pointer_type) {
	if (_set_pointer_icon != nullptr) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL(env);

		env->CallVoidMethod(_bradot_view, _set_pointer_icon, pointer_type);
	}
}

BradotJavaViewWrapper::~BradotJavaViewWrapper() {
	JNIEnv *env = get_jni_env();
	ERR_FAIL_NULL(env);

	env->DeleteGlobalRef(_bradot_view);
	env->DeleteGlobalRef(_cls);
}
