/**************************************************************************/
/*  library_bradot_os.js                                                   */
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

const IDHandler = {
	$IDHandler: {
		_last_id: 0,
		_references: {},

		get: function (p_id) {
			return IDHandler._references[p_id];
		},

		add: function (p_data) {
			const id = ++IDHandler._last_id;
			IDHandler._references[id] = p_data;
			return id;
		},

		remove: function (p_id) {
			delete IDHandler._references[p_id];
		},
	},
};

autoAddDeps(IDHandler, '$IDHandler');
mergeInto(LibraryManager.library, IDHandler);

const BradotConfig = {
	$BradotConfig__postset: 'Module["initConfig"] = BradotConfig.init_config;',
	$BradotConfig__deps: ['$BradotRuntime'],
	$BradotConfig: {
		canvas: null,
		locale: 'en',
		canvas_resize_policy: 2, // Adaptive
		virtual_keyboard: false,
		persistent_drops: false,
		on_execute: null,
		on_exit: null,

		init_config: function (p_opts) {
			BradotConfig.canvas_resize_policy = p_opts['canvasResizePolicy'];
			BradotConfig.canvas = p_opts['canvas'];
			BradotConfig.locale = p_opts['locale'] || BradotConfig.locale;
			BradotConfig.virtual_keyboard = p_opts['virtualKeyboard'];
			BradotConfig.persistent_drops = !!p_opts['persistentDrops'];
			BradotConfig.on_execute = p_opts['onExecute'];
			BradotConfig.on_exit = p_opts['onExit'];
			if (p_opts['focusCanvas']) {
				BradotConfig.canvas.focus();
			}
		},

		locate_file: function (file) {
			return Module['locateFile'](file);
		},
		clear: function () {
			BradotConfig.canvas = null;
			BradotConfig.locale = 'en';
			BradotConfig.canvas_resize_policy = 2;
			BradotConfig.virtual_keyboard = false;
			BradotConfig.persistent_drops = false;
			BradotConfig.on_execute = null;
			BradotConfig.on_exit = null;
		},
	},

	bradot_js_config_canvas_id_get__proxy: 'sync',
	bradot_js_config_canvas_id_get__sig: 'vii',
	bradot_js_config_canvas_id_get: function (p_ptr, p_ptr_max) {
		BradotRuntime.stringToHeap(`#${BradotConfig.canvas.id}`, p_ptr, p_ptr_max);
	},

	bradot_js_config_locale_get__proxy: 'sync',
	bradot_js_config_locale_get__sig: 'vii',
	bradot_js_config_locale_get: function (p_ptr, p_ptr_max) {
		BradotRuntime.stringToHeap(BradotConfig.locale, p_ptr, p_ptr_max);
	},
};

autoAddDeps(BradotConfig, '$BradotConfig');
mergeInto(LibraryManager.library, BradotConfig);

const BradotFS = {
	$BradotFS__deps: ['$FS', '$IDBFS', '$BradotRuntime'],
	$BradotFS__postset: [
		'Module["initFS"] = BradotFS.init;',
		'Module["copyToFS"] = BradotFS.copy_to_fs;',
	].join(''),
	$BradotFS: {
		// ERRNO_CODES works every odd version of emscripten, but this will break too eventually.
		ENOENT: 44,
		_idbfs: false,
		_syncing: false,
		_mount_points: [],

		is_persistent: function () {
			return BradotFS._idbfs ? 1 : 0;
		},

		// Initialize bradot file system, setting up persistent paths.
		// Returns a promise that resolves when the FS is ready.
		// We keep track of mount_points, so that we can properly close the IDBFS
		// since emscripten is not doing it by itself. (emscripten GH#12516).
		init: function (persistentPaths) {
			BradotFS._idbfs = false;
			if (!Array.isArray(persistentPaths)) {
				return Promise.reject(new Error('Persistent paths must be an array'));
			}
			if (!persistentPaths.length) {
				return Promise.resolve();
			}
			BradotFS._mount_points = persistentPaths.slice();

			function createRecursive(dir) {
				try {
					FS.stat(dir);
				} catch (e) {
					if (e.errno !== BradotFS.ENOENT) {
						// Let mkdirTree throw in case, we cannot trust the above check.
						BradotRuntime.error(e);
					}
					FS.mkdirTree(dir);
				}
			}

			BradotFS._mount_points.forEach(function (path) {
				createRecursive(path);
				FS.mount(IDBFS, {}, path);
			});
			return new Promise(function (resolve, reject) {
				FS.syncfs(true, function (err) {
					if (err) {
						BradotFS._mount_points = [];
						BradotFS._idbfs = false;
						BradotRuntime.print(`IndexedDB not available: ${err.message}`);
					} else {
						BradotFS._idbfs = true;
					}
					resolve(err);
				});
			});
		},

		// Deinit bradot file system, making sure to unmount file systems, and close IDBFS(s).
		deinit: function () {
			BradotFS._mount_points.forEach(function (path) {
				try {
					FS.unmount(path);
				} catch (e) {
					BradotRuntime.print('Already unmounted', e);
				}
				if (BradotFS._idbfs && IDBFS.dbs[path]) {
					IDBFS.dbs[path].close();
					delete IDBFS.dbs[path];
				}
			});
			BradotFS._mount_points = [];
			BradotFS._idbfs = false;
			BradotFS._syncing = false;
		},

		sync: function () {
			if (BradotFS._syncing) {
				BradotRuntime.error('Already syncing!');
				return Promise.resolve();
			}
			BradotFS._syncing = true;
			return new Promise(function (resolve, reject) {
				FS.syncfs(false, function (error) {
					if (error) {
						BradotRuntime.error(`Failed to save IDB file system: ${error.message}`);
					}
					BradotFS._syncing = false;
					resolve(error);
				});
			});
		},

		// Copies a buffer to the internal file system. Creating directories recursively.
		copy_to_fs: function (path, buffer) {
			const idx = path.lastIndexOf('/');
			let dir = '/';
			if (idx > 0) {
				dir = path.slice(0, idx);
			}
			try {
				FS.stat(dir);
			} catch (e) {
				if (e.errno !== BradotFS.ENOENT) {
					// Let mkdirTree throw in case, we cannot trust the above check.
					BradotRuntime.error(e);
				}
				FS.mkdirTree(dir);
			}
			FS.writeFile(path, new Uint8Array(buffer));
		},
	},
};
mergeInto(LibraryManager.library, BradotFS);

