/**************************************************************************/
/*  brextension.cpp                                                       */
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

#include "brextension.h"
#include "brextension.compat.inc"

#include "core/config/project_settings.h"
#include "core/object/class_db.h"
#include "core/object/method_bind.h"
#include "brextension_library_loader.h"
#include "brextension_manager.h"

extern void brextension_setup_interface();
extern BRExtensionInterfaceFunctionPtr brextension_get_proc_address(const char *p_name);

typedef BRExtensionBool (*BRExtensionLegacyInitializationFunction)(void *p_interface, BRExtensionClassLibraryPtr p_library, BRExtensionInitialization *r_initialization);

String BRExtension::get_extension_list_config_file() {
	return ProjectSettings::get_singleton()->get_project_data_path().path_join("extension_list.cfg");
}

class BRExtensionMethodBind : public MethodBind {
	BRExtensionClassMethodCall call_func;
	BRExtensionClassMethodValidatedCall validated_call_func;
	BRExtensionClassMethodPtrCall ptrcall_func;
	void *method_userdata;
	bool vararg;
	uint32_t argument_count;
	PropertyInfo return_value_info;
	BradotTypeInfo::Metadata return_value_metadata;
	List<PropertyInfo> arguments_info;
	List<BradotTypeInfo::Metadata> arguments_metadata;

#ifdef TOOLS_ENABLED
	friend class BRExtension;

	StringName name;
	bool is_reloading = false;
	bool valid = true;
#endif

protected:
	virtual Variant::Type _gen_argument_type(int p_arg) const override {
		if (p_arg < 0) {
			return return_value_info.type;
		} else {
			return arguments_info.get(p_arg).type;
		}
	}
	virtual PropertyInfo _gen_argument_type_info(int p_arg) const override {
		if (p_arg < 0) {
			return return_value_info;
		} else {
			return arguments_info.get(p_arg);
		}
	}

public:
#ifdef TOOLS_ENABLED
	virtual bool is_valid() const override { return valid; }
#endif

#ifdef DEBUG_METHODS_ENABLED
	virtual BradotTypeInfo::Metadata get_argument_meta(int p_arg) const override {
		if (p_arg < 0) {
			return return_value_metadata;
		} else {
			return arguments_metadata.get(p_arg);
		}
	}
#endif

	virtual Variant call(Object *p_object, const Variant **p_args, int p_arg_count, Callable::CallError &r_error) const override {
#ifdef TOOLS_ENABLED
		ERR_FAIL_COND_V_MSG(!valid, Variant(), vformat("Cannot call invalid BRExtension method bind '%s'. It's probably cached - you may need to restart Bradot.", name));
		ERR_FAIL_COND_V_MSG(p_object && p_object->is_extension_placeholder(), Variant(), vformat("Cannot call BRExtension method bind '%s' on placeholder instance.", name));
#endif
		Variant ret;
		BRExtensionClassInstancePtr extension_instance = is_static() ? nullptr : p_object->_get_extension_instance();
		BRExtensionCallError ce{ BREXTENSION_CALL_OK, 0, 0 };
		call_func(method_userdata, extension_instance, reinterpret_cast<BRExtensionConstVariantPtr *>(p_args), p_arg_count, (BRExtensionVariantPtr)&ret, &ce);
		r_error.error = Callable::CallError::Error(ce.error);
		r_error.argument = ce.argument;
		r_error.expected = ce.expected;
		return ret;
	}
	virtual void validated_call(Object *p_object, const Variant **p_args, Variant *r_ret) const override {
#ifdef TOOLS_ENABLED
		ERR_FAIL_COND_MSG(!valid, vformat("Cannot call invalid BRExtension method bind '%s'. It's probably cached - you may need to restart Bradot.", name));
		ERR_FAIL_COND_MSG(p_object && p_object->is_extension_placeholder(), vformat("Cannot call BRExtension method bind '%s' on placeholder instance.", name));
#endif
		ERR_FAIL_COND_MSG(vararg, "Vararg methods don't have validated call support. This is most likely an engine bug.");
		BRExtensionClassInstancePtr extension_instance = is_static() ? nullptr : p_object->_get_extension_instance();

		if (validated_call_func) {
			// This is added here, but it's unlikely to be provided by most extensions.
			validated_call_func(method_userdata, extension_instance, reinterpret_cast<BRExtensionConstVariantPtr *>(p_args), (BRExtensionVariantPtr)r_ret);
		} else {
			// If not provided, go via ptrcall, which is faster than resorting to regular call.
			const void **argptrs = (const void **)alloca(argument_count * sizeof(void *));
			for (uint32_t i = 0; i < argument_count; i++) {
				argptrs[i] = VariantInternal::get_opaque_pointer(p_args[i]);
			}

			void *ret_opaque = nullptr;
			if (r_ret) {
				VariantInternal::initialize(r_ret, return_value_info.type);
				ret_opaque = r_ret->get_type() == Variant::NIL ? r_ret : VariantInternal::get_opaque_pointer(r_ret);
			}

			ptrcall_func(method_userdata, extension_instance, reinterpret_cast<BRExtensionConstTypePtr *>(argptrs), (BRExtensionTypePtr)ret_opaque);

			if (r_ret && r_ret->get_type() == Variant::OBJECT) {
				VariantInternal::update_object_id(r_ret);
			}
		}
	}

	virtual void ptrcall(Object *p_object, const void **p_args, void *r_ret) const override {
#ifdef TOOLS_ENABLED
		ERR_FAIL_COND_MSG(!valid, vformat("Cannot call invalid BRExtension method bind '%s'. It's probably cached - you may need to restart Bradot.", name));
		ERR_FAIL_COND_MSG(p_object && p_object->is_extension_placeholder(), vformat("Cannot call BRExtension method bind '%s' on placeholder instance.", name));
#endif
		ERR_FAIL_COND_MSG(vararg, "Vararg methods don't have ptrcall support. This is most likely an engine bug.");
		BRExtensionClassInstancePtr extension_instance = is_static() ? nullptr : p_object->_get_extension_instance();
		ptrcall_func(method_userdata, extension_instance, reinterpret_cast<BRExtensionConstTypePtr *>(p_args), (BRExtensionTypePtr)r_ret);
	}

