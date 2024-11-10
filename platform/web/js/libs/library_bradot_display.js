/**************************************************************************/
/*  library_bradot_display.js                                              */
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

const BradotDisplayVK = {

	$BradotDisplayVK__deps: ['$BradotRuntime', '$BradotConfig', '$BradotEventListeners'],
	$BradotDisplayVK__postset: 'BradotOS.atexit(function(resolve, reject) { BradotDisplayVK.clear(); resolve(); });',
	$BradotDisplayVK: {
		textinput: null,
		textarea: null,

		available: function () {
			return BradotConfig.virtual_keyboard && 'ontouchstart' in window;
		},

		init: function (input_cb) {
			function create(what) {
				const elem = document.createElement(what);
				elem.style.display = 'none';
				elem.style.position = 'absolute';
				elem.style.zIndex = '-1';
				elem.style.background = 'transparent';
				elem.style.padding = '0px';
				elem.style.margin = '0px';
				elem.style.overflow = 'hidden';
				elem.style.width = '0px';
				elem.style.height = '0px';
				elem.style.border = '0px';
				elem.style.outline = 'none';
				elem.readonly = true;
				elem.disabled = true;
				BradotEventListeners.add(elem, 'input', function (evt) {
					const c_str = BradotRuntime.allocString(elem.value);
					input_cb(c_str, elem.selectionEnd);
					BradotRuntime.free(c_str);
				}, false);
				BradotEventListeners.add(elem, 'blur', function (evt) {
					elem.style.display = 'none';
					elem.readonly = true;
					elem.disabled = true;
				}, false);
				BradotConfig.canvas.insertAdjacentElement('beforebegin', elem);
				return elem;
			}
			BradotDisplayVK.textinput = create('input');
			BradotDisplayVK.textarea = create('textarea');
			BradotDisplayVK.updateSize();
		},
		show: function (text, type, start, end) {
			if (!BradotDisplayVK.textinput || !BradotDisplayVK.textarea) {
				return;
			}
			if (BradotDisplayVK.textinput.style.display !== '' || BradotDisplayVK.textarea.style.display !== '') {
				BradotDisplayVK.hide();
			}
			BradotDisplayVK.updateSize();

			let elem = BradotDisplayVK.textinput;
			switch (type) {
			case 0: // KEYBOARD_TYPE_DEFAULT
				elem.type = 'text';
				elem.inputmode = '';
				break;
			case 1: // KEYBOARD_TYPE_MULTILINE
				elem = BradotDisplayVK.textarea;
				break;
			case 2: // KEYBOARD_TYPE_NUMBER
				elem.type = 'text';
				elem.inputmode = 'numeric';
				break;
			case 3: // KEYBOARD_TYPE_NUMBER_DECIMAL
				elem.type = 'text';
				elem.inputmode = 'decimal';
				break;
			case 4: // KEYBOARD_TYPE_PHONE
				elem.type = 'tel';
				elem.inputmode = '';
				break;
			case 5: // KEYBOARD_TYPE_EMAIL_ADDRESS
				elem.type = 'email';
				elem.inputmode = '';
				break;
			case 6: // KEYBOARD_TYPE_PASSWORD
				elem.type = 'password';
				elem.inputmode = '';
				break;
			case 7: // KEYBOARD_TYPE_URL
				elem.type = 'url';
				elem.inputmode = '';
				break;
			default:
				elem.type = 'text';
				elem.inputmode = '';
				break;
			}

			elem.readonly = false;
			elem.disabled = false;
			elem.value = text;
			elem.style.display = 'block';
			elem.focus();
			elem.setSelectionRange(start, end);
		},
		hide: function () {
			if (!BradotDisplayVK.textinput || !BradotDisplayVK.textarea) {
				return;
			}
			[BradotDisplayVK.textinput, BradotDisplayVK.textarea].forEach(function (elem) {
				elem.blur();
				elem.style.display = 'none';
				elem.value = '';
			});
		},
		updateSize: function () {
			if (!BradotDisplayVK.textinput || !BradotDisplayVK.textarea) {
				return;
			}
			const rect = BradotConfig.canvas.getBoundingClientRect();
			function update(elem) {
				elem.style.left = `${rect.left}px`;
				elem.style.top = `${rect.top}px`;
				elem.style.width = `${rect.width}px`;
				elem.style.height = `${rect.height}px`;
			}
			update(BradotDisplayVK.textinput);
			update(BradotDisplayVK.textarea);
		},
		clear: function () {
			if (BradotDisplayVK.textinput) {
				BradotDisplayVK.textinput.remove();
				BradotDisplayVK.textinput = null;
			}
			if (BradotDisplayVK.textarea) {
				BradotDisplayVK.textarea.remove();
				BradotDisplayVK.textarea = null;
			}
		},
	},
};
mergeInto(LibraryManager.library, BradotDisplayVK);

