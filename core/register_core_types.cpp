/**************************************************************************/
/*  register_core_types.cpp                                               */
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

#include "register_core_types.h"

#include "core/config/engine.h"
#include "core/config/project_settings.h"
#include "core/core_bind.h"
#include "core/crypto/aes_context.h"
#include "core/crypto/crypto.h"
#include "core/crypto/hashing_context.h"
#include "core/debugger/engine_profiler.h"
#include "core/extension/brextension.h"
#include "core/extension/brextension_manager.h"
#include "core/input/input.h"
#include "core/input/input_map.h"
#include "core/input/shortcut.h"
#include "core/io/config_file.h"
#include "core/io/dir_access.h"
#include "core/io/dtls_server.h"
#include "core/io/http_client.h"
#include "core/io/image_loader.h"
#include "core/io/json.h"
#include "core/io/marshalls.h"
#include "core/io/missing_resource.h"
#include "core/io/packed_data_container.h"
#include "core/io/packet_peer.h"
#include "core/io/packet_peer_dtls.h"
#include "core/io/packet_peer_udp.h"
#include "core/io/pck_packer.h"
#include "core/io/resource_format_binary.h"
#include "core/io/resource_importer.h"
#include "core/io/resource_uid.h"
#include "core/io/stream_peer_gzip.h"
#include "core/io/stream_peer_tls.h"
#include "core/io/tcp_server.h"
#include "core/io/translation_loader_po.h"
#include "core/io/udp_server.h"
#include "core/io/xml_parser.h"
#include "core/math/a_star.h"
#include "core/math/a_star_grid_2d.h"
#include "core/math/expression.h"
#include "core/math/geometry_2d.h"
#include "core/math/geometry_3d.h"
#include "core/math/random_number_generator.h"
#include "core/math/triangle_mesh.h"
#include "core/object/class_db.h"
#include "core/object/script_language_extension.h"
#include "core/object/undo_redo.h"
#include "core/object/worker_thread_pool.h"
#include "core/os/main_loop.h"
#include "core/os/time.h"
#include "core/string/optimized_translation.h"
#include "core/string/translation.h"
#include "core/string/translation_server.h"

static Ref<ResourceFormatSaverBinary> resource_saver_binary;
static Ref<ResourceFormatLoaderBinary> resource_loader_binary;
static Ref<ResourceFormatImporter> resource_format_importer;
static Ref<ResourceFormatImporterSaver> resource_format_importer_saver;
static Ref<ResourceFormatLoaderImage> resource_format_image;
static Ref<TranslationLoaderPO> resource_format_po;
static Ref<ResourceFormatSaverCrypto> resource_format_saver_crypto;
static Ref<ResourceFormatLoaderCrypto> resource_format_loader_crypto;
static Ref<BRExtensionResourceLoader> resource_loader_brextension;
static Ref<ResourceFormatSaverJSON> resource_saver_json;
static Ref<ResourceFormatLoaderJSON> resource_loader_json;

static core_bind::ResourceLoader *_resource_loader = nullptr;
static core_bind::ResourceSaver *_resource_saver = nullptr;
static core_bind::OS *_os = nullptr;
static core_bind::Engine *_engine = nullptr;
static core_bind::special::ClassDB *_classdb = nullptr;
static core_bind::Marshalls *_marshalls = nullptr;
static core_bind::EngineDebugger *_engine_debugger = nullptr;

static IP *ip = nullptr;
static Time *_time = nullptr;

static core_bind::Geometry2D *_geometry_2d = nullptr;
static core_bind::Geometry3D *_geometry_3d = nullptr;

static WorkerThreadPool *worker_thread_pool = nullptr;

extern Mutex _global_mutex;

static BRExtensionManager *brextension_manager = nullptr;

extern void register_global_constants();
extern void unregister_global_constants();

static ResourceUID *resource_uid = nullptr;

static bool _is_core_extensions_registered = false;

