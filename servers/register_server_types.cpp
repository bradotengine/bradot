/**************************************************************************/
/*  register_server_types.cpp                                             */
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

#include "register_server_types.h"

#include "core/config/engine.h"
#include "core/config/project_settings.h"

#include "audio/audio_effect.h"
#include "audio/audio_stream.h"
#include "audio/effects/audio_effect_amplify.h"
#include "audio/effects/audio_effect_capture.h"
#include "audio/effects/audio_effect_chorus.h"
#include "audio/effects/audio_effect_compressor.h"
#include "audio/effects/audio_effect_delay.h"
#include "audio/effects/audio_effect_distortion.h"
#include "audio/effects/audio_effect_eq.h"
#include "audio/effects/audio_effect_filter.h"
#include "audio/effects/audio_effect_hard_limiter.h"
#include "audio/effects/audio_effect_limiter.h"
#include "audio/effects/audio_effect_panner.h"
#include "audio/effects/audio_effect_phaser.h"
#include "audio/effects/audio_effect_pitch_shift.h"
#include "audio/effects/audio_effect_record.h"
#include "audio/effects/audio_effect_reverb.h"
#include "audio/effects/audio_effect_spectrum_analyzer.h"
#include "audio/effects/audio_effect_stereo_enhance.h"
#include "audio/effects/audio_stream_generator.h"
#include "audio_server.h"
#include "camera/camera_feed.h"
#include "camera_server.h"
#include "debugger/servers_debugger.h"
#include "display/native_menu.h"
#include "display_server.h"
#include "movie_writer/movie_writer.h"
#include "movie_writer/movie_writer_mjpeg.h"
#include "movie_writer/movie_writer_pngwav.h"
#include "rendering/renderer_compositor.h"
#include "rendering/renderer_rd/framebuffer_cache_rd.h"
#include "rendering/renderer_rd/storage_rd/render_data_rd.h"
#include "rendering/renderer_rd/storage_rd/render_scene_buffers_rd.h"
#include "rendering/renderer_rd/storage_rd/render_scene_data_rd.h"
#include "rendering/renderer_rd/uniform_set_cache_rd.h"
#include "rendering/rendering_device.h"
#include "rendering/rendering_device_binds.h"
#include "rendering/storage/render_data.h"
#include "rendering/storage/render_scene_buffers.h"
#include "rendering/storage/render_scene_data.h"
#include "rendering_server.h"
#include "servers/rendering/shader_types.h"
#include "text/text_server_dummy.h"
#include "text/text_server_extension.h"
#include "text_server.h"

// 2D physics and navigation.
#include "navigation_server_2d.h"
#include "physics_server_2d.h"
#include "physics_server_2d_dummy.h"
#include "physics_server_2d_wrap_mt.h"
#include "servers/extensions/physics_server_2d_extension.h"

// 3D physics and navigation (3D navigation is needed for 2D).
#include "navigation_server_3d.h"
#ifndef _3D_DISABLED
#include "physics_server_3d.h"
#include "physics_server_3d_dummy.h"
#include "physics_server_3d_wrap_mt.h"
#include "servers/extensions/physics_server_3d_extension.h"
#include "xr/xr_body_tracker.h"
#include "xr/xr_controller_tracker.h"
#include "xr/xr_face_tracker.h"
#include "xr/xr_hand_tracker.h"
#include "xr/xr_interface.h"
#include "xr/xr_interface_extension.h"
#include "xr/xr_positional_tracker.h"
#include "xr_server.h"
#endif // _3D_DISABLED

ShaderTypes *shader_types = nullptr;

#ifndef _3D_DISABLED
static PhysicsServer3D *_create_dummy_physics_server_3d() {
	return memnew(PhysicsServer3DDummy);
}
#endif // _3D_DISABLED

static PhysicsServer2D *_create_dummy_physics_server_2d() {
	return memnew(PhysicsServer2DDummy);
}