	virtual bool is_vararg() const override {
		return false;
	}

#ifdef TOOLS_ENABLED
	bool try_update(const BRExtensionClassMethodInfo *p_method_info) {
		if (is_static() != (bool)(p_method_info->method_flags & BREXTENSION_METHOD_FLAG_STATIC)) {
			return false;
		}

		if (vararg != (bool)(p_method_info->method_flags & BREXTENSION_METHOD_FLAG_VARARG)) {
			return false;
		}

		if (has_return() != (bool)p_method_info->has_return_value) {
			return false;
		}

		if (has_return() && return_value_info.type != (Variant::Type)p_method_info->return_value_info->type) {
			return false;
		}

		if (argument_count != p_method_info->argument_count) {
			return false;
		}

		List<PropertyInfo>::ConstIterator itr = arguments_info.begin();
		for (uint32_t i = 0; i < p_method_info->argument_count; ++itr, ++i) {
			if (itr->type != (Variant::Type)p_method_info->arguments_info[i].type) {
				return false;
			}
		}

		update(p_method_info);
		return true;
	}
#endif

	void update(const BRExtensionClassMethodInfo *p_method_info) {
#ifdef TOOLS_ENABLED
		name = *reinterpret_cast<StringName *>(p_method_info->name);
#endif
		method_userdata = p_method_info->method_userdata;
		call_func = p_method_info->call_func;
		validated_call_func = nullptr;
		ptrcall_func = p_method_info->ptrcall_func;
		set_name(*reinterpret_cast<StringName *>(p_method_info->name));

		if (p_method_info->has_return_value) {
			return_value_info = PropertyInfo(*p_method_info->return_value_info);
			return_value_metadata = BradotTypeInfo::Metadata(p_method_info->return_value_metadata);
		}

		arguments_info.clear();
		arguments_metadata.clear();
		for (uint32_t i = 0; i < p_method_info->argument_count; i++) {
			arguments_info.push_back(PropertyInfo(p_method_info->arguments_info[i]));
			arguments_metadata.push_back(BradotTypeInfo::Metadata(p_method_info->arguments_metadata[i]));
		}

		set_hint_flags(p_method_info->method_flags);
		argument_count = p_method_info->argument_count;
		vararg = p_method_info->method_flags & BREXTENSION_METHOD_FLAG_VARARG;
		_set_returns(p_method_info->has_return_value);
		_set_const(p_method_info->method_flags & BREXTENSION_METHOD_FLAG_CONST);
		_set_static(p_method_info->method_flags & BREXTENSION_METHOD_FLAG_STATIC);
#ifdef DEBUG_METHODS_ENABLED
		_generate_argument_types(p_method_info->argument_count);
#endif
		set_argument_count(p_method_info->argument_count);

		Vector<Variant> defargs;
		defargs.resize(p_method_info->default_argument_count);
		for (uint32_t i = 0; i < p_method_info->default_argument_count; i++) {
			defargs.write[i] = *static_cast<Variant *>(p_method_info->default_arguments[i]);
		}

		set_default_arguments(defargs);
	}

	explicit BRExtensionMethodBind(const BRExtensionClassMethodInfo *p_method_info) {
		update(p_method_info);
	}
};

#ifndef DISABLE_DEPRECATED
void BRExtension::_register_extension_class(BRExtensionClassLibraryPtr p_library, BRExtensionConstStringNamePtr p_class_name, BRExtensionConstStringNamePtr p_parent_class_name, const BRExtensionClassCreationInfo *p_extension_funcs) {
	const BRExtensionClassCreationInfo4 class_info4 = {
		p_extension_funcs->is_virtual, // BRExtensionBool is_virtual;
		p_extension_funcs->is_abstract, // BRExtensionBool is_abstract;
		true, // BRExtensionBool is_exposed;
		false, // BRExtensionBool is_runtime;
		p_extension_funcs->set_func, // BRExtensionClassSet set_func;
		p_extension_funcs->get_func, // BRExtensionClassGet get_func;
		p_extension_funcs->get_property_list_func, // BRExtensionClassGetPropertyList get_property_list_func;
		nullptr, // BRExtensionClassFreePropertyList2 free_property_list_func;
		p_extension_funcs->property_can_revert_func, // BRExtensionClassPropertyCanRevert property_can_revert_func;
		p_extension_funcs->property_get_revert_func, // BRExtensionClassPropertyGetRevert property_get_revert_func;
		nullptr, // BRExtensionClassValidateProperty validate_property_func;
		nullptr, // BRExtensionClassNotification2 notification_func;
		p_extension_funcs->to_string_func, // BRExtensionClassToString to_string_func;
		p_extension_funcs->reference_func, // BRExtensionClassReference reference_func;
		p_extension_funcs->unreference_func, // BRExtensionClassUnreference unreference_func;
		nullptr, // BRExtensionClassCreateInstance2 create_instance_func; /* this one is mandatory */
		p_extension_funcs->free_instance_func, // BRExtensionClassFreeInstance free_instance_func; /* this one is mandatory */
		nullptr, // BRExtensionClassRecreateInstance recreate_instance_func;
		p_extension_funcs->get_virtual_func, // BRExtensionClassGetVirtual get_virtual_func;
		nullptr, // BRExtensionClassGetVirtualCallData get_virtual_call_data_func;
		nullptr, // BRExtensionClassCallVirtualWithData call_virtual_func;
		p_extension_funcs->class_userdata, // void *class_userdata;
	};

	const ClassCreationDeprecatedInfo legacy = {
		p_extension_funcs->notification_func, // BRExtensionClassNotification notification_func;
		p_extension_funcs->free_property_list_func, // BRExtensionClassFreePropertyList free_property_list_func;
		p_extension_funcs->create_instance_func, // BRExtensionClassCreateInstance create_instance_func;
		p_extension_funcs->get_rid_func, // BRExtensionClassGetRID get_rid;
	};
	_register_extension_class_internal(p_library, p_class_name, p_parent_class_name, &class_info4, &legacy);
}

