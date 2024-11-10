using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using Bradot.Bridge;
using Bradot.NativeInterop;

#nullable enable

namespace Bradot
{
    public partial class BradotObject : IDisposable
    {
        private bool _disposed;
        private static readonly Type _cachedType = typeof(BradotObject);

        internal IntPtr NativePtr;
        private bool _memoryOwn;

        private WeakReference<BradotObject>? _weakReferenceToSelf;

        /// <summary>
        /// Constructs a new <see cref="BradotObject"/>.
        /// </summary>
        public BradotObject() : this(false)
        {
            unsafe
            {
                ConstructAndInitialize(NativeCtor, NativeName, _cachedType, refCounted: false);
            }
        }

        internal BradotObject(IntPtr nativePtr) : this(false)
        {
            // NativePtr must be non-zero before calling ConstructAndInitialize to avoid invoking the constructor NativeCtor.
            // We don't want to invoke the constructor, because we already have a constructed instance in nativePtr.
            NativePtr = nativePtr;
            unsafe
            {
                ConstructAndInitialize(NativeCtor, NativeName, _cachedType, refCounted: false);
            }
        }

        internal unsafe void ConstructAndInitialize(
            delegate* unmanaged<bradot_bool, IntPtr> nativeCtor,
            StringName nativeName,
            Type cachedType,
            bool refCounted
        )
        {
            if (NativePtr == IntPtr.Zero)
            {
                Debug.Assert(nativeCtor != null);

                // Need postinitialization.
                NativePtr = nativeCtor(bradot_bool.True);

                InteropUtils.TieManagedToUnmanaged(this, NativePtr,
                    nativeName, refCounted, GetType(), cachedType);
            }
            else
            {
                InteropUtils.TieManagedToUnmanagedWithPreSetup(this, NativePtr,
                    GetType(), cachedType);
            }

            _weakReferenceToSelf = DisposablesTracker.RegisterBradotObject(this);
        }

        internal BradotObject(bool memoryOwn)
        {
            _memoryOwn = memoryOwn;
        }

        /// <summary>
        /// The pointer to the native instance of this <see cref="BradotObject"/>.
        /// </summary>
        public IntPtr NativeInstance => NativePtr;

        internal static IntPtr GetPtr(BradotObject? instance)
        {
            if (instance == null)
                return IntPtr.Zero;

            // We check if NativePtr is null because this may be called by the debugger.
            // If the debugger puts a breakpoint in one of the base constructors, before
            // NativePtr is assigned, that would result in UB or crashes when calling
            // native functions that receive the pointer, which can happen because the
            // debugger calls ToString() and tries to get the value of properties.
            if (instance._disposed || instance.NativePtr == IntPtr.Zero)
                throw new ObjectDisposedException(instance.GetType().FullName);

            return instance.NativePtr;
        }

        ~BradotObject()
        {
            Dispose(false);
        }

        /// <summary>
        /// Disposes of this <see cref="BradotObject"/>.
        /// </summary>
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Disposes implementation of this <see cref="BradotObject"/>.
        /// </summary>
        protected virtual void Dispose(bool disposing)
        {
            if (_disposed)
                return;

            _disposed = true;

            if (NativePtr != IntPtr.Zero)
            {
                IntPtr gcHandleToFree = NativeFuncs.bradotsharp_internal_object_get_associated_gchandle(NativePtr);

                if (gcHandleToFree != IntPtr.Zero)
                {
                    object? target = GCHandle.FromIntPtr(gcHandleToFree).Target;
                    // The GC handle may have been replaced in another thread. Release it only if
                    // it's associated to this managed instance, or if the target is no longer alive.
                    if (target != this && target != null)
                        gcHandleToFree = IntPtr.Zero;
                }

                if (_memoryOwn)
                {
                    NativeFuncs.bradotsharp_internal_refcounted_disposed(NativePtr, gcHandleToFree,
                        (!disposing).ToBradotBool());
                }
                else
                {
                    NativeFuncs.bradotsharp_internal_object_disposed(NativePtr, gcHandleToFree);
                }

                NativePtr = IntPtr.Zero;
            }

            if (_weakReferenceToSelf != null)
            {
                DisposablesTracker.UnregisterBradotObject(this, _weakReferenceToSelf);
            }
        }

        /// <summary>
        /// Converts this <see cref="BradotObject"/> to a string.
        /// </summary>
        /// <returns>A string representation of this object.</returns>
        public override string ToString()
        {
            NativeFuncs.bradotsharp_object_to_string(GetPtr(this), out bradot_string str);
            using (str)
                return Marshaling.ConvertStringToManaged(str);
        }

