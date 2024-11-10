using System;
using System.Collections.Concurrent;
using System.Runtime.InteropServices;
using Bradot.NativeInterop;

#nullable enable

namespace Bradot
{
    internal static class DisposablesTracker
    {
        [UnmanagedCallersOnly]
        internal static void OnBradotShuttingDown()
        {
            try
            {
                OnBradotShuttingDownImpl();
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
            }
        }

        private static void OnBradotShuttingDownImpl()
        {
            bool isStdoutVerbose;

            try
            {
                isStdoutVerbose = OS.IsStdOutVerbose();
            }
            catch (ObjectDisposedException)
            {
                // OS singleton already disposed. Maybe OnUnloading was called twice.
                isStdoutVerbose = false;
            }

            if (isStdoutVerbose)
                BR.Print("Unloading: Disposing tracked instances...");

            // Dispose Godot Objects first, and only then dispose other disposables
            // like StringName, NodePath, Bradot.Collections.Array/Dictionary, etc.
            // The Godot Object Dispose() method may need any of the later instances.

            foreach (WeakReference<BradotObject> item in BradotObjectInstances.Keys)
            {
                if (item.TryGetTarget(out BradotObject? self))
                    self.Dispose();
            }

            foreach (WeakReference<IDisposable> item in OtherInstances.Keys)
            {
                if (item.TryGetTarget(out IDisposable? self))
                    self.Dispose();
            }

            if (isStdoutVerbose)
                BR.Print("Unloading: Finished disposing tracked instances.");
        }

        private static ConcurrentDictionary<WeakReference<BradotObject>, byte> BradotObjectInstances { get; } =
            new();

        private static ConcurrentDictionary<WeakReference<IDisposable>, byte> OtherInstances { get; } =
            new();

        public static WeakReference<BradotObject> RegisterBradotObject(BradotObject godotObject)
        {
            var weakReferenceToSelf = new WeakReference<BradotObject>(godotObject);
            BradotObjectInstances.TryAdd(weakReferenceToSelf, 0);
            return weakReferenceToSelf;
        }

        public static WeakReference<IDisposable> RegisterDisposable(IDisposable disposable)
        {
            var weakReferenceToSelf = new WeakReference<IDisposable>(disposable);
            OtherInstances.TryAdd(weakReferenceToSelf, 0);
            return weakReferenceToSelf;
        }

        public static void UnregisterBradotObject(BradotObject godotObject, WeakReference<BradotObject> weakReferenceToSelf)
        {
            if (!BradotObjectInstances.TryRemove(weakReferenceToSelf, out _))
                throw new ArgumentException("Godot Object not registered.", nameof(weakReferenceToSelf));
        }

        public static void UnregisterDisposable(WeakReference<IDisposable> weakReference)
        {
            if (!OtherInstances.TryRemove(weakReference, out _))
                throw new ArgumentException("Disposable not registered.", nameof(weakReference));
        }
    }
}
