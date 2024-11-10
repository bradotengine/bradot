using System;
using System.Runtime.InteropServices;
using Bradot.NativeInterop;

namespace Bradot.Bridge
{
    internal static class GCHandleBridge
    {
        [UnmanagedCallersOnly]
        internal static void FreeGCHandle(IntPtr gcHandlePtr)
        {
            try
            {
                CustomGCHandle.Free(GCHandle.FromIntPtr(gcHandlePtr));
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
            }
        }

        // Returns true, if releasing the provided handle is necessary for assembly unloading to succeed.
        // This check is not perfect and only intended to prevent things in BradotTools from being reloaded.
        [UnmanagedCallersOnly]
        internal static bradot_bool GCHandleIsTargetCollectible(IntPtr gcHandlePtr)
        {
            try
            {
                var target = GCHandle.FromIntPtr(gcHandlePtr).Target;

                if (target is Delegate @delegate)
                    return DelegateUtils.IsDelegateCollectible(@delegate).ToBradotBool();

                return target.GetType().IsCollectible.ToBradotBool();
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
                return bradot_bool.True;
            }
        }
    }
}