        /// <summary>
        /// Returns a new <see cref="SignalAwaiter"/> awaiter configured to complete when the instance
        /// <paramref name="source"/> emits the signal specified by the <paramref name="signal"/> parameter.
        /// </summary>
        /// <param name="source">
        /// The instance the awaiter will be listening to.
        /// </param>
        /// <param name="signal">
        /// The signal the awaiter will be waiting for.
        /// </param>
        /// <example>
        /// This sample prints a message once every frame up to 100 times.
        /// <code>
        /// public override void _Ready()
        /// {
        ///     for (int i = 0; i &lt; 100; i++)
        ///     {
        ///         await ToSignal(GetTree(), "process_frame");
        ///         BR.Print($"Frame {i}");
        ///     }
        /// }
        /// </code>
        /// </example>
        /// <returns>
        /// A <see cref="SignalAwaiter"/> that completes when
        /// <paramref name="source"/> emits the <paramref name="signal"/>.
        /// </returns>
        public SignalAwaiter ToSignal(BradotObject source, StringName signal)
        {
            return new SignalAwaiter(source, signal, this);
        }

        internal static Type InternalGetClassNativeBase(Type t)
        {
            var name = t.Assembly.GetName().Name;

            if (name == "BradotSharp" || name == "BradotSharpEditor")
                return t;

            Debug.Assert(t.BaseType is not null, "Script types must derive from a native Bradot type.");

            return InternalGetClassNativeBase(t.BaseType);
        }

        // ReSharper disable once VirtualMemberNeverOverridden.Global
        /// <summary>
        /// Set the value of a property contained in this class.
        /// This method is used by Bradot to assign property values.
        /// Do not call or override this method.
        /// </summary>
        /// <param name="name">Name of the property to set.</param>
        /// <param name="value">Value to set the property to if it was found.</param>
        /// <returns><see langword="true"/> if a property with the given name was found.</returns>
        [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
        protected internal virtual bool SetBradotClassPropertyValue(in bradot_string_name name, in bradot_variant value)
        {
            return false;
        }

        // ReSharper disable once VirtualMemberNeverOverridden.Global
        /// <summary>
        /// Get the value of a property contained in this class.
        /// This method is used by Bradot to retrieve property values.
        /// Do not call or override this method.
        /// </summary>
        /// <param name="name">Name of the property to get.</param>
        /// <param name="value">Value of the property if it was found.</param>
        /// <returns><see langword="true"/> if a property with the given name was found.</returns>
        [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
        protected internal virtual bool GetBradotClassPropertyValue(in bradot_string_name name, out bradot_variant value)
        {
            value = default;
            return false;
        }

        // ReSharper disable once VirtualMemberNeverOverridden.Global
        /// <summary>
        /// Raises the signal with the given name, using the given arguments.
        /// This method is used by Bradot to raise signals from the engine side.\n"
        /// Do not call or override this method.
        /// </summary>
        /// <param name="signal">Name of the signal to raise.</param>
        /// <param name="args">Arguments to use with the raised signal.</param>
        [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
        protected internal virtual void RaiseBradotClassSignalCallbacks(in bradot_string_name signal,
            NativeVariantPtrArgs args)
        {
        }

        internal static IntPtr ClassDB_get_method(StringName type, StringName method)
        {
            var typeSelf = (bradot_string_name)type.NativeValue;
            var methodSelf = (bradot_string_name)method.NativeValue;
            IntPtr methodBind = NativeFuncs.bradotsharp_method_bind_get_method(typeSelf, methodSelf);

            if (methodBind == IntPtr.Zero)
                throw new NativeMethodBindNotFoundException(type + "." + method);

            return methodBind;
        }

        internal static IntPtr ClassDB_get_method_with_compatibility(StringName type, StringName method, ulong hash)
        {
            var typeSelf = (bradot_string_name)type.NativeValue;
            var methodSelf = (bradot_string_name)method.NativeValue;
            IntPtr methodBind = NativeFuncs.bradotsharp_method_bind_get_method_with_compatibility(typeSelf, methodSelf, hash);

            if (methodBind == IntPtr.Zero)
                throw new NativeMethodBindNotFoundException(type + "." + method);

            return methodBind;
        }

        internal static unsafe delegate* unmanaged<bradot_bool, IntPtr> ClassDB_get_constructor(StringName type)
        {
            // for some reason the '??' operator doesn't support 'delegate*'
            var typeSelf = (bradot_string_name)type.NativeValue;
            var nativeConstructor = NativeFuncs.bradotsharp_get_class_constructor(typeSelf);

            if (nativeConstructor == null)
                throw new NativeConstructorNotFoundException(type);

            return nativeConstructor;
        }

        /// <summary>
        /// Saves this instance's state to be restored when reloading assemblies.
        /// Do not call or override this method.
        /// To add data to be saved and restored, implement <see cref="ISerializationListener"/>.
        /// </summary>
        /// <param name="info">Object used to save the data.</param>
        [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
        protected internal virtual void SaveBradotObjectData(BradotSerializationInfo info)
        {
        }

        // TODO: Should this be a constructor overload?
        /// <summary>
        /// Restores this instance's state after reloading assemblies.
        /// Do not call or override this method.
        /// To add data to be saved and restored, implement <see cref="ISerializationListener"/>.
        /// </summary>
        /// <param name="info">Object that contains the previously saved data.</param>
        [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
        protected internal virtual void RestoreBradotObjectData(BradotSerializationInfo info)
        {
        }
    }
}
