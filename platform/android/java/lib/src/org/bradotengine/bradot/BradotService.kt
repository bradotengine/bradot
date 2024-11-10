package org.bradotengine.bradot

import android.app.Service
import android.content.Intent
import android.os.Binder
import android.os.IBinder
import android.util.Log

/**
 * Bradot service responsible for hosting the Bradot engine instance.
 *
 * Note: Still in development, so it's made private and inaccessible until completed.
 */
private class BradotService : Service() {

	companion object {
		private val TAG = BradotService::class.java.simpleName
	}

	private var boundIntent: Intent? = null
	private val godot by lazy {
		Bradot(applicationContext)
	}

	override fun onCreate() {
		super.onCreate()
	}

	override fun onDestroy() {
		super.onDestroy()
	}

	override fun onBind(intent: Intent?): IBinder? {
		if (boundIntent != null) {
			Log.d(TAG, "BradotService already bound")
			return null
		}

		boundIntent = intent
		return BradotHandle(godot)
	}

	override fun onRebind(intent: Intent?) {
		super.onRebind(intent)
	}

	override fun onUnbind(intent: Intent?): Boolean {
		return super.onUnbind(intent)
	}

	override fun onTaskRemoved(rootIntent: Intent?) {
		super.onTaskRemoved(rootIntent)
	}

	class BradotHandle(val bradot: Bradot) : Binder()
}
