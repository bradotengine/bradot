using System;
using System.Runtime.InteropServices;
using Bradot.NativeInterop;

namespace Bradot
{
    public static class Dispatcher
    {
        internal static BradotTaskScheduler DefaultBradotTaskScheduler;

        internal static void InitializeDefaultBradotTaskScheduler()
        {
            DefaultBradotTaskScheduler?.Dispose();
            DefaultBradotTaskScheduler = new BradotTaskScheduler();
        }

        public static BradotSynchronizationContext SynchronizationContext => DefaultBradotTaskScheduler.Context;
    }
}
