using System;
using System.Runtime.InteropServices;
using Bradot.NativeInterop;

namespace Bradot
{
    public static partial class BR
    {
        [UnmanagedCallersOnly]
        internal static void OnCoreApiAssemblyLoaded(bradot_bool isDebug)
        {
            try
            {
                Dispatcher.InitializeDefaultBradotTaskScheduler();

                if (isDebug.ToBool())
                {
                    DebuggingUtils.InstallTraceListener();

                    AppDomain.CurrentDomain.UnhandledException += (_, e) =>
                    {
                        // Exception.ToString() includes the inner exception
                        ExceptionUtils.LogUnhandledException((Exception)e.ExceptionObject);
                    };
                }
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
            }
        }
    }
}