const BradotOS = {
	$BradotOS__deps: ['$BradotRuntime', '$BradotConfig', '$BradotFS'],
	$BradotOS__postset: [
		'Module["request_quit"] = function() { BradotOS.request_quit() };',
		'Module["onExit"] = BradotOS.cleanup;',
		'BradotOS._fs_sync_promise = Promise.resolve();',
	].join(''),
	$BradotOS: {
		request_quit: function () {},
		_async_cbs: [],
		_fs_sync_promise: null,

		atexit: function (p_promise_cb) {
			BradotOS._async_cbs.push(p_promise_cb);
		},

		cleanup: function (exit_code) {
			const cb = BradotConfig.on_exit;
			BradotFS.deinit();
			BradotConfig.clear();
			if (cb) {
				cb(exit_code);
			}
		},

		finish_async: function (callback) {
			BradotOS._fs_sync_promise.then(function (err) {
				const promises = [];
				BradotOS._async_cbs.forEach(function (cb) {
					promises.push(new Promise(cb));
				});
				return Promise.all(promises);
			}).then(function () {
				return BradotFS.sync(); // Final FS sync.
			}).then(function (err) {
				// Always deferred.
				setTimeout(function () {
					callback();
				}, 0);
			});
		},
	},

	bradot_js_os_finish_async__proxy: 'sync',
	bradot_js_os_finish_async__sig: 'vi',
	bradot_js_os_finish_async: function (p_callback) {
		const func = BradotRuntime.get_func(p_callback);
		BradotOS.finish_async(func);
	},

	bradot_js_os_request_quit_cb__proxy: 'sync',
	bradot_js_os_request_quit_cb__sig: 'vi',
	bradot_js_os_request_quit_cb: function (p_callback) {
		BradotOS.request_quit = BradotRuntime.get_func(p_callback);
	},

	bradot_js_os_fs_is_persistent__proxy: 'sync',
	bradot_js_os_fs_is_persistent__sig: 'i',
	bradot_js_os_fs_is_persistent: function () {
		return BradotFS.is_persistent();
	},

	bradot_js_os_fs_sync__proxy: 'sync',
	bradot_js_os_fs_sync__sig: 'vi',
	bradot_js_os_fs_sync: function (callback) {
		const func = BradotRuntime.get_func(callback);
		BradotOS._fs_sync_promise = BradotFS.sync();
		BradotOS._fs_sync_promise.then(function (err) {
			func();
		});
	},

	bradot_js_os_has_feature__proxy: 'sync',
	bradot_js_os_has_feature__sig: 'ii',
	bradot_js_os_has_feature: function (p_ftr) {
		const ftr = BradotRuntime.parseString(p_ftr);
		const ua = navigator.userAgent;
		if (ftr === 'web_macos') {
			return (ua.indexOf('Mac') !== -1) ? 1 : 0;
		}
		if (ftr === 'web_windows') {
			return (ua.indexOf('Windows') !== -1) ? 1 : 0;
		}
		if (ftr === 'web_android') {
			return (ua.indexOf('Android') !== -1) ? 1 : 0;
		}
		if (ftr === 'web_ios') {
			return ((ua.indexOf('iPhone') !== -1) || (ua.indexOf('iPad') !== -1) || (ua.indexOf('iPod') !== -1)) ? 1 : 0;
		}
		if (ftr === 'web_linuxbsd') {
			return ((ua.indexOf('CrOS') !== -1) || (ua.indexOf('BSD') !== -1) || (ua.indexOf('Linux') !== -1) || (ua.indexOf('X11') !== -1)) ? 1 : 0;
		}
		return 0;
	},

	bradot_js_os_execute__proxy: 'sync',
	bradot_js_os_execute__sig: 'ii',
	bradot_js_os_execute: function (p_json) {
		const json_args = BradotRuntime.parseString(p_json);
		const args = JSON.parse(json_args);
		if (BradotConfig.on_execute) {
			BradotConfig.on_execute(args);
			return 0;
		}
		return 1;
	},

	bradot_js_os_shell_open__proxy: 'sync',
	bradot_js_os_shell_open__sig: 'vi',
	bradot_js_os_shell_open: function (p_uri) {
		window.open(BradotRuntime.parseString(p_uri), '_blank');
	},

	bradot_js_os_hw_concurrency_get__proxy: 'sync',
	bradot_js_os_hw_concurrency_get__sig: 'i',
	bradot_js_os_hw_concurrency_get: function () {
		// TODO: Bradot core needs fixing to avoid spawning too many threads (> 24).
		const concurrency = navigator.hardwareConcurrency || 1;
		return concurrency < 2 ? concurrency : 2;
	},

	bradot_js_os_download_buffer__proxy: 'sync',
	bradot_js_os_download_buffer__sig: 'viiii',
	bradot_js_os_download_buffer: function (p_ptr, p_size, p_name, p_mime) {
		const buf = BradotRuntime.heapSlice(HEAP8, p_ptr, p_size);
		const name = BradotRuntime.parseString(p_name);
		const mime = BradotRuntime.parseString(p_mime);
		const blob = new Blob([buf], { type: mime });
		const url = window.URL.createObjectURL(blob);
		const a = document.createElement('a');
		a.href = url;
		a.download = name;
		a.style.display = 'none';
		document.body.appendChild(a);
		a.click();
		a.remove();
		window.URL.revokeObjectURL(url);
	},
};