/*
 * Display server cursor helper.
 * Keeps track of cursor status and custom shapes.
 */
const BradotDisplayCursor = {
	$BradotDisplayCursor__deps: ['$BradotOS', '$BradotConfig'],
	$BradotDisplayCursor__postset: 'BradotOS.atexit(function(resolve, reject) { BradotDisplayCursor.clear(); resolve(); });',
	$BradotDisplayCursor: {
		shape: 'default',
		visible: true,
		cursors: {},
		set_style: function (style) {
			BradotConfig.canvas.style.cursor = style;
		},
		set_shape: function (shape) {
			BradotDisplayCursor.shape = shape;
			let css = shape;
			if (shape in BradotDisplayCursor.cursors) {
				const c = BradotDisplayCursor.cursors[shape];
				css = `url("${c.url}") ${c.x} ${c.y}, default`;
			}
			if (BradotDisplayCursor.visible) {
				BradotDisplayCursor.set_style(css);
			}
		},
		clear: function () {
			BradotDisplayCursor.set_style('');
			BradotDisplayCursor.shape = 'default';
			BradotDisplayCursor.visible = true;
			Object.keys(BradotDisplayCursor.cursors).forEach(function (key) {
				URL.revokeObjectURL(BradotDisplayCursor.cursors[key]);
				delete BradotDisplayCursor.cursors[key];
			});
		},
		lockPointer: function () {
			const canvas = BradotConfig.canvas;
			if (canvas.requestPointerLock) {
				canvas.requestPointerLock();
			}
		},
		releasePointer: function () {
			if (document.exitPointerLock) {
				document.exitPointerLock();
			}
		},
		isPointerLocked: function () {
			return document.pointerLockElement === BradotConfig.canvas;
		},
	},
};
mergeInto(LibraryManager.library, BradotDisplayCursor);

