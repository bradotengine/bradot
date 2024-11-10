/**************************************************************************/
/*  brscript_analyzer.h                                                   */
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

#ifndef BRSCRIPT_ANALYZER_H
#define BRSCRIPT_ANALYZER_H

#include "brscript_cache.h"
#include "brscript_parser.h"

#include "core/object/object.h"
#include "core/object/ref_counted.h"
#include "core/templates/hash_set.h"

class BRScriptAnalyzer {
	BRScriptParser *parser = nullptr;

	template <typename Fn>
	class Finally {
		Fn fn;

	public:
		Finally(Fn p_fn) :
				fn(p_fn) {}
		~Finally() {
			fn();
		}
	};

	const BRScriptParser::EnumNode *current_enum = nullptr;
	BRScriptParser::LambdaNode *current_lambda = nullptr;
	List<BRScriptParser::LambdaNode *> pending_body_resolution_lambdas;
	HashMap<const BRScriptParser::ClassNode *, Ref<BRScriptParserRef>> external_class_parser_cache;
	bool static_context = false;

	// Tests for detecting invalid overloading of script members
	static _FORCE_INLINE_ bool has_member_name_conflict_in_script_class(const StringName &p_name, const BRScriptParser::ClassNode *p_current_class_node, const BRScriptParser::Node *p_member);
	static _FORCE_INLINE_ bool has_member_name_conflict_in_native_type(const StringName &p_name, const StringName &p_native_type_string);
	Error check_native_member_name_conflict(const StringName &p_member_name, const BRScriptParser::Node *p_member_node, const StringName &p_native_type_string);
	Error check_class_member_name_conflict(const BRScriptParser::ClassNode *p_class_node, const StringName &p_member_name, const BRScriptParser::Node *p_member_node);

	void get_class_node_current_scope_classes(BRScriptParser::ClassNode *p_node, List<BRScriptParser::ClassNode *> *p_list, BRScriptParser::Node *p_source);

	Error resolve_class_inheritance(BRScriptParser::ClassNode *p_class, const BRScriptParser::Node *p_source = nullptr);
	Error resolve_class_inheritance(BRScriptParser::ClassNode *p_class, bool p_recursive);
	BRScriptParser::DataType resolve_datatype(BRScriptParser::TypeNode *p_type);

	void decide_suite_type(BRScriptParser::Node *p_suite, BRScriptParser::Node *p_statement);

	void resolve_annotation(BRScriptParser::AnnotationNode *p_annotation);
	void resolve_class_member(BRScriptParser::ClassNode *p_class, const StringName &p_name, const BRScriptParser::Node *p_source = nullptr);
	void resolve_class_member(BRScriptParser::ClassNode *p_class, int p_index, const BRScriptParser::Node *p_source = nullptr);
	void resolve_class_interface(BRScriptParser::ClassNode *p_class, const BRScriptParser::Node *p_source = nullptr);
	void resolve_class_interface(BRScriptParser::ClassNode *p_class, bool p_recursive);
	void resolve_class_body(BRScriptParser::ClassNode *p_class, const BRScriptParser::Node *p_source = nullptr);
	void resolve_class_body(BRScriptParser::ClassNode *p_class, bool p_recursive);
	void resolve_function_signature(BRScriptParser::FunctionNode *p_function, const BRScriptParser::Node *p_source = nullptr, bool p_is_lambda = false);
	void resolve_function_body(BRScriptParser::FunctionNode *p_function, bool p_is_lambda = false);
	void resolve_node(BRScriptParser::Node *p_node, bool p_is_root = true);
	void resolve_suite(BRScriptParser::SuiteNode *p_suite);
	void resolve_assignable(BRScriptParser::AssignableNode *p_assignable, const char *p_kind);
	void resolve_variable(BRScriptParser::VariableNode *p_variable, bool p_is_local);
	void resolve_constant(BRScriptParser::ConstantNode *p_constant, bool p_is_local);
	void resolve_parameter(BRScriptParser::ParameterNode *p_parameter);
	void resolve_if(BRScriptParser::IfNode *p_if);
	void resolve_for(BRScriptParser::ForNode *p_for);
	void resolve_while(BRScriptParser::WhileNode *p_while);
	void resolve_assert(BRScriptParser::AssertNode *p_assert);
	void resolve_match(BRScriptParser::MatchNode *p_match);
	void resolve_match_branch(BRScriptParser::MatchBranchNode *p_match_branch, BRScriptParser::ExpressionNode *p_match_test);
	void resolve_match_pattern(BRScriptParser::PatternNode *p_match_pattern, BRScriptParser::ExpressionNode *p_match_test);
	void resolve_return(BRScriptParser::ReturnNode *p_return);

