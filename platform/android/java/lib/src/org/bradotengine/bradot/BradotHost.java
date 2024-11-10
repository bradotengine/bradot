/**************************************************************************/
/*  BradotHost.java                                                        */
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

import org.bradotengine.bradot.error.Error;
import org.bradotengine.bradot.plugin.BradotPlugin;

import android.app.Activity;

import androidx.annotation.NonNull;

import java.util.Collections;
import java.util.List;
import java.util.Set;

/**
 * Denotate a component (e.g: Activity, Fragment) that hosts the {@link Bradot} engine.
 */
public interface BradotHost {
	/**
	 * Provides a set of command line parameters to setup the {@link Bradot} engine.
	 */
	default List<String> getCommandLine() {
		return Collections.emptyList();
	}

	/**
	 * Invoked on the render thread when setup of the {@link Bradot} engine is complete.
	 */
	default void onBradotSetupCompleted() {}

	/**
	 * Invoked on the render thread when the {@link Bradot} engine main loop has started.
	 */
	default void onBradotMainLoopStarted() {}

	/**
	 * Invoked on the render thread to terminate the given {@link Bradot} engine instance.
	 */
	default void onBradotForceQuit(Bradot instance) {}

	/**
	 * Invoked on the render thread to terminate the {@link Bradot} engine instance with the given id.
	 * @param bradotInstanceId id of the Bradot instance to terminate. See {@code onNewBradotInstanceRequested}
	 *
	 * @return true if successful, false otherwise.
	 */
	default boolean onBradotForceQuit(int bradotInstanceId) {
		return false;
	}

	/**
	 * Invoked on the render thread when the Bradot instance wants to be restarted. It's up to the host
	 * to perform the appropriate action(s).
	 */
	default void onBradotRestartRequested(Bradot instance) {}

	/**
	 * Invoked on the render thread when a new Bradot instance is requested. It's up to the host to
	 * perform the appropriate action(s).
	 *
	 * @param args Arguments used to initialize the new instance.
	 *
	 * @return the id of the new instance. See {@code onBradotForceQuit}
	 */
	default int onNewBradotInstanceRequested(String[] args) {
		return 0;
	}

	/**
	 * Provide access to the Activity hosting the {@link Bradot} engine.
	 */
	Activity getActivity();

	/**
	 * Provide access to the hosted {@link Bradot} engine.
	 */
	Bradot getBradot();

	/**
	 * Returns a set of {@link BradotPlugin} to be registered with the hosted {@link Bradot} engine.
	 */
	default Set<BradotPlugin> getHostPlugins(Bradot engine) {
		return Collections.emptySet();
	}

	/**
	 * Signs the given Android apk
	 *
	 * @param inputPath Path to the apk that should be signed
	 * @param outputPath Path for the signed output apk; can be the same as inputPath
	 * @param keystorePath Path to the keystore to use for signing the apk
	 * @param keystoreUser Keystore user credential
	 * @param keystorePassword Keystore password credential
	 *
	 * @return {@link Error#OK} if signing is successful
	 */
	default Error signApk(@NonNull String inputPath, @NonNull String outputPath, @NonNull String keystorePath, @NonNull String keystoreUser, @NonNull String keystorePassword) {
		return Error.ERR_UNAVAILABLE;
	}

	/**
	 * Verifies the given Android apk is signed
	 *
	 * @param apkPath Path to the apk that should be verified
	 * @return {@link Error#OK} if verification was successful
	 */
	default Error verifyApk(@NonNull String apkPath) {
		return Error.ERR_UNAVAILABLE;
	}

	/**
	 * Returns whether the given feature tag is supported.
	 *
	 * @see <a href="https://docs.bradotengine.org/en/stable/tutorials/export/feature_tags.html">Feature tags</a>
	 */
	default boolean supportsFeature(String featureTag) {
		return false;
	}
}
