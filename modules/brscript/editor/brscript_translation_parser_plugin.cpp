/**************************************************************************/
/*  brscript_translation_parser_plugin.cpp                                */
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

#include "brscript_translation_parser_plugin.h"

#include "../brscript.h"
#include "../brscript_analyzer.h"

#include "core/io/resource_loader.h"

void BRScriptEditorTranslationParserPlugin::get_recognized_extensions(List<String> *r_extensions) const {
	BRScriptLanguage::get_singleton()->get_recognized_extensions(r_extensions);
}

Error BRScriptEditorTranslationParserPlugin::parse_file(const String &p_path, Vector<String> *r_ids, Vector<Vector<String>> *r_ids_ctx_plural) {
	// Extract all translatable strings using the parsed tree from BRScriptParser.
	// The strategy is to find all ExpressionNode and AssignmentNode from the tree and extract strings if relevant, i.e
	// Search strings in ExpressionNode -> CallNode -> tr(), set_text(), set_placeholder() etc.
	// Search strings in AssignmentNode -> text = "__", tooltip_text = "__" etc.

	Error err;
	Ref<Resource> loaded_res = ResourceLoader::load(p_path, "", ResourceFormatLoader::CACHE_MODE_REUSE, &err);
	ERR_FAIL_COND_V_MSG(err, err, "Failed to load " + p_path);

	ids = r_ids;
	ids_ctx_plural = r_ids_ctx_plural;
	Ref<BRScript> brscript = loaded_res;
	String source_code = brscript->get_source_code();

	BRScriptParser parser;
	err = parser.parse(source_code, p_path, false);
	ERR_FAIL_COND_V_MSG(err, err, "Failed to parse BRScript with BRScriptParser.");

	BRScriptAnalyzer analyzer(&parser);
	err = analyzer.analyze();
	ERR_FAIL_COND_V_MSG(err, err, "Failed to analyze BRScript with BRScriptAnalyzer.");

	// Traverse through the parsed tree from BRScriptParser.
	BRScriptParser::ClassNode *c = parser.get_tree();
	_traverse_class(c);

	return OK;
}

bool BRScriptEditorTranslationParserPlugin::_is_constant_string(const BRScriptParser::ExpressionNode *p_expression) {
	ERR_FAIL_NULL_V(p_expression, false);
	return p_expression->is_constant && p_expression->reduced_value.is_string();
}

void BRScriptEditorTranslationParserPlugin::_traverse_class(const BRScriptParser::ClassNode *p_class) {
	for (int i = 0; i < p_class->members.size(); i++) {
		const BRScriptParser::ClassNode::Member &m = p_class->members[i];
		// Other member types can't contain translatable strings.
		switch (m.type) {
			case BRScriptParser::ClassNode::Member::CLASS:
				_traverse_class(m.m_class);
				break;
			case BRScriptParser::ClassNode::Member::FUNCTION:
				_traverse_function(m.function);
				break;
			case BRScriptParser::ClassNode::Member::VARIABLE:
				_assess_expression(m.variable->initializer);
				if (m.variable->property == BRScriptParser::VariableNode::PROP_INLINE) {
					_traverse_function(m.variable->setter);
					_traverse_function(m.variable->getter);
				}
				break;
			default:
				break;
		}
	}
}

void BRScriptEditorTranslationParserPlugin::_traverse_function(const BRScriptParser::FunctionNode *p_func) {
	if (!p_func) {
		return;
	}

	for (int i = 0; i < p_func->parameters.size(); i++) {
		_assess_expression(p_func->parameters[i]->initializer);
	}
	_traverse_block(p_func->body);
}

