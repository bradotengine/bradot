/**************************************************************************/
/*  BradotActivity.kt                                                      */
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

package org.bradotengine.bradot

import android.app.Activity
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Bundle
import android.util.Log
import androidx.annotation.CallSuper
import androidx.annotation.LayoutRes
import androidx.fragment.app.FragmentActivity
import org.bradotengine.bradot.utils.PermissionsUtil
import org.bradotengine.bradot.utils.ProcessPhoenix

/**
 * Base abstract activity for Android apps intending to use Bradot as the primary screen.
 *
 * Also a reference implementation for how to setup and use the [BradotFragment] fragment
 * within an Android app.
 */
abstract class BradotActivity : FragmentActivity(), BradotHost {

	companion object {
		private val TAG = BradotActivity::class.java.simpleName

		@JvmStatic
		protected val EXTRA_NEW_LAUNCH = "new_launch_requested"
	}

	/**
	 * Interaction with the [Bradot] object is delegated to the [BradotFragment] class.
	 */
	protected var bradotFragment: BradotFragment? = null
		private set

	override fun onCreate(savedInstanceState: Bundle?) {
		super.onCreate(savedInstanceState)
		setContentView(getBradotAppLayout())

		handleStartIntent(intent, true)

		val currentFragment = supportFragmentManager.findFragmentById(R.id.bradot_fragment_container)
		if (currentFragment is BradotFragment) {
			Log.v(TAG, "Reusing existing Bradot fragment instance.")
			bradotFragment = currentFragment
		} else {
			Log.v(TAG, "Creating new Bradot fragment instance.")
			bradotFragment = initBradotInstance()
			supportFragmentManager.beginTransaction().replace(R.id.bradot_fragment_container, bradotFragment!!).setPrimaryNavigationFragment(bradotFragment).commitNowAllowingStateLoss()
		}
	}

	@LayoutRes
	protected open fun getBradotAppLayout() = R.layout.bradot_app_layout

	override fun onDestroy() {
		Log.v(TAG, "Destroying BradotActivity $this...")
		super.onDestroy()
	}

	override fun onBradotForceQuit(instance: Bradot) {
		runOnUiThread { terminateBradotInstance(instance) }
	}

	private fun terminateBradotInstance(instance: Bradot) {
		bradotFragment?.let {
			if (instance === it.bradot) {
				Log.v(TAG, "Force quitting Bradot instance")
				ProcessPhoenix.forceQuit(this)
			}
		}
	}

	override fun onBradotRestartRequested(instance: Bradot) {
		runOnUiThread {
			bradotFragment?.let {
				if (instance === it.bradot) {
					// It's very hard to properly de-initialize Bradot on Android to restart the game
					// from scratch. Therefore, we need to kill the whole app process and relaunch it.
					//
					// Restarting only the activity, wouldn't be enough unless it did proper cleanup (including
					// releasing and reloading native libs or resetting their state somehow and clearing static data).
					Log.v(TAG, "Restarting Bradot instance...")
					ProcessPhoenix.triggerRebirth(this)
				}
			}
		}
	}

	override fun onNewIntent(newIntent: Intent) {
		super.onNewIntent(newIntent)
		intent = newIntent

		handleStartIntent(newIntent, false)

		bradotFragment?.onNewIntent(newIntent)
	}

	private fun handleStartIntent(intent: Intent, newLaunch: Boolean) {
		if (!newLaunch) {
			val newLaunchRequested = intent.getBooleanExtra(EXTRA_NEW_LAUNCH, false)
			if (newLaunchRequested) {
				Log.d(TAG, "New launch requested, restarting..")
				val restartIntent = Intent(intent).putExtra(EXTRA_NEW_LAUNCH, false)
				ProcessPhoenix.triggerRebirth(this, restartIntent)
				return
			}
		}
	}

	@CallSuper
	override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
		super.onActivityResult(requestCode, resultCode, data)
		bradotFragment?.onActivityResult(requestCode, resultCode, data)
	}

	@CallSuper
	override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<String>, grantResults: IntArray) {
		super.onRequestPermissionsResult(requestCode, permissions, grantResults)
		bradotFragment?.onRequestPermissionsResult(requestCode, permissions, grantResults)

		// Logging the result of permission requests
		if (requestCode == PermissionsUtil.REQUEST_ALL_PERMISSION_REQ_CODE || requestCode == PermissionsUtil.REQUEST_SINGLE_PERMISSION_REQ_CODE) {
			Log.d(TAG, "Received permissions request result..")
			for (i in permissions.indices) {
				val permissionGranted = grantResults[i] == PackageManager.PERMISSION_GRANTED
				Log.d(TAG, "Permission ${permissions[i]} ${if (permissionGranted) { "granted"} else { "denied" }}")
			}
		}
	}

	override fun onBackPressed() {
		bradotFragment?.onBackPressed() ?: super.onBackPressed()
	}

	override fun getActivity(): Activity? {
		return this
	}

	override fun getBradot(): Bradot? {
		return bradotFragment?.bradot
	}

	/**
	 * Used to initialize the Bradot fragment instance in [onCreate].
	 */
	protected open fun initBradotInstance(): BradotFragment {
		return BradotFragment()
	}
}
