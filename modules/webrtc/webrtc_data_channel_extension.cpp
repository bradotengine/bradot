/**************************************************************************/
/*  webrtc_data_channel_extension.cpp                                     */
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

#include "webrtc_data_channel_extension.h"

void WebRTCDataChannelExtension::_bind_methods() {
	ADD_PROPERTY_DEFAULT("write_mode", WRITE_MODE_BINARY);

	BRVIRTUAL_BIND(_get_packet, "r_buffer", "r_buffer_size");
	BRVIRTUAL_BIND(_put_packet, "p_buffer", "p_buffer_size");
	BRVIRTUAL_BIND(_get_available_packet_count);
	BRVIRTUAL_BIND(_get_max_packet_size);

	BRVIRTUAL_BIND(_poll);
	BRVIRTUAL_BIND(_close);

	BRVIRTUAL_BIND(_set_write_mode, "p_write_mode");
	BRVIRTUAL_BIND(_get_write_mode);

	BRVIRTUAL_BIND(_was_string_packet);
	BRVIRTUAL_BIND(_get_ready_state);
	BRVIRTUAL_BIND(_get_label);
	BRVIRTUAL_BIND(_is_ordered);
	BRVIRTUAL_BIND(_get_id);
	BRVIRTUAL_BIND(_get_max_packet_life_time);
	BRVIRTUAL_BIND(_get_max_retransmits);
	BRVIRTUAL_BIND(_get_protocol);
	BRVIRTUAL_BIND(_is_negotiated);
	BRVIRTUAL_BIND(_get_buffered_amount);
}

Error WebRTCDataChannelExtension::get_packet(const uint8_t **r_buffer, int &r_buffer_size) {
	Error err;
	if (BRVIRTUAL_CALL(_get_packet, r_buffer, &r_buffer_size, err)) {
		return err;
	}
	WARN_PRINT_ONCE("WebRTCDataChannelExtension::_get_packet_native is unimplemented!");
	return FAILED;
}

Error WebRTCDataChannelExtension::put_packet(const uint8_t *p_buffer, int p_buffer_size) {
	Error err;
	if (BRVIRTUAL_CALL(_put_packet, p_buffer, p_buffer_size, err)) {
		return err;
	}
	WARN_PRINT_ONCE("WebRTCDataChannelExtension::_put_packet_native is unimplemented!");
	return FAILED;
}
