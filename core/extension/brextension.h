/**************************************************************************/
/*  brextension.h                                                         */
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

#ifndef BREXTENSION_H
#define BREXTENSION_H

#include "core/extension/brextension_interface.h"
#include "core/extension/brextension_loader.h"
#include "core/io/config_file.h"
#include "core/io/resource_loader.h"
#include "core/object/ref_counted.h"

class BRExtensionMethodBind;

class BRExtension : public Resource {
	BRCLASS(BRExtension, Resource)

	friend class BRExtensionManager;

	Ref<BRExtensionLoader> loader;

	bool reloadable = false;

	struct Extension {
		ObjectBRExtension brextension;

#ifdef TOOLS_ENABLED
		bool is_reloading = false;
		HashMap<StringName, BRExtensionMethodBind *> methods;
		HashSet<ObjectID> instances;

		struct InstanceState {
			List<Pair<String, Variant>> properties;
			bool is_placeholder = false;
		};
		HashMap<ObjectID, InstanceState> instance_state;
#endif
	};

	HashMap<StringName, Extension> extension_classes;

	struct ClassCreationDeprecatedInfo {
#ifndef DISABLE_DEPRECATED
		BRExtensionClassNotification notification_func = nullptr;
		BRExtensionClassFreePropertyList free_property_list_func = nullptr;
		BRExtensionClassCreateInstance create_instance_func = nullptr;
		BRExtensionClassGetRID get_rid_func = nullptr;
#endif // DISABLE_DEPRECATED
	};

#ifndef DISABLE_DEPRECATED
	static void _register_extension_class(BRExtensionClassLibraryPtr p_library, BRExtensionConstStringNamePtr p_class_name, BRExtensionConstStringNamePtr p_parent_class_name, const BRExtensionClassCreationInfo *p_extension_funcs);
	static void _register_extension_class2(BRExtensionClassLibraryPtr p_library, BRExtensionConstStringNamePtr p_class_name, BRExtensionConstStringNamePtr p_parent_class_name, const BRExtensionClassCreationInfo2 *p_extension_funcs);
	static void _register_extension_class3(BRExtensionClassLibraryPtr p_library, BRExtensionConstStringNamePtr p_class_name, BRExtensionConstStringNamePtr p_parent_class_name, const BRExtensionClassCreationInfo3 *p_extension_funcs);
#endif // DISABLE_DEPRECATED
	static void _register_extension_class4(BRExtensionClassLibraryPtr p_library, BRExtensionConstStringNamePtr p_class_name, BRExtensionConstStringNamePtr p_parent_class_name, const BRExtensionClassCreationInfo4 *p_extension_funcs);
	static void _register_extension_class_internal(BRExtensionClassLibraryPtr p_library, BRExtensionConstStringNamePtr p_class_name, BRExtensionConstStringNamePtr p_parent_class_name, const BRExtensionClassCreationInfo4 *p_extension_funcs, const ClassCreationDeprecatedInfo *p_deprecated_funcs = nullptr);
	static void _register_extension_class_method(BRExtensionClassLibraryPtr p_library, BRExtensionConstStringNamePtr p_class_name, const BRExtensionClassMethodInfo *p_method_info);
	static void _register_extension_class_virtual_method(BRExtensionClassLibraryPtr p_library, BRExtensionConstStringNamePtr p_class_name, const BRExtensionClassVirtualMethodInfo *p_method_info);
	static void _register_extension_class_integer_constant(BRExtensionClassLibraryPtr p_library, BRExtensionConstStringNamePtr p_class_name, BRExtensionConstStringNamePtr p_enum_name, BRExtensionConstStringNamePtr p_constant_name, BRExtensionInt p_constant_value, BRExtensionBool p_is_bitfield);
	static void _register_extension_class_property(BRExtensionClassLibraryPtr p_library, BRExtensionConstStringNamePtr p_class_name, const BRExtensionPropertyInfo *p_info, BRExtensionConstStringNamePtr p_setter, BRExtensionConstStringNamePtr p_getter);
	static void _register_extension_class_property_indexed(BRExtensionClassLibraryPtr p_library, BRExtensionConstStringNamePtr p_class_name, const BRExtensionPropertyInfo *p_info, BRExtensionConstStringNamePtr p_setter, BRExtensionConstStringNamePtr p_getter, BRExtensionInt p_index);
	static void _register_extension_class_property_group(BRExtensionClassLibraryPtr p_library, BRExtensionConstStringNamePtr p_class_name, BRExtensionConstStringNamePtr p_group_name, BRExtensionConstStringNamePtr p_prefix);
	static void _register_extension_class_property_subgroup(BRExtensionClassLibraryPtr p_library, BRExtensionConstStringNamePtr p_class_name, BRExtensionConstStringNamePtr p_subgroup_name, BRExtensionConstStringNamePtr p_prefix);
	static void _register_extension_class_signal(BRExtensionClassLibraryPtr p_library, BRExtensionConstStringNamePtr p_class_name, BRExtensionConstStringNamePtr p_signal_name, const BRExtensionPropertyInfo *p_argument_info, BRExtensionInt p_argument_count);
	static void _unregister_extension_class(BRExtensionClassLibraryPtr p_library, BRExtensionConstStringNamePtr p_class_name);
	static void _get_library_path(BRExtensionClassLibraryPtr p_library, BRExtensionStringPtr r_path);