void BRExtension::_register_extension_class2(BRExtensionClassLibraryPtr p_library, BRExtensionConstStringNamePtr p_class_name, BRExtensionConstStringNamePtr p_parent_class_name, const BRExtensionClassCreationInfo2 *p_extension_funcs) {
	const BRExtensionClassCreationInfo4 class_info4 = {
		p_extension_funcs->is_virtual, // BRExtensionBool is_virtual;
		p_extension_funcs->is_abstract, // BRExtensionBool is_abstract;
		p_extension_funcs->is_exposed, // BRExtensionBool is_exposed;
		false, // BRExtensionBool is_runtime;
		p_extension_funcs->set_func, // BRExtensionClassSet set_func;
		p_extension_funcs->get_func, // BRExtensionClassGet get_func;
		p_extension_funcs->get_property_list_func, // BRExtensionClassGetPropertyList get_property_list_func;
		nullptr, // BRExtensionClassFreePropertyList2 free_property_list_func;
		p_extension_funcs->property_can_revert_func, // BRExtensionClassPropertyCanRevert property_can_revert_func;
		p_extension_funcs->property_get_revert_func, // BRExtensionClassPropertyGetRevert property_get_revert_func;
		p_extension_funcs->validate_property_func, // BRExtensionClassValidateProperty validate_property_func;
		p_extension_funcs->notification_func, // BRExtensionClassNotification2 notification_func;
		p_extension_funcs->to_string_func, // BRExtensionClassToString to_string_func;
		p_extension_funcs->reference_func, // BRExtensionClassReference reference_func;
		p_extension_funcs->unreference_func, // BRExtensionClassUnreference unreference_func;
		nullptr, // BRExtensionClassCreateInstance2 create_instance_func; /* this one is mandatory */
		p_extension_funcs->free_instance_func, // BRExtensionClassFreeInstance free_instance_func; /* this one is mandatory */
		p_extension_funcs->recreate_instance_func, // BRExtensionClassRecreateInstance recreate_instance_func;
		p_extension_funcs->get_virtual_func, // BRExtensionClassGetVirtual get_virtual_func;
		p_extension_funcs->get_virtual_call_data_func, // BRExtensionClassGetVirtualCallData get_virtual_call_data_func;
		p_extension_funcs->call_virtual_with_data_func, // BRExtensionClassCallVirtualWithData call_virtual_func;
		p_extension_funcs->class_userdata, // void *class_userdata;
	};

	const ClassCreationDeprecatedInfo legacy = {
		nullptr, // BRExtensionClassNotification notification_func;
		p_extension_funcs->free_property_list_func, // BRExtensionClassFreePropertyList free_property_list_func;
		p_extension_funcs->create_instance_func, // BRExtensionClassCreateInstance create_instance_func;
		p_extension_funcs->get_rid_func, // BRExtensionClassGetRID get_rid;
	};
	_register_extension_class_internal(p_library, p_class_name, p_parent_class_name, &class_info4, &legacy);
}

void BRExtension::_register_extension_class3(BRExtensionClassLibraryPtr p_library, BRExtensionConstStringNamePtr p_class_name, BRExtensionConstStringNamePtr p_parent_class_name, const BRExtensionClassCreationInfo3 *p_extension_funcs) {
	const BRExtensionClassCreationInfo4 class_info4 = {
		p_extension_funcs->is_virtual, // BRExtensionBool is_virtual;
		p_extension_funcs->is_abstract, // BRExtensionBool is_abstract;
		p_extension_funcs->is_exposed, // BRExtensionBool is_exposed;
		p_extension_funcs->is_runtime, // BRExtensionBool is_runtime;
		p_extension_funcs->set_func, // BRExtensionClassSet set_func;
		p_extension_funcs->get_func, // BRExtensionClassGet get_func;
		p_extension_funcs->get_property_list_func, // BRExtensionClassGetPropertyList get_property_list_func;
		p_extension_funcs->free_property_list_func, // BRExtensionClassFreePropertyList free_property_list_func;
		p_extension_funcs->property_can_revert_func, // BRExtensionClassPropertyCanRevert property_can_revert_func;
		p_extension_funcs->property_get_revert_func, // BRExtensionClassPropertyGetRevert property_get_revert_func;
		p_extension_funcs->validate_property_func, // BRExtensionClassValidateProperty validate_property_func;
		p_extension_funcs->notification_func, // BRExtensionClassNotification2 notification_func;
		p_extension_funcs->to_string_func, // BRExtensionClassToString to_string_func;
		p_extension_funcs->reference_func, // BRExtensionClassReference reference_func;
		p_extension_funcs->unreference_func, // BRExtensionClassUnreference unreference_func;
		nullptr, // BRExtensionClassCreateInstance2 create_instance_func; /* this one is mandatory */
		p_extension_funcs->free_instance_func, // BRExtensionClassFreeInstance free_instance_func; /* this one is mandatory */
		p_extension_funcs->recreate_instance_func, // BRExtensionClassRecreateInstance recreate_instance_func;
		p_extension_funcs->get_virtual_func, // BRExtensionClassGetVirtual get_virtual_func;
		p_extension_funcs->get_virtual_call_data_func, // BRExtensionClassGetVirtualCallData get_virtual_call_data_func;
		p_extension_funcs->call_virtual_with_data_func, // BRExtensionClassCallVirtualWithData call_virtual_func;
		p_extension_funcs->class_userdata, // void *class_userdata;
	};

	const ClassCreationDeprecatedInfo legacy = {
		nullptr, // BRExtensionClassNotification notification_func;
		nullptr, // BRExtensionClassFreePropertyList free_property_list_func;
		p_extension_funcs->create_instance_func, // BRExtensionClassCreateInstance2 create_instance_func;
		p_extension_funcs->get_rid_func, // BRExtensionClassGetRID get_rid;
	};
	_register_extension_class_internal(p_library, p_class_name, p_parent_class_name, &class_info4, &legacy);
}

#endif // DISABLE_DEPRECATED

void BRExtension::_register_extension_class4(BRExtensionClassLibraryPtr p_library, BRExtensionConstStringNamePtr p_class_name, BRExtensionConstStringNamePtr p_parent_class_name, const BRExtensionClassCreationInfo4 *p_extension_funcs) {
	_register_extension_class_internal(p_library, p_class_name, p_parent_class_name, p_extension_funcs);
}