const BradotDisplayScreen = {
	$BradotDisplayScreen__deps: ['$BradotConfig', '$BradotOS', '$GL', 'emscripten_webgl_get_current_context'],
	$BradotDisplayScreen: {
		desired_size: [0, 0],
		hidpi: true,
		getPixelRatio: function () {
			return BradotDisplayScreen.hidpi ? window.devicePixelRatio || 1 : 1;
		},
		isFullscreen: function () {
			const elem = document.fullscreenElement || document.mozFullscreenElement
				|| document.webkitFullscreenElement || document.msFullscreenElement;
			if (elem) {
				return elem === BradotConfig.canvas;
			}
			// But maybe knowing the element is not supported.
			return document.fullscreen || document.mozFullScreen
				|| document.webkitIsFullscreen;
		},
		hasFullscreen: function () {
			return document.fullscreenEnabled || document.mozFullScreenEnabled
				|| document.webkitFullscreenEnabled;
		},
		requestFullscreen: function () {
			if (!BradotDisplayScreen.hasFullscreen()) {
				return 1;
			}
			const canvas = BradotConfig.canvas;
			try {
				const promise = (canvas.requestFullscreen || canvas.msRequestFullscreen
					|| canvas.mozRequestFullScreen || canvas.mozRequestFullscreen
					|| canvas.webkitRequestFullscreen
				).call(canvas);
				// Some browsers (Safari) return undefined.
				// For the standard ones, we need to catch it.
				if (promise) {
					promise.catch(function () {
						// nothing to do.
					});
				}
			} catch (e) {
				return 1;
			}
			return 0;
		},
		exitFullscreen: function () {
			if (!BradotDisplayScreen.isFullscreen()) {
				return 0;
			}
			try {
				const promise = document.exitFullscreen();
				if (promise) {
					promise.catch(function () {
						// nothing to do.
					});
				}
			} catch (e) {
				return 1;
			}
			return 0;
		},
		_updateGL: function () {
			const gl_context_handle = _emscripten_webgl_get_current_context();
			const gl = GL.getContext(gl_context_handle);
			if (gl) {
				GL.resizeOffscreenFramebuffer(gl);
			}
		},
		updateSize: function () {
			const isFullscreen = BradotDisplayScreen.isFullscreen();
			const wantsFullWindow = BradotConfig.canvas_resize_policy === 2;
			const noResize = BradotConfig.canvas_resize_policy === 0;
			const dWidth = BradotDisplayScreen.desired_size[0];
			const dHeight = BradotDisplayScreen.desired_size[1];
			const canvas = BradotConfig.canvas;
			let width = dWidth;
			let height = dHeight;
			if (noResize) {
				// Don't resize canvas, just update GL if needed.
				if (canvas.width !== width || canvas.height !== height) {
					BradotDisplayScreen.desired_size = [canvas.width, canvas.height];
					BradotDisplayScreen._updateGL();
					return 1;
				}
				return 0;
			}
			const scale = BradotDisplayScreen.getPixelRatio();
			if (isFullscreen || wantsFullWindow) {
				// We need to match screen size.
				width = window.innerWidth * scale;
				height = window.innerHeight * scale;
			}
			const csw = `${width / scale}px`;
			const csh = `${height / scale}px`;
			if (canvas.style.width !== csw || canvas.style.height !== csh || canvas.width !== width || canvas.height !== height) {
				// Size doesn't match.
				// Resize canvas, set correct CSS pixel size, update GL.
				canvas.width = width;
				canvas.height = height;
				canvas.style.width = csw;
				canvas.style.height = csh;
				BradotDisplayScreen._updateGL();
				return 1;
			}
			return 0;
		},
	},
};
mergeInto(LibraryManager.library, BradotDisplayScreen);

/**
 * Display server interface.
 *
 * Exposes all the functions needed by DisplayServer implementation.
 */