static bool has_server_feature_callback(const String &p_feature) {
	if (RenderingServer::get_singleton()) {
		if (RenderingServer::get_singleton()->has_os_feature(p_feature)) {
			return true;
		}
	}

	return false;
}

static MovieWriterMJPEG *writer_mjpeg = nullptr;
static MovieWriterPNGWAV *writer_pngwav = nullptr;

void register_server_types() {
	OS::get_singleton()->benchmark_begin_measure("Servers", "Register Extensions");

	shader_types = memnew(ShaderTypes);

	BRREGISTER_CLASS(TextServerManager);
	BRREGISTER_ABSTRACT_CLASS(TextServer);
	BRREGISTER_CLASS(TextServerExtension);
	BRREGISTER_CLASS(TextServerDummy);

	BRREGISTER_NATIVE_STRUCT(Glyph, "int start = -1;int end = -1;uint8_t count = 0;uint8_t repeat = 1;uint16_t flags = 0;float x_off = 0.f;float y_off = 0.f;float advance = 0.f;RID font_rid;int font_size = 0;int32_t index = 0");
	BRREGISTER_NATIVE_STRUCT(CaretInfo, "Rect2 leading_caret;Rect2 trailing_caret;TextServer::Direction leading_direction;TextServer::Direction trailing_direction");

	Engine::get_singleton()->add_singleton(Engine::Singleton("TextServerManager", TextServerManager::get_singleton(), "TextServerManager"));

	OS::get_singleton()->set_has_server_feature_callback(has_server_feature_callback);

	BRREGISTER_ABSTRACT_CLASS(DisplayServer);
	BRREGISTER_ABSTRACT_CLASS(RenderingServer);
	BRREGISTER_CLASS(AudioServer);

	BRREGISTER_CLASS(NativeMenu);

	BRREGISTER_CLASS(CameraServer);

	BRREGISTER_ABSTRACT_CLASS(RenderingDevice);

	BRREGISTER_CLASS(AudioStream);
	BRREGISTER_CLASS(AudioStreamPlayback);
	BRREGISTER_VIRTUAL_CLASS(AudioStreamPlaybackResampled);
	BRREGISTER_CLASS(AudioStreamMicrophone);
	BRREGISTER_CLASS(AudioStreamRandomizer);
	BRREGISTER_CLASS(AudioSample);
	BRREGISTER_CLASS(AudioSamplePlayback);
	BRREGISTER_VIRTUAL_CLASS(AudioEffect);
	BRREGISTER_VIRTUAL_CLASS(AudioEffectInstance);
	BRREGISTER_CLASS(AudioEffectEQ);
	BRREGISTER_CLASS(AudioEffectFilter);
	BRREGISTER_CLASS(AudioBusLayout);

	BRREGISTER_CLASS(AudioStreamGenerator);
	BRREGISTER_ABSTRACT_CLASS(AudioStreamGeneratorPlayback);

	{
		//audio effects
		BRREGISTER_CLASS(AudioEffectAmplify);

		BRREGISTER_CLASS(AudioEffectReverb);

		BRREGISTER_CLASS(AudioEffectLowPassFilter);
		BRREGISTER_CLASS(AudioEffectHighPassFilter);
		BRREGISTER_CLASS(AudioEffectBandPassFilter);
		BRREGISTER_CLASS(AudioEffectNotchFilter);
		BRREGISTER_CLASS(AudioEffectBandLimitFilter);
		BRREGISTER_CLASS(AudioEffectLowShelfFilter);
		BRREGISTER_CLASS(AudioEffectHighShelfFilter);

		BRREGISTER_CLASS(AudioEffectEQ6);
		BRREGISTER_CLASS(AudioEffectEQ10);
		BRREGISTER_CLASS(AudioEffectEQ21);

		BRREGISTER_CLASS(AudioEffectDistortion);

		BRREGISTER_CLASS(AudioEffectStereoEnhance);

		BRREGISTER_CLASS(AudioEffectPanner);
		BRREGISTER_CLASS(AudioEffectChorus);
		BRREGISTER_CLASS(AudioEffectDelay);
		BRREGISTER_CLASS(AudioEffectCompressor);
		BRREGISTER_CLASS(AudioEffectLimiter);
		BRREGISTER_CLASS(AudioEffectHardLimiter);
		BRREGISTER_CLASS(AudioEffectPitchShift);
		BRREGISTER_CLASS(AudioEffectPhaser);

		BRREGISTER_CLASS(AudioEffectRecord);
		BRREGISTER_CLASS(AudioEffectSpectrumAnalyzer);
		BRREGISTER_ABSTRACT_CLASS(AudioEffectSpectrumAnalyzerInstance);

		BRREGISTER_CLASS(AudioEffectCapture);
	}

	BRREGISTER_ABSTRACT_CLASS(RenderingDevice);
	BRREGISTER_CLASS(RDTextureFormat);
	BRREGISTER_CLASS(RDTextureView);
	BRREGISTER_CLASS(RDAttachmentFormat);
	BRREGISTER_CLASS(RDFramebufferPass);
	BRREGISTER_CLASS(RDSamplerState);
	BRREGISTER_CLASS(RDVertexAttribute);
	BRREGISTER_CLASS(RDUniform);
	BRREGISTER_CLASS(RDPipelineRasterizationState);
	BRREGISTER_CLASS(RDPipelineMultisampleState);
	BRREGISTER_CLASS(RDPipelineDepthStencilState);
	BRREGISTER_CLASS(RDPipelineColorBlendStateAttachment);
	BRREGISTER_CLASS(RDPipelineColorBlendState);
	BRREGISTER_CLASS(RDShaderSource);
	BRREGISTER_CLASS(RDShaderSPIRV);
	BRREGISTER_CLASS(RDShaderFile);
	BRREGISTER_CLASS(RDPipelineSpecializationConstant);

	BRREGISTER_ABSTRACT_CLASS(RenderData);
	BRREGISTER_CLASS(RenderDataExtension);
	BRREGISTER_CLASS(RenderDataRD);

	BRREGISTER_ABSTRACT_CLASS(RenderSceneData);
	BRREGISTER_CLASS(RenderSceneDataExtension);
	BRREGISTER_CLASS(RenderSceneDataRD);

	BRREGISTER_CLASS(RenderSceneBuffersConfiguration);
	BRREGISTER_ABSTRACT_CLASS(RenderSceneBuffers);
	BRREGISTER_CLASS(RenderSceneBuffersExtension);
	BRREGISTER_CLASS(RenderSceneBuffersRD);

	BRREGISTER_CLASS(FramebufferCacheRD);
	BRREGISTER_CLASS(UniformSetCacheRD);

	BRREGISTER_CLASS(CameraFeed);

	BRREGISTER_VIRTUAL_CLASS(MovieWriter);

	ServersDebugger::initialize();

	// Physics 2D
	BRREGISTER_CLASS(PhysicsServer2DManager);
	Engine::get_singleton()->add_singleton(Engine::Singleton("PhysicsServer2DManager", PhysicsServer2DManager::get_singleton(), "PhysicsServer2DManager"));

	BRREGISTER_ABSTRACT_CLASS(PhysicsServer2D);
	BRREGISTER_VIRTUAL_CLASS(PhysicsServer2DExtension);
	BRREGISTER_VIRTUAL_CLASS(PhysicsDirectBodyState2DExtension);
	BRREGISTER_VIRTUAL_CLASS(PhysicsDirectSpaceState2DExtension);

	BRREGISTER_NATIVE_STRUCT(PhysicsServer2DExtensionRayResult, "Vector2 position;Vector2 normal;RID rid;ObjectID collider_id;Object *collider;int shape");
	BRREGISTER_NATIVE_STRUCT(PhysicsServer2DExtensionShapeResult, "RID rid;ObjectID collider_id;Object *collider;int shape");
	BRREGISTER_NATIVE_STRUCT(PhysicsServer2DExtensionShapeRestInfo, "Vector2 point;Vector2 normal;RID rid;ObjectID collider_id;int shape;Vector2 linear_velocity");
	BRREGISTER_NATIVE_STRUCT(PhysicsServer2DExtensionMotionResult, "Vector2 travel;Vector2 remainder;Vector2 collision_point;Vector2 collision_normal;Vector2 collider_velocity;real_t collision_depth;real_t collision_safe_fraction;real_t collision_unsafe_fraction;int collision_local_shape;ObjectID collider_id;RID collider;int collider_shape");

	BRREGISTER_ABSTRACT_CLASS(PhysicsDirectBodyState2D);
	BRREGISTER_ABSTRACT_CLASS(PhysicsDirectSpaceState2D);
	BRREGISTER_CLASS(PhysicsRayQueryParameters2D);
	BRREGISTER_CLASS(PhysicsPointQueryParameters2D);
	BRREGISTER_CLASS(PhysicsShapeQueryParameters2D);
	BRREGISTER_CLASS(PhysicsTestMotionParameters2D);
	BRREGISTER_CLASS(PhysicsTestMotionResult2D);

	GLOBAL_DEF(PropertyInfo(Variant::STRING, PhysicsServer2DManager::setting_property_name, PROPERTY_HINT_ENUM, "DEFAULT"), "DEFAULT");

	PhysicsServer2DManager::get_singleton()->register_server("Dummy", callable_mp_static(_create_dummy_physics_server_2d));

	BRREGISTER_ABSTRACT_CLASS(NavigationServer2D);
	BRREGISTER_CLASS(NavigationPathQueryParameters2D);
	BRREGISTER_CLASS(NavigationPathQueryResult2D);

#ifndef _3D_DISABLED
	// Physics 3D
	BRREGISTER_CLASS(PhysicsServer3DManager);
	Engine::get_singleton()->add_singleton(Engine::Singleton("PhysicsServer3DManager", PhysicsServer3DManager::get_singleton(), "PhysicsServer3DManager"));

	BRREGISTER_ABSTRACT_CLASS(PhysicsServer3D);
	BRREGISTER_VIRTUAL_CLASS(PhysicsServer3DExtension);
	BRREGISTER_VIRTUAL_CLASS(PhysicsDirectBodyState3DExtension);
	BRREGISTER_VIRTUAL_CLASS(PhysicsDirectSpaceState3DExtension)
	BRREGISTER_VIRTUAL_CLASS(PhysicsServer3DRenderingServerHandler)

	BRREGISTER_NATIVE_STRUCT(PhysicsServer3DExtensionRayResult, "Vector3 position;Vector3 normal;RID rid;ObjectID collider_id;Object *collider;int shape;int face_index");
	BRREGISTER_NATIVE_STRUCT(PhysicsServer3DExtensionShapeResult, "RID rid;ObjectID collider_id;Object *collider;int shape");
	BRREGISTER_NATIVE_STRUCT(PhysicsServer3DExtensionShapeRestInfo, "Vector3 point;Vector3 normal;RID rid;ObjectID collider_id;int shape;Vector3 linear_velocity");
	BRREGISTER_NATIVE_STRUCT(PhysicsServer3DExtensionMotionCollision, "Vector3 position;Vector3 normal;Vector3 collider_velocity;Vector3 collider_angular_velocity;real_t depth;int local_shape;ObjectID collider_id;RID collider;int collider_shape");
	BRREGISTER_NATIVE_STRUCT(PhysicsServer3DExtensionMotionResult, "Vector3 travel;Vector3 remainder;real_t collision_depth;real_t collision_safe_fraction;real_t collision_unsafe_fraction;PhysicsServer3DExtensionMotionCollision collisions[32];int collision_count");

	BRREGISTER_ABSTRACT_CLASS(PhysicsDirectBodyState3D);
	BRREGISTER_ABSTRACT_CLASS(PhysicsDirectSpaceState3D);
	BRREGISTER_CLASS(PhysicsRayQueryParameters3D);
	BRREGISTER_CLASS(PhysicsPointQueryParameters3D);
	BRREGISTER_CLASS(PhysicsShapeQueryParameters3D);
	BRREGISTER_CLASS(PhysicsTestMotionParameters3D);
	BRREGISTER_CLASS(PhysicsTestMotionResult3D);

	GLOBAL_DEF(PropertyInfo(Variant::STRING, PhysicsServer3DManager::setting_property_name, PROPERTY_HINT_ENUM, "DEFAULT"), "DEFAULT");

	PhysicsServer3DManager::get_singleton()->register_server("Dummy", callable_mp_static(_create_dummy_physics_server_3d));

	BRREGISTER_ABSTRACT_CLASS(XRInterface);
	BRREGISTER_CLASS(XRVRS);
	BRREGISTER_CLASS(XRBodyTracker);
	BRREGISTER_CLASS(XRControllerTracker);
	BRREGISTER_CLASS(XRFaceTracker);
	BRREGISTER_CLASS(XRHandTracker);
	BRREGISTER_CLASS(XRInterfaceExtension); // can't register this as virtual because we need a creation function for our extensions.
	BRREGISTER_CLASS(XRPose);
	BRREGISTER_CLASS(XRPositionalTracker);
	BRREGISTER_CLASS(XRServer);
	BRREGISTER_ABSTRACT_CLASS(XRTracker);
#endif // _3D_DISABLED

	BRREGISTER_ABSTRACT_CLASS(NavigationServer3D);
	BRREGISTER_CLASS(NavigationPathQueryParameters3D);
	BRREGISTER_CLASS(NavigationPathQueryResult3D);

	writer_mjpeg = memnew(MovieWriterMJPEG);
	MovieWriter::add_writer(writer_mjpeg);

	writer_pngwav = memnew(MovieWriterPNGWAV);
	MovieWriter::add_writer(writer_pngwav);

	OS::get_singleton()->benchmark_end_measure("Servers", "Register Extensions");
}

