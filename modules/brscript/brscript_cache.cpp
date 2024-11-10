/**************************************************************************/
/*  brscript_cache.cpp                                                    */
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

#include "brscript_cache.h"

#include "brscript.h"
#include "brscript_analyzer.h"
#include "brscript_compiler.h"
#include "brscript_parser.h"

#include "core/io/file_access.h"
#include "core/templates/vector.h"

BRScriptParserRef::Status BRScriptParserRef::get_status() const {
	return status;
}

String BRScriptParserRef::get_path() const {
	return path;
}

uint32_t BRScriptParserRef::get_source_hash() const {
	return source_hash;
}

BRScriptParser *BRScriptParserRef::get_parser() {
	if (parser == nullptr) {
		parser = memnew(BRScriptParser);
	}
	return parser;
}

BRScriptAnalyzer *BRScriptParserRef::get_analyzer() {
	if (analyzer == nullptr) {
		analyzer = memnew(BRScriptAnalyzer(get_parser()));
	}
	return analyzer;
}

Error BRScriptParserRef::raise_status(Status p_new_status) {
	ERR_FAIL_COND_V(clearing, ERR_BUG);
	ERR_FAIL_COND_V(parser == nullptr && status != EMPTY, ERR_BUG);

	while (result == OK && p_new_status > status) {
		switch (status) {
			case EMPTY: {
				// Calling parse will clear the parser, which can destruct another BRScriptParserRef which can clear the last reference to the script with this path, calling remove_script, which clears this BRScriptParserRef.
				// It's ok if its the first thing done here.
				get_parser()->clear();
				status = PARSED;
				String remapped_path = ResourceLoader::path_remap(path);
				if (remapped_path.get_extension().to_lower() == "gdc") {
					Vector<uint8_t> tokens = BRScriptCache::get_binary_tokens(remapped_path);
					source_hash = hash_djb2_buffer(tokens.ptr(), tokens.size());
					result = get_parser()->parse_binary(tokens, path);
				} else {
					String source = BRScriptCache::get_source_code(remapped_path);
					source_hash = source.hash();
					result = get_parser()->parse(source, path, false);
				}
			} break;
			case PARSED: {
				status = INHERITANCE_SOLVED;
				result = get_analyzer()->resolve_inheritance();
			} break;
			case INHERITANCE_SOLVED: {
				status = INTERFACE_SOLVED;
				result = get_analyzer()->resolve_interface();
			} break;
			case INTERFACE_SOLVED: {
				status = FULLY_SOLVED;
				result = get_analyzer()->resolve_body();
			} break;
			case FULLY_SOLVED: {
				return result;
			}
		}
	}

	return result;
}

void BRScriptParserRef::clear() {
	if (clearing) {
		return;
	}
	clearing = true;

	BRScriptParser *lparser = parser;
	BRScriptAnalyzer *lanalyzer = analyzer;

	parser = nullptr;
	analyzer = nullptr;
	status = EMPTY;
	result = OK;
	source_hash = 0;

	clearing = false;

	if (lanalyzer != nullptr) {
		memdelete(lanalyzer);
	}

	if (lparser != nullptr) {
		memdelete(lparser);
	}
}

BRScriptParserRef::~BRScriptParserRef() {
	clear();

	if (!abandoned) {
		MutexLock lock(BRScriptCache::singleton->mutex);
		BRScriptCache::singleton->parser_map.erase(path);
	}
}

BRScriptCache *BRScriptCache::singleton = nullptr;

SafeBinaryMutex<BRScriptCache::BINARY_MUTEX_TAG> &_get_brscript_cache_mutex() {
	return BRScriptCache::mutex;
}

template <>
thread_local SafeBinaryMutex<BRScriptCache::BINARY_MUTEX_TAG>::TLSData SafeBinaryMutex<BRScriptCache::BINARY_MUTEX_TAG>::tls_data(_get_brscript_cache_mutex());
SafeBinaryMutex<BRScriptCache::BINARY_MUTEX_TAG> BRScriptCache::mutex;

