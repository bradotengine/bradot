/**************************************************************************/
/*  register_editor_types.cpp                                             */
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

#include "register_editor_types.h"

#include "core/object/script_language.h"
#include "editor/debugger/debug_adapter/debug_adapter_server.h"
#include "editor/editor_command_palette.h"
#include "editor/editor_feature_profile.h"
#include "editor/editor_file_system.h"
#include "editor/editor_interface.h"
#include "editor/editor_node.h"
#include "editor/editor_paths.h"
#include "editor/editor_resource_picker.h"
#include "editor/editor_resource_preview.h"
#include "editor/editor_script.h"
#include "editor/editor_settings.h"
#include "editor/editor_string_names.h"
#include "editor/editor_translation_parser.h"
#include "editor/editor_undo_redo_manager.h"
#include "editor/export/editor_export_platform.h"
#include "editor/export/editor_export_platform_extension.h"
#include "editor/export/editor_export_platform_pc.h"
#include "editor/export/editor_export_plugin.h"
#include "editor/filesystem_dock.h"
#include "editor/gui/editor_file_dialog.h"
#include "editor/gui/editor_spin_slider.h"
#include "editor/import/3d/resource_importer_obj.h"
#include "editor/import/3d/resource_importer_scene.h"
#include "editor/import/editor_import_plugin.h"
#include "editor/import/resource_importer_bitmask.h"
#include "editor/import/resource_importer_bmfont.h"
#include "editor/import/resource_importer_csv_translation.h"
#include "editor/import/resource_importer_dynamic_font.h"
#include "editor/import/resource_importer_image.h"
#include "editor/import/resource_importer_imagefont.h"
#include "editor/import/resource_importer_layered_texture.h"
#include "editor/import/resource_importer_shader_file.h"
#include "editor/import/resource_importer_texture.h"
#include "editor/import/resource_importer_texture_atlas.h"
#include "editor/import/resource_importer_wav.h"
#include "editor/plugins/animation_tree_editor_plugin.h"
#include "editor/plugins/audio_stream_editor_plugin.h"
#include "editor/plugins/audio_stream_randomizer_editor_plugin.h"
#include "editor/plugins/bit_map_editor_plugin.h"
#include "editor/plugins/bone_map_editor_plugin.h"
#include "editor/plugins/camera_3d_editor_plugin.h"
#include "editor/plugins/cast_2d_editor_plugin.h"
#include "editor/plugins/collision_polygon_2d_editor_plugin.h"
#include "editor/plugins/collision_shape_2d_editor_plugin.h"
#include "editor/plugins/control_editor_plugin.h"
#include "editor/plugins/curve_editor_plugin.h"
#include "editor/plugins/editor_context_menu_plugin.h"
#include "editor/plugins/editor_debugger_plugin.h"
#include "editor/plugins/editor_resource_tooltip_plugins.h"
#include "editor/plugins/font_config_plugin.h"
#include "editor/plugins/gpu_particles_collision_sdf_editor_plugin.h"
#include "editor/plugins/gradient_editor_plugin.h"
#include "editor/plugins/gradient_texture_2d_editor_plugin.h"
#include "editor/plugins/input_event_editor_plugin.h"
#include "editor/plugins/light_occluder_2d_editor_plugin.h"
#include "editor/plugins/lightmap_gi_editor_plugin.h"
#include "editor/plugins/line_2d_editor_plugin.h"
#include "editor/plugins/material_editor_plugin.h"
#include "editor/plugins/mesh_editor_plugin.h"
#include "editor/plugins/mesh_instance_3d_editor_plugin.h"
#include "editor/plugins/mesh_library_editor_plugin.h"
#include "editor/plugins/multimesh_editor_plugin.h"
#include "editor/plugins/navigation_link_2d_editor_plugin.h"
#include "editor/plugins/navigation_obstacle_2d_editor_plugin.h"
#include "editor/plugins/navigation_obstacle_3d_editor_plugin.h"
#include "editor/plugins/navigation_polygon_editor_plugin.h"
#include "editor/plugins/node_3d_editor_gizmos.h"
#include "editor/plugins/occluder_instance_3d_editor_plugin.h"
#include "editor/plugins/packed_scene_editor_plugin.h"
#include "editor/plugins/parallax_background_editor_plugin.h"
#include "editor/plugins/particles_editor_plugin.h"
#include "editor/plugins/path_2d_editor_plugin.h"
#include "editor/plugins/path_3d_editor_plugin.h"
#include "editor/plugins/physical_bone_3d_editor_plugin.h"
#include "editor/plugins/polygon_2d_editor_plugin.h"
#include "editor/plugins/polygon_3d_editor_plugin.h"
#include "editor/plugins/resource_preloader_editor_plugin.h"
#include "editor/plugins/script_editor_plugin.h"
#include "editor/plugins/shader_editor_plugin.h"
#include "editor/plugins/shader_file_editor_plugin.h"
#include "editor/plugins/skeleton_2d_editor_plugin.h"
#include "editor/plugins/skeleton_3d_editor_plugin.h"
#include "editor/plugins/skeleton_ik_3d_editor_plugin.h"
#include "editor/plugins/sprite_2d_editor_plugin.h"
#include "editor/plugins/sprite_frames_editor_plugin.h"
#include "editor/plugins/style_box_editor_plugin.h"
#include "editor/plugins/sub_viewport_preview_editor_plugin.h"
#include "editor/plugins/texture_3d_editor_plugin.h"
#include "editor/plugins/texture_editor_plugin.h"
#include "editor/plugins/texture_layered_editor_plugin.h"
#include "editor/plugins/texture_region_editor_plugin.h"
#include "editor/plugins/theme_editor_plugin.h"
#include "editor/plugins/tiles/tiles_editor_plugin.h"
#include "editor/plugins/tool_button_editor_plugin.h"
#include "editor/plugins/version_control_editor_plugin.h"
#include "editor/plugins/visual_shader_editor_plugin.h"
#include "editor/plugins/voxel_gi_editor_plugin.h"
#include "editor/register_exporters.h"

