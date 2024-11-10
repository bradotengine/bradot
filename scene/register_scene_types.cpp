/**************************************************************************/
/*  register_scene_types.cpp                                              */
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

#include "register_scene_types.h"

#include "core/config/project_settings.h"
#include "core/object/class_db.h"
#include "core/os/os.h"
#include "scene/animation/animation_blend_space_1d.h"
#include "scene/animation/animation_blend_space_2d.h"
#include "scene/animation/animation_blend_tree.h"
#include "scene/animation/animation_mixer.h"
#include "scene/animation/animation_node_state_machine.h"
#include "scene/animation/animation_player.h"
#include "scene/animation/animation_tree.h"
#include "scene/animation/tween.h"
#include "scene/audio/audio_stream_player.h"
#include "scene/debugger/scene_debugger.h"
#include "scene/gui/aspect_ratio_container.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/center_container.h"
#include "scene/gui/check_box.h"
#include "scene/gui/check_button.h"
#include "scene/gui/code_edit.h"
#include "scene/gui/color_picker.h"
#include "scene/gui/color_rect.h"
#include "scene/gui/control.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/file_dialog.h"
#include "scene/gui/flow_container.h"
#include "scene/gui/graph_edit.h"
#include "scene/gui/graph_frame.h"
#include "scene/gui/graph_node.h"
#include "scene/gui/grid_container.h"
#include "scene/gui/item_list.h"
#include "scene/gui/label.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/link_button.h"
#include "scene/gui/margin_container.h"
#include "scene/gui/menu_bar.h"
#include "scene/gui/menu_button.h"
#include "scene/gui/nine_patch_rect.h"
#include "scene/gui/option_button.h"
#include "scene/gui/panel.h"
#include "scene/gui/panel_container.h"
#include "scene/gui/popup_menu.h"
#include "scene/gui/progress_bar.h"
#include "scene/gui/reference_rect.h"
#include "scene/gui/rich_text_effect.h"
#include "scene/gui/rich_text_label.h"
#include "scene/gui/scroll_bar.h"
#include "scene/gui/scroll_container.h"
#include "scene/gui/separator.h"
#include "scene/gui/slider.h"
#include "scene/gui/spin_box.h"
#include "scene/gui/split_container.h"
#include "scene/gui/subviewport_container.h"
#include "scene/gui/tab_bar.h"
#include "scene/gui/tab_container.h"
#include "scene/gui/text_edit.h"
#include "scene/gui/texture_button.h"
#include "scene/gui/texture_progress_bar.h"
#include "scene/gui/texture_rect.h"
#include "scene/gui/tree.h"
#include "scene/gui/video_stream_player.h"
#include "scene/main/canvas_item.h"
#include "scene/main/canvas_layer.h"
#include "scene/main/http_request.h"
#include "scene/main/instance_placeholder.h"
#include "scene/main/missing_node.h"
#include "scene/main/multiplayer_api.h"
#include "scene/main/resource_preloader.h"
#include "scene/main/scene_tree.h"
#include "scene/main/shader_globals_override.h"
#include "scene/main/status_indicator.h"
#include "scene/main/timer.h"
#include "scene/main/viewport.h"
#include "scene/main/window.h"
#include "scene/resources/animated_texture.h"
#include "scene/resources/animation_library.h"
#include "scene/resources/atlas_texture.h"
#include "scene/resources/audio_stream_polyphonic.h"
#include "scene/resources/audio_stream_wav.h"
#include "scene/resources/bit_map.h"
#include "scene/resources/bone_map.h"
#include "scene/resources/camera_attributes.h"
#include "scene/resources/camera_texture.h"
#include "scene/resources/compositor.h"
#include "scene/resources/compressed_texture.h"
#include "scene/resources/curve_texture.h"
#include "scene/resources/environment.h"
#include "scene/resources/external_texture.h"
#include "scene/resources/font.h"
#include "scene/resources/gradient.h"
#include "scene/resources/gradient_texture.h"
#include "scene/resources/image_texture.h"
#include "scene/resources/immediate_mesh.h"
#include "scene/resources/label_settings.h"
#include "scene/resources/material.h"
#include "scene/resources/mesh_data_tool.h"
#include "scene/resources/mesh_texture.h"
#include "scene/resources/multimesh.h"
#include "scene/resources/navigation_mesh.h"
#include "scene/resources/packed_scene.h"
#include "scene/resources/particle_process_material.h"
#include "scene/resources/physics_material.h"
#include "scene/resources/placeholder_textures.h"
#include "scene/resources/portable_compressed_texture.h"
#include "scene/resources/resource_format_text.h"
#include "scene/resources/shader_include.h"
#include "scene/resources/skeleton_profile.h"
#include "scene/resources/sky.h"
#include "scene/resources/style_box.h"
#include "scene/resources/style_box_flat.h"
#include "scene/resources/style_box_line.h"
#include "scene/resources/style_box_texture.h"
#include "scene/resources/surface_tool.h"
#include "scene/resources/syntax_highlighter.h"
#include "scene/resources/text_file.h"
#include "scene/resources/text_line.h"
#include "scene/resources/text_paragraph.h"
#include "scene/resources/texture.h"
#include "scene/resources/texture_rd.h"
#include "scene/resources/theme.h"
#include "scene/resources/video_stream.h"
#include "scene/resources/visual_shader.h"
#include "scene/resources/visual_shader_nodes.h"
#include "scene/resources/visual_shader_particle_nodes.h"
#include "scene/resources/visual_shader_sdf_nodes.h"
#include "scene/resources/world_2d.h"
#include "scene/theme/theme_db.h"

// 2D
#include "scene/2d/animated_sprite_2d.h"
#include "scene/2d/audio_listener_2d.h"
#include "scene/2d/audio_stream_player_2d.h"
#include "scene/2d/back_buffer_copy.h"
#include "scene/2d/camera_2d.h"
#include "scene/2d/canvas_group.h"
#include "scene/2d/canvas_modulate.h"
#include "scene/2d/cpu_particles_2d.h"
#include "scene/2d/gpu_particles_2d.h"
#include "scene/2d/light_2d.h"
#include "scene/2d/light_occluder_2d.h"
#include "scene/2d/line_2d.h"
#include "scene/2d/marker_2d.h"
#include "scene/2d/mesh_instance_2d.h"
#include "scene/2d/multimesh_instance_2d.h"
#include "scene/2d/navigation_agent_2d.h"
#include "scene/2d/navigation_link_2d.h"
#include "scene/2d/navigation_obstacle_2d.h"
#include "scene/2d/navigation_region_2d.h"
#include "scene/2d/parallax_2d.h"
#include "scene/2d/parallax_background.h"
#include "scene/2d/parallax_layer.h"
#include "scene/2d/path_2d.h"
#include "scene/2d/physics/animatable_body_2d.h"
#include "scene/2d/physics/area_2d.h"
#include "scene/2d/physics/character_body_2d.h"
#include "scene/2d/physics/collision_polygon_2d.h"
#include "scene/2d/physics/collision_shape_2d.h"
#include "scene/2d/physics/joints/damped_spring_joint_2d.h"
#include "scene/2d/physics/joints/groove_joint_2d.h"
#include "scene/2d/physics/joints/joint_2d.h"
#include "scene/2d/physics/joints/pin_joint_2d.h"
#include "scene/2d/physics/kinematic_collision_2d.h"
#include "scene/2d/physics/physical_bone_2d.h"
#include "scene/2d/physics/physics_body_2d.h"
#include "scene/2d/physics/ray_cast_2d.h"
#include "scene/2d/physics/rigid_body_2d.h"
#include "scene/2d/physics/shape_cast_2d.h"
#include "scene/2d/physics/static_body_2d.h"
#include "scene/2d/polygon_2d.h"
#include "scene/2d/remote_transform_2d.h"
#include "scene/2d/skeleton_2d.h"
#include "scene/2d/sprite_2d.h"
#include "scene/2d/tile_map.h"
#include "scene/2d/tile_map_layer.h"
#include "scene/2d/touch_screen_button.h"
#include "scene/2d/visible_on_screen_notifier_2d.h"
#include "scene/resources/2d/capsule_shape_2d.h"
#include "scene/resources/2d/circle_shape_2d.h"
#include "scene/resources/2d/concave_polygon_shape_2d.h"
#include "scene/resources/2d/convex_polygon_shape_2d.h"
#include "scene/resources/2d/navigation_mesh_source_geometry_data_2d.h"
#include "scene/resources/2d/navigation_polygon.h"
#include "scene/resources/2d/polygon_path_finder.h"
#include "scene/resources/2d/rectangle_shape_2d.h"
#include "scene/resources/2d/segment_shape_2d.h"
#include "scene/resources/2d/separation_ray_shape_2d.h"
#include "scene/resources/2d/skeleton/skeleton_modification_2d.h"
#include "scene/resources/2d/skeleton/skeleton_modification_2d_ccdik.h"
#include "scene/resources/2d/skeleton/skeleton_modification_2d_fabrik.h"
#include "scene/resources/2d/skeleton/skeleton_modification_2d_jiggle.h"
#include "scene/resources/2d/skeleton/skeleton_modification_2d_lookat.h"
#include "scene/resources/2d/skeleton/skeleton_modification_2d_physicalbones.h"
#include "scene/resources/2d/skeleton/skeleton_modification_2d_stackholder.h"
#include "scene/resources/2d/skeleton/skeleton_modification_2d_twoboneik.h"
#include "scene/resources/2d/skeleton/skeleton_modification_stack_2d.h"
#include "scene/resources/2d/tile_set.h"
#include "scene/resources/2d/world_boundary_shape_2d.h"

