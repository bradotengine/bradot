/**************************************************************************/
/*  editor_resource_conversion_plugin.cpp                                 */
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

#include "editor_resource_conversion_plugin.h"

void EditorResourceConversionPlugin::_bind_methods() {
	BRVIRTUAL_BIND(_converts_to);
	BRVIRTUAL_BIND(_handles, "resource");
	BRVIRTUAL_BIND(_convert, "resource");
}

String EditorResourceConversionPlugin::converts_to() const {
	String ret;
	BRVIRTUAL_CALL(_converts_to, ret);
	return ret;
}

bool EditorResourceConversionPlugin::handles(const Ref<Resource> &p_resource) const {
	bool ret = false;
	BRVIRTUAL_CALL(_handles, p_resource, ret);
	return ret;
}

Ref<Resource> EditorResourceConversionPlugin::convert(const Ref<Resource> &p_resource) const {
	Ref<Resource> ret;
	BRVIRTUAL_CALL(_convert, p_resource, ret);
	return ret;
}