void BRExtension::_register_extension_class_internal(BRExtensionClassLibraryPtr p_library, BRExtensionConstStringNamePtr p_class_name, BRExtensionConstStringNamePtr p_parent_class_name, const BRExtensionClassCreationInfo4 *p_extension_funcs, const ClassCreationDeprecatedInfo *p_deprecated_funcs) {
	BRExtension *self = reinterpret_cast<BRExtension *>(p_library);

	StringName class_name = *reinterpret_cast<const StringName *>(p_class_name);
	StringName parent_class_name = *reinterpret_cast<const StringName *>(p_parent_class_name);
	ERR_FAIL_COND_MSG(!String(class_name).is_valid_unicode_identifier(), vformat("Attempt to register extension class '%s', which is not a valid class identifier.", class_name));
	ERR_FAIL_COND_MSG(ClassDB::class_exists(class_name), vformat("Attempt to register extension class '%s', which appears to be already registered.", class_name));

	Extension *parent_extension = nullptr;

	if (self->extension_classes.has(parent_class_name)) {
		parent_extension = &self->extension_classes[parent_class_name];
	} else if (ClassDB::class_exists(parent_class_name)) {
		if (ClassDB::get_api_type(parent_class_name) == ClassDB::API_EXTENSION || ClassDB::get_api_type(parent_class_name) == ClassDB::API_EDITOR_EXTENSION) {
			ERR_PRINT("Unimplemented yet");
			//inheriting from another extension
		} else {
			//inheriting from engine class
		}
	} else {
		ERR_FAIL_MSG(vformat("Attempt to register an extension class '%s' using non-existing parent class '%s'.", String(class_name), String(parent_class_name)));
	}

#ifdef TOOLS_ENABLED
	Extension *extension = nullptr;
	bool is_runtime = (bool)p_extension_funcs->is_runtime;
	if (self->is_reloading && self->extension_classes.has(class_name)) {
		extension = &self->extension_classes[class_name];
		if (!parent_extension && parent_class_name != extension->brextension.parent_class_name) {
			ERR_FAIL_MSG(vformat("BRExtension class '%s' cannot change parent type from '%s' to '%s' on hot reload. Restart Bradot for this change to take effect.", class_name, extension->brextension.parent_class_name, parent_class_name));
		}
		if (extension->brextension.is_runtime != is_runtime) {
			ERR_PRINT(vformat("BRExtension class '%s' cannot change to/from runtime class on hot reload. Restart Bradot for this change to take effect.", class_name));
			is_runtime = extension->brextension.is_runtime;
		}
		extension->is_reloading = false;
	} else {
		self->extension_classes[class_name] = Extension();
		extension = &self->extension_classes[class_name];
	}
#else
	self->extension_classes[class_name] = Extension();
	Extension *extension = &self->extension_classes[class_name];
#endif

	if (parent_extension) {
		extension->brextension.parent = &parent_extension->brextension;
		parent_extension->brextension.children.push_back(&extension->brextension);
	}

	if (self->reloadable && p_extension_funcs->recreate_instance_func == nullptr) {
		ERR_PRINT(vformat("Extension marked as reloadable, but attempted to register class '%s' which doesn't support reloading. Perhaps your language binding don't support it? Reloading disabled for this extension.", class_name));
		self->reloadable = false;
	}

	extension->brextension.library = self;
	extension->brextension.parent_class_name = parent_class_name;
	extension->brextension.class_name = class_name;
	extension->brextension.editor_class = self->level_initialized == INITIALIZATION_LEVEL_EDITOR;
	extension->brextension.is_virtual = p_extension_funcs->is_virtual;
	extension->brextension.is_abstract = p_extension_funcs->is_abstract;
	extension->brextension.is_exposed = p_extension_funcs->is_exposed;
#ifdef TOOLS_ENABLED
	extension->brextension.is_runtime = is_runtime;
#endif
	extension->brextension.set = p_extension_funcs->set_func;
	extension->brextension.get = p_extension_funcs->get_func;
	extension->brextension.get_property_list = p_extension_funcs->get_property_list_func;
	extension->brextension.free_property_list2 = p_extension_funcs->free_property_list_func;
	extension->brextension.property_can_revert = p_extension_funcs->property_can_revert_func;
	extension->brextension.property_get_revert = p_extension_funcs->property_get_revert_func;
	extension->brextension.validate_property = p_extension_funcs->validate_property_func;
#ifndef DISABLE_DEPRECATED
	if (p_deprecated_funcs) {
		extension->brextension.notification = p_deprecated_funcs->notification_func;
		extension->brextension.free_property_list = p_deprecated_funcs->free_property_list_func;
		extension->brextension.create_instance = p_deprecated_funcs->create_instance_func;
		extension->brextension.get_rid = p_deprecated_funcs->get_rid_func;
	}
#endif // DISABLE_DEPRECATED
	extension->brextension.notification2 = p_extension_funcs->notification_func;
	extension->brextension.to_string = p_extension_funcs->to_string_func;
	extension->brextension.reference = p_extension_funcs->reference_func;
	extension->brextension.unreference = p_extension_funcs->unreference_func;
	extension->brextension.class_userdata = p_extension_funcs->class_userdata;
	extension->brextension.create_instance2 = p_extension_funcs->create_instance_func;
	extension->brextension.free_instance = p_extension_funcs->free_instance_func;
	extension->brextension.recreate_instance = p_extension_funcs->recreate_instance_func;
	extension->brextension.get_virtual = p_extension_funcs->get_virtual_func;
	extension->brextension.get_virtual_call_data = p_extension_funcs->get_virtual_call_data_func;
	extension->brextension.call_virtual_with_data = p_extension_funcs->call_virtual_with_data_func;

	extension->brextension.reloadable = self->reloadable;
#ifdef TOOLS_ENABLED
	if (extension->brextension.reloadable) {
		extension->brextension.tracking_userdata = extension;
		extension->brextension.track_instance = &BRExtension::_track_instance;
		extension->brextension.untrack_instance = &BRExtension::_untrack_instance;
	} else {
		extension->brextension.tracking_userdata = nullptr;
		extension->brextension.track_instance = nullptr;
		extension->brextension.untrack_instance = nullptr;
	}
#endif

	ClassDB::register_extension_class(&extension->brextension);
}

