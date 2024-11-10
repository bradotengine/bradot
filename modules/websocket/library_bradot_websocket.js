/**************************************************************************/
/*  library_bradot_websocket.js                                            */
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

const BradotWebSocket = {
	// Our socket implementation that forwards events to C++.
	$BradotWebSocket__deps: ['$IDHandler', '$BradotRuntime'],
	$BradotWebSocket: {
		// Connection opened, report selected protocol
		_onopen: function (p_id, callback, event) {
			const ref = IDHandler.get(p_id);
			if (!ref) {
				return; // Bradot object is gone.
			}
			const c_str = BradotRuntime.allocString(ref.protocol);
			callback(c_str);
			BradotRuntime.free(c_str);
		},

		// Message received, report content and type (UTF8 vs binary)
		_onmessage: function (p_id, callback, event) {
			const ref = IDHandler.get(p_id);
			if (!ref) {
				return; // Bradot object is gone.
			}
			let buffer;
			let is_string = 0;
			if (event.data instanceof ArrayBuffer) {
				buffer = new Uint8Array(event.data);
			} else if (event.data instanceof Blob) {
				BradotRuntime.error('Blob type not supported');
				return;
			} else if (typeof event.data === 'string') {
				is_string = 1;
				const enc = new TextEncoder('utf-8');
				buffer = new Uint8Array(enc.encode(event.data));
			} else {
				BradotRuntime.error('Unknown message type');
				return;
			}
			const len = buffer.length * buffer.BYTES_PER_ELEMENT;
			const out = BradotRuntime.malloc(len);
			HEAPU8.set(buffer, out);
			callback(out, len, is_string);
			BradotRuntime.free(out);
		},

		// An error happened, 'onclose' will be called after this.
		_onerror: function (p_id, callback, event) {
			const ref = IDHandler.get(p_id);
			if (!ref) {
				return; // Bradot object is gone.
			}
			callback();
		},

		// Connection is closed, this is always fired. Report close code, reason, and clean status.
		_onclose: function (p_id, callback, event) {
			const ref = IDHandler.get(p_id);
			if (!ref) {
				return; // Bradot object is gone.
			}
			const c_str = BradotRuntime.allocString(event.reason);
			callback(event.code, c_str, event.wasClean ? 1 : 0);
			BradotRuntime.free(c_str);
		},

		// Send a message
		send: function (p_id, p_data) {
			const ref = IDHandler.get(p_id);
			if (!ref || ref.readyState !== ref.OPEN) {
				return 1; // Bradot object is gone or socket is not in a ready state.
			}
			ref.send(p_data);
			return 0;
		},

		// Get current bufferedAmount
		bufferedAmount: function (p_id) {
			const ref = IDHandler.get(p_id);
			if (!ref) {
				return 0; // Bradot object is gone.
			}
			return ref.bufferedAmount;
		},

		create: function (socket, p_on_open, p_on_message, p_on_error, p_on_close) {
			const id = IDHandler.add(socket);
			socket.onopen = BradotWebSocket._onopen.bind(null, id, p_on_open);
			socket.onmessage = BradotWebSocket._onmessage.bind(null, id, p_on_message);
			socket.onerror = BradotWebSocket._onerror.bind(null, id, p_on_error);
			socket.onclose = BradotWebSocket._onclose.bind(null, id, p_on_close);
			return id;
		},

		// Closes the JavaScript WebSocket (if not already closing) associated to a given C++ object.
		close: function (p_id, p_code, p_reason) {
			const ref = IDHandler.get(p_id);
			if (ref && ref.readyState < ref.CLOSING) {
				const code = p_code;
				const reason = p_reason;
				ref.close(code, reason);
			}
		},

		// Deletes the reference to a C++ object (closing any connected socket if necessary).
		destroy: function (p_id) {
			const ref = IDHandler.get(p_id);
			if (!ref) {
				return;
			}
			BradotWebSocket.close(p_id, 3001, 'destroyed');
			IDHandler.remove(p_id);
			ref.onopen = null;
			ref.onmessage = null;
			ref.onerror = null;
			ref.onclose = null;
		},
	},

	bradot_js_websocket_create__proxy: 'sync',
	bradot_js_websocket_create__sig: 'iiiiiiii',
	bradot_js_websocket_create: function (p_ref, p_url, p_proto, p_on_open, p_on_message, p_on_error, p_on_close) {
		const on_open = BradotRuntime.get_func(p_on_open).bind(null, p_ref);
		const on_message = BradotRuntime.get_func(p_on_message).bind(null, p_ref);
		const on_error = BradotRuntime.get_func(p_on_error).bind(null, p_ref);
		const on_close = BradotRuntime.get_func(p_on_close).bind(null, p_ref);
		const url = BradotRuntime.parseString(p_url);
		const protos = BradotRuntime.parseString(p_proto);
		let socket = null;
		try {
			if (protos) {
				socket = new WebSocket(url, protos.split(','));
			} else {
				socket = new WebSocket(url);
			}
		} catch (e) {
			return 0;
		}
		socket.binaryType = 'arraybuffer';
		return BradotWebSocket.create(socket, on_open, on_message, on_error, on_close);
	},

	bradot_js_websocket_send__proxy: 'sync',
	bradot_js_websocket_send__sig: 'iiiii',
	bradot_js_websocket_send: function (p_id, p_buf, p_buf_len, p_raw) {
		const bytes_array = new Uint8Array(p_buf_len);
		let i = 0;
		for (i = 0; i < p_buf_len; i++) {
			bytes_array[i] = BradotRuntime.getHeapValue(p_buf + i, 'i8');
		}
		let out = bytes_array.buffer;
		if (!p_raw) {
			out = new TextDecoder('utf-8').decode(bytes_array);
		}
		return BradotWebSocket.send(p_id, out);
	},

	bradot_js_websocket_buffered_amount__proxy: 'sync',
	bradot_js_websocket_buffered_amount__sig: 'ii',
	bradot_js_websocket_buffered_amount: function (p_id) {
		return BradotWebSocket.bufferedAmount(p_id);
	},

	bradot_js_websocket_close__proxy: 'sync',
	bradot_js_websocket_close__sig: 'viii',
	bradot_js_websocket_close: function (p_id, p_code, p_reason) {
		const code = p_code;
		const reason = BradotRuntime.parseString(p_reason);
		BradotWebSocket.close(p_id, code, reason);
	},

	bradot_js_websocket_destroy__proxy: 'sync',
	bradot_js_websocket_destroy__sig: 'vi',
	bradot_js_websocket_destroy: function (p_id) {
		BradotWebSocket.destroy(p_id);
	},
};

autoAddDeps(BradotWebSocket, '$BradotWebSocket');
mergeInto(LibraryManager.library, BradotWebSocket);
