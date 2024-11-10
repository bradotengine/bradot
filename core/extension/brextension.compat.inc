/**************************************************************************/
/*  brextension.compat.inc                                                */
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

#ifndef DISABLE_DEPRECATED

Error BRExtension::_open_library_bind_compat_88418(const String &p_path, const String &p_entry_symbol) {
	return ERR_UNAVAILABLE;
}

void BRExtension::_close_library_bind_compat_88418() {
}

void BRExtension::_initialize_library_bind_compat_88418(InitializationLevel p_level) {
}

void BRExtension::_bind_compatibility_methods() {
	ClassDB::bind_compatibility_method(D_METHOD("open_library", "path", "entry_symbol"), &BRExtension::_open_library_bind_compat_88418);
	ClassDB::bind_compatibility_method(D_METHOD("close_library"), &BRExtension::_close_library_bind_compat_88418);
	ClassDB::bind_compatibility_method(D_METHOD("initialize_library", "level"), &BRExtension::_initialize_library_bind_compat_88418);
}

#endif