void BRExtension::_register_extension_class_method(BRExtensionClassLibraryPtr p_library, BRExtensionConstStringNamePtr p_class_name, const BRExtensionClassMethodInfo *p_method_info) {
	BRExtension *self = reinterpret_cast<BRExtension *>(p_library);

	StringName class_name = *reinterpret_cast<const StringName *>(p_class_name);
	StringName method_name = *reinterpret_cast<const StringName *>(p_method_info->name);
	ERR_FAIL_COND_MSG(!self->extension_classes.has(class_name), vformat("Attempt to register extension method '%s' for unexisting class '%s'.", String(method_name), class_name));

#ifdef TOOLS_ENABLED
	Extension *extension = &self->extension_classes[class_name];
	BRExtensionMethodBind *method = nullptr;

	// If the extension is still marked as reloading, that means it failed to register again.
	if (extension->is_reloading) {
		return;
	}

	if (self->is_reloading && extension->methods.has(method_name)) {
		method = extension->methods[method_name];

		// Try to update the method bind. If it doesn't work (because it's incompatible) then
		// mark as invalid and create a new one.
		if (!method->is_reloading || !method->try_update(p_method_info)) {
			method->valid = false;
			self->invalid_methods.push_back(method);

			method = nullptr;
		}
	}

	if (method == nullptr) {
		method = memnew(BRExtensionMethodBind(p_method_info));
		method->set_instance_class(class_name);
		extension->methods[method_name] = method;
	} else {
		method->is_reloading = false;
	}
#else
	BRExtensionMethodBind *method = memnew(BRExtensionMethodBind(p_method_info));
	method->set_instance_class(class_name);
#endif

	ClassDB::bind_method_custom(class_name, method);
}

void BRExtension::_register_extension_class_virtual_method(BRExtensionClassLibraryPtr p_library, BRExtensionConstStringNamePtr p_class_name, const BRExtensionClassVirtualMethodInfo *p_method_info) {
	StringName class_name = *reinterpret_cast<const StringName *>(p_class_name);
	ClassDB::add_extension_class_virtual_method(class_name, p_method_info);
}

void BRExtension::_register_extension_class_integer_constant(BRExtensionClassLibraryPtr p_library, BRExtensionConstStringNamePtr p_class_name, BRExtensionConstStringNamePtr p_enum_name, BRExtensionConstStringNamePtr p_constant_name, BRExtensionInt p_constant_value, BRExtensionBool p_is_bitfield) {
	BRExtension *self = reinterpret_cast<BRExtension *>(p_library);

	StringName class_name = *reinterpret_cast<const StringName *>(p_class_name);
	StringName enum_name = *reinterpret_cast<const StringName *>(p_enum_name);
	StringName constant_name = *reinterpret_cast<const StringName *>(p_constant_name);
	ERR_FAIL_COND_MSG(!self->extension_classes.has(class_name), vformat("Attempt to register extension constant '%s' for unexisting class '%s'.", constant_name, class_name));

#ifdef TOOLS_ENABLED
	// If the extension is still marked as reloading, that means it failed to register again.
	Extension *extension = &self->extension_classes[class_name];
	if (extension->is_reloading) {
		return;
	}
#endif

	ClassDB::bind_integer_constant(class_name, enum_name, constant_name, p_constant_value, p_is_bitfield);
}

void BRExtension::_register_extension_class_property(BRExtensionClassLibraryPtr p_library, BRExtensionConstStringNamePtr p_class_name, const BRExtensionPropertyInfo *p_info, BRExtensionConstStringNamePtr p_setter, BRExtensionConstStringNamePtr p_getter) {
	_register_extension_class_property_indexed(p_library, p_class_name, p_info, p_setter, p_getter, -1);
}

void BRExtension::_register_extension_class_property_indexed(BRExtensionClassLibraryPtr p_library, BRExtensionConstStringNamePtr p_class_name, const BRExtensionPropertyInfo *p_info, BRExtensionConstStringNamePtr p_setter, BRExtensionConstStringNamePtr p_getter, BRExtensionInt p_index) {
	BRExtension *self = reinterpret_cast<BRExtension *>(p_library);

	StringName class_name = *reinterpret_cast<const StringName *>(p_class_name);
	StringName setter = *reinterpret_cast<const StringName *>(p_setter);
	StringName getter = *reinterpret_cast<const StringName *>(p_getter);
	String property_name = *reinterpret_cast<const StringName *>(p_info->name);
	ERR_FAIL_COND_MSG(!self->extension_classes.has(class_name), vformat("Attempt to register extension class property '%s' for unexisting class '%s'.", property_name, class_name));

#ifdef TOOLS_ENABLED
	// If the extension is still marked as reloading, that means it failed to register again.
	Extension *extension = &self->extension_classes[class_name];
	if (extension->is_reloading) {
		return;
	}
#endif

	PropertyInfo pinfo(*p_info);

	ClassDB::add_property(class_name, pinfo, setter, getter, p_index);
}

void BRExtension::_register_extension_class_property_group(BRExtensionClassLibraryPtr p_library, BRExtensionConstStringNamePtr p_class_name, BRExtensionConstStringPtr p_group_name, BRExtensionConstStringPtr p_prefix) {
	BRExtension *self = reinterpret_cast<BRExtension *>(p_library);

	StringName class_name = *reinterpret_cast<const StringName *>(p_class_name);
	String group_name = *reinterpret_cast<const String *>(p_group_name);
	String prefix = *reinterpret_cast<const String *>(p_prefix);
	ERR_FAIL_COND_MSG(!self->extension_classes.has(class_name), vformat("Attempt to register extension class property group '%s' for unexisting class '%s'.", group_name, class_name));

#ifdef TOOLS_ENABLED
	// If the extension is still marked as reloading, that means it failed to register again.
	Extension *extension = &self->extension_classes[class_name];
	if (extension->is_reloading) {
		return;
	}
#endif

	ClassDB::add_property_group(class_name, group_name, prefix);
}

void BRExtension::_register_extension_class_property_subgroup(BRExtensionClassLibraryPtr p_library, BRExtensionConstStringNamePtr p_class_name, BRExtensionConstStringPtr p_subgroup_name, BRExtensionConstStringPtr p_prefix) {
	BRExtension *self = reinterpret_cast<BRExtension *>(p_library);

	StringName class_name = *reinterpret_cast<const StringName *>(p_class_name);
	String subgroup_name = *reinterpret_cast<const String *>(p_subgroup_name);
	String prefix = *reinterpret_cast<const String *>(p_prefix);
	ERR_FAIL_COND_MSG(!self->extension_classes.has(class_name), vformat("Attempt to register extension class property subgroup '%s' for unexisting class '%s'.", subgroup_name, class_name));

#ifdef TOOLS_ENABLED
	// If the extension is still marked as reloading, that means it failed to register again.
	Extension *extension = &self->extension_classes[class_name];
	if (extension->is_reloading) {
		return;
	}
#endif

	ClassDB::add_property_subgroup(class_name, subgroup_name, prefix);
}

