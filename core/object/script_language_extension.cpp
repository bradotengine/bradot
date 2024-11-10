/**************************************************************************/
/*  script_language_extension.cpp                                         */
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

#include "script_language_extension.h"

void ScriptExtension::_bind_methods() {
	BRVIRTUAL_BIND(_editor_can_reload_from_file);
	BRVIRTUAL_BIND(_placeholder_erased, "placeholder");

	BRVIRTUAL_BIND(_can_instantiate);
	BRVIRTUAL_BIND(_get_base_script);
	BRVIRTUAL_BIND(_get_global_name);
	BRVIRTUAL_BIND(_inherits_script, "script");

	BRVIRTUAL_BIND(_get_instance_base_type);
	BRVIRTUAL_BIND(_instance_create, "for_object");
	BRVIRTUAL_BIND(_placeholder_instance_create, "for_object");

	BRVIRTUAL_BIND(_instance_has, "object");

	BRVIRTUAL_BIND(_has_source_code);
	BRVIRTUAL_BIND(_get_source_code);

	BRVIRTUAL_BIND(_set_source_code, "code");
	BRVIRTUAL_BIND(_reload, "keep_state");

	BRVIRTUAL_BIND(_get_documentation);
	BRVIRTUAL_BIND(_get_class_icon_path);

	BRVIRTUAL_BIND(_has_method, "method");
	BRVIRTUAL_BIND(_has_static_method, "method");

	BRVIRTUAL_BIND(_get_script_method_argument_count, "method");

	BRVIRTUAL_BIND(_get_method_info, "method");

	BRVIRTUAL_BIND(_is_tool);
	BRVIRTUAL_BIND(_is_valid);
	BRVIRTUAL_BIND(_is_abstract);
	BRVIRTUAL_BIND(_get_language);

	BRVIRTUAL_BIND(_has_script_signal, "signal");
	BRVIRTUAL_BIND(_get_script_signal_list);

	BRVIRTUAL_BIND(_has_property_default_value, "property");
	BRVIRTUAL_BIND(_get_property_default_value, "property");

	BRVIRTUAL_BIND(_update_exports);
	BRVIRTUAL_BIND(_get_script_method_list);
	BRVIRTUAL_BIND(_get_script_property_list);

	BRVIRTUAL_BIND(_get_member_line, "member");

	BRVIRTUAL_BIND(_get_constants);
	BRVIRTUAL_BIND(_get_members);
	BRVIRTUAL_BIND(_is_placeholder_fallback_enabled);

	BRVIRTUAL_BIND(_get_rpc_config);
}