#ifndef _3D_DISABLED
#include "scene/3d/audio_listener_3d.h"
#include "scene/3d/audio_stream_player_3d.h"
#include "scene/3d/bone_attachment_3d.h"
#include "scene/3d/camera_3d.h"
#include "scene/3d/cpu_particles_3d.h"
#include "scene/3d/decal.h"
#include "scene/3d/fog_volume.h"
#include "scene/3d/gpu_particles_3d.h"
#include "scene/3d/gpu_particles_collision_3d.h"
#include "scene/3d/importer_mesh_instance_3d.h"
#include "scene/3d/label_3d.h"
#include "scene/3d/light_3d.h"
#include "scene/3d/lightmap_gi.h"
#include "scene/3d/lightmap_probe.h"
#include "scene/3d/marker_3d.h"
#include "scene/3d/mesh_instance_3d.h"
#include "scene/3d/multimesh_instance_3d.h"
#include "scene/3d/navigation_agent_3d.h"
#include "scene/3d/navigation_link_3d.h"
#include "scene/3d/navigation_obstacle_3d.h"
#include "scene/3d/navigation_region_3d.h"
#include "scene/3d/node_3d.h"
#include "scene/3d/occluder_instance_3d.h"
#include "scene/3d/path_3d.h"
#include "scene/3d/physical_bone_simulator_3d.h"
#include "scene/3d/physics/animatable_body_3d.h"
#include "scene/3d/physics/area_3d.h"
#include "scene/3d/physics/character_body_3d.h"
#include "scene/3d/physics/collision_polygon_3d.h"
#include "scene/3d/physics/collision_shape_3d.h"
#include "scene/3d/physics/joints/cone_twist_joint_3d.h"
#include "scene/3d/physics/joints/generic_6dof_joint_3d.h"
#include "scene/3d/physics/joints/hinge_joint_3d.h"
#include "scene/3d/physics/joints/joint_3d.h"
#include "scene/3d/physics/joints/pin_joint_3d.h"
#include "scene/3d/physics/joints/slider_joint_3d.h"
#include "scene/3d/physics/kinematic_collision_3d.h"
#include "scene/3d/physics/physical_bone_3d.h"
#include "scene/3d/physics/physics_body_3d.h"
#include "scene/3d/physics/ray_cast_3d.h"
#include "scene/3d/physics/rigid_body_3d.h"
#include "scene/3d/physics/shape_cast_3d.h"
#include "scene/3d/physics/spring_arm_3d.h"
#include "scene/3d/physics/static_body_3d.h"
#include "scene/3d/physics/vehicle_body_3d.h"
#include "scene/3d/reflection_probe.h"
#include "scene/3d/remote_transform_3d.h"
#include "scene/3d/skeleton_3d.h"
#include "scene/3d/skeleton_ik_3d.h"
#include "scene/3d/skeleton_modifier_3d.h"
#include "scene/3d/soft_body_3d.h"
#include "scene/3d/sprite_3d.h"
#include "scene/3d/visible_on_screen_notifier_3d.h"
#include "scene/3d/voxel_gi.h"
#include "scene/3d/world_environment.h"
#include "scene/3d/xr_body_modifier_3d.h"
#include "scene/3d/xr_face_modifier_3d.h"
#include "scene/3d/xr_hand_modifier_3d.h"
#include "scene/3d/xr_nodes.h"
#include "scene/animation/root_motion_view.h"
#include "scene/resources/3d/box_shape_3d.h"
#include "scene/resources/3d/capsule_shape_3d.h"
#include "scene/resources/3d/concave_polygon_shape_3d.h"
#include "scene/resources/3d/convex_polygon_shape_3d.h"
#include "scene/resources/3d/cylinder_shape_3d.h"
#include "scene/resources/3d/fog_material.h"
#include "scene/resources/3d/height_map_shape_3d.h"
#include "scene/resources/3d/importer_mesh.h"
#include "scene/resources/3d/mesh_library.h"
#include "scene/resources/3d/navigation_mesh_source_geometry_data_3d.h"
#include "scene/resources/3d/primitive_meshes.h"
#include "scene/resources/3d/separation_ray_shape_3d.h"
#include "scene/resources/3d/sky_material.h"
#include "scene/resources/3d/sphere_shape_3d.h"
#include "scene/resources/3d/world_3d.h"
#include "scene/resources/3d/world_boundary_shape_3d.h"
#endif // _3D_DISABLED

static Ref<ResourceFormatSaverText> resource_saver_text;
static Ref<ResourceFormatLoaderText> resource_loader_text;

static Ref<ResourceFormatLoaderCompressedTexture2D> resource_loader_stream_texture;
static Ref<ResourceFormatLoaderCompressedTextureLayered> resource_loader_texture_layered;
static Ref<ResourceFormatLoaderCompressedTexture3D> resource_loader_texture_3d;

static Ref<ResourceFormatSaverShader> resource_saver_shader;
static Ref<ResourceFormatLoaderShader> resource_loader_shader;

static Ref<ResourceFormatSaverShaderInclude> resource_saver_shader_include;
static Ref<ResourceFormatLoaderShaderInclude> resource_loader_shader_include;

