/**************************************************************************/
/*  editor_internal_calls.cpp                                             */
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

#include "editor_internal_calls.h"

#include "../csharp_script.h"
#include "../bradotsharp_dirs.h"
#include "../interop_types.h"
#include "../utils/macos_utils.h"
#include "../utils/path_utils.h"
#include "code_completion.h"

#include "core/config/project_settings.h"
#include "core/os/os.h"
#include "core/version.h"
#include "editor/debugger/editor_debugger_node.h"
#include "editor/editor_main_screen.h"
#include "editor/editor_node.h"
#include "editor/editor_paths.h"
#include "editor/editor_settings.h"
#include "editor/export/lipo.h"
#include "editor/gui/editor_run_bar.h"
#include "editor/plugins/script_editor_plugin.h"
#include "editor/themes/editor_scale.h"
#include "main/main.h"

#ifdef UNIX_ENABLED
#include <unistd.h> // access
#endif

#ifdef __cplusplus
extern "C" {
#endif

void bradot_icall_BradotSharpDirs_ResMetadataDir(bradot_string *r_dest) {
	memnew_placement(r_dest, String(BradotSharpDirs::get_res_metadata_dir()));
}

void bradot_icall_BradotSharpDirs_MonoUserDir(bradot_string *r_dest) {
	memnew_placement(r_dest, String(BradotSharpDirs::get_mono_user_dir()));
}

void bradot_icall_BradotSharpDirs_BuildLogsDirs(bradot_string *r_dest) {
#ifdef TOOLS_ENABLED
	memnew_placement(r_dest, String(BradotSharpDirs::get_build_logs_dir()));
#else
	return nullptr;
#endif
}

void bradot_icall_BradotSharpDirs_DataEditorToolsDir(bradot_string *r_dest) {
#ifdef TOOLS_ENABLED
	memnew_placement(r_dest, String(BradotSharpDirs::get_data_editor_tools_dir()));
#else
	return nullptr;
#endif
}

void bradot_icall_BradotSharpDirs_CSharpProjectName(bradot_string *r_dest) {
	memnew_placement(r_dest, String(path::get_csharp_project_name()));
}

void bradot_icall_EditorProgress_Create(const bradot_string *p_task, const bradot_string *p_label, int32_t p_amount, bool p_can_cancel) {
	String task = *reinterpret_cast<const String *>(p_task);
	String label = *reinterpret_cast<const String *>(p_label);
	EditorNode::progress_add_task(task, label, p_amount, (bool)p_can_cancel);
}

void bradot_icall_EditorProgress_Dispose(const bradot_string *p_task) {
	String task = *reinterpret_cast<const String *>(p_task);
	EditorNode::progress_end_task(task);
}

bool bradot_icall_EditorProgress_Step(const bradot_string *p_task, const bradot_string *p_state, int32_t p_step, bool p_force_refresh) {
	String task = *reinterpret_cast<const String *>(p_task);
	String state = *reinterpret_cast<const String *>(p_state);
	return EditorNode::progress_task_step(task, state, p_step, (bool)p_force_refresh);
}

void bradot_icall_Internal_FullExportTemplatesDir(bradot_string *r_dest) {
	String full_templates_dir = EditorPaths::get_singleton()->get_export_templates_dir().path_join(VERSION_FULL_CONFIG);
	memnew_placement(r_dest, String(full_templates_dir));
}

bool bradot_icall_Internal_IsMacOSAppBundleInstalled(const bradot_string *p_bundle_id) {
#ifdef MACOS_ENABLED
	String bundle_id = *reinterpret_cast<const String *>(p_bundle_id);
	return (bool)macos_is_app_bundle_installed(bundle_id);
#else
	(void)p_bundle_id; // UNUSED
	return (bool)false;
#endif
}

bool bradot_icall_Internal_LipOCreateFile(const bradot_string *p_output_path, const bradot_packed_array *p_files) {
	String output_path = *reinterpret_cast<const String *>(p_output_path);
	PackedStringArray files = *reinterpret_cast<const PackedStringArray *>(p_files);
	LipO lip;
	return lip.create_file(output_path, files);
}

bool bradot_icall_Internal_BradotIs32Bits() {
	return sizeof(void *) == 4;
}

bool bradot_icall_Internal_BradotIsRealTDouble() {
#ifdef REAL_T_IS_DOUBLE
	return (bool)true;
#else
	return (bool)false;
#endif
}

void bradot_icall_Internal_BradotMainIteration() {
	Main::iteration();
}

bool bradot_icall_Internal_IsAssembliesReloadingNeeded() {
#ifdef BR_MONO_HOT_RELOAD
	return (bool)CSharpLanguage::get_singleton()->is_assembly_reloading_needed();
#else
	return (bool)false;
#endif
}

void bradot_icall_Internal_ReloadAssemblies(bool p_soft_reload) {
#ifdef BR_MONO_HOT_RELOAD
	callable_mp(mono_bind::BradotSharp::get_singleton(), &mono_bind::BradotSharp::reload_assemblies).call_deferred(p_soft_reload);
#endif
}

void bradot_icall_Internal_EditorDebuggerNodeReloadScripts() {
	EditorDebuggerNode::get_singleton()->reload_all_scripts();
}

bool bradot_icall_Internal_ScriptEditorEdit(Resource *p_resource, int32_t p_line, int32_t p_col, bool p_grab_focus) {
	Ref<Resource> resource = p_resource;
	return (bool)ScriptEditor::get_singleton()->edit(resource, p_line, p_col, (bool)p_grab_focus);
}

void bradot_icall_Internal_EditorNodeShowScriptScreen() {
	EditorNode::get_editor_main_screen()->select(EditorMainScreen::EDITOR_SCRIPT);
}

void bradot_icall_Internal_EditorRunPlay() {
	EditorRunBar::get_singleton()->play_main_scene();
}

void bradot_icall_Internal_EditorRunStop() {
	EditorRunBar::get_singleton()->stop_playing();
}

void bradot_icall_Internal_EditorPlugin_AddControlToEditorRunBar(Control *p_control) {
	EditorRunBar::get_singleton()->get_buttons_container()->add_child(p_control);
}

void bradot_icall_Internal_ScriptEditorDebugger_ReloadScripts() {
	EditorDebuggerNode *ed = EditorDebuggerNode::get_singleton();
	if (ed) {
		ed->reload_all_scripts();
	}
}

void bradot_icall_Internal_CodeCompletionRequest(int32_t p_kind, const bradot_string *p_script_file, bradot_packed_array *r_ret) {
	String script_file = *reinterpret_cast<const String *>(p_script_file);
	PackedStringArray suggestions = gdmono::get_code_completion((gdmono::CompletionKind)p_kind, script_file);
	memnew_placement(r_ret, PackedStringArray(suggestions));
}

float bradot_icall_Globals_EditorScale() {
	return EDSCALE;
}

void bradot_icall_Globals_GlobalDef(const bradot_string *p_setting, const bradot_variant *p_default_value, bool p_restart_if_changed, bradot_variant *r_result) {
	String setting = *reinterpret_cast<const String *>(p_setting);
	Variant default_value = *reinterpret_cast<const Variant *>(p_default_value);
	Variant result = _GLOBAL_DEF(setting, default_value, (bool)p_restart_if_changed);
	memnew_placement(r_result, Variant(result));
}

void bradot_icall_Globals_EditorDef(const bradot_string *p_setting, const bradot_variant *p_default_value, bool p_restart_if_changed, bradot_variant *r_result) {
	String setting = *reinterpret_cast<const String *>(p_setting);
	Variant default_value = *reinterpret_cast<const Variant *>(p_default_value);
	Variant result = _EDITOR_DEF(setting, default_value, (bool)p_restart_if_changed);
	memnew_placement(r_result, Variant(result));
}

void bradot_icall_Globals_EditorDefShortcut(const bradot_string *p_setting, const bradot_string *p_name, Key p_keycode, bool p_physical, bradot_variant *r_result) {
	String setting = *reinterpret_cast<const String *>(p_setting);
	String name = *reinterpret_cast<const String *>(p_name);
	Ref<Shortcut> result = ED_SHORTCUT(setting, name, p_keycode, p_physical);
	memnew_placement(r_result, Variant(result));
}

void bradot_icall_Globals_EditorGetShortcut(const bradot_string *p_setting, Ref<Shortcut> *r_result) {
	String setting = *reinterpret_cast<const String *>(p_setting);
	Ref<Shortcut> result = ED_GET_SHORTCUT(setting);
	memnew_placement(r_result, Variant(result));
}

void bradot_icall_Globals_EditorShortcutOverride(const bradot_string *p_setting, const bradot_string *p_feature, Key p_keycode, bool p_physical) {
	String setting = *reinterpret_cast<const String *>(p_setting);
	String feature = *reinterpret_cast<const String *>(p_feature);
	ED_SHORTCUT_OVERRIDE(setting, feature, p_keycode, p_physical);
}

void bradot_icall_Globals_TTR(const bradot_string *p_text, bradot_string *r_dest) {
	String text = *reinterpret_cast<const String *>(p_text);
	memnew_placement(r_dest, String(TTR(text)));
}

void bradot_icall_Utils_OS_GetPlatformName(bradot_string *r_dest) {
	String os_name = OS::get_singleton()->get_name();
	memnew_placement(r_dest, String(os_name));
}

bool bradot_icall_Utils_OS_UnixFileHasExecutableAccess(const bradot_string *p_file_path) {
#ifdef UNIX_ENABLED
	String file_path = *reinterpret_cast<const String *>(p_file_path);
	return access(file_path.utf8().get_data(), X_OK) == 0;
#else
	ERR_FAIL_V(false);
#endif
}

#ifdef __cplusplus
}
#endif