autoAddDeps(BradotOS, '$BradotOS');
mergeInto(LibraryManager.library, BradotOS);

/*
 * Bradot event listeners.
 * Keeps track of registered event listeners so it can remove them on shutdown.
 */
const BradotEventListeners = {
	$BradotEventListeners__deps: ['$BradotOS'],
	$BradotEventListeners__postset: 'BradotOS.atexit(function(resolve, reject) { BradotEventListeners.clear(); resolve(); });',
	$BradotEventListeners: {
		handlers: [],

		has: function (target, event, method, capture) {
			return BradotEventListeners.handlers.findIndex(function (e) {
				return e.target === target && e.event === event && e.method === method && e.capture === capture;
			}) !== -1;
		},

		add: function (target, event, method, capture) {
			if (BradotEventListeners.has(target, event, method, capture)) {
				return;
			}
			function Handler(p_target, p_event, p_method, p_capture) {
				this.target = p_target;
				this.event = p_event;
				this.method = p_method;
				this.capture = p_capture;
			}
			BradotEventListeners.handlers.push(new Handler(target, event, method, capture));
			target.addEventListener(event, method, capture);
		},

		clear: function () {
			BradotEventListeners.handlers.forEach(function (h) {
				h.target.removeEventListener(h.event, h.method, h.capture);
			});
			BradotEventListeners.handlers.length = 0;
		},
	},
};
mergeInto(LibraryManager.library, BradotEventListeners);

const BradotPWA = {

	$BradotPWA__deps: ['$BradotRuntime', '$BradotEventListeners'],
	$BradotPWA: {
		hasUpdate: false,

		updateState: function (cb, reg) {
			if (!reg) {
				return;
			}
			if (!reg.active) {
				return;
			}
			if (reg.waiting) {
				BradotPWA.hasUpdate = true;
				cb();
			}
			BradotEventListeners.add(reg, 'updatefound', function () {
				const installing = reg.installing;
				BradotEventListeners.add(installing, 'statechange', function () {
					if (installing.state === 'installed') {
						BradotPWA.hasUpdate = true;
						cb();
					}
				});
			});
		},
	},

	bradot_js_pwa_cb__proxy: 'sync',
	bradot_js_pwa_cb__sig: 'vi',
	bradot_js_pwa_cb: function (p_update_cb) {
		if ('serviceWorker' in navigator) {
			const cb = BradotRuntime.get_func(p_update_cb);
			navigator.serviceWorker.getRegistration().then(BradotPWA.updateState.bind(null, cb));
		}
	},

	bradot_js_pwa_update__proxy: 'sync',
	bradot_js_pwa_update__sig: 'i',
	bradot_js_pwa_update: function () {
		if ('serviceWorker' in navigator && BradotPWA.hasUpdate) {
			navigator.serviceWorker.getRegistration().then(function (reg) {
				if (!reg || !reg.waiting) {
					return;
				}
				reg.waiting.postMessage('update');
			});
			return 0;
		}
		return 1;
	},
};

autoAddDeps(BradotPWA, '$BradotPWA');
mergeInto(LibraryManager.library, BradotPWA);
