using System;
using System.Runtime.InteropServices;
using Bradot.NativeInterop;

namespace Bradot.Bridge
{
    internal static class CSharpInstanceBridge
    {
        [UnmanagedCallersOnly]
        internal static unsafe bradot_bool Call(IntPtr godotObjectGCHandle, bradot_string_name* method,
            bradot_variant** args, int argCount, bradot_variant_call_error* refCallError, bradot_variant* ret)
        {
            try
            {
                var godotObject = (BradotObject)GCHandle.FromIntPtr(godotObjectGCHandle).Target;

                if (godotObject == null)
                {
                    *ret = default;
                    (*refCallError).Error = bradot_variant_call_error_error.BRADOT_CALL_ERROR_CALL_ERROR_INSTANCE_IS_NULL;
                    return bradot_bool.False;
                }

                bool methodInvoked = godotObject.InvokeBradotClassMethod(CustomUnsafe.AsRef(method),
                    new NativeVariantPtrArgs(args, argCount), out bradot_variant retValue);

                if (!methodInvoked)
                {
                    *ret = default;
                    // This is important, as it tells Object::call that no method was called.
                    // Otherwise, it would prevent Object::call from calling native methods.
                    (*refCallError).Error = bradot_variant_call_error_error.BRADOT_CALL_ERROR_CALL_ERROR_INVALID_METHOD;
                    return bradot_bool.False;
                }

                *ret = retValue;
                return bradot_bool.True;
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
                *ret = default;
                return bradot_bool.False;
            }
        }

        [UnmanagedCallersOnly]
        internal static unsafe bradot_bool Set(IntPtr godotObjectGCHandle, bradot_string_name* name, bradot_variant* value)
        {
            try
            {
                var godotObject = (BradotObject)GCHandle.FromIntPtr(godotObjectGCHandle).Target;

                if (godotObject == null)
                    throw new InvalidOperationException();

                if (godotObject.SetBradotClassPropertyValue(CustomUnsafe.AsRef(name), CustomUnsafe.AsRef(value)))
                {
                    return bradot_bool.True;
                }

                var nameManaged = StringName.CreateTakingOwnershipOfDisposableValue(
                    NativeFuncs.bradotsharp_string_name_new_copy(CustomUnsafe.AsRef(name)));

                Variant valueManaged = Variant.CreateCopyingBorrowed(*value);

                return godotObject._Set(nameManaged, valueManaged).ToBradotBool();
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
                return bradot_bool.False;
            }
        }

        [UnmanagedCallersOnly]
        internal static unsafe bradot_bool Get(IntPtr godotObjectGCHandle, bradot_string_name* name,
            bradot_variant* outRet)
        {
            try
            {
                var godotObject = (BradotObject)GCHandle.FromIntPtr(godotObjectGCHandle).Target;

                if (godotObject == null)
                    throw new InvalidOperationException();

                // Properties
                if (godotObject.GetBradotClassPropertyValue(CustomUnsafe.AsRef(name), out bradot_variant outRetValue))
                {
                    *outRet = outRetValue;
                    return bradot_bool.True;
                }

                // Signals
                if (godotObject.HasBradotClassSignal(CustomUnsafe.AsRef(name)))
                {
                    bradot_signal signal = new bradot_signal(NativeFuncs.bradotsharp_string_name_new_copy(*name), godotObject.GetInstanceId());
                    *outRet = VariantUtils.CreateFromSignalTakingOwnershipOfDisposableValue(signal);
                    return bradot_bool.True;
                }

                // Methods
                if (godotObject.HasBradotClassMethod(CustomUnsafe.AsRef(name)))
                {
                    bradot_callable method = new bradot_callable(NativeFuncs.bradotsharp_string_name_new_copy(*name), godotObject.GetInstanceId());
                    *outRet = VariantUtils.CreateFromCallableTakingOwnershipOfDisposableValue(method);
                    return bradot_bool.True;
                }

                var nameManaged = StringName.CreateTakingOwnershipOfDisposableValue(
                    NativeFuncs.bradotsharp_string_name_new_copy(CustomUnsafe.AsRef(name)));

                Variant ret = godotObject._Get(nameManaged);

                if (ret.VariantType == Variant.Type.Nil)
                {
                    *outRet = default;
                    return bradot_bool.False;
                }

                *outRet = ret.CopyNativeVariant();
                return bradot_bool.True;
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
                *outRet = default;
                return bradot_bool.False;
            }
        }