	BRExtensionInitialization initialization;
	int32_t level_initialized = -1;

#ifdef TOOLS_ENABLED
	bool is_reloading = false;
	Vector<BRExtensionMethodBind *> invalid_methods;
	Vector<ObjectID> instance_bindings;

	static void _track_instance(void *p_user_data, void *p_instance);
	static void _untrack_instance(void *p_user_data, void *p_instance);

	void _clear_extension(Extension *p_extension);

	// Only called by BRExtensionManager during the reload process.
	void prepare_reload();
	void finish_reload();
	void clear_instance_bindings();
#endif

	static HashMap<StringName, BRExtensionInterfaceFunctionPtr> brextension_interface_functions;

protected:
	static void _bind_methods();

public:
	HashMap<String, String> class_icon_paths;

	virtual bool editor_can_reload_from_file() override { return false; } // Reloading is handled in a special way.

	static String get_extension_list_config_file();

	const Ref<BRExtensionLoader> get_loader() const { return loader; }

	Error open_library(const String &p_path, const Ref<BRExtensionLoader> &p_loader);
	void close_library();
	bool is_library_open() const;

	enum InitializationLevel {
		INITIALIZATION_LEVEL_CORE = BREXTENSION_INITIALIZATION_CORE,
		INITIALIZATION_LEVEL_SERVERS = BREXTENSION_INITIALIZATION_SERVERS,
		INITIALIZATION_LEVEL_SCENE = BREXTENSION_INITIALIZATION_SCENE,
		INITIALIZATION_LEVEL_EDITOR = BREXTENSION_INITIALIZATION_EDITOR
	};

protected:
#ifndef DISABLE_DEPRECATED
	Error _open_library_bind_compat_88418(const String &p_path, const String &p_entry_symbol);
	void _close_library_bind_compat_88418();
	void _initialize_library_bind_compat_88418(InitializationLevel p_level);
	static void _bind_compatibility_methods();
#endif

public:
#ifdef TOOLS_ENABLED
	bool is_reloadable() const { return reloadable; }
	void set_reloadable(bool p_reloadable) { reloadable = p_reloadable; }

	bool has_library_changed() const;

	void track_instance_binding(Object *p_object);
	void untrack_instance_binding(Object *p_object);
#endif

	InitializationLevel get_minimum_library_initialization_level() const;
	void initialize_library(InitializationLevel p_level);
	void deinitialize_library(InitializationLevel p_level);

	static void register_interface_function(const StringName &p_function_name, BRExtensionInterfaceFunctionPtr p_function_pointer);
	static BRExtensionInterfaceFunctionPtr get_interface_function(const StringName &p_function_name);
	static void initialize_brextensions();
	static void finalize_brextensions();

	BRExtension();
	~BRExtension();
};

VARIANT_ENUM_CAST(BRExtension::InitializationLevel)

class BRExtensionResourceLoader : public ResourceFormatLoader {
public:
	static Error load_brextension_resource(const String &p_path, Ref<BRExtension> &p_extension);

	virtual Ref<Resource> load(const String &p_path, const String &p_original_path, Error *r_error, bool p_use_sub_threads = false, float *r_progress = nullptr, CacheMode p_cache_mode = CACHE_MODE_REUSE) override;
	virtual void get_recognized_extensions(List<String> *p_extensions) const override;
	virtual bool handles_type(const String &p_type) const override;
	virtual String get_resource_type(const String &p_path) const override;
};

#ifdef TOOLS_ENABLED
class BRExtensionEditorPlugins {
private:
	static Vector<StringName> extension_classes;

protected:
	friend class EditorNode;

	// Since this in core, we can't directly reference EditorNode, so it will
	// set these function pointers in its constructor.
	typedef void (*EditorPluginRegisterFunc)(const StringName &p_class_name);
	static EditorPluginRegisterFunc editor_node_add_plugin;
	static EditorPluginRegisterFunc editor_node_remove_plugin;

public:
	static void add_extension_class(const StringName &p_class_name);
	static void remove_extension_class(const StringName &p_class_name);

	static const Vector<StringName> &get_extension_classes() {
		return extension_classes;
	}
};

class BRExtensionEditorHelp {
protected:
	friend class EditorHelp;

	// Similarly to EditorNode above, we need to be able to ask EditorHelp to parse
	// new documentation data. Note though that, differently from EditorHelp, this
	// is initialized even _before_ it gets instantiated, as we need to rely on
	// this method while initializing the engine.
	typedef void (*EditorHelpLoadXmlBufferFunc)(const uint8_t *p_buffer, int p_size);
	static EditorHelpLoadXmlBufferFunc editor_help_load_xml_buffer;

	typedef void (*EditorHelpRemoveClassFunc)(const String &p_class);
	static EditorHelpRemoveClassFunc editor_help_remove_class;

public:
	static void load_xml_buffer(const uint8_t *p_buffer, int p_size);
	static void remove_class(const String &p_class);
};

#endif // TOOLS_ENABLED

#endif // BREXTENSION_H