void BRScriptEditorTranslationParserPlugin::_traverse_block(const BRScriptParser::SuiteNode *p_suite) {
	if (!p_suite) {
		return;
	}

	const Vector<BRScriptParser::Node *> &statements = p_suite->statements;
	for (int i = 0; i < statements.size(); i++) {
		const BRScriptParser::Node *statement = statements[i];

		// BREAK, BREAKPOINT, CONSTANT, CONTINUE, and PASS are skipped because they can't contain translatable strings.
		switch (statement->type) {
			case BRScriptParser::Node::ASSERT: {
				const BRScriptParser::AssertNode *assert_node = static_cast<const BRScriptParser::AssertNode *>(statement);
				_assess_expression(assert_node->condition);
				_assess_expression(assert_node->message);
			} break;
			case BRScriptParser::Node::ASSIGNMENT: {
				_assess_assignment(static_cast<const BRScriptParser::AssignmentNode *>(statement));
			} break;
			case BRScriptParser::Node::FOR: {
				const BRScriptParser::ForNode *for_node = static_cast<const BRScriptParser::ForNode *>(statement);
				_assess_expression(for_node->list);
				_traverse_block(for_node->loop);
			} break;
			case BRScriptParser::Node::IF: {
				const BRScriptParser::IfNode *if_node = static_cast<const BRScriptParser::IfNode *>(statement);
				_assess_expression(if_node->condition);
				_traverse_block(if_node->true_block);
				_traverse_block(if_node->false_block);
			} break;
			case BRScriptParser::Node::MATCH: {
				const BRScriptParser::MatchNode *match_node = static_cast<const BRScriptParser::MatchNode *>(statement);
				_assess_expression(match_node->test);
				for (int j = 0; j < match_node->branches.size(); j++) {
					_traverse_block(match_node->branches[j]->guard_body);
					_traverse_block(match_node->branches[j]->block);
				}
			} break;
			case BRScriptParser::Node::RETURN: {
				_assess_expression(static_cast<const BRScriptParser::ReturnNode *>(statement)->return_value);
			} break;
			case BRScriptParser::Node::VARIABLE: {
				_assess_expression(static_cast<const BRScriptParser::VariableNode *>(statement)->initializer);
			} break;
			case BRScriptParser::Node::WHILE: {
				const BRScriptParser::WhileNode *while_node = static_cast<const BRScriptParser::WhileNode *>(statement);
				_assess_expression(while_node->condition);
				_traverse_block(while_node->loop);
			} break;
			default: {
				if (statement->is_expression()) {
					_assess_expression(static_cast<const BRScriptParser::ExpressionNode *>(statement));
				}
			} break;
		}
	}
}

void BRScriptEditorTranslationParserPlugin::_assess_expression(const BRScriptParser::ExpressionNode *p_expression) {
	// Explore all ExpressionNodes to find CallNodes which contain translation strings, such as tr(), set_text() etc.
	// tr() can be embedded quite deep within multiple ExpressionNodes so need to dig down to search through all ExpressionNodes.
	if (!p_expression) {
		return;
	}

	// GET_NODE, IDENTIFIER, LITERAL, PRELOAD, SELF, and TYPE are skipped because they can't contain translatable strings.
	switch (p_expression->type) {
		case BRScriptParser::Node::ARRAY: {
			const BRScriptParser::ArrayNode *array_node = static_cast<const BRScriptParser::ArrayNode *>(p_expression);
			for (int i = 0; i < array_node->elements.size(); i++) {
				_assess_expression(array_node->elements[i]);
			}
		} break;
		case BRScriptParser::Node::ASSIGNMENT: {
			_assess_assignment(static_cast<const BRScriptParser::AssignmentNode *>(p_expression));
		} break;
		case BRScriptParser::Node::AWAIT: {
			_assess_expression(static_cast<const BRScriptParser::AwaitNode *>(p_expression)->to_await);
		} break;
		case BRScriptParser::Node::BINARY_OPERATOR: {
			const BRScriptParser::BinaryOpNode *binary_op_node = static_cast<const BRScriptParser::BinaryOpNode *>(p_expression);
			_assess_expression(binary_op_node->left_operand);
			_assess_expression(binary_op_node->right_operand);
		} break;
		case BRScriptParser::Node::CALL: {
			_assess_call(static_cast<const BRScriptParser::CallNode *>(p_expression));
		} break;
		case BRScriptParser::Node::CAST: {
			_assess_expression(static_cast<const BRScriptParser::CastNode *>(p_expression)->operand);
		} break;
		case BRScriptParser::Node::DICTIONARY: {
			const BRScriptParser::DictionaryNode *dict_node = static_cast<const BRScriptParser::DictionaryNode *>(p_expression);
			for (int i = 0; i < dict_node->elements.size(); i++) {
				_assess_expression(dict_node->elements[i].key);
				_assess_expression(dict_node->elements[i].value);
			}
		} break;
		case BRScriptParser::Node::LAMBDA: {
			_traverse_function(static_cast<const BRScriptParser::LambdaNode *>(p_expression)->function);
		} break;
		case BRScriptParser::Node::SUBSCRIPT: {
			const BRScriptParser::SubscriptNode *subscript_node = static_cast<const BRScriptParser::SubscriptNode *>(p_expression);
			_assess_expression(subscript_node->base);
			if (!subscript_node->is_attribute) {
				_assess_expression(subscript_node->index);
			}
		} break;
		case BRScriptParser::Node::TERNARY_OPERATOR: {
			const BRScriptParser::TernaryOpNode *ternary_op_node = static_cast<const BRScriptParser::TernaryOpNode *>(p_expression);
			_assess_expression(ternary_op_node->condition);
			_assess_expression(ternary_op_node->true_expr);
			_assess_expression(ternary_op_node->false_expr);
		} break;
		case BRScriptParser::Node::TYPE_TEST: {
			_assess_expression(static_cast<const BRScriptParser::TypeTestNode *>(p_expression)->operand);
		} break;
		case BRScriptParser::Node::UNARY_OPERATOR: {
			_assess_expression(static_cast<const BRScriptParser::UnaryOpNode *>(p_expression)->operand);
		} break;
		default: {
		} break;
	}
}