void register_core_types() {
	OS::get_singleton()->benchmark_begin_measure("Core", "Register Types");

	//consistency check
	static_assert(sizeof(Callable) <= 16);

	ObjectDB::setup();

	StringName::setup();
	_time = memnew(Time);
	ResourceLoader::initialize();

	register_global_constants();

	Variant::register_types();

	CoreStringNames::create();

	resource_format_po.instantiate();
	ResourceLoader::add_resource_format_loader(resource_format_po);

	resource_saver_binary.instantiate();
	ResourceSaver::add_resource_format_saver(resource_saver_binary);
	resource_loader_binary.instantiate();
	ResourceLoader::add_resource_format_loader(resource_loader_binary);

	resource_format_importer.instantiate();
	ResourceLoader::add_resource_format_loader(resource_format_importer);

	resource_format_importer_saver.instantiate();
	ResourceSaver::add_resource_format_saver(resource_format_importer_saver);

	resource_format_image.instantiate();
	ResourceLoader::add_resource_format_loader(resource_format_image);

	BRREGISTER_CLASS(Object);

	BRREGISTER_ABSTRACT_CLASS(Script);
	BRREGISTER_ABSTRACT_CLASS(ScriptLanguage);

	BRREGISTER_VIRTUAL_CLASS(ScriptExtension);
	BRREGISTER_VIRTUAL_CLASS(ScriptLanguageExtension);

	BRREGISTER_CLASS(RefCounted);
	BRREGISTER_CLASS(WeakRef);
	BRREGISTER_CLASS(Resource);
	BRREGISTER_VIRTUAL_CLASS(MissingResource);
	BRREGISTER_CLASS(Image);

	BRREGISTER_CLASS(Shortcut);
	BRREGISTER_ABSTRACT_CLASS(InputEvent);
	BRREGISTER_ABSTRACT_CLASS(InputEventWithModifiers);
	BRREGISTER_ABSTRACT_CLASS(InputEventFromWindow);
	BRREGISTER_CLASS(InputEventKey);
	BRREGISTER_CLASS(InputEventShortcut);
	BRREGISTER_ABSTRACT_CLASS(InputEventMouse);
	BRREGISTER_CLASS(InputEventMouseButton);
	BRREGISTER_CLASS(InputEventMouseMotion);
	BRREGISTER_CLASS(InputEventJoypadButton);
	BRREGISTER_CLASS(InputEventJoypadMotion);
	BRREGISTER_CLASS(InputEventScreenDrag);
	BRREGISTER_CLASS(InputEventScreenTouch);
	BRREGISTER_CLASS(InputEventAction);
	BRREGISTER_ABSTRACT_CLASS(InputEventGesture);
	BRREGISTER_CLASS(InputEventMagnifyGesture);
	BRREGISTER_CLASS(InputEventPanGesture);
	BRREGISTER_CLASS(InputEventMIDI);

	// Network
	BRREGISTER_ABSTRACT_CLASS(IP);

	BRREGISTER_ABSTRACT_CLASS(StreamPeer);
	BRREGISTER_CLASS(StreamPeerExtension);
	BRREGISTER_CLASS(StreamPeerBuffer);
	BRREGISTER_CLASS(StreamPeerGZIP);
	BRREGISTER_CLASS(StreamPeerTCP);
	BRREGISTER_CLASS(TCPServer);

	BRREGISTER_ABSTRACT_CLASS(PacketPeer);
	BRREGISTER_CLASS(PacketPeerExtension);
	BRREGISTER_CLASS(PacketPeerStream);
	BRREGISTER_CLASS(PacketPeerUDP);
	BRREGISTER_CLASS(UDPServer);

	BRREGISTER_ABSTRACT_CLASS(WorkerThreadPool);

	ClassDB::register_custom_instance_class<HTTPClient>();

	// Crypto
	BRREGISTER_CLASS(HashingContext);
	BRREGISTER_CLASS(AESContext);
	ClassDB::register_custom_instance_class<X509Certificate>();
	ClassDB::register_custom_instance_class<CryptoKey>();
	BRREGISTER_ABSTRACT_CLASS(TLSOptions);
	ClassDB::register_custom_instance_class<HMACContext>();
	ClassDB::register_custom_instance_class<Crypto>();
	ClassDB::register_custom_instance_class<StreamPeerTLS>();
	ClassDB::register_custom_instance_class<PacketPeerDTLS>();
	ClassDB::register_custom_instance_class<DTLSServer>();

	resource_format_saver_crypto.instantiate();
	ResourceSaver::add_resource_format_saver(resource_format_saver_crypto);
	resource_format_loader_crypto.instantiate();
	ResourceLoader::add_resource_format_loader(resource_format_loader_crypto);

	resource_loader_json.instantiate();
	ResourceLoader::add_resource_format_loader(resource_loader_json);

	resource_saver_json.instantiate();
	ResourceSaver::add_resource_format_saver(resource_saver_json);

	BRREGISTER_CLASS(MainLoop);
	BRREGISTER_CLASS(Translation);
	BRREGISTER_CLASS(TranslationDomain);
	BRREGISTER_CLASS(OptimizedTranslation);
	BRREGISTER_CLASS(UndoRedo);
	BRREGISTER_CLASS(TriangleMesh);

	BRREGISTER_CLASS(ResourceFormatLoader);
	BRREGISTER_CLASS(ResourceFormatSaver);

	BRREGISTER_ABSTRACT_CLASS(FileAccess);
	BRREGISTER_ABSTRACT_CLASS(DirAccess);
	BRREGISTER_CLASS(core_bind::Thread);
	BRREGISTER_CLASS(core_bind::Mutex);
	BRREGISTER_CLASS(core_bind::Semaphore);

	BRREGISTER_CLASS(XMLParser);
	BRREGISTER_CLASS(JSON);

	BRREGISTER_CLASS(ConfigFile);

	BRREGISTER_CLASS(PCKPacker);

	BRREGISTER_CLASS(PackedDataContainer);
	BRREGISTER_ABSTRACT_CLASS(PackedDataContainerRef);
	BRREGISTER_CLASS(AStar3D);
	BRREGISTER_CLASS(AStar2D);
	BRREGISTER_CLASS(AStarGrid2D);
	BRREGISTER_CLASS(EncodedObjectAsID);
	BRREGISTER_CLASS(RandomNumberGenerator);

	BRREGISTER_ABSTRACT_CLASS(ImageFormatLoader);
	BRREGISTER_CLASS(ImageFormatLoaderExtension);
	BRREGISTER_ABSTRACT_CLASS(ResourceImporter);

	BRREGISTER_CLASS(BRExtension);

	BRREGISTER_ABSTRACT_CLASS(BRExtensionManager);

	BRREGISTER_ABSTRACT_CLASS(ResourceUID);

	BRREGISTER_CLASS(EngineProfiler);

	resource_uid = memnew(ResourceUID);

	brextension_manager = memnew(BRExtensionManager);

	resource_loader_brextension.instantiate();
	ResourceLoader::add_resource_format_loader(resource_loader_brextension);

	ip = IP::create();

	_geometry_2d = memnew(core_bind::Geometry2D);
	_geometry_3d = memnew(core_bind::Geometry3D);

	_resource_loader = memnew(core_bind::ResourceLoader);
	_resource_saver = memnew(core_bind::ResourceSaver);
	_os = memnew(core_bind::OS);
	_engine = memnew(core_bind::Engine);
	_classdb = memnew(core_bind::special::ClassDB);
	_marshalls = memnew(core_bind::Marshalls);
	_engine_debugger = memnew(core_bind::EngineDebugger);

	BRREGISTER_NATIVE_STRUCT(ObjectID, "uint64_t id = 0");
	BRREGISTER_NATIVE_STRUCT(AudioFrame, "float left;float right");
	BRREGISTER_NATIVE_STRUCT(ScriptLanguageExtensionProfilingInfo, "StringName signature;uint64_t call_count;uint64_t total_time;uint64_t self_time");

	worker_thread_pool = memnew(WorkerThreadPool);

	OS::get_singleton()->benchmark_end_measure("Core", "Register Types");
}