void register_editor_types() {
	OS::get_singleton()->benchmark_begin_measure("Editor", "Register Types");

	ResourceLoader::set_timestamp_on_load(true);
	ResourceSaver::set_timestamp_on_save(true);

	EditorStringNames::create();

	BRREGISTER_CLASS(EditorPaths);
	BRREGISTER_CLASS(EditorPlugin);
	BRREGISTER_CLASS(EditorTranslationParserPlugin);
	BRREGISTER_CLASS(EditorImportPlugin);
	BRREGISTER_CLASS(EditorScript);
	BRREGISTER_CLASS(EditorSelection);
	BRREGISTER_CLASS(EditorFileDialog);
	BRREGISTER_CLASS(EditorSettings);
	BRREGISTER_CLASS(EditorNode3DGizmo);
	BRREGISTER_CLASS(EditorNode3DGizmoPlugin);
	BRREGISTER_ABSTRACT_CLASS(EditorResourcePreview);
	BRREGISTER_CLASS(EditorResourcePreviewGenerator);
	BRREGISTER_CLASS(EditorResourceTooltipPlugin);
	BRREGISTER_ABSTRACT_CLASS(EditorFileSystem);
	BRREGISTER_CLASS(EditorFileSystemDirectory);
	BRREGISTER_CLASS(EditorVCSInterface);
	BRREGISTER_ABSTRACT_CLASS(ScriptEditor);
	BRREGISTER_ABSTRACT_CLASS(ScriptEditorBase);
	BRREGISTER_CLASS(EditorSyntaxHighlighter);
	BRREGISTER_ABSTRACT_CLASS(EditorInterface);
	BRREGISTER_CLASS(EditorExportPlugin);
	BRREGISTER_ABSTRACT_CLASS(EditorExportPlatform);
	BRREGISTER_ABSTRACT_CLASS(EditorExportPlatformPC);
	BRREGISTER_CLASS(EditorExportPlatformExtension);
	BRREGISTER_ABSTRACT_CLASS(EditorExportPreset);

	register_exporter_types();

	BRREGISTER_CLASS(EditorResourceConversionPlugin);
	BRREGISTER_CLASS(EditorSceneFormatImporter);
	BRREGISTER_CLASS(EditorScenePostImportPlugin);
	BRREGISTER_CLASS(EditorInspector);
	BRREGISTER_CLASS(EditorInspectorPlugin);
	BRREGISTER_CLASS(EditorProperty);
	BRREGISTER_CLASS(ScriptCreateDialog);
	BRREGISTER_CLASS(EditorFeatureProfile);
	BRREGISTER_CLASS(EditorSpinSlider);
	BRREGISTER_CLASS(EditorResourcePicker);
	BRREGISTER_CLASS(EditorScriptPicker);
	BRREGISTER_ABSTRACT_CLASS(EditorUndoRedoManager);
	BRREGISTER_CLASS(EditorContextMenuPlugin);

	BRREGISTER_ABSTRACT_CLASS(FileSystemDock);
	BRREGISTER_VIRTUAL_CLASS(EditorFileSystemImportFormatSupportQuery);

	BRREGISTER_CLASS(EditorScenePostImport);
	BRREGISTER_CLASS(EditorCommandPalette);
	BRREGISTER_CLASS(EditorDebuggerPlugin);
	BRREGISTER_ABSTRACT_CLASS(EditorDebuggerSession);

	// Required to document import options in the class reference.
	BRREGISTER_CLASS(ResourceImporterBitMap);
	BRREGISTER_CLASS(ResourceImporterBMFont);
	BRREGISTER_CLASS(ResourceImporterCSVTranslation);
	BRREGISTER_CLASS(ResourceImporterDynamicFont);
	BRREGISTER_CLASS(ResourceImporterImage);
	BRREGISTER_CLASS(ResourceImporterImageFont);
	BRREGISTER_CLASS(ResourceImporterLayeredTexture);
	BRREGISTER_CLASS(ResourceImporterOBJ);
	BRREGISTER_CLASS(ResourceImporterScene);
	BRREGISTER_CLASS(ResourceImporterShaderFile);
	BRREGISTER_CLASS(ResourceImporterTexture);
	BRREGISTER_CLASS(ResourceImporterTextureAtlas);
	BRREGISTER_CLASS(ResourceImporterWAV);

	// This list is alphabetized, and plugins that depend on Node2D are in their own section below.
	EditorPlugins::add_by_type<AnimationTreeEditorPlugin>();
	EditorPlugins::add_by_type<AudioStreamEditorPlugin>();
	EditorPlugins::add_by_type<AudioStreamRandomizerEditorPlugin>();
	EditorPlugins::add_by_type<BitMapEditorPlugin>();
	EditorPlugins::add_by_type<BoneMapEditorPlugin>();
	EditorPlugins::add_by_type<Camera3DEditorPlugin>();
	EditorPlugins::add_by_type<ControlEditorPlugin>();
	EditorPlugins::add_by_type<CPUParticles3DEditorPlugin>();
	EditorPlugins::add_by_type<CurveEditorPlugin>();
	EditorPlugins::add_by_type<DebugAdapterServer>();
	EditorPlugins::add_by_type<FontEditorPlugin>();
	EditorPlugins::add_by_type<GPUParticles3DEditorPlugin>();
	EditorPlugins::add_by_type<GPUParticlesCollisionSDF3DEditorPlugin>();
	EditorPlugins::add_by_type<GradientEditorPlugin>();
	EditorPlugins::add_by_type<GradientTexture2DEditorPlugin>();
	EditorPlugins::add_by_type<InputEventEditorPlugin>();
	EditorPlugins::add_by_type<LightmapGIEditorPlugin>();
	EditorPlugins::add_by_type<MaterialEditorPlugin>();
	EditorPlugins::add_by_type<MeshEditorPlugin>();
	EditorPlugins::add_by_type<MeshInstance3DEditorPlugin>();
	EditorPlugins::add_by_type<MeshLibraryEditorPlugin>();
	EditorPlugins::add_by_type<MultiMeshEditorPlugin>();
	EditorPlugins::add_by_type<NavigationObstacle3DEditorPlugin>();
	EditorPlugins::add_by_type<OccluderInstance3DEditorPlugin>();
	EditorPlugins::add_by_type<PackedSceneEditorPlugin>();
	EditorPlugins::add_by_type<Path3DEditorPlugin>();
	EditorPlugins::add_by_type<PhysicalBone3DEditorPlugin>();
	EditorPlugins::add_by_type<Polygon3DEditorPlugin>();
	EditorPlugins::add_by_type<ResourcePreloaderEditorPlugin>();
	EditorPlugins::add_by_type<ShaderEditorPlugin>();
	EditorPlugins::add_by_type<ShaderFileEditorPlugin>();
	EditorPlugins::add_by_type<Skeleton3DEditorPlugin>();
	EditorPlugins::add_by_type<SkeletonIK3DEditorPlugin>();
	EditorPlugins::add_by_type<SpriteFramesEditorPlugin>();
	EditorPlugins::add_by_type<StyleBoxEditorPlugin>();
	EditorPlugins::add_by_type<SubViewportPreviewEditorPlugin>();
	EditorPlugins::add_by_type<Texture3DEditorPlugin>();
	EditorPlugins::add_by_type<TextureEditorPlugin>();
	EditorPlugins::add_by_type<TextureLayeredEditorPlugin>();
	EditorPlugins::add_by_type<TextureRegionEditorPlugin>();
	EditorPlugins::add_by_type<ThemeEditorPlugin>();
	EditorPlugins::add_by_type<ToolButtonEditorPlugin>();
	EditorPlugins::add_by_type<VoxelGIEditorPlugin>();

	// 2D
	EditorPlugins::add_by_type<CollisionPolygon2DEditorPlugin>();
	EditorPlugins::add_by_type<CollisionShape2DEditorPlugin>();
	EditorPlugins::add_by_type<CPUParticles2DEditorPlugin>();
	EditorPlugins::add_by_type<GPUParticles2DEditorPlugin>();
	EditorPlugins::add_by_type<LightOccluder2DEditorPlugin>();
	EditorPlugins::add_by_type<Line2DEditorPlugin>();
	EditorPlugins::add_by_type<NavigationLink2DEditorPlugin>();
	EditorPlugins::add_by_type<NavigationObstacle2DEditorPlugin>();
	EditorPlugins::add_by_type<NavigationPolygonEditorPlugin>();
	EditorPlugins::add_by_type<ParallaxBackgroundEditorPlugin>();
	EditorPlugins::add_by_type<Path2DEditorPlugin>();
	EditorPlugins::add_by_type<Polygon2DEditorPlugin>();
	EditorPlugins::add_by_type<Cast2DEditorPlugin>();
	EditorPlugins::add_by_type<Skeleton2DEditorPlugin>();
	EditorPlugins::add_by_type<Sprite2DEditorPlugin>();
	EditorPlugins::add_by_type<TileSetEditorPlugin>();
	EditorPlugins::add_by_type<TileMapEditorPlugin>();

	// For correct doc generation.
	GLOBAL_DEF("editor/run/main_run_args", "");

	GLOBAL_DEF(PropertyInfo(Variant::STRING, "editor/script/templates_search_path", PROPERTY_HINT_DIR), "res://script_templates");

	GLOBAL_DEF("editor/naming/default_signal_callback_name", "_on_{node_name}_{signal_name}");
	GLOBAL_DEF("editor/naming/default_signal_callback_to_self_name", "_on_{signal_name}");
	GLOBAL_DEF(PropertyInfo(Variant::INT, "editor/naming/scene_name_casing", PROPERTY_HINT_ENUM, "Auto,PascalCase,snake_case,kebab-case"), EditorNode::SCENE_NAME_CASING_SNAKE_CASE);
	GLOBAL_DEF(PropertyInfo(Variant::INT, "editor/naming/script_name_casing", PROPERTY_HINT_ENUM, "Auto,PascalCase,snake_case,kebab-case"), ScriptLanguage::SCRIPT_NAME_CASING_AUTO);

	GLOBAL_DEF("editor/import/reimport_missing_imported_files", true);
	GLOBAL_DEF("editor/import/use_multiple_threads", true);

	GLOBAL_DEF(PropertyInfo(Variant::INT, "editor/import/atlas_max_width", PROPERTY_HINT_RANGE, "128,8192,1,or_greater"), 2048);

	GLOBAL_DEF("editor/export/convert_text_resources_to_binary", true);

	GLOBAL_DEF("editor/version_control/plugin_name", "");
	GLOBAL_DEF("editor/version_control/autoload_on_startup", false);

	EditorInterface::create();
	Engine::Singleton ei_singleton = Engine::Singleton("EditorInterface", EditorInterface::get_singleton());
	ei_singleton.editor_only = true;
	Engine::get_singleton()->add_singleton(ei_singleton);

	// Required as BRExtensions can register docs at init time way before this
	// class is actually instantiated.
	EditorHelp::init_gdext_pointers();

	OS::get_singleton()->benchmark_end_measure("Editor", "Register Types");
}

void unregister_editor_types() {
	OS::get_singleton()->benchmark_begin_measure("Editor", "Unregister Types");

	EditorNode::cleanup();
	EditorInterface::free();

	if (EditorPaths::get_singleton()) {
		EditorPaths::free();
	}
	EditorStringNames::free();

	OS::get_singleton()->benchmark_end_measure("Editor", "Unregister Types");
}
