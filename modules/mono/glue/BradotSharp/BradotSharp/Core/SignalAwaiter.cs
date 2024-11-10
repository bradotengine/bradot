using System;
using System.Runtime.InteropServices;
using Bradot.NativeInterop;

namespace Bradot
{
    public class SignalAwaiter : IAwaiter<Variant[]>, IAwaitable<Variant[]>
    {
        private bool _completed;
        private Variant[] _result;
        private Action _continuation;

        public SignalAwaiter(BradotObject source, StringName signal, BradotObject target)
        {
            var awaiterGcHandle = CustomGCHandle.AllocStrong(this);
            using bradot_string_name signalSrc = NativeFuncs.bradotsharp_string_name_new_copy(
                (bradot_string_name)(signal?.NativeValue ?? default));
            NativeFuncs.bradotsharp_internal_signal_awaiter_connect(BradotObject.GetPtr(source), in signalSrc,
                BradotObject.GetPtr(target), GCHandle.ToIntPtr(awaiterGcHandle));
        }

        public bool IsCompleted => _completed;

        public void OnCompleted(Action continuation)
        {
            _continuation = continuation;
        }

        public Variant[] GetResult() => _result;

        public IAwaiter<Variant[]> GetAwaiter() => this;

        [UnmanagedCallersOnly]
        internal static unsafe void SignalCallback(IntPtr awaiterGCHandlePtr, bradot_variant** args, int argCount,
            bradot_bool* outAwaiterIsNull)
        {
            try
            {
                var awaiter = (SignalAwaiter)GCHandle.FromIntPtr(awaiterGCHandlePtr).Target;

                if (awaiter == null)
                {
                    *outAwaiterIsNull = bradot_bool.True;
                    return;
                }

                *outAwaiterIsNull = bradot_bool.False;

                awaiter._completed = true;

                Variant[] signalArgs = new Variant[argCount];

                for (int i = 0; i < argCount; i++)
                    signalArgs[i] = Variant.CreateCopyingBorrowed(*args[i]);

                awaiter._result = signalArgs;

                awaiter._continuation?.Invoke();
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
                *outAwaiterIsNull = bradot_bool.False;
            }
        }
    }
}
