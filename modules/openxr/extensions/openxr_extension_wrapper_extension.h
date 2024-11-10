/**************************************************************************/
/*  openxr_extension_wrapper_extension.h                                  */
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

#ifndef OPENXR_EXTENSION_WRAPPER_EXTENSION_H
#define OPENXR_EXTENSION_WRAPPER_EXTENSION_H

#include "../openxr_api_extension.h"
#include "openxr_extension_wrapper.h"

#include "core/object/ref_counted.h"
#include "core/os/os.h"
#include "core/os/thread_safe.h"
#include "core/variant/native_ptr.h"
#include "core/variant/typed_array.h"

class OpenXRExtensionWrapperExtension : public Object, public OpenXRExtensionWrapper, public OpenXRCompositionLayerProvider {
	BRCLASS(OpenXRExtensionWrapperExtension, Object);

protected:
	_THREAD_SAFE_CLASS_

	static void _bind_methods();

	Ref<OpenXRAPIExtension> openxr_api;

public:
	virtual HashMap<String, bool *> get_requested_extensions() override;

	BRVIRTUAL0R(Dictionary, _get_requested_extensions);

	virtual void *set_system_properties_and_get_next_pointer(void *p_next_pointer) override;
	virtual void *set_instance_create_info_and_get_next_pointer(void *p_next_pointer) override;
	virtual void *set_session_create_and_get_next_pointer(void *p_next_pointer) override;
	virtual void *set_swapchain_create_info_and_get_next_pointer(void *p_next_pointer) override;
	virtual void *set_hand_joint_locations_and_get_next_pointer(int p_hand_index, void *p_next_pointer) override;

	virtual int get_composition_layer_count() override;
	virtual XrCompositionLayerBaseHeader *get_composition_layer(int p_index) override;
	virtual int get_composition_layer_order(int p_index) override;

	//TODO workaround as BRExtensionPtr<void> return type results in build error in bradot-cpp
	BRVIRTUAL1R(uint64_t, _set_system_properties_and_get_next_pointer, BRExtensionPtr<void>);
	BRVIRTUAL1R(uint64_t, _set_instance_create_info_and_get_next_pointer, BRExtensionPtr<void>);
	BRVIRTUAL1R(uint64_t, _set_session_create_and_get_next_pointer, BRExtensionPtr<void>);
	BRVIRTUAL1R(uint64_t, _set_swapchain_create_info_and_get_next_pointer, BRExtensionPtr<void>);
	BRVIRTUAL2R(uint64_t, _set_hand_joint_locations_and_get_next_pointer, int, BRExtensionPtr<void>);
	BRVIRTUAL0R(int, _get_composition_layer_count);
	BRVIRTUAL1R(uint64_t, _get_composition_layer, int);
	BRVIRTUAL1R(int, _get_composition_layer_order, int);

	virtual PackedStringArray get_suggested_tracker_names() override;

	BRVIRTUAL0R(PackedStringArray, _get_suggested_tracker_names);

	virtual void on_register_metadata() override;
	virtual void on_before_instance_created() override;
	virtual void on_instance_created(const XrInstance p_instance) override;
	virtual void on_instance_destroyed() override;
	virtual void on_session_created(const XrSession p_session) override;
	virtual void on_process() override;
	virtual void on_pre_render() override;
	virtual void on_main_swapchains_created() override;
	virtual void on_session_destroyed() override;
	virtual void on_pre_draw_viewport(RID p_render_target) override;
	virtual void on_post_draw_viewport(RID p_render_target) override;

	BRVIRTUAL0(_on_register_metadata);
	BRVIRTUAL0(_on_before_instance_created);
	BRVIRTUAL1(_on_instance_created, uint64_t);
	BRVIRTUAL0(_on_instance_destroyed);
	BRVIRTUAL1(_on_session_created, uint64_t);
	BRVIRTUAL0(_on_process);
	BRVIRTUAL0(_on_pre_render);
	BRVIRTUAL0(_on_main_swapchains_created);
	BRVIRTUAL0(_on_session_destroyed);
	BRVIRTUAL1(_on_pre_draw_viewport, RID);
	BRVIRTUAL1(_on_post_draw_viewport, RID);

	virtual void on_state_idle() override;
	virtual void on_state_ready() override;
	virtual void on_state_synchronized() override;
	virtual void on_state_visible() override;
	virtual void on_state_focused() override;
	virtual void on_state_stopping() override;
	virtual void on_state_loss_pending() override;
	virtual void on_state_exiting() override;

	BRVIRTUAL0(_on_state_idle);
	BRVIRTUAL0(_on_state_ready);
	BRVIRTUAL0(_on_state_synchronized);
	BRVIRTUAL0(_on_state_visible);
	BRVIRTUAL0(_on_state_focused);
	BRVIRTUAL0(_on_state_stopping);
	BRVIRTUAL0(_on_state_loss_pending);
	BRVIRTUAL0(_on_state_exiting);

	virtual bool on_event_polled(const XrEventDataBuffer &p_event) override;

	BRVIRTUAL1R(bool, _on_event_polled, BRExtensionConstPtr<void>);

	virtual void *set_viewport_composition_layer_and_get_next_pointer(const XrCompositionLayerBaseHeader *p_layer, const Dictionary &p_property_values, void *p_next_pointer) override;
	virtual void on_viewport_composition_layer_destroyed(const XrCompositionLayerBaseHeader *p_layer) override;
	virtual void get_viewport_composition_layer_extension_properties(List<PropertyInfo> *p_property_list) override;
	virtual Dictionary get_viewport_composition_layer_extension_property_defaults() override;
	virtual void *set_android_surface_swapchain_create_info_and_get_next_pointer(const Dictionary &p_property_values, void *p_next_pointer) override;

	BRVIRTUAL3R(uint64_t, _set_viewport_composition_layer_and_get_next_pointer, BRExtensionConstPtr<void>, Dictionary, BRExtensionPtr<void>);
	BRVIRTUAL1(_on_viewport_composition_layer_destroyed, BRExtensionConstPtr<void>);
	BRVIRTUAL0R(TypedArray<Dictionary>, _get_viewport_composition_layer_extension_properties);
	BRVIRTUAL0R(Dictionary, _get_viewport_composition_layer_extension_property_defaults);
	BRVIRTUAL2R(uint64_t, _set_android_surface_swapchain_create_info_and_get_next_pointer, Dictionary, BRExtensionPtr<void>);

	Ref<OpenXRAPIExtension> get_openxr_api();

	void register_extension_wrapper();

	OpenXRExtensionWrapperExtension();
	virtual ~OpenXRExtensionWrapperExtension() override;
};

#endif // OPENXR_EXTENSION_WRAPPER_EXTENSION_H