        [UnmanagedCallersOnly]
        internal static void CallDispose(IntPtr godotObjectGCHandle, bradot_bool okIfNull)
        {
            try
            {
                var godotObject = (BradotObject)GCHandle.FromIntPtr(godotObjectGCHandle).Target;

                if (okIfNull.ToBool())
                    godotObject?.Dispose();
                else
                    godotObject!.Dispose();
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
            }
        }

        [UnmanagedCallersOnly]
        internal static unsafe void CallToString(IntPtr godotObjectGCHandle, bradot_string* outRes, bradot_bool* outValid)
        {
            try
            {
                var self = (BradotObject)GCHandle.FromIntPtr(godotObjectGCHandle).Target;

                if (self == null)
                {
                    *outRes = default;
                    *outValid = bradot_bool.False;
                    return;
                }

                var resultStr = self.ToString();

                if (resultStr == null)
                {
                    *outRes = default;
                    *outValid = bradot_bool.False;
                    return;
                }

                *outRes = Marshaling.ConvertStringToNative(resultStr);
                *outValid = bradot_bool.True;
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
                *outRes = default;
                *outValid = bradot_bool.False;
            }
        }

        [UnmanagedCallersOnly]
        internal static unsafe bradot_bool HasMethodUnknownParams(IntPtr godotObjectGCHandle, bradot_string_name* method)
        {
            try
            {
                var godotObject = (BradotObject)GCHandle.FromIntPtr(godotObjectGCHandle).Target;

                if (godotObject == null)
                    return bradot_bool.False;

                return godotObject.HasBradotClassMethod(CustomUnsafe.AsRef(method)).ToBradotBool();
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
                return bradot_bool.False;
            }
        }

        [UnmanagedCallersOnly]
        internal static unsafe void SerializeState(
            IntPtr godotObjectGCHandle,
            bradot_dictionary* propertiesState,
            bradot_dictionary* signalEventsState
        )
        {
            try
            {
                var godotObject = (BradotObject)GCHandle.FromIntPtr(godotObjectGCHandle).Target;

                if (godotObject == null)
                    return;

                // Call OnBeforeSerialize

                // ReSharper disable once SuspiciousTypeConversion.Global
                if (godotObject is ISerializationListener serializationListener)
                    serializationListener.OnBeforeSerialize();

                // Save instance state

                using var info = BradotSerializationInfo.CreateCopyingBorrowed(
                    *propertiesState, *signalEventsState);

                godotObject.SaveBradotObjectData(info);
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
            }
        }

        [UnmanagedCallersOnly]
        internal static unsafe void DeserializeState(
            IntPtr godotObjectGCHandle,
            bradot_dictionary* propertiesState,
            bradot_dictionary* signalEventsState
        )
        {
            try
            {
                var godotObject = (BradotObject)GCHandle.FromIntPtr(godotObjectGCHandle).Target;

                if (godotObject == null)
                    return;

                // Restore instance state

                using var info = BradotSerializationInfo.CreateCopyingBorrowed(
                    *propertiesState, *signalEventsState);

                godotObject.RestoreBradotObjectData(info);

                // Call OnAfterDeserialize

                // ReSharper disable once SuspiciousTypeConversion.Global
                if (godotObject is ISerializationListener serializationListener)
                    serializationListener.OnAfterDeserialize();
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
            }
        }
    }
}