void ScriptLanguageExtension::_bind_methods() {
	BRVIRTUAL_BIND(_get_name);
	BRVIRTUAL_BIND(_init);
	BRVIRTUAL_BIND(_get_type);
	BRVIRTUAL_BIND(_get_extension);
	BRVIRTUAL_BIND(_finish);

	BRVIRTUAL_BIND(_get_reserved_words);
	BRVIRTUAL_BIND(_is_control_flow_keyword, "keyword");
	BRVIRTUAL_BIND(_get_comment_delimiters);
	BRVIRTUAL_BIND(_get_doc_comment_delimiters);
	BRVIRTUAL_BIND(_get_string_delimiters);
	BRVIRTUAL_BIND(_make_template, "template", "class_name", "base_class_name");
	BRVIRTUAL_BIND(_get_built_in_templates, "object");
	BRVIRTUAL_BIND(_is_using_templates);
	BRVIRTUAL_BIND(_validate, "script", "path", "validate_functions", "validate_errors", "validate_warnings", "validate_safe_lines");

	BRVIRTUAL_BIND(_validate_path, "path");
	BRVIRTUAL_BIND(_create_script);
#ifndef DISABLE_DEPRECATED
	BRVIRTUAL_BIND(_has_named_classes);
#endif
	BRVIRTUAL_BIND(_supports_builtin_mode);
	BRVIRTUAL_BIND(_supports_documentation);
	BRVIRTUAL_BIND(_can_inherit_from_file);
	BRVIRTUAL_BIND(_find_function, "function", "code");
	BRVIRTUAL_BIND(_make_function, "class_name", "function_name", "function_args");
	BRVIRTUAL_BIND(_can_make_function);
	BRVIRTUAL_BIND(_open_in_external_editor, "script", "line", "column");
	BRVIRTUAL_BIND(_overrides_external_editor);
	BRVIRTUAL_BIND(_preferred_file_name_casing);

	BRVIRTUAL_BIND(_complete_code, "code", "path", "owner");
	BRVIRTUAL_BIND(_lookup_code, "code", "symbol", "path", "owner");
	BRVIRTUAL_BIND(_auto_indent_code, "code", "from_line", "to_line");

	BRVIRTUAL_BIND(_add_global_constant, "name", "value");
	BRVIRTUAL_BIND(_add_named_global_constant, "name", "value");
	BRVIRTUAL_BIND(_remove_named_global_constant, "name");

	BRVIRTUAL_BIND(_thread_enter);
	BRVIRTUAL_BIND(_thread_exit);
	BRVIRTUAL_BIND(_debug_get_error);
	BRVIRTUAL_BIND(_debug_get_stack_level_count);

	BRVIRTUAL_BIND(_debug_get_stack_level_line, "level");
	BRVIRTUAL_BIND(_debug_get_stack_level_function, "level");
	BRVIRTUAL_BIND(_debug_get_stack_level_source, "level");
	BRVIRTUAL_BIND(_debug_get_stack_level_locals, "level", "max_subitems", "max_depth");
	BRVIRTUAL_BIND(_debug_get_stack_level_members, "level", "max_subitems", "max_depth");
	BRVIRTUAL_BIND(_debug_get_stack_level_instance, "level");
	BRVIRTUAL_BIND(_debug_get_globals, "max_subitems", "max_depth");
	BRVIRTUAL_BIND(_debug_parse_stack_level_expression, "level", "expression", "max_subitems", "max_depth");

	BRVIRTUAL_BIND(_debug_get_current_stack_info);

	BRVIRTUAL_BIND(_reload_all_scripts);
	BRVIRTUAL_BIND(_reload_scripts, "scripts", "soft_reload");
	BRVIRTUAL_BIND(_reload_tool_script, "script", "soft_reload");

	BRVIRTUAL_BIND(_get_recognized_extensions);
	BRVIRTUAL_BIND(_get_public_functions);
	BRVIRTUAL_BIND(_get_public_constants);
	BRVIRTUAL_BIND(_get_public_annotations);

	BRVIRTUAL_BIND(_profiling_start);
	BRVIRTUAL_BIND(_profiling_stop);
	BRVIRTUAL_BIND(_profiling_set_save_native_calls, "enable");

	BRVIRTUAL_BIND(_profiling_get_accumulated_data, "info_array", "info_max");
	BRVIRTUAL_BIND(_profiling_get_frame_data, "info_array", "info_max");

	BRVIRTUAL_BIND(_frame);

	BRVIRTUAL_BIND(_handles_global_class_type, "type");
	BRVIRTUAL_BIND(_get_global_class_name, "path");

	BIND_ENUM_CONSTANT(LOOKUP_RESULT_SCRIPT_LOCATION);
	BIND_ENUM_CONSTANT(LOOKUP_RESULT_CLASS);
	BIND_ENUM_CONSTANT(LOOKUP_RESULT_CLASS_CONSTANT);
	BIND_ENUM_CONSTANT(LOOKUP_RESULT_CLASS_PROPERTY);
	BIND_ENUM_CONSTANT(LOOKUP_RESULT_CLASS_METHOD);
	BIND_ENUM_CONSTANT(LOOKUP_RESULT_CLASS_SIGNAL);
	BIND_ENUM_CONSTANT(LOOKUP_RESULT_CLASS_ENUM);
	BIND_ENUM_CONSTANT(LOOKUP_RESULT_CLASS_TBD_GLOBALSCOPE);
	BIND_ENUM_CONSTANT(LOOKUP_RESULT_CLASS_ANNOTATION);
	BIND_ENUM_CONSTANT(LOOKUP_RESULT_MAX);

	BIND_ENUM_CONSTANT(LOCATION_LOCAL);
	BIND_ENUM_CONSTANT(LOCATION_PARENT_MASK);
	BIND_ENUM_CONSTANT(LOCATION_OTHER_USER_CODE);
	BIND_ENUM_CONSTANT(LOCATION_OTHER);

	BIND_ENUM_CONSTANT(CODE_COMPLETION_KIND_CLASS);
	BIND_ENUM_CONSTANT(CODE_COMPLETION_KIND_FUNCTION);
	BIND_ENUM_CONSTANT(CODE_COMPLETION_KIND_SIGNAL);
	BIND_ENUM_CONSTANT(CODE_COMPLETION_KIND_VARIABLE);
	BIND_ENUM_CONSTANT(CODE_COMPLETION_KIND_MEMBER);
	BIND_ENUM_CONSTANT(CODE_COMPLETION_KIND_ENUM);
	BIND_ENUM_CONSTANT(CODE_COMPLETION_KIND_CONSTANT);
	BIND_ENUM_CONSTANT(CODE_COMPLETION_KIND_NODE_PATH);
	BIND_ENUM_CONSTANT(CODE_COMPLETION_KIND_FILE_PATH);
	BIND_ENUM_CONSTANT(CODE_COMPLETION_KIND_PLAIN_TEXT);
	BIND_ENUM_CONSTANT(CODE_COMPLETION_KIND_MAX);
}