// The order in this array must match the declaration order of
// the methods in 'BradotTools/Internals/Internal.cs'.
static const void *unmanaged_callbacks[]{
	(void *)bradot_icall_BradotSharpDirs_ResMetadataDir,
	(void *)bradot_icall_BradotSharpDirs_MonoUserDir,
	(void *)bradot_icall_BradotSharpDirs_BuildLogsDirs,
	(void *)bradot_icall_BradotSharpDirs_DataEditorToolsDir,
	(void *)bradot_icall_BradotSharpDirs_CSharpProjectName,
	(void *)bradot_icall_EditorProgress_Create,
	(void *)bradot_icall_EditorProgress_Dispose,
	(void *)bradot_icall_EditorProgress_Step,
	(void *)bradot_icall_Internal_FullExportTemplatesDir,
	(void *)bradot_icall_Internal_IsMacOSAppBundleInstalled,
	(void *)bradot_icall_Internal_LipOCreateFile,
	(void *)bradot_icall_Internal_BradotIs32Bits,
	(void *)bradot_icall_Internal_BradotIsRealTDouble,
	(void *)bradot_icall_Internal_BradotMainIteration,
	(void *)bradot_icall_Internal_IsAssembliesReloadingNeeded,
	(void *)bradot_icall_Internal_ReloadAssemblies,
	(void *)bradot_icall_Internal_EditorDebuggerNodeReloadScripts,
	(void *)bradot_icall_Internal_ScriptEditorEdit,
	(void *)bradot_icall_Internal_EditorNodeShowScriptScreen,
	(void *)bradot_icall_Internal_EditorRunPlay,
	(void *)bradot_icall_Internal_EditorRunStop,
	(void *)bradot_icall_Internal_EditorPlugin_AddControlToEditorRunBar,
	(void *)bradot_icall_Internal_ScriptEditorDebugger_ReloadScripts,
	(void *)bradot_icall_Internal_CodeCompletionRequest,
	(void *)bradot_icall_Globals_EditorScale,
	(void *)bradot_icall_Globals_GlobalDef,
	(void *)bradot_icall_Globals_EditorDef,
	(void *)bradot_icall_Globals_EditorDefShortcut,
	(void *)bradot_icall_Globals_EditorGetShortcut,
	(void *)bradot_icall_Globals_EditorShortcutOverride,
	(void *)bradot_icall_Globals_TTR,
	(void *)bradot_icall_Utils_OS_GetPlatformName,
	(void *)bradot_icall_Utils_OS_UnixFileHasExecutableAccess,
};

const void **bradotsharp::get_editor_interop_funcs(int32_t &r_size) {
	r_size = sizeof(unmanaged_callbacks);
	return unmanaged_callbacks;
}