void register_core_settings() {
	// Since in register core types, globals may not be present.
	GLOBAL_DEF(PropertyInfo(Variant::INT, "network/limits/tcp/connect_timeout_seconds", PROPERTY_HINT_RANGE, "1,1800,1"), (30));
	GLOBAL_DEF_RST(PropertyInfo(Variant::INT, "network/limits/packet_peer_stream/max_buffer_po2", PROPERTY_HINT_RANGE, "0,64,1,or_greater"), (16));
	GLOBAL_DEF(PropertyInfo(Variant::STRING, "network/tls/certificate_bundle_override", PROPERTY_HINT_FILE, "*.crt"), "");

	GLOBAL_DEF("threading/worker_pool/max_threads", -1);
	GLOBAL_DEF("threading/worker_pool/low_priority_thread_ratio", 0.3);
}

void register_core_singletons() {
	OS::get_singleton()->benchmark_begin_measure("Core", "Register Singletons");

	BRREGISTER_CLASS(ProjectSettings);
	BRREGISTER_ABSTRACT_CLASS(IP);
	BRREGISTER_CLASS(core_bind::Geometry2D);
	BRREGISTER_CLASS(core_bind::Geometry3D);
	BRREGISTER_CLASS(core_bind::ResourceLoader);
	BRREGISTER_CLASS(core_bind::ResourceSaver);
	BRREGISTER_CLASS(core_bind::OS);
	BRREGISTER_CLASS(core_bind::Engine);
	BRREGISTER_CLASS(core_bind::special::ClassDB);
	BRREGISTER_CLASS(core_bind::Marshalls);
	BRREGISTER_CLASS(TranslationServer);
	BRREGISTER_ABSTRACT_CLASS(Input);
	BRREGISTER_CLASS(InputMap);
	BRREGISTER_CLASS(Expression);
	BRREGISTER_CLASS(core_bind::EngineDebugger);
	BRREGISTER_CLASS(Time);

	Engine::get_singleton()->add_singleton(Engine::Singleton("ProjectSettings", ProjectSettings::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("IP", IP::get_singleton(), "IP"));
	Engine::get_singleton()->add_singleton(Engine::Singleton("Geometry2D", core_bind::Geometry2D::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("Geometry3D", core_bind::Geometry3D::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("ResourceLoader", core_bind::ResourceLoader::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("ResourceSaver", core_bind::ResourceSaver::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("OS", core_bind::OS::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("Engine", core_bind::Engine::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("ClassDB", _classdb));
	Engine::get_singleton()->add_singleton(Engine::Singleton("Marshalls", core_bind::Marshalls::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("TranslationServer", TranslationServer::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("Input", Input::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("InputMap", InputMap::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("EngineDebugger", core_bind::EngineDebugger::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("Time", Time::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("BRExtensionManager", BRExtensionManager::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("ResourceUID", ResourceUID::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("WorkerThreadPool", worker_thread_pool));

	OS::get_singleton()->benchmark_end_measure("Core", "Register Singletons");
}

void register_core_extensions() {
	OS::get_singleton()->benchmark_begin_measure("Core", "Register Extensions");

	// Hardcoded for now.
	BRExtension::initialize_brextensions();
	brextension_manager->load_extensions();
	brextension_manager->initialize_extensions(BRExtension::INITIALIZATION_LEVEL_CORE);
	_is_core_extensions_registered = true;

	OS::get_singleton()->benchmark_end_measure("Core", "Register Extensions");
}

void unregister_core_extensions() {
	OS::get_singleton()->benchmark_begin_measure("Core", "Unregister Extensions");

	if (_is_core_extensions_registered) {
		brextension_manager->deinitialize_extensions(BRExtension::INITIALIZATION_LEVEL_CORE);
	}
	BRExtension::finalize_brextensions();

	OS::get_singleton()->benchmark_end_measure("Core", "Unregister Extensions");
}

void unregister_core_types() {
	OS::get_singleton()->benchmark_begin_measure("Core", "Unregister Types");

	// Destroy singletons in reverse order to ensure dependencies are not broken.

	memdelete(worker_thread_pool);

	memdelete(_engine_debugger);
	memdelete(_marshalls);
	memdelete(_classdb);
	memdelete(_engine);
	memdelete(_os);
	memdelete(_resource_saver);
	memdelete(_resource_loader);

	memdelete(_geometry_3d);
	memdelete(_geometry_2d);

	memdelete(brextension_manager);

	memdelete(resource_uid);

	if (ip) {
		memdelete(ip);
	}

	ResourceLoader::remove_resource_format_loader(resource_format_image);
	resource_format_image.unref();

	ResourceSaver::remove_resource_format_saver(resource_saver_binary);
	resource_saver_binary.unref();

	ResourceLoader::remove_resource_format_loader(resource_loader_binary);
	resource_loader_binary.unref();

	ResourceLoader::remove_resource_format_loader(resource_format_importer);
	resource_format_importer.unref();

	ResourceSaver::remove_resource_format_saver(resource_format_importer_saver);
	resource_format_importer_saver.unref();

	ResourceLoader::remove_resource_format_loader(resource_format_po);
	resource_format_po.unref();

	ResourceSaver::remove_resource_format_saver(resource_format_saver_crypto);
	resource_format_saver_crypto.unref();
	ResourceLoader::remove_resource_format_loader(resource_format_loader_crypto);
	resource_format_loader_crypto.unref();

	ResourceSaver::remove_resource_format_saver(resource_saver_json);
	resource_saver_json.unref();

	ResourceLoader::remove_resource_format_loader(resource_loader_json);
	resource_loader_json.unref();

	ResourceLoader::remove_resource_format_loader(resource_loader_brextension);
	resource_loader_brextension.unref();

	ResourceLoader::finalize();

	ClassDB::cleanup_defaults();
	memdelete(_time);
	ObjectDB::cleanup();

	Variant::unregister_types();

	unregister_global_constants();

	ResourceCache::clear();
	ClassDB::cleanup();
	CoreStringNames::free();
	StringName::cleanup();

	OS::get_singleton()->benchmark_end_measure("Core", "Unregister Types");
}