void BRScriptEditorTranslationParserPlugin::_assess_assignment(const BRScriptParser::AssignmentNode *p_assignment) {
	_assess_expression(p_assignment->assignee);
	_assess_expression(p_assignment->assigned_value);

	// Extract the translatable strings coming from assignments. For example, get_node("Label").text = "____"

	StringName assignee_name;
	if (p_assignment->assignee->type == BRScriptParser::Node::IDENTIFIER) {
		assignee_name = static_cast<const BRScriptParser::IdentifierNode *>(p_assignment->assignee)->name;
	} else if (p_assignment->assignee->type == BRScriptParser::Node::SUBSCRIPT) {
		const BRScriptParser::SubscriptNode *subscript = static_cast<const BRScriptParser::SubscriptNode *>(p_assignment->assignee);
		if (subscript->is_attribute && subscript->attribute) {
			assignee_name = subscript->attribute->name;
		} else if (subscript->index && _is_constant_string(subscript->index)) {
			assignee_name = subscript->index->reduced_value;
		}
	}

	if (assignee_name != StringName() && assignment_patterns.has(assignee_name) && _is_constant_string(p_assignment->assigned_value)) {
		// If the assignment is towards one of the extract patterns (text, tooltip_text etc.), and the value is a constant string, we collect the string.
		ids->push_back(p_assignment->assigned_value->reduced_value);
	} else if (assignee_name == fd_filters) {
		// Extract from `get_node("FileDialog").filters = <filter array>`.
		_extract_fd_filter_array(p_assignment->assigned_value);
	}
}

void BRScriptEditorTranslationParserPlugin::_assess_call(const BRScriptParser::CallNode *p_call) {
	_assess_expression(p_call->callee);
	for (int i = 0; i < p_call->arguments.size(); i++) {
		_assess_expression(p_call->arguments[i]);
	}

	// Extract the translatable strings coming from function calls. For example:
	// tr("___"), get_node("Label").set_text("____"), get_node("LineEdit").set_placeholder("____").

	StringName function_name = p_call->function_name;

	// Variables for extracting tr() and tr_n().
	Vector<String> id_ctx_plural;
	id_ctx_plural.resize(3);
	bool extract_id_ctx_plural = true;

	if (function_name == tr_func || function_name == atr_func) {
		// Extract from `tr(id, ctx)` or `atr(id, ctx)`.
		for (int i = 0; i < p_call->arguments.size(); i++) {
			if (_is_constant_string(p_call->arguments[i])) {
				id_ctx_plural.write[i] = p_call->arguments[i]->reduced_value;
			} else {
				// Avoid adding something like tr("Flying dragon", var_context_level_1). We want to extract both id and context together.
				extract_id_ctx_plural = false;
			}
		}
		if (extract_id_ctx_plural) {
			ids_ctx_plural->push_back(id_ctx_plural);
		}
	} else if (function_name == trn_func || function_name == atrn_func) {
		// Extract from `tr_n(id, plural, n, ctx)` or `atr_n(id, plural, n, ctx)`.
		Vector<int> indices;
		indices.push_back(0);
		indices.push_back(3);
		indices.push_back(1);
		for (int i = 0; i < indices.size(); i++) {
			if (indices[i] >= p_call->arguments.size()) {
				continue;
			}

			if (_is_constant_string(p_call->arguments[indices[i]])) {
				id_ctx_plural.write[i] = p_call->arguments[indices[i]]->reduced_value;
			} else {
				extract_id_ctx_plural = false;
			}
		}
		if (extract_id_ctx_plural) {
			ids_ctx_plural->push_back(id_ctx_plural);
		}
	} else if (first_arg_patterns.has(function_name)) {
		if (!p_call->arguments.is_empty() && _is_constant_string(p_call->arguments[0])) {
			ids->push_back(p_call->arguments[0]->reduced_value);
		}
	} else if (second_arg_patterns.has(function_name)) {
		if (p_call->arguments.size() > 1 && _is_constant_string(p_call->arguments[1])) {
			ids->push_back(p_call->arguments[1]->reduced_value);
		}
	} else if (function_name == fd_add_filter) {
		// Extract the 'JPE Images' in this example - get_node("FileDialog").add_filter("*.jpg; JPE Images").
		if (!p_call->arguments.is_empty()) {
			_extract_fd_filter_string(p_call->arguments[0]);
		}
	} else if (function_name == fd_set_filter) {
		// Extract from `get_node("FileDialog").set_filters(<filter array>)`.
		if (!p_call->arguments.is_empty()) {
			_extract_fd_filter_array(p_call->arguments[0]);
		}
	}
}

