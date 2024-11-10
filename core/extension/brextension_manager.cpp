/**************************************************************************/
/*  brextension_manager.cpp                                               */
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

#include "brextension_manager.h"

#include "core/extension/brextension_compat_hashes.h"
#include "core/extension/brextension_library_loader.h"
#include "core/io/dir_access.h"
#include "core/io/file_access.h"
#include "core/object/script_language.h"

BRExtensionManager::LoadStatus BRExtensionManager::_load_extension_internal(const Ref<BRExtension> &p_extension, bool p_first_load) {
	if (level >= 0) { // Already initialized up to some level.
		int32_t minimum_level = 0;
		if (!p_first_load) {
			minimum_level = p_extension->get_minimum_library_initialization_level();
			if (minimum_level < MIN(level, BRExtension::INITIALIZATION_LEVEL_SCENE)) {
				return LOAD_STATUS_NEEDS_RESTART;
			}
		}
		// Initialize up to current level.
		for (int32_t i = minimum_level; i <= level; i++) {
			p_extension->initialize_library(BRExtension::InitializationLevel(i));
		}
	}

	for (const KeyValue<String, String> &kv : p_extension->class_icon_paths) {
		brextension_class_icon_paths[kv.key] = kv.value;
	}

#ifdef TOOLS_ENABLED
	// Signals that a new extension is loaded so BRScript can register new class names.
	emit_signal("extension_loaded", p_extension);
#endif

	return LOAD_STATUS_OK;
}

BRExtensionManager::LoadStatus BRExtensionManager::_unload_extension_internal(const Ref<BRExtension> &p_extension) {
#ifdef TOOLS_ENABLED
	// Signals that a new extension is unloading so BRScript can unregister class names.
	emit_signal("extension_unloading", p_extension);
#endif

	if (level >= 0) { // Already initialized up to some level.
		// Deinitialize down from current level.
		for (int32_t i = level; i >= BRExtension::INITIALIZATION_LEVEL_CORE; i--) {
			p_extension->deinitialize_library(BRExtension::InitializationLevel(i));
		}
	}

	for (const KeyValue<String, String> &kv : p_extension->class_icon_paths) {
		brextension_class_icon_paths.erase(kv.key);
	}

	return LOAD_STATUS_OK;
}

BRExtensionManager::LoadStatus BRExtensionManager::load_extension(const String &p_path) {
	Ref<BRExtensionLibraryLoader> loader;
	loader.instantiate();
	return BRExtensionManager::get_singleton()->load_extension_with_loader(p_path, loader);
}

BRExtensionManager::LoadStatus BRExtensionManager::load_extension_with_loader(const String &p_path, const Ref<BRExtensionLoader> &p_loader) {
	DEV_ASSERT(p_loader.is_valid());

	if (brextension_map.has(p_path)) {
		return LOAD_STATUS_ALREADY_LOADED;
	}

	Ref<BRExtension> extension;
	extension.instantiate();
	Error err = extension->open_library(p_path, p_loader);
	if (err != OK) {
		return LOAD_STATUS_FAILED;
	}

	LoadStatus status = _load_extension_internal(extension, true);
	if (status != LOAD_STATUS_OK) {
		return status;
	}

	extension->set_path(p_path);
	brextension_map[p_path] = extension;
	return LOAD_STATUS_OK;
}

BRExtensionManager::LoadStatus BRExtensionManager::reload_extension(const String &p_path) {
#ifndef TOOLS_ENABLED
	ERR_FAIL_V_MSG(LOAD_STATUS_FAILED, "BRExtensions can only be reloaded in an editor build.");
#else
	ERR_FAIL_COND_V_MSG(!Engine::get_singleton()->is_extension_reloading_enabled(), LOAD_STATUS_FAILED, "BRExtension reloading is disabled.");

	if (!brextension_map.has(p_path)) {
		return LOAD_STATUS_NOT_LOADED;
	}

	Ref<BRExtension> extension = brextension_map[p_path];
	ERR_FAIL_COND_V_MSG(!extension->is_reloadable(), LOAD_STATUS_FAILED, vformat("This BRExtension is not marked as 'reloadable' or doesn't support reloading: %s.", p_path));

	LoadStatus status;

	extension->prepare_reload();

	// Unload library if it's open. It may not be open if the developer made a
	// change that broke loading in a previous hot-reload attempt.
	if (extension->is_library_open()) {
		status = _unload_extension_internal(extension);
		if (status != LOAD_STATUS_OK) {
			// We need to clear these no matter what.
			extension->clear_instance_bindings();
			return status;
		}

		extension->clear_instance_bindings();
		extension->close_library();
	}

	Error err = extension->open_library(p_path, extension->loader);
	if (err != OK) {
		return LOAD_STATUS_FAILED;
	}

	status = _load_extension_internal(extension, false);
	if (status != LOAD_STATUS_OK) {
		return status;
	}

	extension->finish_reload();

	return LOAD_STATUS_OK;
#endif
}