void register_scene_types() {
	OS::get_singleton()->benchmark_begin_measure("Scene", "Register Types");

	SceneStringNames::create();

	OS::get_singleton()->yield(); // may take time to init

	Node::init_node_hrcr();

	resource_loader_stream_texture.instantiate();
	ResourceLoader::add_resource_format_loader(resource_loader_stream_texture);

	resource_loader_texture_layered.instantiate();
	ResourceLoader::add_resource_format_loader(resource_loader_texture_layered);

	resource_loader_texture_3d.instantiate();
	ResourceLoader::add_resource_format_loader(resource_loader_texture_3d);

	resource_saver_text.instantiate();
	ResourceSaver::add_resource_format_saver(resource_saver_text, true);

	resource_loader_text.instantiate();
	ResourceLoader::add_resource_format_loader(resource_loader_text, true);

	resource_saver_shader.instantiate();
	ResourceSaver::add_resource_format_saver(resource_saver_shader, true);

	resource_loader_shader.instantiate();
	ResourceLoader::add_resource_format_loader(resource_loader_shader, true);

	resource_saver_shader_include.instantiate();
	ResourceSaver::add_resource_format_saver(resource_saver_shader_include, true);

	resource_loader_shader_include.instantiate();
	ResourceLoader::add_resource_format_loader(resource_loader_shader_include, true);

	OS::get_singleton()->yield(); // may take time to init

	BRREGISTER_CLASS(Object);

	BRREGISTER_CLASS(Node);
	BRREGISTER_VIRTUAL_CLASS(MissingNode);
	BRREGISTER_ABSTRACT_CLASS(InstancePlaceholder);

	BRREGISTER_ABSTRACT_CLASS(Viewport);
	BRREGISTER_CLASS(SubViewport);
	BRREGISTER_CLASS(ViewportTexture);

	BRREGISTER_VIRTUAL_CLASS(CompositorEffect);

	BRREGISTER_ABSTRACT_CLASS(MultiplayerPeer);
	BRREGISTER_CLASS(MultiplayerPeerExtension);
	BRREGISTER_ABSTRACT_CLASS(MultiplayerAPI);
	BRREGISTER_CLASS(MultiplayerAPIExtension);

	BRREGISTER_CLASS(HTTPRequest);
	BRREGISTER_CLASS(Timer);
	BRREGISTER_CLASS(CanvasLayer);
	BRREGISTER_CLASS(CanvasModulate);
	BRREGISTER_CLASS(ResourcePreloader);
	BRREGISTER_CLASS(Window);

	BRREGISTER_CLASS(StatusIndicator);

	/* REGISTER GUI */

	BRREGISTER_CLASS(ButtonGroup);
	BRREGISTER_VIRTUAL_CLASS(BaseButton);

	OS::get_singleton()->yield(); // may take time to init

	BRREGISTER_CLASS(Control);
	BRREGISTER_CLASS(Button);
	BRREGISTER_CLASS(Label);
	BRREGISTER_ABSTRACT_CLASS(ScrollBar);
	BRREGISTER_CLASS(HScrollBar);
	BRREGISTER_CLASS(VScrollBar);
	BRREGISTER_CLASS(ProgressBar);
	BRREGISTER_ABSTRACT_CLASS(Slider);
	BRREGISTER_CLASS(HSlider);
	BRREGISTER_CLASS(VSlider);
	BRREGISTER_CLASS(Popup);
	BRREGISTER_CLASS(PopupPanel);
	BRREGISTER_CLASS(CheckBox);
	BRREGISTER_CLASS(CheckButton);
	BRREGISTER_CLASS(LinkButton);
	BRREGISTER_CLASS(Panel);
	BRREGISTER_VIRTUAL_CLASS(Range);

	OS::get_singleton()->yield(); // may take time to init

	BRREGISTER_CLASS(TextureRect);
	BRREGISTER_CLASS(ColorRect);
	BRREGISTER_CLASS(NinePatchRect);
	BRREGISTER_CLASS(ReferenceRect);
	BRREGISTER_CLASS(AspectRatioContainer);
	BRREGISTER_CLASS(TabContainer);
	BRREGISTER_CLASS(TabBar);
	BRREGISTER_ABSTRACT_CLASS(Separator);
	BRREGISTER_CLASS(HSeparator);
	BRREGISTER_CLASS(VSeparator);
	BRREGISTER_CLASS(TextureButton);
	BRREGISTER_CLASS(Container);
	BRREGISTER_CLASS(BoxContainer);
	BRREGISTER_CLASS(HBoxContainer);
	BRREGISTER_CLASS(VBoxContainer);
	BRREGISTER_CLASS(GridContainer);
	BRREGISTER_CLASS(CenterContainer);
	BRREGISTER_CLASS(ScrollContainer);
	BRREGISTER_CLASS(PanelContainer);
	BRREGISTER_CLASS(FlowContainer);
	BRREGISTER_CLASS(HFlowContainer);
	BRREGISTER_CLASS(VFlowContainer);
	BRREGISTER_CLASS(MarginContainer);

	OS::get_singleton()->yield(); // may take time to init

	BRREGISTER_CLASS(TextureProgressBar);
	BRREGISTER_CLASS(ItemList);

	BRREGISTER_CLASS(LineEdit);
	BRREGISTER_CLASS(VideoStreamPlayer);
	BRREGISTER_VIRTUAL_CLASS(VideoStreamPlayback);
	BRREGISTER_VIRTUAL_CLASS(VideoStream);

#ifndef ADVANCED_GUI_DISABLED
	BRREGISTER_CLASS(FileDialog);

	BRREGISTER_CLASS(PopupMenu);
	BRREGISTER_CLASS(Tree);

	BRREGISTER_CLASS(TextEdit);
	BRREGISTER_CLASS(CodeEdit);
	BRREGISTER_CLASS(SyntaxHighlighter);
	BRREGISTER_CLASS(CodeHighlighter);

	BRREGISTER_ABSTRACT_CLASS(TreeItem);
	BRREGISTER_CLASS(MenuBar);
	BRREGISTER_CLASS(MenuButton);
	BRREGISTER_CLASS(OptionButton);
	BRREGISTER_CLASS(SpinBox);
	BRREGISTER_CLASS(ColorPicker);
	BRREGISTER_CLASS(ColorPickerButton);
	BRREGISTER_CLASS(RichTextLabel);
	BRREGISTER_CLASS(RichTextEffect);
	BRREGISTER_CLASS(CharFXTransform);

	BRREGISTER_CLASS(AcceptDialog);
	BRREGISTER_CLASS(ConfirmationDialog);

	BRREGISTER_CLASS(SubViewportContainer);
	BRREGISTER_CLASS(SplitContainer);
	BRREGISTER_CLASS(HSplitContainer);
	BRREGISTER_CLASS(VSplitContainer);

	BRREGISTER_CLASS(GraphElement);
	BRREGISTER_CLASS(GraphNode);
	BRREGISTER_CLASS(GraphFrame);
	BRREGISTER_CLASS(GraphEdit);

	OS::get_singleton()->yield(); // may take time to init

	bool swap_cancel_ok = false;
	if (DisplayServer::get_singleton()) {
		swap_cancel_ok = GLOBAL_DEF_NOVAL("gui/common/swap_cancel_ok", bool(DisplayServer::get_singleton()->get_swap_cancel_ok()));
	}
	AcceptDialog::set_swap_cancel_ok(swap_cancel_ok);
#endif

	int root_dir = GLOBAL_GET("internationalization/rendering/root_node_layout_direction");
	Control::set_root_layout_direction(root_dir);
	Window::set_root_layout_direction(root_dir);

	/* REGISTER ANIMATION */
	BRREGISTER_CLASS(Tween);
	BRREGISTER_ABSTRACT_CLASS(Tweener);
	BRREGISTER_CLASS(PropertyTweener);
	BRREGISTER_CLASS(IntervalTweener);
	BRREGISTER_CLASS(CallbackTweener);
	BRREGISTER_CLASS(MethodTweener);

	BRREGISTER_ABSTRACT_CLASS(AnimationMixer);
	BRREGISTER_CLASS(AnimationPlayer);
	BRREGISTER_CLASS(AnimationTree);
	BRREGISTER_CLASS(AnimationNode);
	BRREGISTER_CLASS(AnimationRootNode);
	BRREGISTER_CLASS(AnimationNodeBlendTree);
	BRREGISTER_CLASS(AnimationNodeBlendSpace1D);
	BRREGISTER_CLASS(AnimationNodeBlendSpace2D);
	BRREGISTER_CLASS(AnimationNodeStateMachine);
	BRREGISTER_CLASS(AnimationNodeStateMachinePlayback);

	BRREGISTER_CLASS(AnimationNodeSync);
	BRREGISTER_CLASS(AnimationNodeStateMachineTransition);
	BRREGISTER_CLASS(AnimationNodeOutput);
	BRREGISTER_CLASS(AnimationNodeOneShot);
	BRREGISTER_CLASS(AnimationNodeAnimation);
	BRREGISTER_CLASS(AnimationNodeAdd2);
	BRREGISTER_CLASS(AnimationNodeAdd3);
	BRREGISTER_CLASS(AnimationNodeBlend2);
	BRREGISTER_CLASS(AnimationNodeBlend3);
	BRREGISTER_CLASS(AnimationNodeSub2);
	BRREGISTER_CLASS(AnimationNodeTimeScale);
	BRREGISTER_CLASS(AnimationNodeTimeSeek);
	BRREGISTER_CLASS(AnimationNodeTransition);

	BRREGISTER_CLASS(ShaderGlobalsOverride); // can be used in any shader

	OS::get_singleton()->yield(); // may take time to init

	/* REGISTER 3D */

#ifndef _3D_DISABLED
	BRREGISTER_CLASS(Node3D);
	BRREGISTER_ABSTRACT_CLASS(Node3DGizmo);
	BRREGISTER_CLASS(Skin);
	BRREGISTER_ABSTRACT_CLASS(SkinReference);
	BRREGISTER_CLASS(Skeleton3D);
	BRREGISTER_CLASS(ImporterMesh);
	BRREGISTER_CLASS(ImporterMeshInstance3D);
	BRREGISTER_VIRTUAL_CLASS(VisualInstance3D);
	BRREGISTER_VIRTUAL_CLASS(GeometryInstance3D);
	BRREGISTER_CLASS(Camera3D);
	BRREGISTER_CLASS(AudioListener3D);
	BRREGISTER_CLASS(XRCamera3D);
	BRREGISTER_CLASS(XRNode3D);
	BRREGISTER_CLASS(XRController3D);
	BRREGISTER_CLASS(XRAnchor3D);
	BRREGISTER_CLASS(XROrigin3D);
	BRREGISTER_CLASS(XRBodyModifier3D);
	BRREGISTER_CLASS(XRHandModifier3D);
	BRREGISTER_CLASS(XRFaceModifier3D);
	BRREGISTER_CLASS(MeshInstance3D);
	BRREGISTER_CLASS(OccluderInstance3D);
	BRREGISTER_ABSTRACT_CLASS(Occluder3D);
	BRREGISTER_CLASS(ArrayOccluder3D);
	BRREGISTER_CLASS(QuadOccluder3D);
	BRREGISTER_CLASS(BoxOccluder3D);
	BRREGISTER_CLASS(SphereOccluder3D);
	BRREGISTER_CLASS(PolygonOccluder3D);
	BRREGISTER_ABSTRACT_CLASS(SpriteBase3D);
	BRREGISTER_CLASS(Sprite3D);
	BRREGISTER_CLASS(AnimatedSprite3D);
	BRREGISTER_CLASS(Label3D);
	BRREGISTER_ABSTRACT_CLASS(Light3D);
	BRREGISTER_CLASS(DirectionalLight3D);
	BRREGISTER_CLASS(OmniLight3D);
	BRREGISTER_CLASS(SpotLight3D);
	BRREGISTER_CLASS(ReflectionProbe);
	BRREGISTER_CLASS(Decal);
	BRREGISTER_CLASS(VoxelGI);
	BRREGISTER_CLASS(VoxelGIData);
	BRREGISTER_CLASS(LightmapGI);
	BRREGISTER_CLASS(LightmapGIData);
	BRREGISTER_CLASS(LightmapProbe);
	BRREGISTER_ABSTRACT_CLASS(Lightmapper);
	BRREGISTER_CLASS(GPUParticles3D);
	BRREGISTER_ABSTRACT_CLASS(GPUParticlesCollision3D);
	BRREGISTER_CLASS(GPUParticlesCollisionBox3D);
	BRREGISTER_CLASS(GPUParticlesCollisionSphere3D);
	BRREGISTER_CLASS(GPUParticlesCollisionSDF3D);
	BRREGISTER_CLASS(GPUParticlesCollisionHeightField3D);
	BRREGISTER_ABSTRACT_CLASS(GPUParticlesAttractor3D);
	BRREGISTER_CLASS(GPUParticlesAttractorBox3D);
	BRREGISTER_CLASS(GPUParticlesAttractorSphere3D);
	BRREGISTER_CLASS(GPUParticlesAttractorVectorField3D);
	BRREGISTER_CLASS(CPUParticles3D);
	BRREGISTER_CLASS(Marker3D);
	BRREGISTER_CLASS(RootMotionView);
	BRREGISTER_VIRTUAL_CLASS(SkeletonModifier3D);

	OS::get_singleton()->yield(); // may take time to init

	BRREGISTER_ABSTRACT_CLASS(CollisionObject3D);
	BRREGISTER_ABSTRACT_CLASS(PhysicsBody3D);
	BRREGISTER_CLASS(StaticBody3D);
	BRREGISTER_CLASS(AnimatableBody3D);
	BRREGISTER_CLASS(RigidBody3D);
	BRREGISTER_CLASS(KinematicCollision3D);
	BRREGISTER_CLASS(CharacterBody3D);
	BRREGISTER_CLASS(SpringArm3D);

	BRREGISTER_CLASS(PhysicalBoneSimulator3D);
	BRREGISTER_CLASS(PhysicalBone3D);
	BRREGISTER_CLASS(SoftBody3D);

	BRREGISTER_CLASS(SkeletonIK3D);
	BRREGISTER_CLASS(BoneAttachment3D);

	BRREGISTER_CLASS(VehicleBody3D);
	BRREGISTER_CLASS(VehicleWheel3D);
	BRREGISTER_CLASS(Area3D);
	BRREGISTER_CLASS(CollisionShape3D);
	BRREGISTER_CLASS(CollisionPolygon3D);
	BRREGISTER_CLASS(RayCast3D);
	BRREGISTER_CLASS(ShapeCast3D);
	BRREGISTER_CLASS(MultiMeshInstance3D);

	BRREGISTER_CLASS(Curve3D);
	BRREGISTER_CLASS(Path3D);
	BRREGISTER_CLASS(PathFollow3D);
	BRREGISTER_CLASS(VisibleOnScreenNotifier3D);
	BRREGISTER_CLASS(VisibleOnScreenEnabler3D);
	BRREGISTER_CLASS(WorldEnvironment);
	BRREGISTER_CLASS(FogVolume);
	BRREGISTER_CLASS(FogMaterial);
	BRREGISTER_CLASS(RemoteTransform3D);

	BRREGISTER_ABSTRACT_CLASS(Joint3D);
	BRREGISTER_CLASS(PinJoint3D);
	BRREGISTER_CLASS(HingeJoint3D);
	BRREGISTER_CLASS(SliderJoint3D);
	BRREGISTER_CLASS(ConeTwistJoint3D);
	BRREGISTER_CLASS(Generic6DOFJoint3D);

	BRREGISTER_CLASS(NavigationRegion3D);
	BRREGISTER_CLASS(NavigationAgent3D);
	BRREGISTER_CLASS(NavigationObstacle3D);
	BRREGISTER_CLASS(NavigationLink3D);

	OS::get_singleton()->yield(); // may take time to init
#endif // _3D_DISABLED

	/* REGISTER SHADER */

	BRREGISTER_CLASS(Shader);
	BRREGISTER_CLASS(VisualShader);
	BRREGISTER_CLASS(ShaderInclude);
	BRREGISTER_ABSTRACT_CLASS(VisualShaderNode);
	BRREGISTER_CLASS(VisualShaderNodeCustom);
	BRREGISTER_CLASS(VisualShaderNodeInput);
	BRREGISTER_ABSTRACT_CLASS(VisualShaderNodeOutput);
	BRREGISTER_ABSTRACT_CLASS(VisualShaderNodeResizableBase);
	BRREGISTER_ABSTRACT_CLASS(VisualShaderNodeGroupBase);
	BRREGISTER_ABSTRACT_CLASS(VisualShaderNodeConstant);
	BRREGISTER_ABSTRACT_CLASS(VisualShaderNodeVectorBase);
	BRREGISTER_CLASS(VisualShaderNodeFrame);
#ifndef DISABLE_DEPRECATED
	BRREGISTER_CLASS(VisualShaderNodeComment); // Deprecated, just for compatibility.
#endif
	BRREGISTER_CLASS(VisualShaderNodeFloatConstant);
	BRREGISTER_CLASS(VisualShaderNodeIntConstant);
	BRREGISTER_CLASS(VisualShaderNodeUIntConstant);
	BRREGISTER_CLASS(VisualShaderNodeBooleanConstant);
	BRREGISTER_CLASS(VisualShaderNodeColorConstant);
	BRREGISTER_CLASS(VisualShaderNodeVec2Constant);
	BRREGISTER_CLASS(VisualShaderNodeVec3Constant);
	BRREGISTER_CLASS(VisualShaderNodeVec4Constant);
	BRREGISTER_CLASS(VisualShaderNodeTransformConstant);
	BRREGISTER_CLASS(VisualShaderNodeFloatOp);
	BRREGISTER_CLASS(VisualShaderNodeIntOp);
	BRREGISTER_CLASS(VisualShaderNodeUIntOp);
	BRREGISTER_CLASS(VisualShaderNodeVectorOp);
	BRREGISTER_CLASS(VisualShaderNodeColorOp);
	BRREGISTER_CLASS(VisualShaderNodeTransformOp);
	BRREGISTER_CLASS(VisualShaderNodeTransformVecMult);
	BRREGISTER_CLASS(VisualShaderNodeFloatFunc);
	BRREGISTER_CLASS(VisualShaderNodeIntFunc);
	BRREGISTER_CLASS(VisualShaderNodeUIntFunc);
	BRREGISTER_CLASS(VisualShaderNodeVectorFunc);
	BRREGISTER_CLASS(VisualShaderNodeColorFunc);
	BRREGISTER_CLASS(VisualShaderNodeTransformFunc);
	BRREGISTER_CLASS(VisualShaderNodeUVFunc);
	BRREGISTER_CLASS(VisualShaderNodeUVPolarCoord);
	BRREGISTER_CLASS(VisualShaderNodeDotProduct);
	BRREGISTER_CLASS(VisualShaderNodeVectorLen);
	BRREGISTER_CLASS(VisualShaderNodeDeterminant);
	BRREGISTER_CLASS(VisualShaderNodeDerivativeFunc);
	BRREGISTER_CLASS(VisualShaderNodeClamp);
	BRREGISTER_CLASS(VisualShaderNodeFaceForward);
	BRREGISTER_CLASS(VisualShaderNodeOuterProduct);
	BRREGISTER_CLASS(VisualShaderNodeSmoothStep);
	BRREGISTER_CLASS(VisualShaderNodeStep);
	BRREGISTER_CLASS(VisualShaderNodeVectorDistance);
	BRREGISTER_CLASS(VisualShaderNodeVectorRefract);
	BRREGISTER_CLASS(VisualShaderNodeMix);
	BRREGISTER_CLASS(VisualShaderNodeVectorCompose);
	BRREGISTER_CLASS(VisualShaderNodeTransformCompose);
	BRREGISTER_CLASS(VisualShaderNodeVectorDecompose);
	BRREGISTER_CLASS(VisualShaderNodeTransformDecompose);
	BRREGISTER_CLASS(VisualShaderNodeTexture);
	BRREGISTER_CLASS(VisualShaderNodeCurveTexture);
	BRREGISTER_CLASS(VisualShaderNodeCurveXYZTexture);
	BRREGISTER_ABSTRACT_CLASS(VisualShaderNodeSample3D);
	BRREGISTER_CLASS(VisualShaderNodeTexture2DArray);
	BRREGISTER_CLASS(VisualShaderNodeTexture3D);
	BRREGISTER_CLASS(VisualShaderNodeCubemap);
	BRREGISTER_ABSTRACT_CLASS(VisualShaderNodeParameter);
	BRREGISTER_CLASS(VisualShaderNodeParameterRef);
	BRREGISTER_CLASS(VisualShaderNodeFloatParameter);
	BRREGISTER_CLASS(VisualShaderNodeIntParameter);
	BRREGISTER_CLASS(VisualShaderNodeUIntParameter);
	BRREGISTER_CLASS(VisualShaderNodeBooleanParameter);
	BRREGISTER_CLASS(VisualShaderNodeColorParameter);
	BRREGISTER_CLASS(VisualShaderNodeVec2Parameter);
	BRREGISTER_CLASS(VisualShaderNodeVec3Parameter);
	BRREGISTER_CLASS(VisualShaderNodeVec4Parameter);
	BRREGISTER_CLASS(VisualShaderNodeTransformParameter);
	BRREGISTER_ABSTRACT_CLASS(VisualShaderNodeTextureParameter);
	BRREGISTER_CLASS(VisualShaderNodeTexture2DParameter);
	BRREGISTER_CLASS(VisualShaderNodeTextureParameterTriplanar);
	BRREGISTER_CLASS(VisualShaderNodeTexture2DArrayParameter);
	BRREGISTER_CLASS(VisualShaderNodeTexture3DParameter);
	BRREGISTER_CLASS(VisualShaderNodeCubemapParameter);
	BRREGISTER_CLASS(VisualShaderNodeLinearSceneDepth);
	BRREGISTER_CLASS(VisualShaderNodeWorldPositionFromDepth);
	BRREGISTER_CLASS(VisualShaderNodeScreenNormalWorldSpace);
	BRREGISTER_CLASS(VisualShaderNodeIf);
	BRREGISTER_CLASS(VisualShaderNodeSwitch);
	BRREGISTER_CLASS(VisualShaderNodeFresnel);
	BRREGISTER_CLASS(VisualShaderNodeExpression);
	BRREGISTER_CLASS(VisualShaderNodeGlobalExpression);
	BRREGISTER_CLASS(VisualShaderNodeIs);
	BRREGISTER_CLASS(VisualShaderNodeCompare);
	BRREGISTER_CLASS(VisualShaderNodeMultiplyAdd);
	BRREGISTER_CLASS(VisualShaderNodeBillboard);
	BRREGISTER_CLASS(VisualShaderNodeDistanceFade);
	BRREGISTER_CLASS(VisualShaderNodeProximityFade);
	BRREGISTER_CLASS(VisualShaderNodeRandomRange);
	BRREGISTER_CLASS(VisualShaderNodeRemap);
	BRREGISTER_CLASS(VisualShaderNodeRotationByAxis);
	BRREGISTER_ABSTRACT_CLASS(VisualShaderNodeVarying);
	BRREGISTER_CLASS(VisualShaderNodeVaryingSetter);
	BRREGISTER_CLASS(VisualShaderNodeVaryingGetter);
	BRREGISTER_CLASS(VisualShaderNodeReroute);

	BRREGISTER_CLASS(VisualShaderNodeSDFToScreenUV);
	BRREGISTER_CLASS(VisualShaderNodeScreenUVToSDF);
	BRREGISTER_CLASS(VisualShaderNodeTextureSDF);
	BRREGISTER_CLASS(VisualShaderNodeTextureSDFNormal);
	BRREGISTER_CLASS(VisualShaderNodeSDFRaymarch);

	BRREGISTER_CLASS(VisualShaderNodeParticleOutput);
	BRREGISTER_ABSTRACT_CLASS(VisualShaderNodeParticleEmitter);
	BRREGISTER_CLASS(VisualShaderNodeParticleSphereEmitter);
	BRREGISTER_CLASS(VisualShaderNodeParticleBoxEmitter);
	BRREGISTER_CLASS(VisualShaderNodeParticleRingEmitter);
	BRREGISTER_CLASS(VisualShaderNodeParticleMeshEmitter);
	BRREGISTER_CLASS(VisualShaderNodeParticleMultiplyByAxisAngle);
	BRREGISTER_CLASS(VisualShaderNodeParticleConeVelocity);
	BRREGISTER_CLASS(VisualShaderNodeParticleRandomness);
	BRREGISTER_CLASS(VisualShaderNodeParticleAccelerator);
	BRREGISTER_CLASS(VisualShaderNodeParticleEmit);

	BRREGISTER_VIRTUAL_CLASS(Material);
	BRREGISTER_CLASS(PlaceholderMaterial);
	BRREGISTER_CLASS(ShaderMaterial);
	BRREGISTER_ABSTRACT_CLASS(CanvasItem);
	BRREGISTER_CLASS(CanvasTexture);
	BRREGISTER_CLASS(CanvasItemMaterial);
	SceneTree::add_idle_callback(CanvasItemMaterial::flush_changes);
	CanvasItemMaterial::init_shaders();

	/* REGISTER 2D */

	BRREGISTER_CLASS(Node2D);
	BRREGISTER_CLASS(CanvasGroup);
	BRREGISTER_CLASS(CPUParticles2D);
	BRREGISTER_CLASS(GPUParticles2D);
	BRREGISTER_CLASS(Sprite2D);
	BRREGISTER_CLASS(SpriteFrames);
	BRREGISTER_CLASS(AnimatedSprite2D);
	BRREGISTER_CLASS(Marker2D);
	BRREGISTER_CLASS(Line2D);
	BRREGISTER_CLASS(MeshInstance2D);
	BRREGISTER_CLASS(MultiMeshInstance2D);
	BRREGISTER_ABSTRACT_CLASS(CollisionObject2D);
	BRREGISTER_ABSTRACT_CLASS(PhysicsBody2D);
	BRREGISTER_CLASS(StaticBody2D);
	BRREGISTER_CLASS(AnimatableBody2D);
	BRREGISTER_CLASS(RigidBody2D);
	BRREGISTER_CLASS(CharacterBody2D);
	BRREGISTER_CLASS(KinematicCollision2D);
	BRREGISTER_CLASS(Area2D);
	BRREGISTER_CLASS(CollisionShape2D);
	BRREGISTER_CLASS(CollisionPolygon2D);
	BRREGISTER_CLASS(RayCast2D);
	BRREGISTER_CLASS(ShapeCast2D);
	BRREGISTER_CLASS(VisibleOnScreenNotifier2D);
	BRREGISTER_CLASS(VisibleOnScreenEnabler2D);
	BRREGISTER_CLASS(Polygon2D);
	BRREGISTER_CLASS(Skeleton2D);
	BRREGISTER_CLASS(Bone2D);
	BRREGISTER_ABSTRACT_CLASS(Light2D);
	BRREGISTER_CLASS(PointLight2D);
	BRREGISTER_CLASS(DirectionalLight2D);
	BRREGISTER_CLASS(LightOccluder2D);
	BRREGISTER_CLASS(OccluderPolygon2D);
	BRREGISTER_CLASS(BackBufferCopy);

	OS::get_singleton()->yield(); // may take time to init

	BRREGISTER_CLASS(Camera2D);
	BRREGISTER_CLASS(AudioListener2D);
	BRREGISTER_ABSTRACT_CLASS(Joint2D);
	BRREGISTER_CLASS(PinJoint2D);
	BRREGISTER_CLASS(GrooveJoint2D);
	BRREGISTER_CLASS(DampedSpringJoint2D);
	BRREGISTER_CLASS(TileSet);
	BRREGISTER_ABSTRACT_CLASS(TileSetSource);
	BRREGISTER_CLASS(TileSetAtlasSource);
	BRREGISTER_CLASS(TileSetScenesCollectionSource);
	BRREGISTER_CLASS(TileMapPattern);
	BRREGISTER_CLASS(TileData);
	BRREGISTER_CLASS(TileMap);
	BRREGISTER_CLASS(TileMapLayer);
	BRREGISTER_CLASS(Parallax2D);
	BRREGISTER_CLASS(ParallaxBackground);
	BRREGISTER_CLASS(ParallaxLayer);
	BRREGISTER_CLASS(TouchScreenButton);
	BRREGISTER_CLASS(RemoteTransform2D);

	BRREGISTER_CLASS(SkeletonModificationStack2D);
	BRREGISTER_CLASS(SkeletonModification2D);
	BRREGISTER_CLASS(SkeletonModification2DLookAt);
	BRREGISTER_CLASS(SkeletonModification2DCCDIK);
	BRREGISTER_CLASS(SkeletonModification2DFABRIK);
	BRREGISTER_CLASS(SkeletonModification2DJiggle);
	BRREGISTER_CLASS(SkeletonModification2DTwoBoneIK);
	BRREGISTER_CLASS(SkeletonModification2DStackHolder);

	BRREGISTER_CLASS(PhysicalBone2D);
	BRREGISTER_CLASS(SkeletonModification2DPhysicalBones);

	OS::get_singleton()->yield(); // may take time to init

	/* REGISTER RESOURCES */

	BRREGISTER_ABSTRACT_CLASS(Shader);
	BRREGISTER_CLASS(ParticleProcessMaterial);
	SceneTree::add_idle_callback(ParticleProcessMaterial::flush_changes);
	ParticleProcessMaterial::init_shaders();

	BRREGISTER_VIRTUAL_CLASS(Mesh);
	BRREGISTER_CLASS(MeshConvexDecompositionSettings);
	BRREGISTER_CLASS(ArrayMesh);
	BRREGISTER_CLASS(PlaceholderMesh);
	BRREGISTER_CLASS(ImmediateMesh);
	BRREGISTER_CLASS(MultiMesh);
	BRREGISTER_CLASS(SurfaceTool);
	BRREGISTER_CLASS(MeshDataTool);

#ifndef _3D_DISABLED
	BRREGISTER_CLASS(AudioStreamPlayer3D);
	BRREGISTER_VIRTUAL_CLASS(PrimitiveMesh);
	BRREGISTER_CLASS(BoxMesh);
	BRREGISTER_CLASS(CapsuleMesh);
	BRREGISTER_CLASS(CylinderMesh);
	BRREGISTER_CLASS(PlaneMesh);
	BRREGISTER_CLASS(PrismMesh);
	BRREGISTER_CLASS(QuadMesh);
	BRREGISTER_CLASS(SphereMesh);
	BRREGISTER_CLASS(TextMesh);
	BRREGISTER_CLASS(TorusMesh);
	BRREGISTER_CLASS(TubeTrailMesh);
	BRREGISTER_CLASS(RibbonTrailMesh);
	BRREGISTER_CLASS(PointMesh);
	BRREGISTER_ABSTRACT_CLASS(BaseMaterial3D);
	BRREGISTER_CLASS(StandardMaterial3D);
	BRREGISTER_CLASS(ORMMaterial3D);
	BRREGISTER_CLASS(ProceduralSkyMaterial);
	BRREGISTER_CLASS(PanoramaSkyMaterial);
	BRREGISTER_CLASS(PhysicalSkyMaterial);
	BaseMaterial3D::init_shaders();

	BRREGISTER_CLASS(MeshLibrary);
	BRREGISTER_CLASS(NavigationMeshSourceGeometryData3D);

	OS::get_singleton()->yield(); // may take time to init

	BRREGISTER_ABSTRACT_CLASS(Shape3D);
	BRREGISTER_CLASS(SeparationRayShape3D);
	BRREGISTER_CLASS(SphereShape3D);
	BRREGISTER_CLASS(BoxShape3D);
	BRREGISTER_CLASS(CapsuleShape3D);
	BRREGISTER_CLASS(CylinderShape3D);
	BRREGISTER_CLASS(HeightMapShape3D);
	BRREGISTER_CLASS(WorldBoundaryShape3D);
	BRREGISTER_CLASS(ConvexPolygonShape3D);
	BRREGISTER_CLASS(ConcavePolygonShape3D);
	BRREGISTER_CLASS(World3D);

	OS::get_singleton()->yield(); // may take time to init
#endif // _3D_DISABLED

	BRREGISTER_CLASS(PhysicsMaterial);
	BRREGISTER_CLASS(Compositor);
	BRREGISTER_CLASS(Environment);
	BRREGISTER_VIRTUAL_CLASS(CameraAttributes);
	BRREGISTER_CLASS(CameraAttributesPhysical);
	BRREGISTER_CLASS(CameraAttributesPractical);
	BRREGISTER_CLASS(World2D);
	BRREGISTER_VIRTUAL_CLASS(Texture);
	BRREGISTER_VIRTUAL_CLASS(Texture2D);
	BRREGISTER_CLASS(Sky);
	BRREGISTER_CLASS(CompressedTexture2D);
	BRREGISTER_CLASS(PortableCompressedTexture2D);
	BRREGISTER_CLASS(ImageTexture);
	BRREGISTER_CLASS(AtlasTexture);
	BRREGISTER_CLASS(MeshTexture);
	BRREGISTER_CLASS(CurveTexture);
	BRREGISTER_CLASS(CurveXYZTexture);
	BRREGISTER_CLASS(GradientTexture1D);
	BRREGISTER_CLASS(GradientTexture2D);
	BRREGISTER_CLASS(AnimatedTexture);
	BRREGISTER_CLASS(CameraTexture);
	BRREGISTER_CLASS(ExternalTexture);
	BRREGISTER_VIRTUAL_CLASS(TextureLayered);
	BRREGISTER_ABSTRACT_CLASS(ImageTextureLayered);
	BRREGISTER_VIRTUAL_CLASS(Texture3D);
	BRREGISTER_CLASS(ImageTexture3D);
	BRREGISTER_CLASS(CompressedTexture3D);
	BRREGISTER_CLASS(Cubemap);
	BRREGISTER_CLASS(CubemapArray);
	BRREGISTER_CLASS(Texture2DArray);
	BRREGISTER_ABSTRACT_CLASS(CompressedTextureLayered);
	BRREGISTER_CLASS(CompressedCubemap);
	BRREGISTER_CLASS(CompressedCubemapArray);
	BRREGISTER_CLASS(CompressedTexture2DArray);
	BRREGISTER_CLASS(PlaceholderTexture2D);
	BRREGISTER_CLASS(PlaceholderTexture3D);
	BRREGISTER_ABSTRACT_CLASS(PlaceholderTextureLayered);
	BRREGISTER_CLASS(PlaceholderTexture2DArray);
	BRREGISTER_CLASS(PlaceholderCubemap);
	BRREGISTER_CLASS(PlaceholderCubemapArray);

	// These classes are part of renderer_rd
	BRREGISTER_CLASS(Texture2DRD);
	BRREGISTER_ABSTRACT_CLASS(TextureLayeredRD);
	BRREGISTER_CLASS(Texture2DArrayRD);
	BRREGISTER_CLASS(TextureCubemapRD);
	BRREGISTER_CLASS(TextureCubemapArrayRD);
	BRREGISTER_CLASS(Texture3DRD);

	BRREGISTER_CLASS(Animation);
	BRREGISTER_CLASS(AnimationLibrary);

	BRREGISTER_ABSTRACT_CLASS(Font);
	BRREGISTER_CLASS(FontFile);
	BRREGISTER_CLASS(FontVariation);
	BRREGISTER_CLASS(SystemFont);

	BRREGISTER_CLASS(Curve);

	BRREGISTER_CLASS(LabelSettings);

	BRREGISTER_CLASS(TextLine);
	BRREGISTER_CLASS(TextParagraph);

	BRREGISTER_VIRTUAL_CLASS(StyleBox);
	BRREGISTER_CLASS(StyleBoxEmpty);
	BRREGISTER_CLASS(StyleBoxTexture);
	BRREGISTER_CLASS(StyleBoxFlat);
	BRREGISTER_CLASS(StyleBoxLine);
	BRREGISTER_CLASS(Theme);

	BRREGISTER_CLASS(BitMap);
	BRREGISTER_CLASS(Gradient);

	BRREGISTER_CLASS(SkeletonProfile);
	BRREGISTER_CLASS(SkeletonProfileHumanoid);
	BRREGISTER_CLASS(BoneMap);

	OS::get_singleton()->yield(); // may take time to init

	BRREGISTER_CLASS(AudioStreamPlayer);
	BRREGISTER_CLASS(AudioStreamWAV);
	BRREGISTER_CLASS(AudioStreamPolyphonic);
	BRREGISTER_ABSTRACT_CLASS(AudioStreamPlaybackPolyphonic);

	OS::get_singleton()->yield(); // may take time to init

	BRREGISTER_CLASS(AudioStreamPlayer2D);
	BRREGISTER_ABSTRACT_CLASS(Shape2D);
	BRREGISTER_CLASS(WorldBoundaryShape2D);
	BRREGISTER_CLASS(SegmentShape2D);
	BRREGISTER_CLASS(SeparationRayShape2D);
	BRREGISTER_CLASS(CircleShape2D);
	BRREGISTER_CLASS(RectangleShape2D);
	BRREGISTER_CLASS(CapsuleShape2D);
	BRREGISTER_CLASS(ConvexPolygonShape2D);
	BRREGISTER_CLASS(ConcavePolygonShape2D);
	BRREGISTER_CLASS(Curve2D);
	BRREGISTER_CLASS(Path2D);
	BRREGISTER_CLASS(PathFollow2D);
	BRREGISTER_CLASS(PolygonPathFinder);

	BRREGISTER_CLASS(NavigationMesh);
	BRREGISTER_CLASS(NavigationMeshSourceGeometryData2D);
	BRREGISTER_CLASS(NavigationPolygon);
	BRREGISTER_CLASS(NavigationRegion2D);
	BRREGISTER_CLASS(NavigationAgent2D);
	BRREGISTER_CLASS(NavigationObstacle2D);
	BRREGISTER_CLASS(NavigationLink2D);

	OS::get_singleton()->yield(); // may take time to init

	BRREGISTER_ABSTRACT_CLASS(SceneState);
	BRREGISTER_CLASS(PackedScene);

	BRREGISTER_CLASS(SceneTree);
	BRREGISTER_ABSTRACT_CLASS(SceneTreeTimer); // sorry, you can't create it

#ifndef DISABLE_DEPRECATED
	// Dropped in 4.0, near approximation.
	ClassDB::add_compatibility_class("AnimationTreePlayer", "AnimationTree");
	ClassDB::add_compatibility_class("BakedLightmap", "LightmapGI");
	ClassDB::add_compatibility_class("BakedLightmapData", "LightmapGIData");
	ClassDB::add_compatibility_class("BitmapFont", "FontFile");
	ClassDB::add_compatibility_class("DynamicFont", "FontFile");
	ClassDB::add_compatibility_class("DynamicFontData", "FontFile");
	ClassDB::add_compatibility_class("Navigation3D", "Node3D");
	ClassDB::add_compatibility_class("Navigation2D", "Node2D");
	ClassDB::add_compatibility_class("OpenSimplexNoise", "FastNoiseLite");
	ClassDB::add_compatibility_class("ProximityGroup", "Node3D");
	ClassDB::add_compatibility_class("ToolButton", "Button");
	ClassDB::add_compatibility_class("YSort", "Node2D");
	// Portal and room occlusion was replaced by raster occlusion (OccluderInstance3D node).
	ClassDB::add_compatibility_class("Portal", "Node3D");
	ClassDB::add_compatibility_class("Room", "Node3D");
	ClassDB::add_compatibility_class("RoomManager", "Node3D");
	ClassDB::add_compatibility_class("RoomGroup", "Node3D");
	ClassDB::add_compatibility_class("Occluder", "Node3D");
	// The OccluderShapeSphere resource (used in the old Occluder node) is not present anymore.
	ClassDB::add_compatibility_class("OccluderShapeSphere", "Resource");

	// Renamed in 4.0.
	// Keep alphabetical ordering to easily locate classes and avoid duplicates.
	ClassDB::add_compatibility_class("AnimatedSprite", "AnimatedSprite2D");
	ClassDB::add_compatibility_class("Area", "Area3D");
	ClassDB::add_compatibility_class("ARVRCamera", "XRCamera3D");
	ClassDB::add_compatibility_class("ARVRController", "XRController3D");
	ClassDB::add_compatibility_class("ARVRAnchor", "XRAnchor3D");
	ClassDB::add_compatibility_class("ARVRInterface", "XRInterface");
	ClassDB::add_compatibility_class("ARVROrigin", "XROrigin3D");
	ClassDB::add_compatibility_class("ARVRPositionalTracker", "XRPositionalTracker");
	ClassDB::add_compatibility_class("ARVRServer", "XRServer");
	ClassDB::add_compatibility_class("AStar", "AStar3D");
	ClassDB::add_compatibility_class("BoneAttachment", "BoneAttachment3D");
	ClassDB::add_compatibility_class("BoxShape", "BoxShape3D");
	ClassDB::add_compatibility_class("Camera", "Camera3D");
	ClassDB::add_compatibility_class("CapsuleShape", "CapsuleShape3D");
	ClassDB::add_compatibility_class("ClippedCamera", "ClippedCamera3D");
	ClassDB::add_compatibility_class("CollisionObject", "CollisionObject3D");
	ClassDB::add_compatibility_class("CollisionPolygon", "CollisionPolygon3D");
	ClassDB::add_compatibility_class("CollisionShape", "CollisionShape3D");
	ClassDB::add_compatibility_class("ConcavePolygonShape", "ConcavePolygonShape3D");
	ClassDB::add_compatibility_class("ConeTwistJoint", "ConeTwistJoint3D");
	ClassDB::add_compatibility_class("ConvexPolygonShape", "ConvexPolygonShape3D");
	ClassDB::add_compatibility_class("CPUParticles", "CPUParticles3D");
	ClassDB::add_compatibility_class("CSGBox", "CSGBox3D");
	ClassDB::add_compatibility_class("CSGCombiner", "CSGCombiner3D");
	ClassDB::add_compatibility_class("CSGCylinder", "CSGCylinder3D");
	ClassDB::add_compatibility_class("CSGMesh", "CSGMesh3D");
	ClassDB::add_compatibility_class("CSGPolygon", "CSGPolygon3D");
	ClassDB::add_compatibility_class("CSGPrimitive", "CSGPrimitive3D");
	ClassDB::add_compatibility_class("CSGShape", "CSGShape3D");
	ClassDB::add_compatibility_class("CSGSphere", "CSGSphere3D");
	ClassDB::add_compatibility_class("CSGTorus", "CSGTorus3D");
	ClassDB::add_compatibility_class("CubeMesh", "BoxMesh");
	ClassDB::add_compatibility_class("CylinderShape", "CylinderShape3D");
	ClassDB::add_compatibility_class("DirectionalLight", "DirectionalLight3D");
	ClassDB::add_compatibility_class("EditorSpatialGizmo", "EditorNode3DGizmo");
	ClassDB::add_compatibility_class("EditorSpatialGizmoPlugin", "EditorNode3DGizmoPlugin");
	ClassDB::add_compatibility_class("Generic6DOFJoint", "Generic6DOFJoint3D");
	ClassDB::add_compatibility_class("GIProbe", "VoxelGI");
	ClassDB::add_compatibility_class("GIProbeData", "VoxelGIData");
	ClassDB::add_compatibility_class("GradientTexture", "GradientTexture1D");
	ClassDB::add_compatibility_class("HeightMapShape", "HeightMapShape3D");
	ClassDB::add_compatibility_class("HingeJoint", "HingeJoint3D");
	ClassDB::add_compatibility_class("Joint", "Joint3D");
	ClassDB::add_compatibility_class("KinematicBody", "CharacterBody3D");
	ClassDB::add_compatibility_class("KinematicBody2D", "CharacterBody2D");
	ClassDB::add_compatibility_class("KinematicCollision", "KinematicCollision3D");
	ClassDB::add_compatibility_class("Light", "Light3D");
	ClassDB::add_compatibility_class("Light2D", "PointLight2D");
	ClassDB::add_compatibility_class("LineShape2D", "WorldBoundaryShape2D");
	ClassDB::add_compatibility_class("Listener", "AudioListener3D");
	ClassDB::add_compatibility_class("MeshInstance", "MeshInstance3D");
	ClassDB::add_compatibility_class("MultiMeshInstance", "MultiMeshInstance3D");
	ClassDB::add_compatibility_class("NavigationAgent", "NavigationAgent3D");
	ClassDB::add_compatibility_class("NavigationMeshInstance", "NavigationRegion3D");
	ClassDB::add_compatibility_class("NavigationObstacle", "NavigationObstacle3D");
	ClassDB::add_compatibility_class("NavigationPolygonInstance", "NavigationRegion2D");
	ClassDB::add_compatibility_class("NavigationRegion", "NavigationRegion3D");
	ClassDB::add_compatibility_class("Navigation2DServer", "NavigationServer2D");
	ClassDB::add_compatibility_class("NavigationServer", "NavigationServer3D");
	ClassDB::add_compatibility_class("OmniLight", "OmniLight3D");
	ClassDB::add_compatibility_class("PanoramaSky", "Sky");
	ClassDB::add_compatibility_class("Particles", "GPUParticles3D");
	ClassDB::add_compatibility_class("Particles2D", "GPUParticles2D");
	ClassDB::add_compatibility_class("ParticlesMaterial", "ParticleProcessMaterial");
	ClassDB::add_compatibility_class("Path", "Path3D");
	ClassDB::add_compatibility_class("PathFollow", "PathFollow3D");
	ClassDB::add_compatibility_class("PhysicalBone", "PhysicalBone3D");
	ClassDB::add_compatibility_class("Physics2DDirectBodyState", "PhysicsDirectBodyState2D");
	ClassDB::add_compatibility_class("Physics2DDirectSpaceState", "PhysicsDirectSpaceState2D");
	ClassDB::add_compatibility_class("Physics2DServer", "PhysicsServer2D");
	ClassDB::add_compatibility_class("Physics2DShapeQueryParameters", "PhysicsShapeQueryParameters2D");
	ClassDB::add_compatibility_class("Physics2DTestMotionResult", "PhysicsTestMotionResult2D");
	ClassDB::add_compatibility_class("PhysicsBody", "PhysicsBody3D");
	ClassDB::add_compatibility_class("PhysicsDirectBodyState", "PhysicsDirectBodyState3D");
	ClassDB::add_compatibility_class("PhysicsDirectSpaceState", "PhysicsDirectSpaceState3D");
	ClassDB::add_compatibility_class("PhysicsServer", "PhysicsServer3D");
	ClassDB::add_compatibility_class("PhysicsShapeQueryParameters", "PhysicsShapeQueryParameters3D");
	ClassDB::add_compatibility_class("PinJoint", "PinJoint3D");
	ClassDB::add_compatibility_class("PlaneShape", "WorldBoundaryShape3D");
	ClassDB::add_compatibility_class("Position2D", "Marker2D");
	ClassDB::add_compatibility_class("Position3D", "Marker3D");
	ClassDB::add_compatibility_class("ProceduralSky", "Sky");
	ClassDB::add_compatibility_class("RayCast", "RayCast3D");
	ClassDB::add_compatibility_class("RayShape", "SeparationRayShape3D");
	ClassDB::add_compatibility_class("RayShape2D", "SeparationRayShape2D");
	ClassDB::add_compatibility_class("RemoteTransform", "RemoteTransform3D");
	ClassDB::add_compatibility_class("RigidBody", "RigidBody3D");
	ClassDB::add_compatibility_class("RigidDynamicBody2D", "RigidBody2D");
	ClassDB::add_compatibility_class("RigidDynamicBody3D", "RigidBody3D");
	ClassDB::add_compatibility_class("Shape", "Shape3D");
	ClassDB::add_compatibility_class("ShortCut", "Shortcut");
	ClassDB::add_compatibility_class("Skeleton", "Skeleton3D");
	ClassDB::add_compatibility_class("SkeletonIK", "SkeletonIK3D");
	ClassDB::add_compatibility_class("SliderJoint", "SliderJoint3D");
	ClassDB::add_compatibility_class("SoftBody", "SoftBody3D");
	ClassDB::add_compatibility_class("SoftDynamicBody3D", "SoftBody3D");
	ClassDB::add_compatibility_class("Spatial", "Node3D");
	ClassDB::add_compatibility_class("SpatialGizmo", "Node3DGizmo");
	ClassDB::add_compatibility_class("SpatialMaterial", "StandardMaterial3D");
	ClassDB::add_compatibility_class("SphereShape", "SphereShape3D");
	ClassDB::add_compatibility_class("SpotLight", "SpotLight3D");
	ClassDB::add_compatibility_class("SpringArm", "SpringArm3D");
	ClassDB::add_compatibility_class("Sprite", "Sprite2D");
	ClassDB::add_compatibility_class("StaticBody", "StaticBody3D");
	ClassDB::add_compatibility_class("StreamTexture", "CompressedTexture2D");
	ClassDB::add_compatibility_class("TextureProgress", "TextureProgressBar");
	ClassDB::add_compatibility_class("VehicleBody", "VehicleBody3D");
	ClassDB::add_compatibility_class("VehicleWheel", "VehicleWheel3D");
	ClassDB::add_compatibility_class("VideoPlayer", "VideoStreamPlayer");
	ClassDB::add_compatibility_class("ViewportContainer", "SubViewportContainer");
	ClassDB::add_compatibility_class("Viewport", "SubViewport");
	ClassDB::add_compatibility_class("VisibilityEnabler", "VisibleOnScreenEnabler3D");
	ClassDB::add_compatibility_class("VisibilityNotifier", "VisibleOnScreenNotifier3D");
	ClassDB::add_compatibility_class("VisibilityNotifier2D", "VisibleOnScreenNotifier2D");
	ClassDB::add_compatibility_class("VisibilityNotifier3D", "VisibleOnScreenNotifier3D");
	ClassDB::add_compatibility_class("VisualServer", "RenderingServer");
	ClassDB::add_compatibility_class("World", "World3D");

	// VisualShader classes.
	ClassDB::add_compatibility_class("VisualShaderNodeScalarConstant", "VisualShaderNodeFloatConstant");
	ClassDB::add_compatibility_class("VisualShaderNodeScalarFunc", "VisualShaderNodeFloatFunc");
	ClassDB::add_compatibility_class("VisualShaderNodeScalarOp", "VisualShaderNodeFloatOp");
	ClassDB::add_compatibility_class("VisualShaderNodeScalarClamp", "VisualShaderNodeClamp");
	ClassDB::add_compatibility_class("VisualShaderNodeVectorClamp", "VisualShaderNodeClamp");
	ClassDB::add_compatibility_class("VisualShaderNodeScalarInterp", "VisualShaderNodeMix");
	ClassDB::add_compatibility_class("VisualShaderNodeVectorInterp", "VisualShaderNodeMix");
	ClassDB::add_compatibility_class("VisualShaderNodeVectorScalarMix", "VisualShaderNodeMix");
	ClassDB::add_compatibility_class("VisualShaderNodeScalarSmoothStep", "VisualShaderNodeSmoothStep");
	ClassDB::add_compatibility_class("VisualShaderNodeVectorSmoothStep", "VisualShaderNodeSmoothStep");
	ClassDB::add_compatibility_class("VisualShaderNodeVectorScalarSmoothStep", "VisualShaderNodeSmoothStep");
	ClassDB::add_compatibility_class("VisualShaderNodeVectorScalarStep", "VisualShaderNodeStep");
	ClassDB::add_compatibility_class("VisualShaderNodeScalarSwitch", "VisualShaderNodeSwitch");
	ClassDB::add_compatibility_class("VisualShaderNodeScalarTransformMult", "VisualShaderNodeTransformOp");
	ClassDB::add_compatibility_class("VisualShaderNodeScalarDerivativeFunc", "VisualShaderNodeDerivativeFunc");
	ClassDB::add_compatibility_class("VisualShaderNodeVectorDerivativeFunc", "VisualShaderNodeDerivativeFunc");

	ClassDB::add_compatibility_class("VisualShaderNodeBooleanUniform", "VisualShaderNodeBooleanParameter");
	ClassDB::add_compatibility_class("VisualShaderNodeColorUniform", "VisualShaderNodeColorParameter");
	ClassDB::add_compatibility_class("VisualShaderNodeScalarUniform", "VisualShaderNodeFloatParameter");
	ClassDB::add_compatibility_class("VisualShaderNodeCubeMapUniform", "VisualShaderNodeCubeMapParameter");
	ClassDB::add_compatibility_class("VisualShaderNodeTextureUniform", "VisualShaderNodeTexture2DParameter");
	ClassDB::add_compatibility_class("VisualShaderNodeTextureUniformTriplanar", "VisualShaderNodeTextureParameterTriplanar");
	ClassDB::add_compatibility_class("VisualShaderNodeTransformUniform", "VisualShaderNodeTransformParameter");
	ClassDB::add_compatibility_class("VisualShaderNodeVec3Uniform", "VisualShaderNodeVec3Parameter");
	ClassDB::add_compatibility_class("VisualShaderNodeUniform", "VisualShaderNodeParameter");
	ClassDB::add_compatibility_class("VisualShaderNodeUniformRef", "VisualShaderNodeParameterRef");

	// Renamed during 4.0 alpha, added to ease transition between alphas.
	ClassDB::add_compatibility_class("AudioStreamOGGVorbis", "AudioStreamOggVorbis");
	ClassDB::add_compatibility_class("AudioStreamSample", "AudioStreamWAV");
	ClassDB::add_compatibility_class("OGGPacketSequence", "OggPacketSequence");
	ClassDB::add_compatibility_class("StreamCubemap", "CompressedCubemap");
	ClassDB::add_compatibility_class("StreamCubemapArray", "CompressedCubemapArray");
	ClassDB::add_compatibility_class("StreamTexture2D", "CompressedTexture2D");
	ClassDB::add_compatibility_class("StreamTexture2DArray", "CompressedTexture2DArray");
	ClassDB::add_compatibility_class("StreamTexture3D", "CompressedTexture3D");
	ClassDB::add_compatibility_class("StreamTextureLayered", "CompressedTextureLayered");
	ClassDB::add_compatibility_class("VisualShaderNodeFloatUniform", "VisualShaderNodeFloatParameter");
#endif /* DISABLE_DEPRECATED */

	OS::get_singleton()->yield(); // may take time to init

	for (int i = 0; i < 20; i++) {
		GLOBAL_DEF_BASIC(vformat("%s/layer_%d", PNAME("layer_names/2d_render"), i + 1), "");
		GLOBAL_DEF_BASIC(vformat("%s/layer_%d", PNAME("layer_names/3d_render"), i + 1), "");
	}

	for (int i = 0; i < 32; i++) {
		GLOBAL_DEF_BASIC(vformat("%s/layer_%d", PNAME("layer_names/2d_physics"), i + 1), "");
		GLOBAL_DEF_BASIC(vformat("%s/layer_%d", PNAME("layer_names/2d_navigation"), i + 1), "");
		GLOBAL_DEF_BASIC(vformat("%s/layer_%d", PNAME("layer_names/3d_physics"), i + 1), "");
		GLOBAL_DEF_BASIC(vformat("%s/layer_%d", PNAME("layer_names/3d_navigation"), i + 1), "");
	}

	for (int i = 0; i < 32; i++) {
		GLOBAL_DEF_BASIC(vformat("%s/layer_%d", PNAME("layer_names/avoidance"), i + 1), "");
	}

	if (RenderingServer::get_singleton()) {
		// RenderingServer needs to exist for this to succeed.
		ColorPicker::init_shaders();
		GraphEdit::init_shaders();
	}

	SceneDebugger::initialize();

	OS::get_singleton()->benchmark_end_measure("Scene", "Register Types");
}

