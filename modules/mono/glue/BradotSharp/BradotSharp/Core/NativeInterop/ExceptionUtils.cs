using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.CompilerServices;
using System.Text;

#nullable enable

namespace Bradot.NativeInterop
{
    internal static class ExceptionUtils
    {
        public static void PushError(string message)
        {
            BR.PushError(message);
        }

        private static void OnExceptionLoggerException(Exception loggerException, Exception exceptionToLog)
        {
            try
            {
                // This better not throw
                PushError(string.Concat("Exception thrown while trying to log another exception...",
                    "\n### Exception ###\n", exceptionToLog.ToString(),
                    "\n### Logger exception ###\n", loggerException.ToString()));
            }
            catch (Exception)
            {
                // Well, too bad...
            }
        }

        private record struct StackInfoTuple(string? File, string Func, int Line);

        private static void CollectExceptionInfo(Exception exception, List<StackInfoTuple> globalFrames,
            StringBuilder excMsg)
        {
            if (excMsg.Length > 0)
                excMsg.Append(" ---> ");
            excMsg.Append(exception.GetType().FullName);
            excMsg.Append(": ");
            excMsg.Append(exception.Message);

            var innerExc = exception.InnerException;

            if (innerExc != null)
            {
                CollectExceptionInfo(innerExc, globalFrames, excMsg);
                globalFrames.Add(new("", "--- End of inner exception stack trace ---", 0));
            }

            var stackTrace = new StackTrace(exception, fNeedFileInfo: true);

            foreach (StackFrame frame in stackTrace.GetFrames())
            {
                DebuggingUtils.GetStackFrameMethodDecl(frame, out string methodDecl);
                globalFrames.Add(new(frame.GetFileName(), methodDecl, frame.GetFileLineNumber()));
            }
        }

        private static void SendToScriptDebugger(Exception e)
        {
            var globalFrames = new List<StackInfoTuple>();

            var excMsg = new StringBuilder();

            CollectExceptionInfo(e, globalFrames, excMsg);

            string file = globalFrames.Count > 0 ? globalFrames[0].File ?? "" : "";
            string func = globalFrames.Count > 0 ? globalFrames[0].Func : "";
            int line = globalFrames.Count > 0 ? globalFrames[0].Line : 0;
            string errorMsg = e.GetType().FullName ?? "";

            using bradot_string nFile = Marshaling.ConvertStringToNative(file);
            using bradot_string nFunc = Marshaling.ConvertStringToNative(func);
            using bradot_string nErrorMsg = Marshaling.ConvertStringToNative(errorMsg);
            using bradot_string nExcMsg = Marshaling.ConvertStringToNative(excMsg.ToString());

            using DebuggingUtils.bradot_stack_info_vector stackInfoVector = default;

            stackInfoVector.Resize(globalFrames.Count);

            unsafe
            {
                for (int i = 0; i < globalFrames.Count; i++)
                {
                    DebuggingUtils.bradot_stack_info* stackInfo = &stackInfoVector.Elements[i];

                    var globalFrame = globalFrames[i];

                    // Assign directly to element in Vector. This way we don't need to worry
                    // about disposal if an exception is thrown. The Vector takes care of it.
                    stackInfo->File = Marshaling.ConvertStringToNative(globalFrame.File);
                    stackInfo->Func = Marshaling.ConvertStringToNative(globalFrame.Func);
                    stackInfo->Line = globalFrame.Line;
                }

                NativeFuncs.bradotsharp_internal_script_debugger_send_error(nFunc, nFile, line,
                    nErrorMsg, nExcMsg, bradot_error_handler_type.ERR_HANDLER_ERROR, stackInfoVector);
            }
        }

        public static void LogException(Exception e)
        {
            try
            {
                if (NativeFuncs.bradotsharp_internal_script_debugger_is_active().ToBool())
                {
                    SendToScriptDebugger(e);
                }
                else
                {
                    BR.PushError(e.ToString());
                }
            }
            catch (Exception unexpected)
            {
                OnExceptionLoggerException(unexpected, e);
            }
        }

        public static void LogUnhandledException(Exception e)
        {
            try
            {
                if (NativeFuncs.bradotsharp_internal_script_debugger_is_active().ToBool())
                {
                    SendToScriptDebugger(e);
                }

                // In this case, print it as well in addition to sending it to the script debugger
                BR.PushError("Unhandled exception\n" + e);
            }
            catch (Exception unexpected)
            {
                OnExceptionLoggerException(unexpected, e);
            }
        }

        [Conditional("DEBUG")]
        public unsafe static void DebugCheckCallError(bradot_string_name method, IntPtr instance, bradot_variant** args, int argCount, bradot_variant_call_error error)
        {
            if (error.Error != bradot_variant_call_error_error.BRADOT_CALL_ERROR_CALL_OK)
            {
                using bradot_variant instanceVariant = VariantUtils.CreateFromBradotObjectPtr(instance);
                string where = GetCallErrorWhere(ref error, method, &instanceVariant, args, argCount);
                string errorText = GetCallErrorMessage(error, where, args);
                BR.PushError(errorText);
            }
        }

