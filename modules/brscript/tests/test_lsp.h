/**************************************************************************/
/*  test_lsp.h                                                            */
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

#ifndef TEST_LSP_H
#define TEST_LSP_H

#ifdef TOOLS_ENABLED

#include "tests/test_macros.h"

#include "../language_server/brscript_extend_parser.h"
#include "../language_server/brscript_language_protocol.h"
#include "../language_server/brscript_workspace.h"
#include "../language_server/bradot_lsp.h"

#include "core/io/dir_access.h"
#include "core/io/file_access_pack.h"
#include "core/os/os.h"
#include "editor/editor_help.h"
#include "editor/editor_node.h"
#include "modules/brscript/brscript_analyzer.h"
#include "modules/regex/regex.h"

#include "thirdparty/doctest/doctest.h"

template <>
struct doctest::StringMaker<lsp::Position> {
	static doctest::String convert(const lsp::Position &p_val) {
		return p_val.to_string().utf8().get_data();
	}
};

template <>
struct doctest::StringMaker<lsp::Range> {
	static doctest::String convert(const lsp::Range &p_val) {
		return p_val.to_string().utf8().get_data();
	}
};

template <>
struct doctest::StringMaker<BradotPosition> {
	static doctest::String convert(const BradotPosition &p_val) {
		return p_val.to_string().utf8().get_data();
	}
};

namespace BRScriptTests {

// LSP BRScript test scripts are located inside project of other BRScript tests:
// Cannot reset `ProjectSettings` (singleton) -> Cannot load another workspace and resources in there.
// -> Reuse BRScript test project. LSP specific scripts are then placed inside `lsp` folder.
//    Access via `res://lsp/my_script.br`.
const String root = "modules/brscript/tests/scripts/";

/*
 * After use:
 * * `memdelete` returned `BRScriptLanguageProtocol`.
 * * Call `BRScriptTests::::finish_language`.
 */
BRScriptLanguageProtocol *initialize(const String &p_root) {
	Error err = OK;
	Ref<DirAccess> dir(DirAccess::open(p_root, &err));
	REQUIRE_MESSAGE(err == OK, "Could not open specified root directory");
	String absolute_root = dir->get_current_dir();
	init_language(absolute_root);

	BRScriptLanguageProtocol *proto = memnew(BRScriptLanguageProtocol);

	Ref<BRScriptWorkspace> workspace = BRScriptLanguageProtocol::get_singleton()->get_workspace();
	workspace->root = absolute_root;
	// On windows: `C:/...` -> `C%3A/...`.
	workspace->root_uri = "file:///" + absolute_root.lstrip("/").replace_first(":", "%3A");

	return proto;
}

lsp::Position pos(const int p_line, const int p_character) {
	lsp::Position p;
	p.line = p_line;
	p.character = p_character;
	return p;
}

lsp::Range range(const lsp::Position p_start, const lsp::Position p_end) {
	lsp::Range r;
	r.start = p_start;
	r.end = p_end;
	return r;
}

lsp::TextDocumentPositionParams pos_in(const lsp::DocumentUri &p_uri, const lsp::Position p_pos) {
	lsp::TextDocumentPositionParams params;
	params.textDocument.uri = p_uri;
	params.position = p_pos;
	return params;
}

const lsp::DocumentSymbol *test_resolve_symbol_at(const String &p_uri, const lsp::Position p_pos, const String &p_expected_uri, const String &p_expected_name, const lsp::Range &p_expected_range) {
	Ref<BRScriptWorkspace> workspace = BRScriptLanguageProtocol::get_singleton()->get_workspace();

	lsp::TextDocumentPositionParams params = pos_in(p_uri, p_pos);
	const lsp::DocumentSymbol *symbol = workspace->resolve_symbol(params);
	CHECK(symbol);

	if (symbol) {
		CHECK_EQ(symbol->uri, p_expected_uri);
		CHECK_EQ(symbol->name, p_expected_name);
		CHECK_EQ(symbol->selectionRange, p_expected_range);
	}

	return symbol;
}

struct InlineTestData {
	lsp::Range range;
	String text;
	String name;
	String ref;