void unregister_scene_types() {
	OS::get_singleton()->benchmark_begin_measure("Scene", "Unregister Types");

	SceneDebugger::deinitialize();

	ResourceLoader::remove_resource_format_loader(resource_loader_texture_layered);
	resource_loader_texture_layered.unref();

	ResourceLoader::remove_resource_format_loader(resource_loader_texture_3d);
	resource_loader_texture_3d.unref();

	ResourceLoader::remove_resource_format_loader(resource_loader_stream_texture);
	resource_loader_stream_texture.unref();

	ResourceSaver::remove_resource_format_saver(resource_saver_text);
	resource_saver_text.unref();

	ResourceLoader::remove_resource_format_loader(resource_loader_text);
	resource_loader_text.unref();

	ResourceSaver::remove_resource_format_saver(resource_saver_shader);
	resource_saver_shader.unref();

	ResourceLoader::remove_resource_format_loader(resource_loader_shader);
	resource_loader_shader.unref();

	ResourceSaver::remove_resource_format_saver(resource_saver_shader_include);
	resource_saver_shader_include.unref();

	ResourceLoader::remove_resource_format_loader(resource_loader_shader_include);
	resource_loader_shader_include.unref();

	// StandardMaterial3D is not initialized when 3D is disabled, so it shouldn't be cleaned up either
#ifndef _3D_DISABLED
	BaseMaterial3D::finish_shaders();
	PhysicalSkyMaterial::cleanup_shader();
	PanoramaSkyMaterial::cleanup_shader();
	ProceduralSkyMaterial::cleanup_shader();
	FogMaterial::cleanup_shader();
#endif // _3D_DISABLED

	ParticleProcessMaterial::finish_shaders();
	CanvasItemMaterial::finish_shaders();
	ColorPicker::finish_shaders();
	GraphEdit::finish_shaders();
	SceneStringNames::free();

	OS::get_singleton()->benchmark_end_measure("Scene", "Unregister Types");
}

void register_scene_singletons() {
	OS::get_singleton()->benchmark_begin_measure("Scene", "Register Singletons");

	BRREGISTER_CLASS(ThemeDB);

	Engine::get_singleton()->add_singleton(Engine::Singleton("ThemeDB", ThemeDB::get_singleton()));

	OS::get_singleton()->benchmark_end_measure("Scene", "Register Singletons");
}