void BRScriptCache::move_script(const String &p_from, const String &p_to) {
	if (singleton == nullptr || p_from == p_to) {
		return;
	}

	MutexLock lock(singleton->mutex);

	if (singleton->cleared) {
		return;
	}

	remove_parser(p_from);

	if (singleton->shallow_brscript_cache.has(p_from) && !p_from.is_empty()) {
		singleton->shallow_brscript_cache[p_to] = singleton->shallow_brscript_cache[p_from];
	}
	singleton->shallow_brscript_cache.erase(p_from);

	if (singleton->full_brscript_cache.has(p_from) && !p_from.is_empty()) {
		singleton->full_brscript_cache[p_to] = singleton->full_brscript_cache[p_from];
	}
	singleton->full_brscript_cache.erase(p_from);
}

void BRScriptCache::remove_script(const String &p_path) {
	if (singleton == nullptr) {
		return;
	}

	MutexLock lock(singleton->mutex);

	if (singleton->cleared) {
		return;
	}

	if (HashMap<String, Vector<ObjectID>>::Iterator E = singleton->abandoned_parser_map.find(p_path)) {
		for (ObjectID parser_ref_id : E->value) {
			Ref<BRScriptParserRef> parser_ref{ ObjectDB::get_instance(parser_ref_id) };
			if (parser_ref.is_valid()) {
				parser_ref->clear();
			}
		}
	}

	singleton->abandoned_parser_map.erase(p_path);

	if (singleton->parser_map.has(p_path)) {
		singleton->parser_map[p_path]->clear();
	}

	remove_parser(p_path);

	singleton->dependencies.erase(p_path);
	singleton->shallow_brscript_cache.erase(p_path);
	singleton->full_brscript_cache.erase(p_path);
}

Ref<BRScriptParserRef> BRScriptCache::get_parser(const String &p_path, BRScriptParserRef::Status p_status, Error &r_error, const String &p_owner) {
	MutexLock lock(singleton->mutex);
	Ref<BRScriptParserRef> ref;
	if (!p_owner.is_empty()) {
		singleton->dependencies[p_owner].insert(p_path);
		singleton->parser_inverse_dependencies[p_path].insert(p_owner);
	}
	if (singleton->parser_map.has(p_path)) {
		ref = Ref<BRScriptParserRef>(singleton->parser_map[p_path]);
		if (ref.is_null()) {
			r_error = ERR_INVALID_DATA;
			return ref;
		}
	} else {
		String remapped_path = ResourceLoader::path_remap(p_path);
		if (!FileAccess::exists(remapped_path)) {
			r_error = ERR_FILE_NOT_FOUND;
			return ref;
		}
		ref.instantiate();
		ref->path = p_path;
		singleton->parser_map[p_path] = ref.ptr();
	}
	r_error = ref->raise_status(p_status);

	return ref;
}

bool BRScriptCache::has_parser(const String &p_path) {
	MutexLock lock(singleton->mutex);
	return singleton->parser_map.has(p_path);
}

void BRScriptCache::remove_parser(const String &p_path) {
	MutexLock lock(singleton->mutex);

	if (singleton->parser_map.has(p_path)) {
		BRScriptParserRef *parser_ref = singleton->parser_map[p_path];
		parser_ref->abandoned = true;
		singleton->abandoned_parser_map[p_path].push_back(parser_ref->get_instance_id());
	}

	// Can't clear the parser because some other parser might be currently using it in the chain of calls.
	singleton->parser_map.erase(p_path);

	// Have to copy while iterating, because parser_inverse_dependencies is modified.
	HashSet<String> ideps = singleton->parser_inverse_dependencies[p_path];
	singleton->parser_inverse_dependencies.erase(p_path);
	for (String idep_path : ideps) {
		remove_parser(idep_path);
	}
}

