/**************************************************************************/
/*  brscript_translation_parser_plugin.h                                  */
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

#ifndef BRSCRIPT_TRANSLATION_PARSER_PLUGIN_H
#define BRSCRIPT_TRANSLATION_PARSER_PLUGIN_H

#include "../brscript_parser.h"

#include "core/templates/hash_set.h"
#include "editor/editor_translation_parser.h"

class BRScriptEditorTranslationParserPlugin : public EditorTranslationParserPlugin {
	BRCLASS(BRScriptEditorTranslationParserPlugin, EditorTranslationParserPlugin);

	Vector<String> *ids = nullptr;
	Vector<Vector<String>> *ids_ctx_plural = nullptr;

	// List of patterns used for extracting translation strings.
	StringName tr_func = "tr";
	StringName trn_func = "tr_n";
	StringName atr_func = "atr";
	StringName atrn_func = "atr_n";
	HashSet<StringName> assignment_patterns;
	HashSet<StringName> first_arg_patterns;
	HashSet<StringName> second_arg_patterns;
	// FileDialog patterns.
	StringName fd_add_filter = "add_filter";
	StringName fd_set_filter = "set_filters";
	StringName fd_filters = "filters";

	static bool _is_constant_string(const BRScriptParser::ExpressionNode *p_expression);

	void _traverse_class(const BRScriptParser::ClassNode *p_class);
	void _traverse_function(const BRScriptParser::FunctionNode *p_func);
	void _traverse_block(const BRScriptParser::SuiteNode *p_suite);

	void _assess_expression(const BRScriptParser::ExpressionNode *p_expression);
	void _assess_assignment(const BRScriptParser::AssignmentNode *p_assignment);
	void _assess_call(const BRScriptParser::CallNode *p_call);

	void _extract_fd_filter_string(const BRScriptParser::ExpressionNode *p_expression);
	void _extract_fd_filter_array(const BRScriptParser::ExpressionNode *p_expression);

public:
	virtual Error parse_file(const String &p_path, Vector<String> *r_ids, Vector<Vector<String>> *r_ids_ctx_plural) override;
	virtual void get_recognized_extensions(List<String> *r_extensions) const override;

	BRScriptEditorTranslationParserPlugin();
};

#endif // BRSCRIPT_TRANSLATION_PARSER_PLUGIN_H