const BradotDisplay = {
	$BradotDisplay__deps: ['$BradotConfig', '$BradotRuntime', '$BradotDisplayCursor', '$BradotEventListeners', '$BradotDisplayScreen', '$BradotDisplayVK'],
	$BradotDisplay: {
		window_icon: '',
		getDPI: function () {
			// devicePixelRatio is given in dppx
			// https://drafts.csswg.org/css-values/#resolution
			// > due to the 1:96 fixed ratio of CSS *in* to CSS *px*, 1dppx is equivalent to 96dpi.
			const dpi = Math.round(window.devicePixelRatio * 96);
			return dpi >= 96 ? dpi : 96;
		},
	},

	bradot_js_display_is_swap_ok_cancel__proxy: 'sync',
	bradot_js_display_is_swap_ok_cancel__sig: 'i',
	bradot_js_display_is_swap_ok_cancel: function () {
		const win = (['Windows', 'Win64', 'Win32', 'WinCE']);
		const plat = navigator.platform || '';
		if (win.indexOf(plat) !== -1) {
			return 1;
		}
		return 0;
	},

	bradot_js_tts_is_speaking__proxy: 'sync',
	bradot_js_tts_is_speaking__sig: 'i',
	bradot_js_tts_is_speaking: function () {
		return window.speechSynthesis.speaking;
	},

	bradot_js_tts_is_paused__proxy: 'sync',
	bradot_js_tts_is_paused__sig: 'i',
	bradot_js_tts_is_paused: function () {
		return window.speechSynthesis.paused;
	},

	bradot_js_tts_get_voices__proxy: 'sync',
	bradot_js_tts_get_voices__sig: 'vi',
	bradot_js_tts_get_voices: function (p_callback) {
		const func = BradotRuntime.get_func(p_callback);
		try {
			const arr = [];
			const voices = window.speechSynthesis.getVoices();
			for (let i = 0; i < voices.length; i++) {
				arr.push(`${voices[i].lang};${voices[i].name}`);
			}
			const c_ptr = BradotRuntime.allocStringArray(arr);
			func(arr.length, c_ptr);
			BradotRuntime.freeStringArray(c_ptr, arr.length);
		} catch (e) {
			// Fail graciously.
		}
	},

	bradot_js_tts_speak__proxy: 'sync',
	bradot_js_tts_speak__sig: 'viiiffii',
	bradot_js_tts_speak: function (p_text, p_voice, p_volume, p_pitch, p_rate, p_utterance_id, p_callback) {
		const func = BradotRuntime.get_func(p_callback);

		function listener_end(evt) {
			evt.currentTarget.cb(1 /* TTS_UTTERANCE_ENDED */, evt.currentTarget.id, 0);
		}

		function listener_start(evt) {
			evt.currentTarget.cb(0 /* TTS_UTTERANCE_STARTED */, evt.currentTarget.id, 0);
		}

		function listener_error(evt) {
			evt.currentTarget.cb(2 /* TTS_UTTERANCE_CANCELED */, evt.currentTarget.id, 0);
		}

		function listener_bound(evt) {
			evt.currentTarget.cb(3 /* TTS_UTTERANCE_BOUNDARY */, evt.currentTarget.id, evt.charIndex);
		}

		const utterance = new SpeechSynthesisUtterance(BradotRuntime.parseString(p_text));
		utterance.rate = p_rate;
		utterance.pitch = p_pitch;
		utterance.volume = p_volume / 100.0;
		utterance.addEventListener('end', listener_end);
		utterance.addEventListener('start', listener_start);
		utterance.addEventListener('error', listener_error);
		utterance.addEventListener('boundary', listener_bound);
		utterance.id = p_utterance_id;
		utterance.cb = func;
		const voice = BradotRuntime.parseString(p_voice);
		const voices = window.speechSynthesis.getVoices();
		for (let i = 0; i < voices.length; i++) {
			if (voices[i].name === voice) {
				utterance.voice = voices[i];
				break;
			}
		}
		window.speechSynthesis.resume();
		window.speechSynthesis.speak(utterance);
	},

	bradot_js_tts_pause__proxy: 'sync',
	bradot_js_tts_pause__sig: 'v',
	bradot_js_tts_pause: function () {
		window.speechSynthesis.pause();
	},

	bradot_js_tts_resume__proxy: 'sync',
	bradot_js_tts_resume__sig: 'v',
	bradot_js_tts_resume: function () {
		window.speechSynthesis.resume();
	},

	bradot_js_tts_stop__proxy: 'sync',
	bradot_js_tts_stop__sig: 'v',
	bradot_js_tts_stop: function () {
		window.speechSynthesis.cancel();
		window.speechSynthesis.resume();
	},

	bradot_js_display_alert__proxy: 'sync',
	bradot_js_display_alert__sig: 'vi',
	bradot_js_display_alert: function (p_text) {
		window.alert(BradotRuntime.parseString(p_text)); // eslint-disable-line no-alert
	},

	bradot_js_display_screen_dpi_get__proxy: 'sync',
	bradot_js_display_screen_dpi_get__sig: 'i',
	bradot_js_display_screen_dpi_get: function () {
		return BradotDisplay.getDPI();
	},

	bradot_js_display_pixel_ratio_get__proxy: 'sync',
	bradot_js_display_pixel_ratio_get__sig: 'f',
	bradot_js_display_pixel_ratio_get: function () {
		return BradotDisplayScreen.getPixelRatio();
	},

	bradot_js_display_fullscreen_request__proxy: 'sync',
	bradot_js_display_fullscreen_request__sig: 'i',
	bradot_js_display_fullscreen_request: function () {
		return BradotDisplayScreen.requestFullscreen();
	},

	bradot_js_display_fullscreen_exit__proxy: 'sync',
	bradot_js_display_fullscreen_exit__sig: 'i',
	bradot_js_display_fullscreen_exit: function () {
		return BradotDisplayScreen.exitFullscreen();
	},

	bradot_js_display_desired_size_set__proxy: 'sync',
	bradot_js_display_desired_size_set__sig: 'vii',
	bradot_js_display_desired_size_set: function (width, height) {
		BradotDisplayScreen.desired_size = [width, height];
		BradotDisplayScreen.updateSize();
	},

	bradot_js_display_size_update__proxy: 'sync',
	bradot_js_display_size_update__sig: 'i',
	bradot_js_display_size_update: function () {
		const updated = BradotDisplayScreen.updateSize();
		if (updated) {
			BradotDisplayVK.updateSize();
		}
		return updated;
	},

	bradot_js_display_screen_size_get__proxy: 'sync',
	bradot_js_display_screen_size_get__sig: 'vii',
	bradot_js_display_screen_size_get: function (width, height) {
		const scale = BradotDisplayScreen.getPixelRatio();
		BradotRuntime.setHeapValue(width, window.screen.width * scale, 'i32');
		BradotRuntime.setHeapValue(height, window.screen.height * scale, 'i32');
	},

	bradot_js_display_window_size_get__proxy: 'sync',
	bradot_js_display_window_size_get__sig: 'vii',
	bradot_js_display_window_size_get: function (p_width, p_height) {
		BradotRuntime.setHeapValue(p_width, BradotConfig.canvas.width, 'i32');
		BradotRuntime.setHeapValue(p_height, BradotConfig.canvas.height, 'i32');
	},

	bradot_js_display_has_webgl__proxy: 'sync',
	bradot_js_display_has_webgl__sig: 'ii',
	bradot_js_display_has_webgl: function (p_version) {
		if (p_version !== 1 && p_version !== 2) {
			return false;
		}
		try {
			return !!document.createElement('canvas').getContext(p_version === 2 ? 'webgl2' : 'webgl');
		} catch (e) { /* Not available */ }
		return false;
	},

	/*
	 * Canvas
	 */
	bradot_js_display_canvas_focus__proxy: 'sync',
	bradot_js_display_canvas_focus__sig: 'v',
	bradot_js_display_canvas_focus: function () {
		BradotConfig.canvas.focus();
	},

	bradot_js_display_canvas_is_focused__proxy: 'sync',
	bradot_js_display_canvas_is_focused__sig: 'i',
	bradot_js_display_canvas_is_focused: function () {
		return document.activeElement === BradotConfig.canvas;
	},

	/*
	 * Touchscreen
	 */
	bradot_js_display_touchscreen_is_available__proxy: 'sync',
	bradot_js_display_touchscreen_is_available__sig: 'i',
	bradot_js_display_touchscreen_is_available: function () {
		return 'ontouchstart' in window;
	},

	/*
	 * Clipboard
	 */
	bradot_js_display_clipboard_set__proxy: 'sync',
	bradot_js_display_clipboard_set__sig: 'ii',
	bradot_js_display_clipboard_set: function (p_text) {
		const text = BradotRuntime.parseString(p_text);
		if (!navigator.clipboard || !navigator.clipboard.writeText) {
			return 1;
		}
		navigator.clipboard.writeText(text).catch(function (e) {
			// Setting OS clipboard is only possible from an input callback.
			BradotRuntime.error('Setting OS clipboard is only possible from an input callback for the Web platform. Exception:', e);
		});
		return 0;
	},

	bradot_js_display_clipboard_get__proxy: 'sync',
	bradot_js_display_clipboard_get__sig: 'ii',
	bradot_js_display_clipboard_get: function (callback) {
		const func = BradotRuntime.get_func(callback);
		try {
			navigator.clipboard.readText().then(function (result) {
				const ptr = BradotRuntime.allocString(result);
				func(ptr);
				BradotRuntime.free(ptr);
			}).catch(function (e) {
				// Fail graciously.
			});
		} catch (e) {
			// Fail graciously.
		}
	},

	/*
	 * Window
	 */
	bradot_js_display_window_title_set__proxy: 'sync',
	bradot_js_display_window_title_set__sig: 'vi',
	bradot_js_display_window_title_set: function (p_data) {
		document.title = BradotRuntime.parseString(p_data);
	},

	bradot_js_display_window_icon_set__proxy: 'sync',
	bradot_js_display_window_icon_set__sig: 'vii',
	bradot_js_display_window_icon_set: function (p_ptr, p_len) {
		let link = document.getElementById('-gd-engine-icon');
		const old_icon = BradotDisplay.window_icon;
		if (p_ptr) {
			if (link === null) {
				link = document.createElement('link');
				link.rel = 'icon';
				link.id = '-gd-engine-icon';
				document.head.appendChild(link);
			}
			const png = new Blob([BradotRuntime.heapSlice(HEAPU8, p_ptr, p_len)], { type: 'image/png' });
			BradotDisplay.window_icon = URL.createObjectURL(png);
			link.href = BradotDisplay.window_icon;
		} else {
			if (link) {
				link.remove();
			}
			BradotDisplay.window_icon = null;
		}
		if (old_icon) {
			URL.revokeObjectURL(old_icon);
		}
	},

	/*
	 * Cursor
	 */
	bradot_js_display_cursor_set_visible__proxy: 'sync',
	bradot_js_display_cursor_set_visible__sig: 'vi',
	bradot_js_display_cursor_set_visible: function (p_visible) {
		const visible = p_visible !== 0;
		if (visible === BradotDisplayCursor.visible) {
			return;
		}
		BradotDisplayCursor.visible = visible;
		if (visible) {
			BradotDisplayCursor.set_shape(BradotDisplayCursor.shape);
		} else {
			BradotDisplayCursor.set_style('none');
		}
	},

	bradot_js_display_cursor_is_hidden__proxy: 'sync',
	bradot_js_display_cursor_is_hidden__sig: 'i',
	bradot_js_display_cursor_is_hidden: function () {
		return !BradotDisplayCursor.visible;
	},

	bradot_js_display_cursor_set_shape__proxy: 'sync',
	bradot_js_display_cursor_set_shape__sig: 'vi',
	bradot_js_display_cursor_set_shape: function (p_string) {
		BradotDisplayCursor.set_shape(BradotRuntime.parseString(p_string));
	},

	bradot_js_display_cursor_set_custom_shape__proxy: 'sync',
	bradot_js_display_cursor_set_custom_shape__sig: 'viiiii',
	bradot_js_display_cursor_set_custom_shape: function (p_shape, p_ptr, p_len, p_hotspot_x, p_hotspot_y) {
		const shape = BradotRuntime.parseString(p_shape);
		const old_shape = BradotDisplayCursor.cursors[shape];
		if (p_len > 0) {
			const png = new Blob([BradotRuntime.heapSlice(HEAPU8, p_ptr, p_len)], { type: 'image/png' });
			const url = URL.createObjectURL(png);
			BradotDisplayCursor.cursors[shape] = {
				url: url,
				x: p_hotspot_x,
				y: p_hotspot_y,
			};
		} else {
			delete BradotDisplayCursor.cursors[shape];
		}
		if (shape === BradotDisplayCursor.shape) {
			BradotDisplayCursor.set_shape(BradotDisplayCursor.shape);
		}
		if (old_shape) {
			URL.revokeObjectURL(old_shape.url);
		}
	},

	bradot_js_display_cursor_lock_set__proxy: 'sync',
	bradot_js_display_cursor_lock_set__sig: 'vi',
	bradot_js_display_cursor_lock_set: function (p_lock) {
		if (p_lock) {
			BradotDisplayCursor.lockPointer();
		} else {
			BradotDisplayCursor.releasePointer();
		}
	},

	bradot_js_display_cursor_is_locked__proxy: 'sync',
	bradot_js_display_cursor_is_locked__sig: 'i',
	bradot_js_display_cursor_is_locked: function () {
		return BradotDisplayCursor.isPointerLocked() ? 1 : 0;
	},

	/*
	 * Listeners
	 */
	bradot_js_display_fullscreen_cb__proxy: 'sync',
	bradot_js_display_fullscreen_cb__sig: 'vi',
	bradot_js_display_fullscreen_cb: function (callback) {
		const canvas = BradotConfig.canvas;
		const func = BradotRuntime.get_func(callback);
		function change_cb(evt) {
			if (evt.target === canvas) {
				func(BradotDisplayScreen.isFullscreen());
			}
		}
		BradotEventListeners.add(document, 'fullscreenchange', change_cb, false);
		BradotEventListeners.add(document, 'mozfullscreenchange', change_cb, false);
		BradotEventListeners.add(document, 'webkitfullscreenchange', change_cb, false);
	},

	bradot_js_display_window_blur_cb__proxy: 'sync',
	bradot_js_display_window_blur_cb__sig: 'vi',
	bradot_js_display_window_blur_cb: function (callback) {
		const func = BradotRuntime.get_func(callback);
		BradotEventListeners.add(window, 'blur', function () {
			func();
		}, false);
	},

	bradot_js_display_notification_cb__proxy: 'sync',
	bradot_js_display_notification_cb__sig: 'viiiii',
	bradot_js_display_notification_cb: function (callback, p_enter, p_exit, p_in, p_out) {
		const canvas = BradotConfig.canvas;
		const func = BradotRuntime.get_func(callback);
		const notif = [p_enter, p_exit, p_in, p_out];
		['mouseover', 'mouseleave', 'focus', 'blur'].forEach(function (evt_name, idx) {
			BradotEventListeners.add(canvas, evt_name, function () {
				func(notif[idx]);
			}, true);
		});
	},

	bradot_js_display_setup_canvas__proxy: 'sync',
	bradot_js_display_setup_canvas__sig: 'viiii',
	bradot_js_display_setup_canvas: function (p_width, p_height, p_fullscreen, p_hidpi) {
		const canvas = BradotConfig.canvas;
		BradotEventListeners.add(canvas, 'contextmenu', function (ev) {
			ev.preventDefault();
		}, false);
		BradotEventListeners.add(canvas, 'webglcontextlost', function (ev) {
			alert('WebGL context lost, please reload the page'); // eslint-disable-line no-alert
			ev.preventDefault();
		}, false);
		BradotDisplayScreen.hidpi = !!p_hidpi;
		switch (BradotConfig.canvas_resize_policy) {
		case 0: // None
			BradotDisplayScreen.desired_size = [canvas.width, canvas.height];
			break;
		case 1: // Project
			BradotDisplayScreen.desired_size = [p_width, p_height];
			break;
		default: // Full window
			// Ensure we display in the right place, the size will be handled by updateSize
			canvas.style.position = 'absolute';
			canvas.style.top = 0;
			canvas.style.left = 0;
			break;
		}
		BradotDisplayScreen.updateSize();
		if (p_fullscreen) {
			BradotDisplayScreen.requestFullscreen();
		}
	},

	/*
	 * Virtual Keyboard
	 */
	bradot_js_display_vk_show__proxy: 'sync',
	bradot_js_display_vk_show__sig: 'viiii',
	bradot_js_display_vk_show: function (p_text, p_type, p_start, p_end) {
		const text = BradotRuntime.parseString(p_text);
		const start = p_start > 0 ? p_start : 0;
		const end = p_end > 0 ? p_end : start;
		BradotDisplayVK.show(text, p_type, start, end);
	},

	bradot_js_display_vk_hide__proxy: 'sync',
	bradot_js_display_vk_hide__sig: 'v',
	bradot_js_display_vk_hide: function () {
		BradotDisplayVK.hide();
	},

	bradot_js_display_vk_available__proxy: 'sync',
	bradot_js_display_vk_available__sig: 'i',
	bradot_js_display_vk_available: function () {
		return BradotDisplayVK.available();
	},

	bradot_js_display_tts_available__proxy: 'sync',
	bradot_js_display_tts_available__sig: 'i',
	bradot_js_display_tts_available: function () {
		return 'speechSynthesis' in window;
	},

	bradot_js_display_vk_cb__proxy: 'sync',
	bradot_js_display_vk_cb__sig: 'vi',
	bradot_js_display_vk_cb: function (p_input_cb) {
		const input_cb = BradotRuntime.get_func(p_input_cb);
		if (BradotDisplayVK.available()) {
			BradotDisplayVK.init(input_cb);
		}
	},
};

autoAddDeps(BradotDisplay, '$BradotDisplay');
mergeInto(LibraryManager.library, BradotDisplay);