BRExtensionManager::LoadStatus BRExtensionManager::unload_extension(const String &p_path) {
	if (!brextension_map.has(p_path)) {
		return LOAD_STATUS_NOT_LOADED;
	}

	Ref<BRExtension> extension = brextension_map[p_path];

	LoadStatus status = _unload_extension_internal(extension);
	if (status != LOAD_STATUS_OK) {
		return status;
	}

	brextension_map.erase(p_path);
	return LOAD_STATUS_OK;
}

bool BRExtensionManager::is_extension_loaded(const String &p_path) const {
	return brextension_map.has(p_path);
}

Vector<String> BRExtensionManager::get_loaded_extensions() const {
	Vector<String> ret;
	for (const KeyValue<String, Ref<BRExtension>> &E : brextension_map) {
		ret.push_back(E.key);
	}
	return ret;
}
Ref<BRExtension> BRExtensionManager::get_extension(const String &p_path) {
	HashMap<String, Ref<BRExtension>>::Iterator E = brextension_map.find(p_path);
	ERR_FAIL_COND_V(!E, Ref<BRExtension>());
	return E->value;
}

bool BRExtensionManager::class_has_icon_path(const String &p_class) const {
	// TODO: Check that the icon belongs to a registered class somehow.
	return brextension_class_icon_paths.has(p_class);
}

String BRExtensionManager::class_get_icon_path(const String &p_class) const {
	// TODO: Check that the icon belongs to a registered class somehow.
	if (brextension_class_icon_paths.has(p_class)) {
		return brextension_class_icon_paths[p_class];
	}
	return "";
}

void BRExtensionManager::initialize_extensions(BRExtension::InitializationLevel p_level) {
	ERR_FAIL_COND(int32_t(p_level) - 1 != level);
	for (KeyValue<String, Ref<BRExtension>> &E : brextension_map) {
		E.value->initialize_library(p_level);
	}
	level = p_level;
}

void BRExtensionManager::deinitialize_extensions(BRExtension::InitializationLevel p_level) {
	ERR_FAIL_COND(int32_t(p_level) != level);
	for (KeyValue<String, Ref<BRExtension>> &E : brextension_map) {
		E.value->deinitialize_library(p_level);
	}
	level = int32_t(p_level) - 1;
}

#ifdef TOOLS_ENABLED
void BRExtensionManager::track_instance_binding(void *p_token, Object *p_object) {
	for (KeyValue<String, Ref<BRExtension>> &E : brextension_map) {
		if (E.value.ptr() == p_token) {
			if (E.value->is_reloadable()) {
				E.value->track_instance_binding(p_object);
				return;
			}
		}
	}
}

void BRExtensionManager::untrack_instance_binding(void *p_token, Object *p_object) {
	for (KeyValue<String, Ref<BRExtension>> &E : brextension_map) {
		if (E.value.ptr() == p_token) {
			if (E.value->is_reloadable()) {
				E.value->untrack_instance_binding(p_object);
				return;
			}
		}
	}
}

void BRExtensionManager::_reload_all_scripts() {
	for (int i = 0; i < ScriptServer::get_language_count(); i++) {
		ScriptServer::get_language(i)->reload_all_scripts();
	}
}
#endif // TOOLS_ENABLED

void BRExtensionManager::load_extensions() {
	Ref<FileAccess> f = FileAccess::open(BRExtension::get_extension_list_config_file(), FileAccess::READ);
	while (f.is_valid() && !f->eof_reached()) {
		String s = f->get_line().strip_edges();
		if (!s.is_empty()) {
			LoadStatus err = load_extension(s);
			ERR_CONTINUE_MSG(err == LOAD_STATUS_FAILED, vformat("Error loading extension: '%s'.", s));
		}
	}

	OS::get_singleton()->load_platform_brextensions();
}

void BRExtensionManager::reload_extensions() {
#ifdef TOOLS_ENABLED
	bool reloaded = false;
	for (const KeyValue<String, Ref<BRExtension>> &E : brextension_map) {
		if (!E.value->is_reloadable()) {
			continue;
		}

		if (E.value->has_library_changed()) {
			reloaded = true;
			reload_extension(E.value->get_path());
		}
	}

	if (reloaded) {
		emit_signal("extensions_reloaded");

		// Reload all scripts to clear out old references.
		callable_mp_static(&BRExtensionManager::_reload_all_scripts).call_deferred();
	}
#endif
}

