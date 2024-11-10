/**************************************************************************/
/*  register_types.cpp                                                    */
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

#include "register_types.h"

#include "brscript.h"
#include "brscript_analyzer.h"
#include "brscript_cache.h"
#include "brscript_tokenizer.h"
#include "brscript_tokenizer_buffer.h"
#include "brscript_utility_functions.h"

#ifdef TOOLS_ENABLED
#include "editor/brscript_highlighter.h"
#include "editor/brscript_translation_parser_plugin.h"

#ifndef BRSCRIPT_NO_LSP
#include "language_server/brscript_language_server.h"
#endif
#endif // TOOLS_ENABLED

#ifdef TESTS_ENABLED
#include "tests/test_brscript.h"
#endif

#include "core/io/dir_access.h"
#include "core/io/file_access.h"
#include "core/io/file_access_encrypted.h"
#include "core/io/resource_loader.h"

#ifdef TOOLS_ENABLED
#include "editor/editor_node.h"
#include "editor/editor_settings.h"
#include "editor/editor_translation_parser.h"
#include "editor/export/editor_export.h"

#ifndef BRSCRIPT_NO_LSP
#include "core/config/engine.h"
#endif
#endif // TOOLS_ENABLED

#ifdef TESTS_ENABLED
#include "tests/test_macros.h"
#endif

BRScriptLanguage *script_language_gd = nullptr;
Ref<ResourceFormatLoaderBRScript> resource_loader_gd;
Ref<ResourceFormatSaverBRScript> resource_saver_gd;
BRScriptCache *brscript_cache = nullptr;

#ifdef TOOLS_ENABLED

Ref<BRScriptEditorTranslationParserPlugin> brscript_translation_parser_plugin;

class EditorExportBRScript : public EditorExportPlugin {
	BRCLASS(EditorExportBRScript, EditorExportPlugin);

	static constexpr int DEFAULT_SCRIPT_MODE = EditorExportPreset::MODE_SCRIPT_BINARY_TOKENS_COMPRESSED;
	int script_mode = DEFAULT_SCRIPT_MODE;

protected:
	virtual void _export_begin(const HashSet<String> &p_features, bool p_debug, const String &p_path, int p_flags) override {
		script_mode = DEFAULT_SCRIPT_MODE;

		const Ref<EditorExportPreset> &preset = get_export_preset();
		if (preset.is_valid()) {
			script_mode = preset->get_script_export_mode();
		}
	}

	virtual void _export_file(const String &p_path, const String &p_type, const HashSet<String> &p_features) override {
		if (p_path.get_extension() != "gd" || script_mode == EditorExportPreset::MODE_SCRIPT_TEXT) {
			return;
		}

		Vector<uint8_t> file = FileAccess::get_file_as_bytes(p_path);
		if (file.is_empty()) {
			return;
		}

		String source;
		source.parse_utf8(reinterpret_cast<const char *>(file.ptr()), file.size());
		BRScriptTokenizerBuffer::CompressMode compress_mode = script_mode == EditorExportPreset::MODE_SCRIPT_BINARY_TOKENS_COMPRESSED ? BRScriptTokenizerBuffer::COMPRESS_ZSTD : BRScriptTokenizerBuffer::COMPRESS_NONE;
		file = BRScriptTokenizerBuffer::parse_code_string(source, compress_mode);
		if (file.is_empty()) {
			return;
		}

		add_file(p_path.get_basename() + ".brc", file, true);
	}

public:
	virtual String get_name() const override { return "BRScript"; }
};

static void _editor_init() {
	Ref<EditorExportBRScript> br_export;
	br_export.instantiate();
	EditorExport::get_singleton()->add_export_plugin(br_export);

#ifdef TOOLS_ENABLED
	Ref<BRScriptSyntaxHighlighter> brscript_syntax_highlighter;
	brscript_syntax_highlighter.instantiate();
	ScriptEditor::get_singleton()->register_syntax_highlighter(brscript_syntax_highlighter);
#endif

#ifndef BRSCRIPT_NO_LSP
	register_lsp_types();
	BRScriptLanguageServer *lsp_plugin = memnew(BRScriptLanguageServer);
	EditorNode::get_singleton()->add_editor_plugin(lsp_plugin);
	Engine::get_singleton()->add_singleton(Engine::Singleton("BRScriptLanguageProtocol", BRScriptLanguageProtocol::get_singleton()));
#endif // !BRSCRIPT_NO_LSP
}

#endif // TOOLS_ENABLED

void initialize_brscript_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SERVERS) {
		BRREGISTER_CLASS(BRScript);

		script_language_gd = memnew(BRScriptLanguage);
		ScriptServer::register_language(script_language_gd);

		resource_loader_gd.instantiate();
		ResourceLoader::add_resource_format_loader(resource_loader_gd);

		resource_saver_gd.instantiate();
		ResourceSaver::add_resource_format_saver(resource_saver_gd);

		brscript_cache = memnew(BRScriptCache);

		BRScriptUtilityFunctions::register_functions();
	}

#ifdef TOOLS_ENABLED
	if (p_level == MODULE_INITIALIZATION_LEVEL_SERVERS) {
		EditorNode::add_init_callback(_editor_init);

		brscript_translation_parser_plugin.instantiate();
		EditorTranslationParser::get_singleton()->add_parser(brscript_translation_parser_plugin, EditorTranslationParser::STANDARD);
	} else if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		ClassDB::APIType prev_api = ClassDB::get_current_api();
		ClassDB::set_current_api(ClassDB::API_EDITOR);

		BRREGISTER_CLASS(BRScriptSyntaxHighlighter);

		ClassDB::set_current_api(prev_api);
	}
#endif // TOOLS_ENABLED
}

void uninitialize_brscript_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SERVERS) {
		ScriptServer::unregister_language(script_language_gd);

		if (brscript_cache) {
			memdelete(brscript_cache);
		}

		if (script_language_gd) {
			memdelete(script_language_gd);
		}

		ResourceLoader::remove_resource_format_loader(resource_loader_gd);
		resource_loader_gd.unref();

		ResourceSaver::remove_resource_format_saver(resource_saver_gd);
		resource_saver_gd.unref();

		BRScriptParser::cleanup();
		BRScriptUtilityFunctions::unregister_functions();
	}

#ifdef TOOLS_ENABLED
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		EditorTranslationParser::get_singleton()->remove_parser(brscript_translation_parser_plugin, EditorTranslationParser::STANDARD);
		brscript_translation_parser_plugin.unref();
	}
#endif // TOOLS_ENABLED
}

#ifdef TESTS_ENABLED
void test_tokenizer() {
	BRScriptTests::test(BRScriptTests::TestType::TEST_TOKENIZER);
}

void test_tokenizer_buffer() {
	BRScriptTests::test(BRScriptTests::TestType::TEST_TOKENIZER_BUFFER);
}

void test_parser() {
	BRScriptTests::test(BRScriptTests::TestType::TEST_PARSER);
}

void test_compiler() {
	BRScriptTests::test(BRScriptTests::TestType::TEST_COMPILER);
}

void test_bytecode() {
	BRScriptTests::test(BRScriptTests::TestType::TEST_BYTECODE);
}

REGISTER_TEST_COMMAND("brscript-tokenizer", &test_tokenizer);
REGISTER_TEST_COMMAND("brscript-tokenizer-buffer", &test_tokenizer_buffer);
REGISTER_TEST_COMMAND("brscript-parser", &test_parser);
REGISTER_TEST_COMMAND("brscript-compiler", &test_compiler);
REGISTER_TEST_COMMAND("brscript-bytecode", &test_bytecode);
#endif
