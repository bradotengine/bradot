/**************************************************************************/
/*  brextension_manager.h                                                 */
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

#ifndef BREXTENSION_MANAGER_H
#define BREXTENSION_MANAGER_H

#include "core/extension/brextension.h"

class BRExtensionManager : public Object {
	BRCLASS(BRExtensionManager, Object);

	int32_t level = -1;
	HashMap<String, Ref<BRExtension>> brextension_map;
	HashMap<String, String> brextension_class_icon_paths;

	static void _bind_methods();

	static BRExtensionManager *singleton;

public:
	enum LoadStatus {
		LOAD_STATUS_OK,
		LOAD_STATUS_FAILED,
		LOAD_STATUS_ALREADY_LOADED,
		LOAD_STATUS_NOT_LOADED,
		LOAD_STATUS_NEEDS_RESTART,
	};

private:
	LoadStatus _load_extension_internal(const Ref<BRExtension> &p_extension, bool p_first_load);
	LoadStatus _unload_extension_internal(const Ref<BRExtension> &p_extension);

#ifdef TOOLS_ENABLED
	static void _reload_all_scripts();
#endif

public:
	LoadStatus load_extension(const String &p_path);
	LoadStatus load_extension_with_loader(const String &p_path, const Ref<BRExtensionLoader> &p_loader);
	LoadStatus reload_extension(const String &p_path);
	LoadStatus unload_extension(const String &p_path);
	bool is_extension_loaded(const String &p_path) const;
	Vector<String> get_loaded_extensions() const;
	Ref<BRExtension> get_extension(const String &p_path);

	bool class_has_icon_path(const String &p_class) const;
	String class_get_icon_path(const String &p_class) const;

	void initialize_extensions(BRExtension::InitializationLevel p_level);
	void deinitialize_extensions(BRExtension::InitializationLevel p_level);

#ifdef TOOLS_ENABLED
	void track_instance_binding(void *p_token, Object *p_object);
	void untrack_instance_binding(void *p_token, Object *p_object);
#endif

	static BRExtensionManager *get_singleton();

	void load_extensions();
	void reload_extensions();
	bool ensure_extensions_loaded(const HashSet<String> &p_extensions);

	BRExtensionManager();
	~BRExtensionManager();
};

VARIANT_ENUM_CAST(BRExtensionManager::LoadStatus)

#endif // BREXTENSION_MANAGER_H