void BRExtension::_register_extension_class_signal(BRExtensionClassLibraryPtr p_library, BRExtensionConstStringNamePtr p_class_name, BRExtensionConstStringNamePtr p_signal_name, const BRExtensionPropertyInfo *p_argument_info, BRExtensionInt p_argument_count) {
	BRExtension *self = reinterpret_cast<BRExtension *>(p_library);

	StringName class_name = *reinterpret_cast<const StringName *>(p_class_name);
	StringName signal_name = *reinterpret_cast<const StringName *>(p_signal_name);
	ERR_FAIL_COND_MSG(!self->extension_classes.has(class_name), vformat("Attempt to register extension class signal '%s' for unexisting class '%s'.", signal_name, class_name));

#ifdef TOOLS_ENABLED
	// If the extension is still marked as reloading, that means it failed to register again.
	Extension *extension = &self->extension_classes[class_name];
	if (extension->is_reloading) {
		return;
	}
#endif

	MethodInfo s;
	s.name = signal_name;
	for (int i = 0; i < p_argument_count; i++) {
		PropertyInfo arg(p_argument_info[i]);
		s.arguments.push_back(arg);
	}
	ClassDB::add_signal(class_name, s);
}

void BRExtension::_unregister_extension_class(BRExtensionClassLibraryPtr p_library, BRExtensionConstStringNamePtr p_class_name) {
	BRExtension *self = reinterpret_cast<BRExtension *>(p_library);

	StringName class_name = *reinterpret_cast<const StringName *>(p_class_name);
	ERR_FAIL_COND_MSG(!self->extension_classes.has(class_name), vformat("Attempt to unregister unexisting extension class '%s'.", class_name));

	Extension *ext = &self->extension_classes[class_name];
#ifdef TOOLS_ENABLED
	if (ext->is_reloading) {
		self->_clear_extension(ext);
	}
#endif
	ERR_FAIL_COND_MSG(ext->brextension.children.size(), vformat("Attempt to unregister class '%s' while other extension classes inherit from it.", class_name));

#ifdef TOOLS_ENABLED
	ClassDB::unregister_extension_class(class_name, !ext->is_reloading);
#else
	ClassDB::unregister_extension_class(class_name);
#endif

	if (ext->brextension.parent != nullptr) {
		ext->brextension.parent->children.erase(&ext->brextension);
	}

#ifdef TOOLS_ENABLED
	if (!ext->is_reloading) {
		self->extension_classes.erase(class_name);
	}

	BRExtensionEditorHelp::remove_class(class_name);
#else
	self->extension_classes.erase(class_name);
#endif
}

void BRExtension::_get_library_path(BRExtensionClassLibraryPtr p_library, BRExtensionUninitializedStringPtr r_path) {
	BRExtension *self = reinterpret_cast<BRExtension *>(p_library);

	Ref<BRExtensionLibraryLoader> library_loader = self->loader;
	String library_path;
	if (library_loader.is_valid()) {
		library_path = library_loader->library_path;
	}

	memnew_placement(r_path, String(library_path));
}

HashMap<StringName, BRExtensionInterfaceFunctionPtr> BRExtension::brextension_interface_functions;

void BRExtension::register_interface_function(const StringName &p_function_name, BRExtensionInterfaceFunctionPtr p_function_pointer) {
	ERR_FAIL_COND_MSG(brextension_interface_functions.has(p_function_name), vformat("Attempt to register interface function '%s', which appears to be already registered.", p_function_name));
	brextension_interface_functions.insert(p_function_name, p_function_pointer);
}

BRExtensionInterfaceFunctionPtr BRExtension::get_interface_function(const StringName &p_function_name) {
	BRExtensionInterfaceFunctionPtr *function = brextension_interface_functions.getptr(p_function_name);
	ERR_FAIL_NULL_V_MSG(function, nullptr, vformat("Attempt to get non-existent interface function: '%s'.", String(p_function_name)));
	return *function;
}

Error BRExtension::open_library(const String &p_path, const Ref<BRExtensionLoader> &p_loader) {
	ERR_FAIL_COND_V_MSG(p_loader.is_null(), FAILED, "Can't open BRExtension without a loader.");
	loader = p_loader;

	Error err = loader->open_library(p_path);

	ERR_FAIL_COND_V_MSG(err == ERR_FILE_NOT_FOUND, err, vformat("BRExtension dynamic library not found: '%s'.", p_path));
	ERR_FAIL_COND_V_MSG(err != OK, err, vformat("Can't open BRExtension dynamic library: '%s'.", p_path));

	err = loader->initialize(&brextension_get_proc_address, this, &initialization);

	if (err != OK) {
		// Errors already logged in initialize().
		loader->close_library();
		return err;
	}

	level_initialized = -1;

	return OK;
}

void BRExtension::close_library() {
	ERR_FAIL_COND(!is_library_open());
	loader->close_library();

	class_icon_paths.clear();

#ifdef TOOLS_ENABLED
	instance_bindings.clear();
#endif
}

bool BRExtension::is_library_open() const {
	return loader.is_valid() && loader->is_library_open();
}

BRExtension::InitializationLevel BRExtension::get_minimum_library_initialization_level() const {
	ERR_FAIL_COND_V(!is_library_open(), INITIALIZATION_LEVEL_CORE);
	return InitializationLevel(initialization.minimum_initialization_level);
}

void BRExtension::initialize_library(InitializationLevel p_level) {
	ERR_FAIL_COND(!is_library_open());
	ERR_FAIL_COND_MSG(p_level <= int32_t(level_initialized), vformat("Level '%d' must be higher than the current level '%d'", p_level, level_initialized));

	level_initialized = int32_t(p_level);

	ERR_FAIL_NULL(initialization.initialize);

	initialization.initialize(initialization.userdata, BRExtensionInitializationLevel(p_level));
}
void BRExtension::deinitialize_library(InitializationLevel p_level) {
	ERR_FAIL_COND(!is_library_open());
	ERR_FAIL_COND(p_level > int32_t(level_initialized));

	level_initialized = int32_t(p_level) - 1;

	ERR_FAIL_NULL(initialization.deinitialize);

	initialization.deinitialize(initialization.userdata, BRExtensionInitializationLevel(p_level));
}