void BRScriptEditorTranslationParserPlugin::_extract_fd_filter_string(const BRScriptParser::ExpressionNode *p_expression) {
	// Extract the name in "extension ; name".
	if (_is_constant_string(p_expression)) {
		PackedStringArray arr = p_expression->reduced_value.operator String().split(";", true);
		ERR_FAIL_COND_MSG(arr.size() != 2, "Argument for setting FileDialog has bad format.");
		ids->push_back(arr[1].strip_edges());
	}
}

void BRScriptEditorTranslationParserPlugin::_extract_fd_filter_array(const BRScriptParser::ExpressionNode *p_expression) {
	const BRScriptParser::ArrayNode *array_node = nullptr;

	if (p_expression->type == BRScriptParser::Node::ARRAY) {
		// Extract from `["*.png ; PNG Images","*.br ; BRScript Files"]` (implicit cast to `PackedStringArray`).
		array_node = static_cast<const BRScriptParser::ArrayNode *>(p_expression);
	} else if (p_expression->type == BRScriptParser::Node::CALL) {
		// Extract from `PackedStringArray(["*.png ; PNG Images","*.br ; BRScript Files"])`.
		const BRScriptParser::CallNode *call_node = static_cast<const BRScriptParser::CallNode *>(p_expression);
		if (call_node->get_callee_type() == BRScriptParser::Node::IDENTIFIER && call_node->function_name == SNAME("PackedStringArray") && !call_node->arguments.is_empty() && call_node->arguments[0]->type == BRScriptParser::Node::ARRAY) {
			array_node = static_cast<const BRScriptParser::ArrayNode *>(call_node->arguments[0]);
		}
	}

	if (array_node) {
		for (int i = 0; i < array_node->elements.size(); i++) {
			_extract_fd_filter_string(array_node->elements[i]);
		}
	}
}

BRScriptEditorTranslationParserPlugin::BRScriptEditorTranslationParserPlugin() {
	assignment_patterns.insert("text");
	assignment_patterns.insert("placeholder_text");
	assignment_patterns.insert("tooltip_text");

	first_arg_patterns.insert("set_text");
	first_arg_patterns.insert("set_tooltip_text");
	first_arg_patterns.insert("set_placeholder");
	first_arg_patterns.insert("add_tab");
	first_arg_patterns.insert("add_check_item");
	first_arg_patterns.insert("add_item");
	first_arg_patterns.insert("add_multistate_item");
	first_arg_patterns.insert("add_radio_check_item");
	first_arg_patterns.insert("add_separator");
	first_arg_patterns.insert("add_submenu_item");

	second_arg_patterns.insert("set_tab_title");
	second_arg_patterns.insert("add_icon_check_item");
	second_arg_patterns.insert("add_icon_item");
	second_arg_patterns.insert("add_icon_radio_check_item");
	second_arg_patterns.insert("set_item_text");
}