bool BRExtensionManager::ensure_extensions_loaded(const HashSet<String> &p_extensions) {
	Vector<String> extensions_added;
	Vector<String> extensions_removed;

	for (const String &E : p_extensions) {
		if (!is_extension_loaded(E)) {
			extensions_added.push_back(E);
		}
	}

	Vector<String> loaded_extensions = get_loaded_extensions();
	for (const String &loaded_extension : loaded_extensions) {
		if (!p_extensions.has(loaded_extension)) {
			// The extension may not have a .brextension file.
			const Ref<BRExtension> extension = BRExtensionManager::get_singleton()->get_extension(loaded_extension);
			if (!extension->get_loader()->library_exists()) {
				extensions_removed.push_back(loaded_extension);
			}
		}
	}

	String extension_list_config_file = BRExtension::get_extension_list_config_file();
	if (p_extensions.size()) {
		if (extensions_added.size() || extensions_removed.size()) {
			// Extensions were added or removed.
			Ref<FileAccess> f = FileAccess::open(extension_list_config_file, FileAccess::WRITE);
			for (const String &E : p_extensions) {
				f->store_line(E);
			}
		}
	} else {
		if (loaded_extensions.size() || FileAccess::exists(extension_list_config_file)) {
			// Extensions were removed.
			Ref<DirAccess> da = DirAccess::create(DirAccess::ACCESS_RESOURCES);
			da->remove(extension_list_config_file);
		}
	}

	bool needs_restart = false;
	for (const String &extension : extensions_added) {
		BRExtensionManager::LoadStatus st = BRExtensionManager::get_singleton()->load_extension(extension);
		if (st == BRExtensionManager::LOAD_STATUS_NEEDS_RESTART) {
			needs_restart = true;
		}
	}

	for (const String &extension : extensions_removed) {
		BRExtensionManager::LoadStatus st = BRExtensionManager::get_singleton()->unload_extension(extension);
		if (st == BRExtensionManager::LOAD_STATUS_NEEDS_RESTART) {
			needs_restart = true;
		}
	}

#ifdef TOOLS_ENABLED
	if (extensions_added.size() || extensions_removed.size()) {
		// Emitting extensions_reloaded so EditorNode can reload Inspector and regenerate documentation.
		emit_signal("extensions_reloaded");

		// Reload all scripts to clear out old references.
		callable_mp_static(&BRExtensionManager::_reload_all_scripts).call_deferred();
	}
#endif

	return needs_restart;
}

BRExtensionManager *BRExtensionManager::get_singleton() {
	return singleton;
}

void BRExtensionManager::_bind_methods() {
	ClassDB::bind_method(D_METHOD("load_extension", "path"), &BRExtensionManager::load_extension);
	ClassDB::bind_method(D_METHOD("reload_extension", "path"), &BRExtensionManager::reload_extension);
	ClassDB::bind_method(D_METHOD("unload_extension", "path"), &BRExtensionManager::unload_extension);
	ClassDB::bind_method(D_METHOD("is_extension_loaded", "path"), &BRExtensionManager::is_extension_loaded);

	ClassDB::bind_method(D_METHOD("get_loaded_extensions"), &BRExtensionManager::get_loaded_extensions);
	ClassDB::bind_method(D_METHOD("get_extension", "path"), &BRExtensionManager::get_extension);

	BIND_ENUM_CONSTANT(LOAD_STATUS_OK);
	BIND_ENUM_CONSTANT(LOAD_STATUS_FAILED);
	BIND_ENUM_CONSTANT(LOAD_STATUS_ALREADY_LOADED);
	BIND_ENUM_CONSTANT(LOAD_STATUS_NOT_LOADED);
	BIND_ENUM_CONSTANT(LOAD_STATUS_NEEDS_RESTART);

	ADD_SIGNAL(MethodInfo("extensions_reloaded"));
	ADD_SIGNAL(MethodInfo("extension_loaded", PropertyInfo(Variant::OBJECT, "extension", PROPERTY_HINT_RESOURCE_TYPE, "BRExtension")));
	ADD_SIGNAL(MethodInfo("extension_unloading", PropertyInfo(Variant::OBJECT, "extension", PROPERTY_HINT_RESOURCE_TYPE, "BRExtension")));
}

BRExtensionManager *BRExtensionManager::singleton = nullptr;

BRExtensionManager::BRExtensionManager() {
	ERR_FAIL_COND(singleton != nullptr);
	singleton = this;

#ifndef DISABLE_DEPRECATED
	BRExtensionCompatHashes::initialize();
#endif
}

BRExtensionManager::~BRExtensionManager() {
	if (singleton == this) {
		singleton = nullptr;
	}
#ifndef DISABLE_DEPRECATED
	BRExtensionCompatHashes::finalize();
#endif
}