String BRScriptCache::get_source_code(const String &p_path) {
	Vector<uint8_t> source_file;
	Error err;
	Ref<FileAccess> f = FileAccess::open(p_path, FileAccess::READ, &err);
	ERR_FAIL_COND_V(err, "");

	uint64_t len = f->get_length();
	source_file.resize(len + 1);
	uint64_t r = f->get_buffer(source_file.ptrw(), len);
	ERR_FAIL_COND_V(r != len, "");
	source_file.write[len] = 0;

	String source;
	if (source.parse_utf8((const char *)source_file.ptr()) != OK) {
		ERR_FAIL_V_MSG("", "Script '" + p_path + "' contains invalid unicode (UTF-8), so it was not loaded. Please ensure that scripts are saved in valid UTF-8 unicode.");
	}
	return source;
}

Vector<uint8_t> BRScriptCache::get_binary_tokens(const String &p_path) {
	Vector<uint8_t> buffer;
	Error err = OK;
	Ref<FileAccess> f = FileAccess::open(p_path, FileAccess::READ, &err);
	ERR_FAIL_COND_V_MSG(err != OK, buffer, "Failed to open binary BRScript file '" + p_path + "'.");

	uint64_t len = f->get_length();
	buffer.resize(len);
	uint64_t read = f->get_buffer(buffer.ptrw(), buffer.size());
	ERR_FAIL_COND_V_MSG(read != len, Vector<uint8_t>(), "Failed to read binary BRScript file '" + p_path + "'.");

	return buffer;
}

Ref<BRScript> BRScriptCache::get_shallow_script(const String &p_path, Error &r_error, const String &p_owner) {
	MutexLock lock(singleton->mutex);

	if (!p_owner.is_empty()) {
		singleton->dependencies[p_owner].insert(p_path);
	}
	if (singleton->full_brscript_cache.has(p_path)) {
		return singleton->full_brscript_cache[p_path];
	}
	if (singleton->shallow_brscript_cache.has(p_path)) {
		return singleton->shallow_brscript_cache[p_path];
	}

	const String remapped_path = ResourceLoader::path_remap(p_path);

	Ref<BRScript> script;
	script.instantiate();
	script->set_path_cache(p_path);
	if (remapped_path.get_extension().to_lower() == "gdc") {
		Vector<uint8_t> buffer = get_binary_tokens(remapped_path);
		if (buffer.is_empty()) {
			r_error = ERR_FILE_CANT_READ;
		}
		script->set_binary_tokens_source(buffer);
	} else {
		r_error = script->load_source_code(remapped_path);
	}

	if (r_error) {
		return Ref<BRScript>(); // Returns null and does not cache when the script fails to load.
	}

	Ref<BRScriptParserRef> parser_ref = get_parser(p_path, BRScriptParserRef::PARSED, r_error);
	if (r_error == OK) {
		BRScriptCompiler::make_scripts(script.ptr(), parser_ref->get_parser()->get_tree(), true);
	}

	singleton->shallow_brscript_cache[p_path] = script;

	return script;
}

Ref<BRScript> BRScriptCache::get_full_script(const String &p_path, Error &r_error, const String &p_owner, bool p_update_from_disk) {
	MutexLock lock(singleton->mutex);

	if (!p_owner.is_empty()) {
		singleton->dependencies[p_owner].insert(p_path);
	}

	Ref<BRScript> script;
	r_error = OK;
	if (singleton->full_brscript_cache.has(p_path)) {
		script = singleton->full_brscript_cache[p_path];
		if (!p_update_from_disk) {
			return script;
		}
	}

	if (script.is_null()) {
		script = get_shallow_script(p_path, r_error);
		// Only exit early if script failed to load, otherwise let reload report errors.
		if (script.is_null()) {
			return script;
		}
	}
	script->set_path(p_path, true);

	const String remapped_path = ResourceLoader::path_remap(p_path);

	if (p_update_from_disk) {
		if (remapped_path.get_extension().to_lower() == "gdc") {
			Vector<uint8_t> buffer = get_binary_tokens(remapped_path);
			if (buffer.is_empty()) {
				r_error = ERR_FILE_CANT_READ;
				return script;
			}
			script->set_binary_tokens_source(buffer);
		} else {
			r_error = script->load_source_code(remapped_path);
			if (r_error) {
				return script;
			}
		}
	}

	// Allowing lifting the lock might cause a script to be reloaded multiple times,
	// which, as a last resort deadlock prevention strategy, is a good tradeoff.
	uint32_t allowance_id = WorkerThreadPool::thread_enter_unlock_allowance_zone(singleton->mutex);
	r_error = script->reload(true);
	WorkerThreadPool::thread_exit_unlock_allowance_zone(allowance_id);
	if (r_error) {
		return script;
	}

	singleton->full_brscript_cache[p_path] = script;
	singleton->shallow_brscript_cache.erase(p_path);

	return script;
}