	// Reduction functions.
	void reduce_expression(BRScriptParser::ExpressionNode *p_expression, bool p_is_root = false);
	void reduce_array(BRScriptParser::ArrayNode *p_array);
	void reduce_assignment(BRScriptParser::AssignmentNode *p_assignment);
	void reduce_await(BRScriptParser::AwaitNode *p_await);
	void reduce_binary_op(BRScriptParser::BinaryOpNode *p_binary_op);
	void reduce_call(BRScriptParser::CallNode *p_call, bool p_is_await = false, bool p_is_root = false);
	void reduce_cast(BRScriptParser::CastNode *p_cast);
	void reduce_dictionary(BRScriptParser::DictionaryNode *p_dictionary);
	void reduce_get_node(BRScriptParser::GetNodeNode *p_get_node);
	void reduce_identifier(BRScriptParser::IdentifierNode *p_identifier, bool can_be_builtin = false);
	void reduce_identifier_from_base(BRScriptParser::IdentifierNode *p_identifier, BRScriptParser::DataType *p_base = nullptr);
	void reduce_lambda(BRScriptParser::LambdaNode *p_lambda);
	void reduce_literal(BRScriptParser::LiteralNode *p_literal);
	void reduce_preload(BRScriptParser::PreloadNode *p_preload);
	void reduce_self(BRScriptParser::SelfNode *p_self);
	void reduce_subscript(BRScriptParser::SubscriptNode *p_subscript, bool p_can_be_pseudo_type = false);
	void reduce_ternary_op(BRScriptParser::TernaryOpNode *p_ternary_op, bool p_is_root = false);
	void reduce_type_test(BRScriptParser::TypeTestNode *p_type_test);
	void reduce_unary_op(BRScriptParser::UnaryOpNode *p_unary_op);

	Variant make_expression_reduced_value(BRScriptParser::ExpressionNode *p_expression, bool &is_reduced);
	Variant make_array_reduced_value(BRScriptParser::ArrayNode *p_array, bool &is_reduced);
	Variant make_dictionary_reduced_value(BRScriptParser::DictionaryNode *p_dictionary, bool &is_reduced);
	Variant make_subscript_reduced_value(BRScriptParser::SubscriptNode *p_subscript, bool &is_reduced);

