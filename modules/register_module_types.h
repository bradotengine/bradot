/**************************************************************************/
/*  register_module_types.h                                               */
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

#ifndef REGISTER_MODULE_TYPES_H
#define REGISTER_MODULE_TYPES_H

#include "core/extension/brextension_interface.h"

enum ModuleInitializationLevel {
	MODULE_INITIALIZATION_LEVEL_CORE = BREXTENSION_INITIALIZATION_CORE,
	MODULE_INITIALIZATION_LEVEL_SERVERS = BREXTENSION_INITIALIZATION_SERVERS,
	MODULE_INITIALIZATION_LEVEL_SCENE = BREXTENSION_INITIALIZATION_SCENE,
	MODULE_INITIALIZATION_LEVEL_EDITOR = BREXTENSION_INITIALIZATION_EDITOR
};

void initialize_modules(ModuleInitializationLevel p_level);
void uninitialize_modules(ModuleInitializationLevel p_level);

#endif // REGISTER_MODULE_TYPES_H