Ref<BRScript> BRScriptCache::get_cached_script(const String &p_path) {
	MutexLock lock(singleton->mutex);

	if (singleton->full_brscript_cache.has(p_path)) {
		return singleton->full_brscript_cache[p_path];
	}

	if (singleton->shallow_brscript_cache.has(p_path)) {
		return singleton->shallow_brscript_cache[p_path];
	}

	return Ref<BRScript>();
}

Error BRScriptCache::finish_compiling(const String &p_owner) {
	MutexLock lock(singleton->mutex);

	// Mark this as compiled.
	Ref<BRScript> script = get_cached_script(p_owner);
	singleton->full_brscript_cache[p_owner] = script;
	singleton->shallow_brscript_cache.erase(p_owner);

	HashSet<String> depends = singleton->dependencies[p_owner];

	Error err = OK;
	for (const String &E : depends) {
		Error this_err = OK;
		// No need to save the script. We assume it's already referenced in the owner.
		get_full_script(E, this_err);

		if (this_err != OK) {
			err = this_err;
		}
	}

	singleton->dependencies.erase(p_owner);

	return err;
}

void BRScriptCache::add_static_script(Ref<BRScript> p_script) {
	ERR_FAIL_COND_MSG(p_script.is_null(), "Trying to cache empty script as static.");
	ERR_FAIL_COND_MSG(!p_script->is_valid(), "Trying to cache non-compiled script as static.");
	singleton->static_brscript_cache[p_script->get_fully_qualified_name()] = p_script;
}

void BRScriptCache::remove_static_script(const String &p_fqcn) {
	singleton->static_brscript_cache.erase(p_fqcn);
}

void BRScriptCache::clear() {
	if (singleton == nullptr) {
		return;
	}

	MutexLock lock(singleton->mutex);

	if (singleton->cleared) {
		return;
	}
	singleton->cleared = true;

	singleton->parser_inverse_dependencies.clear();

	for (const KeyValue<String, Vector<ObjectID>> &KV : singleton->abandoned_parser_map) {
		for (ObjectID parser_ref_id : KV.value) {
			Ref<BRScriptParserRef> parser_ref{ ObjectDB::get_instance(parser_ref_id) };
			if (parser_ref.is_valid()) {
				parser_ref->clear();
			}
		}
	}

	singleton->abandoned_parser_map.clear();

	RBSet<Ref<BRScriptParserRef>> parser_map_refs;
	for (KeyValue<String, BRScriptParserRef *> &E : singleton->parser_map) {
		parser_map_refs.insert(E.value);
	}

	singleton->parser_map.clear();

	for (Ref<BRScriptParserRef> &E : parser_map_refs) {
		if (E.is_valid()) {
			E->clear();
		}
	}

	parser_map_refs.clear();
	singleton->shallow_brscript_cache.clear();
	singleton->full_brscript_cache.clear();
}

BRScriptCache::BRScriptCache() {
	singleton = this;
}

BRScriptCache::~BRScriptCache() {
	if (!cleared) {
		clear();
	}
	singleton = nullptr;
}
