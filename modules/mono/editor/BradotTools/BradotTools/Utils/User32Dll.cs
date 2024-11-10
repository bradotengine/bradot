using System.Runtime.InteropServices;

namespace BradotTools.Utils
{
    public static class User32Dll
    {
        [DllImport("user32.dll")]
        public static extern bool AllowSetForegroundWindow(int dwProcessId);
    }
}