        [Conditional("DEBUG")]
        public unsafe static void DebugCheckCallError(in bradot_callable callable, bradot_variant** args, int argCount, bradot_variant_call_error error)
        {
            if (error.Error != bradot_variant_call_error_error.BRADOT_CALL_ERROR_CALL_OK)
            {
                using bradot_variant callableVariant = VariantUtils.CreateFromCallableTakingOwnershipOfDisposableValue(callable);
                string where = $"callable '{VariantUtils.ConvertToString(callableVariant)}'";
                string errorText = GetCallErrorMessage(error, where, args);
                BR.PushError(errorText);
            }
        }

        private unsafe static string GetCallErrorWhere(ref bradot_variant_call_error error, bradot_string_name method, bradot_variant* instance, bradot_variant** args, int argCount)
        {
            string? methodstr = null;
            string basestr = GetVariantTypeName(instance);

            if (method == BradotObject.MethodName.Call || (basestr == "Bradot.TreeItem" && method == TreeItem.MethodName.CallRecursive))
            {
                if (argCount >= 1)
                {
                    methodstr = VariantUtils.ConvertToString(*args[0]);
                    if (error.Error == bradot_variant_call_error_error.BRADOT_CALL_ERROR_CALL_ERROR_INVALID_ARGUMENT)
                    {
                        error.Argument += 1;
                    }
                }
            }

            if (string.IsNullOrEmpty(methodstr))
            {
                methodstr = StringName.CreateTakingOwnershipOfDisposableValue(method);
            }

            return $"function '{methodstr}' in base '{basestr}'";
        }

        private unsafe static string GetCallErrorMessage(bradot_variant_call_error error, string where, bradot_variant** args)
        {
            switch (error.Error)
            {
                case bradot_variant_call_error_error.BRADOT_CALL_ERROR_CALL_ERROR_INVALID_ARGUMENT:
                {
                    int errorarg = error.Argument;
                    // Handle the Object to Object case separately as we don't have further class details.
#if DEBUG
                    if (error.Expected == Variant.Type.Object && args[errorarg]->Type == error.Expected)
                    {
                        return $"Invalid type in {where}. The Object-derived class of argument {errorarg + 1} (" + GetVariantTypeName(args[errorarg]) + ") is not a subclass of the expected argument class.";
                    }
                    else if (error.Expected == Variant.Type.Array && args[errorarg]->Type == error.Expected)
                    {
                        return $"Invalid type in {where}. The array of argument {errorarg + 1} (" + GetVariantTypeName(args[errorarg]) + ") does not have the same element type as the expected typed array argument.";
                    }
                    else
#endif
                    {
                        return $"Invalid type in {where}. Cannot convert argument {errorarg + 1} from {args[errorarg]->Type} to {error.Expected}.";
                    }
                }
                case bradot_variant_call_error_error.BRADOT_CALL_ERROR_CALL_ERROR_TOO_MANY_ARGUMENTS:
                case bradot_variant_call_error_error.BRADOT_CALL_ERROR_CALL_ERROR_TOO_FEW_ARGUMENTS:
                    return $"Invalid call to {where}. Expected {error.Expected} arguments.";
                case bradot_variant_call_error_error.BRADOT_CALL_ERROR_CALL_ERROR_INVALID_METHOD:
                    return $"Invalid call. Nonexistent {where}.";
                case bradot_variant_call_error_error.BRADOT_CALL_ERROR_CALL_ERROR_INSTANCE_IS_NULL:
                    return $"Attempt to call {where} on a null instance.";
                case bradot_variant_call_error_error.BRADOT_CALL_ERROR_CALL_ERROR_METHOD_NOT_CONST:
                    return $"Attempt to call {where} on a const instance.";
                default:
                    return $"Bug, call error: #{error.Error}";
            }
        }

        private unsafe static string GetVariantTypeName(bradot_variant* variant)
        {
            if (variant->Type == Variant.Type.Object)
            {
                BradotObject obj = VariantUtils.ConvertToBradotObject(*variant);
                if (obj == null)
                {
                    return "null instance";
                }
                else if (!BradotObject.IsInstanceValid(obj))
                {
                    return "previously freed";
                }
                else
                {
                    return obj.GetType().ToString();
                }
            }

            return variant->Type.ToString();
        }

        internal static void ThrowIfNullPtr(IntPtr ptr, [CallerArgumentExpression("ptr")] string? paramName = null)
        {
            if (ptr == IntPtr.Zero)
            {
                throw new ArgumentNullException(paramName);
            }
        }
    }
}
