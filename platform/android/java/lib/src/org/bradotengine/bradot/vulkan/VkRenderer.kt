/**************************************************************************/
/*  VkRenderer.kt                                                         */
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

@file:JvmName("VkRenderer")
package org.bradotengine.bradot.vulkan

import android.util.Log
import android.view.Surface
import org.bradotengine.bradot.BradotLib
import org.bradotengine.bradot.plugin.BradotPluginRegistry

/**
 * Responsible to setting up and driving the Vulkan rendering logic.
 *
 * <h3>Threading</h3>
 * The renderer will be called on a separate thread, so that rendering
 * performance is decoupled from the UI thread. Clients typically need to
 * communicate with the renderer from the UI thread, because that's where
 * input events are received. Clients can communicate using any of the
 * standard Java techniques for cross-thread communication, or they can
 * use the  [VkSurfaceView.queueOnVkThread] convenience method.
 *
 * @see [VkSurfaceView.startRenderer]
 */
internal class VkRenderer {

	companion object {
		private val TAG = VkRenderer::class.java.simpleName
	}

	private val pluginRegistry: BradotPluginRegistry = BradotPluginRegistry.getPluginRegistry()

	/**
	 * Called when the surface is created and signals the beginning of rendering.
	 */
	fun onVkSurfaceCreated(surface: Surface) {
		BradotLib.newcontext(surface)

		for (plugin in pluginRegistry.getAllPlugins()) {
			plugin.onVkSurfaceCreated(surface)
		}
	}

	/**
	 * Called after the surface is created and whenever its size changes.
	 */
	fun onVkSurfaceChanged(surface: Surface, width: Int, height: Int) {
		BradotLib.resize(surface, width, height)

		for (plugin in pluginRegistry.getAllPlugins()) {
			plugin.onVkSurfaceChanged(surface, width, height)
		}
	}

	/**
	 * Called to draw the current frame.
	 */
	fun onVkDrawFrame() {
		BradotLib.step()
		for (plugin in pluginRegistry.getAllPlugins()) {
			plugin.onVkDrawFrame()
		}
	}

	/**
	 * Called when the rendering thread is resumed.
	 */
	fun onVkResume() {
		BradotLib.onRendererResumed()
	}

	/**
	 * Called when the rendering thread is paused.
	 */
	fun onVkPause() {
		BradotLib.onRendererPaused()
	}

	/**
	 * Invoked when the render thread is in the process of shutting down.
	 */
	fun onRenderThreadExiting() {
		Log.d(TAG, "Destroying Bradot Engine")
		BradotLib.ondestroy()
	}
}