void BRExtension::_bind_methods() {
	ClassDB::bind_method(D_METHOD("is_library_open"), &BRExtension::is_library_open);
	ClassDB::bind_method(D_METHOD("get_minimum_library_initialization_level"), &BRExtension::get_minimum_library_initialization_level);

	BIND_ENUM_CONSTANT(INITIALIZATION_LEVEL_CORE);
	BIND_ENUM_CONSTANT(INITIALIZATION_LEVEL_SERVERS);
	BIND_ENUM_CONSTANT(INITIALIZATION_LEVEL_SCENE);
	BIND_ENUM_CONSTANT(INITIALIZATION_LEVEL_EDITOR);
}

BRExtension::BRExtension() {
}

BRExtension::~BRExtension() {
	if (is_library_open()) {
		close_library();
	}
#ifdef TOOLS_ENABLED
	// If we have any invalid method binds still laying around, we can finally free them!
	for (BRExtensionMethodBind *E : invalid_methods) {
		memdelete(E);
	}
#endif
}

void BRExtension::initialize_brextensions() {
	brextension_setup_interface();

#ifndef DISABLE_DEPRECATED
	register_interface_function("classdb_register_extension_class", (BRExtensionInterfaceFunctionPtr)&BRExtension::_register_extension_class);
	register_interface_function("classdb_register_extension_class2", (BRExtensionInterfaceFunctionPtr)&BRExtension::_register_extension_class2);
	register_interface_function("classdb_register_extension_class3", (BRExtensionInterfaceFunctionPtr)&BRExtension::_register_extension_class3);
#endif // DISABLE_DEPRECATED
	register_interface_function("classdb_register_extension_class4", (BRExtensionInterfaceFunctionPtr)&BRExtension::_register_extension_class4);
	register_interface_function("classdb_register_extension_class_method", (BRExtensionInterfaceFunctionPtr)&BRExtension::_register_extension_class_method);
	register_interface_function("classdb_register_extension_class_virtual_method", (BRExtensionInterfaceFunctionPtr)&BRExtension::_register_extension_class_virtual_method);
	register_interface_function("classdb_register_extension_class_integer_constant", (BRExtensionInterfaceFunctionPtr)&BRExtension::_register_extension_class_integer_constant);
	register_interface_function("classdb_register_extension_class_property", (BRExtensionInterfaceFunctionPtr)&BRExtension::_register_extension_class_property);
	register_interface_function("classdb_register_extension_class_property_indexed", (BRExtensionInterfaceFunctionPtr)&BRExtension::_register_extension_class_property_indexed);
	register_interface_function("classdb_register_extension_class_property_group", (BRExtensionInterfaceFunctionPtr)&BRExtension::_register_extension_class_property_group);
	register_interface_function("classdb_register_extension_class_property_subgroup", (BRExtensionInterfaceFunctionPtr)&BRExtension::_register_extension_class_property_subgroup);
	register_interface_function("classdb_register_extension_class_signal", (BRExtensionInterfaceFunctionPtr)&BRExtension::_register_extension_class_signal);
	register_interface_function("classdb_unregister_extension_class", (BRExtensionInterfaceFunctionPtr)&BRExtension::_unregister_extension_class);
	register_interface_function("get_library_path", (BRExtensionInterfaceFunctionPtr)&BRExtension::_get_library_path);
}

void BRExtension::finalize_brextensions() {
	brextension_interface_functions.clear();
}

Error BRExtensionResourceLoader::load_brextension_resource(const String &p_path, Ref<BRExtension> &p_extension) {
	ERR_FAIL_COND_V_MSG(p_extension.is_valid() && p_extension->is_library_open(), ERR_ALREADY_IN_USE, "Cannot load BRExtension resource into already opened library.");

	BRExtensionManager *extension_manager = BRExtensionManager::get_singleton();

	BRExtensionManager::LoadStatus status = extension_manager->load_extension(p_path);
	if (status != BRExtensionManager::LOAD_STATUS_OK && status != BRExtensionManager::LOAD_STATUS_ALREADY_LOADED) {
		// Errors already logged in load_extension().
		return FAILED;
	}

	p_extension = extension_manager->get_extension(p_path);
	return OK;
}

Ref<Resource> BRExtensionResourceLoader::load(const String &p_path, const String &p_original_path, Error *r_error, bool p_use_sub_threads, float *r_progress, CacheMode p_cache_mode) {
	// We can't have two BRExtension resource object representing the same library, because
	// loading (or unloading) a BRExtension affects global data. So, we need reuse the same
	// object if one has already been loaded (even if caching is disabled at the resource
	// loader level).
	BRExtensionManager *manager = BRExtensionManager::get_singleton();
	if (manager->is_extension_loaded(p_path)) {
		return manager->get_extension(p_path);
	}

	Ref<BRExtension> lib;
	Error err = load_brextension_resource(p_path, lib);
	if (err != OK && r_error) {
		// Errors already logged in load_brextension_resource().
		*r_error = err;
	}
	return lib;
}

void BRExtensionResourceLoader::get_recognized_extensions(List<String> *p_extensions) const {
	p_extensions->push_back("brextension");
}

bool BRExtensionResourceLoader::handles_type(const String &p_type) const {
	return p_type == "BRExtension";
}

String BRExtensionResourceLoader::get_resource_type(const String &p_path) const {
	String el = p_path.get_extension().to_lower();
	if (el == "brextension") {
		return "BRExtension";
	}
	return "";
}

#ifdef TOOLS_ENABLED
bool BRExtension::has_library_changed() const {
	return loader->has_library_changed();
}

void BRExtension::prepare_reload() {
	is_reloading = true;

	for (KeyValue<StringName, Extension> &E : extension_classes) {
		E.value.is_reloading = true;

		for (KeyValue<StringName, BRExtensionMethodBind *> &M : E.value.methods) {
			M.value->is_reloading = true;
		}

		for (const ObjectID &obj_id : E.value.instances) {
			Object *obj = ObjectDB::get_instance(obj_id);
			if (!obj) {
				continue;
			}

			// Store instance state so it can be restored after reload.
			List<Pair<String, Variant>> state;
			List<PropertyInfo> prop_list;
			obj->get_property_list(&prop_list);
			for (const PropertyInfo &P : prop_list) {
				if (!(P.usage & PROPERTY_USAGE_STORAGE)) {
					continue;
				}

				Variant value = obj->get(P.name);
				Variant default_value = ClassDB::class_get_default_property_value(obj->get_class_name(), P.name);

				if (default_value.get_type() != Variant::NIL && bool(Variant::evaluate(Variant::OP_EQUAL, value, default_value))) {
					continue;
				}

				if (P.type == Variant::OBJECT && value.is_zero() && !(P.usage & PROPERTY_USAGE_STORE_IF_NULL)) {
					continue;
				}

				state.push_back(Pair<String, Variant>(P.name, value));
			}
			E.value.instance_state[obj_id] = {
				state, // List<Pair<String, Variant>> properties;
				obj->is_extension_placeholder(), // bool is_placeholder;
			};
		}
	}
}