	// Helpers.
	Array make_array_from_element_datatype(const BRScriptParser::DataType &p_element_datatype, const BRScriptParser::Node *p_source_node = nullptr);
	Dictionary make_dictionary_from_element_datatype(const BRScriptParser::DataType &p_key_element_datatype, const BRScriptParser::DataType &p_value_element_datatype, const BRScriptParser::Node *p_source_node = nullptr);
	BRScriptParser::DataType type_from_variant(const Variant &p_value, const BRScriptParser::Node *p_source);
	static BRScriptParser::DataType type_from_metatype(const BRScriptParser::DataType &p_meta_type);
	BRScriptParser::DataType type_from_property(const PropertyInfo &p_property, bool p_is_arg = false, bool p_is_readonly = false) const;
	BRScriptParser::DataType make_global_class_meta_type(const StringName &p_class_name, const BRScriptParser::Node *p_source);
	bool get_function_signature(BRScriptParser::Node *p_source, bool p_is_constructor, BRScriptParser::DataType base_type, const StringName &p_function, BRScriptParser::DataType &r_return_type, List<BRScriptParser::DataType> &r_par_types, int &r_default_arg_count, BitField<MethodFlags> &r_method_flags, StringName *r_native_class = nullptr);
	bool function_signature_from_info(const MethodInfo &p_info, BRScriptParser::DataType &r_return_type, List<BRScriptParser::DataType> &r_par_types, int &r_default_arg_count, BitField<MethodFlags> &r_method_flags);
	void validate_call_arg(const List<BRScriptParser::DataType> &p_par_types, int p_default_args_count, bool p_is_vararg, const BRScriptParser::CallNode *p_call);
	void validate_call_arg(const MethodInfo &p_method, const BRScriptParser::CallNode *p_call);
	BRScriptParser::DataType get_operation_type(Variant::Operator p_operation, const BRScriptParser::DataType &p_a, const BRScriptParser::DataType &p_b, bool &r_valid, const BRScriptParser::Node *p_source);
	BRScriptParser::DataType get_operation_type(Variant::Operator p_operation, const BRScriptParser::DataType &p_a, bool &r_valid, const BRScriptParser::Node *p_source);
	void update_const_expression_builtin_type(BRScriptParser::ExpressionNode *p_expression, const BRScriptParser::DataType &p_type, const char *p_usage, bool p_is_cast = false);
	void update_array_literal_element_type(BRScriptParser::ArrayNode *p_array, const BRScriptParser::DataType &p_element_type);
	void update_dictionary_literal_element_type(BRScriptParser::DictionaryNode *p_dictionary, const BRScriptParser::DataType &p_key_element_type, const BRScriptParser::DataType &p_value_element_type);
	bool is_type_compatible(const BRScriptParser::DataType &p_target, const BRScriptParser::DataType &p_source, bool p_allow_implicit_conversion = false, const BRScriptParser::Node *p_source_node = nullptr);
	void push_error(const String &p_message, const BRScriptParser::Node *p_origin = nullptr);
	void mark_node_unsafe(const BRScriptParser::Node *p_node);
	void downgrade_node_type_source(BRScriptParser::Node *p_node);
	void mark_lambda_use_self();
	void resolve_pending_lambda_bodies();
	bool class_exists(const StringName &p_class) const;
	void reduce_identifier_from_base_set_class(BRScriptParser::IdentifierNode *p_identifier, BRScriptParser::DataType p_identifier_datatype);
	Ref<BRScriptParserRef> ensure_cached_external_parser_for_class(const BRScriptParser::ClassNode *p_class, const BRScriptParser::ClassNode *p_from_class, const char *p_context, const BRScriptParser::Node *p_source);
	Ref<BRScriptParserRef> find_cached_external_parser_for_class(const BRScriptParser::ClassNode *p_class, const Ref<BRScriptParserRef> &p_dependant_parser);
	Ref<BRScriptParserRef> find_cached_external_parser_for_class(const BRScriptParser::ClassNode *p_class, BRScriptParser *p_dependant_parser);
	Ref<BRScript> get_depended_shallow_script(const String &p_path, Error &r_error);
#ifdef DEBUG_ENABLED
	void is_shadowing(BRScriptParser::IdentifierNode *p_identifier, const String &p_context, const bool p_in_local_scope);
#endif

public:
	Error resolve_inheritance();
	Error resolve_interface();
	Error resolve_body();
	Error resolve_dependencies();
	Error analyze();

	Variant make_variable_default_value(BRScriptParser::VariableNode *p_variable);
	static bool check_type_compatibility(const BRScriptParser::DataType &p_target, const BRScriptParser::DataType &p_source, bool p_allow_implicit_conversion = false, const BRScriptParser::Node *p_source_node = nullptr);

	BRScriptAnalyzer(BRScriptParser *p_parser);
};

#endif // BRSCRIPT_ANALYZER_H