	static bool try_parse(const Vector<String> &p_lines, const int p_line_number, InlineTestData &r_data) {
		String line = p_lines[p_line_number];

		RegEx regex = RegEx("^\\t*#[ |]*(?<range>(?<left><)?\\^+)(\\s+(?<name>(?!->)\\S+))?(\\s+->\\s+(?<ref>\\S+))?");
		Ref<RegExMatch> match = regex.search(line);
		if (match.is_null()) {
			return false;
		}

		// Find first line without leading comment above current line.
		int target_line = p_line_number;
		while (target_line >= 0) {
			String dedented = p_lines[target_line].lstrip("\t");
			if (!dedented.begins_with("#")) {
				break;
			}
			target_line--;
		}
		if (target_line < 0) {
			return false;
		}
		r_data.range.start.line = r_data.range.end.line = target_line;

		String marker = match->get_string("range");
		int i = line.find(marker);
		REQUIRE(i >= 0);
		r_data.range.start.character = i;
		if (!match->get_string("left").is_empty()) {
			// Include `#` (comment char) in range.
			r_data.range.start.character--;
		}
		r_data.range.end.character = i + marker.length();

		String target = p_lines[target_line];
		r_data.text = target.substr(r_data.range.start.character, r_data.range.end.character - r_data.range.start.character);

		r_data.name = match->get_string("name");
		r_data.ref = match->get_string("ref");

		return true;
	}
};

Vector<InlineTestData> read_tests(const String &p_path) {
	Error err;
	String source = FileAccess::get_file_as_string(p_path, &err);
	REQUIRE_MESSAGE(err == OK, vformat("Cannot read '%s'", p_path));

	// Format:
	// ```brscript
	// var foo = bar + baz
	// #   | |   | |   ^^^ name -> ref
	// #   | |   ^^^ -> ref
	// #   ^^^ name
	//
	// func my_func():
	// #    ^^^^^^^ name
	//     var value = foo + 42
	//     #   ^^^^^ name
	//     print(value)
	//     #     ^^^^^ -> ref
	// ```
	//
	// * `^`: Range marker.
	// * `name`: Unique name. Can contain any characters except whitespace chars.
	// * `ref`: Reference to unique name.
	//
	// Notes:
	// * If range should include first content-char (which is occupied by `#`): use `<` for next marker.
	//   -> Range expands 1 to left (-> includes `#`).
	//   * Note: Means: Range cannot be single char directly marked by `#`, but must be at least two chars (marked with `#<`).
	// * Comment must start at same ident as line its marked (-> because of tab alignment...).
	// * Use spaces to align after `#`! -> for correct alignment
	// * Between `#` and `^` can be spaces or `|` (to better visualize what's marked below).
	PackedStringArray lines = source.split("\n");

	PackedStringArray names;
	Vector<InlineTestData> data;
	for (int i = 0; i < lines.size(); i++) {
		InlineTestData d;
		if (InlineTestData::try_parse(lines, i, d)) {
			if (!d.name.is_empty()) {
				// Safety check: names must be unique.
				if (names.has(d.name)) {
					FAIL(vformat("Duplicated name '%s' in '%s'. Names must be unique!", d.name, p_path));
				}
				names.append(d.name);
			}

			data.append(d);
		}
	}

	return data;
}

void test_resolve_symbol(const String &p_uri, const InlineTestData &p_test_data, const Vector<InlineTestData> &p_all_data) {
	if (p_test_data.ref.is_empty()) {
		return;
	}

	SUBCASE(vformat("Can resolve symbol '%s' at %s to '%s'", p_test_data.text, p_test_data.range.to_string(), p_test_data.ref).utf8().get_data()) {
		const InlineTestData *target = nullptr;
		for (int i = 0; i < p_all_data.size(); i++) {
			if (p_all_data[i].name == p_test_data.ref) {
				target = &p_all_data[i];
				break;
			}
		}
		REQUIRE_MESSAGE(target, vformat("No target for ref '%s'", p_test_data.ref));

		Ref<BRScriptWorkspace> workspace = BRScriptLanguageProtocol::get_singleton()->get_workspace();
		lsp::Position pos = p_test_data.range.start;

		SUBCASE("start of identifier") {
			pos.character = p_test_data.range.start.character;
			test_resolve_symbol_at(p_uri, pos, p_uri, target->text, target->range);
		}

		SUBCASE("inside identifier") {
			pos.character = (p_test_data.range.end.character + p_test_data.range.start.character) / 2;
			test_resolve_symbol_at(p_uri, pos, p_uri, target->text, target->range);
		}

		SUBCASE("end of identifier") {
			pos.character = p_test_data.range.end.character;
			test_resolve_symbol_at(p_uri, pos, p_uri, target->text, target->range);
		}
	}
}

Vector<InlineTestData> filter_ref_towards(const Vector<InlineTestData> &p_data, const String &p_name) {
	Vector<InlineTestData> res;

	for (const InlineTestData &d : p_data) {
		if (d.ref == p_name) {
			res.append(d);
		}
	}

	return res;
}

void test_resolve_symbols(const String &p_uri, const Vector<InlineTestData> &p_test_data, const Vector<InlineTestData> &p_all_data) {
	for (const InlineTestData &d : p_test_data) {
		test_resolve_symbol(p_uri, d, p_all_data);
	}
}

void assert_no_errors_in(const String &p_path) {
	Error err;
	String source = FileAccess::get_file_as_string(p_path, &err);
	REQUIRE_MESSAGE(err == OK, vformat("Cannot read '%s'", p_path));

	BRScriptParser parser;
	err = parser.parse(source, p_path, true);
	REQUIRE_MESSAGE(err == OK, vformat("Errors while parsing '%s'", p_path));

	BRScriptAnalyzer analyzer(&parser);
	err = analyzer.analyze();
	REQUIRE_MESSAGE(err == OK, vformat("Errors while analyzing '%s'", p_path));
}

inline lsp::Position lsp_pos(int line, int character) {
	lsp::Position p;
	p.line = line;
	p.character = character;
	return p;
}

void test_position_roundtrip(lsp::Position p_lsp, BradotPosition p_gd, const PackedStringArray &p_lines) {
	BradotPosition actual_gd = BradotPosition::from_lsp(p_lsp, p_lines);
	CHECK_EQ(p_gd, actual_gd);
	lsp::Position actual_lsp = p_gd.to_lsp(p_lines);
	CHECK_EQ(p_lsp, actual_lsp);
}

// Note:
// * Cursor is BETWEEN chars
//	 * `va|r` -> cursor between `a`&`r`
//   * `var`
//        ^
//      -> Character on `r` -> cursor between `a`&`r`s for tests:
// * Line & Char:
//   * LSP: both 0-based
//   * Bradot: both 1-based
TEST_SUITE("[Modules][BRScript][LSP]") {
	TEST_CASE("Can convert positions to and from Bradot") {
		String code = R"(extends Node

var member := 42

func f():
		var value := 42
		return value + member)";
		PackedStringArray lines = code.split("\n");

		SUBCASE("line after end") {
			lsp::Position lsp = lsp_pos(7, 0);
			BradotPosition gd(8, 1);
			test_position_roundtrip(lsp, gd, lines);
		}
		SUBCASE("first char in first line") {
			lsp::Position lsp = lsp_pos(0, 0);
			BradotPosition gd(1, 1);
			test_position_roundtrip(lsp, gd, lines);
		}

		SUBCASE("with tabs") {
			// On `v` in `value` in `var value := ...`.
			lsp::Position lsp = lsp_pos(5, 6);
			BradotPosition gd(6, 13);
			test_position_roundtrip(lsp, gd, lines);
		}

		SUBCASE("doesn't fail with column outside of character length") {
			lsp::Position lsp = lsp_pos(2, 100);
			BradotPosition::from_lsp(lsp, lines);

			BradotPosition gd(3, 100);
			gd.to_lsp(lines);
		}

		SUBCASE("doesn't fail with line outside of line length") {
			lsp::Position lsp = lsp_pos(200, 100);
			BradotPosition::from_lsp(lsp, lines);

			BradotPosition gd(300, 100);
			gd.to_lsp(lines);
		}

		SUBCASE("special case: negative line for root class") {
			BradotPosition gd(-1, 0);
			lsp::Position expected = lsp_pos(0, 0);
			lsp::Position actual = gd.to_lsp(lines);
			CHECK_EQ(actual, expected);
		}
		SUBCASE("special case: lines.length() + 1 for root class") {
			BradotPosition gd(lines.size() + 1, 0);
			lsp::Position expected = lsp_pos(lines.size(), 0);
			lsp::Position actual = gd.to_lsp(lines);
			CHECK_EQ(actual, expected);
		}
	}
	TEST_CASE("[workspace][resolve_symbol]") {
		BRScriptLanguageProtocol *proto = initialize(root);
		REQUIRE(proto);
		Ref<BRScriptWorkspace> workspace = BRScriptLanguageProtocol::get_singleton()->get_workspace();

		{
			String path = "res://lsp/local_variables.br";
			assert_no_errors_in(path);
			String uri = workspace->get_file_uri(path);
			Vector<InlineTestData> all_test_data = read_tests(path);
			SUBCASE("Can get correct ranges for public variables") {
				Vector<InlineTestData> test_data = filter_ref_towards(all_test_data, "member");
				test_resolve_symbols(uri, test_data, all_test_data);
			}
			SUBCASE("Can get correct ranges for local variables") {
				Vector<InlineTestData> test_data = filter_ref_towards(all_test_data, "test");
				test_resolve_symbols(uri, test_data, all_test_data);
			}
			SUBCASE("Can get correct ranges for local parameters") {
				Vector<InlineTestData> test_data = filter_ref_towards(all_test_data, "arg");
				test_resolve_symbols(uri, test_data, all_test_data);
			}
		}

		SUBCASE("Can get correct ranges for indented variables") {
			String path = "res://lsp/indentation.br";
			assert_no_errors_in(path);
			String uri = workspace->get_file_uri(path);
			Vector<InlineTestData> all_test_data = read_tests(path);
			test_resolve_symbols(uri, all_test_data, all_test_data);
		}

		SUBCASE("Can get correct ranges for scopes") {
			String path = "res://lsp/scopes.br";
			assert_no_errors_in(path);
			String uri = workspace->get_file_uri(path);
			Vector<InlineTestData> all_test_data = read_tests(path);
			test_resolve_symbols(uri, all_test_data, all_test_data);
		}

		SUBCASE("Can get correct ranges for lambda") {
			String path = "res://lsp/lambdas.br";
			assert_no_errors_in(path);
			String uri = workspace->get_file_uri(path);
			Vector<InlineTestData> all_test_data = read_tests(path);
			test_resolve_symbols(uri, all_test_data, all_test_data);
		}

		SUBCASE("Can get correct ranges for inner class") {
			String path = "res://lsp/class.br";
			assert_no_errors_in(path);
			String uri = workspace->get_file_uri(path);
			Vector<InlineTestData> all_test_data = read_tests(path);
			test_resolve_symbols(uri, all_test_data, all_test_data);
		}

		SUBCASE("Can get correct ranges for inner class") {
			String path = "res://lsp/enums.br";
			assert_no_errors_in(path);
			String uri = workspace->get_file_uri(path);
			Vector<InlineTestData> all_test_data = read_tests(path);
			test_resolve_symbols(uri, all_test_data, all_test_data);
		}

		SUBCASE("Can get correct ranges for shadowing & shadowed variables") {
			String path = "res://lsp/shadowing_initializer.br";
			assert_no_errors_in(path);
			String uri = workspace->get_file_uri(path);
			Vector<InlineTestData> all_test_data = read_tests(path);
			test_resolve_symbols(uri, all_test_data, all_test_data);
		}

		SUBCASE("Can get correct ranges for properties and getter/setter") {
			String path = "res://lsp/properties.br";
			assert_no_errors_in(path);
			String uri = workspace->get_file_uri(path);
			Vector<InlineTestData> all_test_data = read_tests(path);
			test_resolve_symbols(uri, all_test_data, all_test_data);
		}

		memdelete(proto);
		finish_language();
	}
}

} // namespace BRScriptTests

#endif // TOOLS_ENABLED

#endif // TEST_LSP_H