void BRExtension::_clear_extension(Extension *p_extension) {
	// Clear out hierarchy information because it may change.
	p_extension->brextension.parent = nullptr;
	p_extension->brextension.children.clear();

	// Clear all objects of any BRExtension data. It will become its native parent class
	// until the reload can reset the object with the new BRExtension data.
	for (const ObjectID &obj_id : p_extension->instances) {
		Object *obj = ObjectDB::get_instance(obj_id);
		if (!obj) {
			continue;
		}

		obj->clear_internal_extension();
	}
}

void BRExtension::track_instance_binding(Object *p_object) {
	instance_bindings.push_back(p_object->get_instance_id());
}

void BRExtension::untrack_instance_binding(Object *p_object) {
	instance_bindings.erase(p_object->get_instance_id());
}

void BRExtension::clear_instance_bindings() {
	for (ObjectID obj_id : instance_bindings) {
		Object *obj = ObjectDB::get_instance(obj_id);
		if (!obj) {
			continue;
		}

		obj->free_instance_binding(this);
	}
	instance_bindings.clear();
}

void BRExtension::finish_reload() {
	is_reloading = false;

	// Clean up any classes or methods that didn't get re-added.
	Vector<StringName> classes_to_remove;
	for (KeyValue<StringName, Extension> &E : extension_classes) {
		if (E.value.is_reloading) {
			E.value.is_reloading = false;
			classes_to_remove.push_back(E.key);
		}

		Vector<StringName> methods_to_remove;
		for (KeyValue<StringName, BRExtensionMethodBind *> &M : E.value.methods) {
			if (M.value->is_reloading) {
				M.value->valid = false;
				invalid_methods.push_back(M.value);

				M.value->is_reloading = false;
				methods_to_remove.push_back(M.key);
			}
		}
		for (const StringName &method_name : methods_to_remove) {
			E.value.methods.erase(method_name);
		}
	}
	for (const StringName &class_name : classes_to_remove) {
		extension_classes.erase(class_name);
	}

	// Reset any the extension on instances made from the classes that remain.
	for (KeyValue<StringName, Extension> &E : extension_classes) {
		// Loop over 'instance_state' rather than 'instance' because new instances
		// may have been created when re-initializing the extension.
		for (const KeyValue<ObjectID, Extension::InstanceState> &S : E.value.instance_state) {
			Object *obj = ObjectDB::get_instance(S.key);
			if (!obj) {
				continue;
			}

			if (S.value.is_placeholder) {
				obj->reset_internal_extension(ClassDB::get_placeholder_extension(E.value.brextension.class_name));
			} else {
				obj->reset_internal_extension(&E.value.brextension);
			}
		}
	}

	// Now that all the classes are back, restore the state.
	for (KeyValue<StringName, Extension> &E : extension_classes) {
		for (const KeyValue<ObjectID, Extension::InstanceState> &S : E.value.instance_state) {
			Object *obj = ObjectDB::get_instance(S.key);
			if (!obj) {
				continue;
			}

			for (const Pair<String, Variant> &state : S.value.properties) {
				obj->set(state.first, state.second);
			}
		}
	}

	// Finally, let the objects know that we are done reloading them.
	for (KeyValue<StringName, Extension> &E : extension_classes) {
		for (const KeyValue<ObjectID, Extension::InstanceState> &S : E.value.instance_state) {
			Object *obj = ObjectDB::get_instance(S.key);
			if (!obj) {
				continue;
			}

			obj->notification(NOTIFICATION_EXTENSION_RELOADED);
		}

		// Clear the instance state, we're done looping.
		E.value.instance_state.clear();
	}
}

void BRExtension::_track_instance(void *p_user_data, void *p_instance) {
	Extension *extension = reinterpret_cast<Extension *>(p_user_data);
	Object *obj = reinterpret_cast<Object *>(p_instance);

	extension->instances.insert(obj->get_instance_id());
}

void BRExtension::_untrack_instance(void *p_user_data, void *p_instance) {
	Extension *extension = reinterpret_cast<Extension *>(p_user_data);
	Object *obj = reinterpret_cast<Object *>(p_instance);

	extension->instances.erase(obj->get_instance_id());
}

Vector<StringName> BRExtensionEditorPlugins::extension_classes;
BRExtensionEditorPlugins::EditorPluginRegisterFunc BRExtensionEditorPlugins::editor_node_add_plugin = nullptr;
BRExtensionEditorPlugins::EditorPluginRegisterFunc BRExtensionEditorPlugins::editor_node_remove_plugin = nullptr;

void BRExtensionEditorPlugins::add_extension_class(const StringName &p_class_name) {
	if (editor_node_add_plugin) {
		editor_node_add_plugin(p_class_name);
	} else {
		extension_classes.push_back(p_class_name);
	}
}

void BRExtensionEditorPlugins::remove_extension_class(const StringName &p_class_name) {
	if (editor_node_remove_plugin) {
		editor_node_remove_plugin(p_class_name);
	} else {
		extension_classes.erase(p_class_name);
	}
}

BRExtensionEditorHelp::EditorHelpLoadXmlBufferFunc BRExtensionEditorHelp::editor_help_load_xml_buffer = nullptr;
BRExtensionEditorHelp::EditorHelpRemoveClassFunc BRExtensionEditorHelp::editor_help_remove_class = nullptr;

void BRExtensionEditorHelp::load_xml_buffer(const uint8_t *p_buffer, int p_size) {
	ERR_FAIL_NULL(editor_help_load_xml_buffer);
	editor_help_load_xml_buffer(p_buffer, p_size);
}

void BRExtensionEditorHelp::remove_class(const String &p_class) {
	ERR_FAIL_NULL(editor_help_remove_class);
	editor_help_remove_class(p_class);
}
#endif // TOOLS_ENABLED