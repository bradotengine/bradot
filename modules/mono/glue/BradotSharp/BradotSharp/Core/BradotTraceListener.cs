using System.Diagnostics;

namespace Bradot
{
    internal class BradotTraceListener : TraceListener
    {
        public override void Write(string message)
        {
            BR.PrintRaw(message);
        }

        public override void WriteLine(string message)
        {
            BR.Print(message);
        }

        public override void Fail(string message, string detailMessage)
        {
            BR.PrintErr("Assertion failed: ", message);
            BR.PrintErr("  Details: ", detailMessage);

            try
            {
                string stackTrace = new StackTrace(true).ToString();
                BR.PrintErr(stackTrace);
            }
            catch
            {
                // ignored
            }
        }
    }
}
