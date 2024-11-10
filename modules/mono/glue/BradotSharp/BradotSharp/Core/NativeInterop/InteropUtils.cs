using System;
using System.Runtime.InteropServices;
using Bradot.Bridge;

// ReSharper disable InconsistentNaming

namespace Bradot.NativeInterop
{
    internal static class InteropUtils
    {
        public static BradotObject UnmanagedGetManaged(IntPtr unmanaged)
        {
            // The native pointer may be null
            if (unmanaged == IntPtr.Zero)
                return null;

            IntPtr gcHandlePtr;
            bradot_bool hasCsScriptInstance;

            // First try to get the tied managed instance from a CSharpInstance script instance

            gcHandlePtr = NativeFuncs.bradotsharp_internal_unmanaged_get_script_instance_managed(
                unmanaged, out hasCsScriptInstance);

            if (gcHandlePtr != IntPtr.Zero)
                return (BradotObject)GCHandle.FromIntPtr(gcHandlePtr).Target;

            // Otherwise, if the object has a CSharpInstance script instance, return null

            if (hasCsScriptInstance.ToBool())
                return null;

            // If it doesn't have a CSharpInstance script instance, try with native instance bindings

            gcHandlePtr = NativeFuncs.bradotsharp_internal_unmanaged_get_instance_binding_managed(unmanaged);

            object target = gcHandlePtr != IntPtr.Zero ? GCHandle.FromIntPtr(gcHandlePtr).Target : null;

            if (target != null)
                return (BradotObject)target;

            // If the native instance binding GC handle target was collected, create a new one

            gcHandlePtr = NativeFuncs.bradotsharp_internal_unmanaged_instance_binding_create_managed(
                unmanaged, gcHandlePtr);

            return gcHandlePtr != IntPtr.Zero ? (BradotObject)GCHandle.FromIntPtr(gcHandlePtr).Target : null;
        }

        public static void TieManagedToUnmanaged(BradotObject managed, IntPtr unmanaged,
            StringName nativeName, bool refCounted, Type type, Type nativeType)
        {
            var gcHandle = refCounted ?
                CustomGCHandle.AllocWeak(managed) :
                CustomGCHandle.AllocStrong(managed, type);

            if (type == nativeType)
            {
                var nativeNameSelf = (bradot_string_name)nativeName.NativeValue;
                NativeFuncs.bradotsharp_internal_tie_native_managed_to_unmanaged(
                    GCHandle.ToIntPtr(gcHandle), unmanaged, nativeNameSelf, refCounted.ToBradotBool());
            }
            else
            {
                unsafe
                {
                    // We don't dispose `script` ourselves here.
                    // `tie_user_managed_to_unmanaged` does it for us to avoid another P/Invoke call.
                    bradot_ref script;
                    ScriptManagerBridge.GetOrLoadOrCreateScriptForType(type, &script);

                    // IMPORTANT: This must be called after GetOrCreateScriptBridgeForType
                    NativeFuncs.bradotsharp_internal_tie_user_managed_to_unmanaged(
                        GCHandle.ToIntPtr(gcHandle), unmanaged, &script, refCounted.ToBradotBool());
                }
            }
        }

        public static void TieManagedToUnmanagedWithPreSetup(BradotObject managed, IntPtr unmanaged,
            Type type, Type nativeType)
        {
            if (type == nativeType)
                return;

            var strongGCHandle = CustomGCHandle.AllocStrong(managed);
            NativeFuncs.bradotsharp_internal_tie_managed_to_unmanaged_with_pre_setup(
                GCHandle.ToIntPtr(strongGCHandle), unmanaged);
        }

        public static BradotObject EngineGetSingleton(string name)
        {
            using bradot_string src = Marshaling.ConvertStringToNative(name);
            return UnmanagedGetManaged(NativeFuncs.bradotsharp_engine_get_singleton(src));
        }
    }
}