void unregister_server_types() {
	OS::get_singleton()->benchmark_begin_measure("Servers", "Unregister Extensions");

	ServersDebugger::deinitialize();
	memdelete(shader_types);
	memdelete(writer_mjpeg);
	memdelete(writer_pngwav);

	OS::get_singleton()->benchmark_end_measure("Servers", "Unregister Extensions");
}

void register_server_singletons() {
	OS::get_singleton()->benchmark_begin_measure("Servers", "Register Singletons");

	Engine::get_singleton()->add_singleton(Engine::Singleton("AudioServer", AudioServer::get_singleton(), "AudioServer"));
	Engine::get_singleton()->add_singleton(Engine::Singleton("CameraServer", CameraServer::get_singleton(), "CameraServer"));
	Engine::get_singleton()->add_singleton(Engine::Singleton("DisplayServer", DisplayServer::get_singleton(), "DisplayServer"));
	Engine::get_singleton()->add_singleton(Engine::Singleton("NativeMenu", NativeMenu::get_singleton(), "NativeMenu"));
	Engine::get_singleton()->add_singleton(Engine::Singleton("NavigationServer2D", NavigationServer2D::get_singleton(), "NavigationServer2D"));
	Engine::get_singleton()->add_singleton(Engine::Singleton("NavigationServer3D", NavigationServer3D::get_singleton(), "NavigationServer3D"));
	Engine::get_singleton()->add_singleton(Engine::Singleton("RenderingServer", RenderingServer::get_singleton(), "RenderingServer"));

	Engine::get_singleton()->add_singleton(Engine::Singleton("PhysicsServer2D", PhysicsServer2D::get_singleton(), "PhysicsServer2D"));
#ifndef _3D_DISABLED
	Engine::get_singleton()->add_singleton(Engine::Singleton("PhysicsServer3D", PhysicsServer3D::get_singleton(), "PhysicsServer3D"));
	Engine::get_singleton()->add_singleton(Engine::Singleton("XRServer", XRServer::get_singleton(), "XRServer"));
#endif // _3D_DISABLED

	OS::get_singleton()->benchmark_end_measure("Servers", "Register Singletons");
}
