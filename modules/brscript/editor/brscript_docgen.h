/**************************************************************************/
/*  brscript_docgen.h                                                     */
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

#ifndef BRSCRIPT_DOCGEN_H
#define BRSCRIPT_DOCGEN_H

#include "../brscript_parser.h"

#include "core/doc_data.h"

class BRScriptDocGen {
	using GDP = BRScriptParser;
	using BRType = GDP::DataType;

	static HashMap<String, String> singletons; // Script path to singleton name.

	static String _get_script_name(const String &p_path);
	static String _get_class_name(const GDP::ClassNode &p_class);
	static void _doctype_from_brtype(const BRType &p_brtype, String &r_type, String &r_enum, bool p_is_return = false);
	static String _docvalue_from_variant(const Variant &p_variant, int p_recursion_level = 1);
	static String _docvalue_from_expression(const GDP::ExpressionNode *p_expression);
	static void _generate_docs(BRScript *p_script, const GDP::ClassNode *p_class);

public:
	static void generate_docs(BRScript *p_script, const GDP::ClassNode *p_class);
};

#endif // BRSCRIPT_DOCGEN_H
