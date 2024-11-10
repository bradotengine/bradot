/**************************************************************************/
/*  BradotRenderView.java                                                  */
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

package org.bradotengine.bradot;

import org.bradotengine.bradot.input.BradotInputHandler;
import org.bradotengine.bradot.utils.DeviceUtils;

import android.view.SurfaceView;

public interface BradotRenderView {
	SurfaceView getView();

	/**
	 * Starts the thread that will drive Bradot's rendering.
	 */
	void startRenderer();

	/**
	 * Queues a runnable to be run on the rendering thread.
	 */
	void queueOnRenderThread(Runnable event);

	void onActivityPaused();

	void onActivityStopped();

	void onActivityResumed();

	void onActivityStarted();

	void onActivityDestroyed();

	BradotInputHandler getInputHandler();

	void configurePointerIcon(int pointerType, String imagePath, float hotSpotX, float hotSpotY);

	void setPointerIcon(int pointerType);

	/**
	 * @return true if pointer capture is supported.
	 */
	default boolean canCapturePointer() {
		// Pointer capture is not supported on Horizon OS
		return !DeviceUtils.isHorizonOSDevice() && getInputHandler().canCapturePointer();
	}
}
