/**************************************************************************/
/*  BradotRenderer.java                                                    */
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

package org.bradotengine.bradot.gl;

import org.bradotengine.bradot.BradotLib;
import org.bradotengine.bradot.plugin.BradotPlugin;
import org.bradotengine.bradot.plugin.BradotPluginRegistry;

import android.util.Log;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Bradot's GL renderer implementation.
 */
public class BradotRenderer implements GLSurfaceView.Renderer {
	private final String TAG = BradotRenderer.class.getSimpleName();

	private final BradotPluginRegistry pluginRegistry;
	private boolean activityJustResumed = false;

	public BradotRenderer() {
		this.pluginRegistry = BradotPluginRegistry.getPluginRegistry();
	}

	public boolean onDrawFrame(GL10 gl) {
		if (activityJustResumed) {
			BradotLib.onRendererResumed();
			activityJustResumed = false;
		}

		boolean swapBuffers = BradotLib.step();
		for (BradotPlugin plugin : pluginRegistry.getAllPlugins()) {
			plugin.onGLDrawFrame(gl);
		}

		return swapBuffers;
	}

	@Override
	public void onRenderThreadExiting() {
		Log.d(TAG, "Destroying Bradot Engine");
		BradotLib.ondestroy();
	}

	public void onSurfaceChanged(GL10 gl, int width, int height) {
		BradotLib.resize(null, width, height);
		for (BradotPlugin plugin : pluginRegistry.getAllPlugins()) {
			plugin.onGLSurfaceChanged(gl, width, height);
		}
	}

	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		BradotLib.newcontext(null);
		for (BradotPlugin plugin : pluginRegistry.getAllPlugins()) {
			plugin.onGLSurfaceCreated(gl, config);
		}
	}

	public void onActivityResumed() {
		// We defer invoking BradotLib.onRendererResumed() until the first draw frame call.
		// This ensures we have a valid GL context and surface when we do so.
		activityJustResumed = true;
	}

	public void onActivityPaused() {
		BradotLib.onRendererPaused();
	}
}
