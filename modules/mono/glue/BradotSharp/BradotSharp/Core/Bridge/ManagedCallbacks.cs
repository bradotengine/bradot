using System;
using System.Runtime.InteropServices;
using Bradot.NativeInterop;

namespace Bradot.Bridge
{
    [StructLayout(LayoutKind.Sequential)]
    public unsafe struct ManagedCallbacks
    {
        // @formatter:off
        public delegate* unmanaged<IntPtr, bradot_variant**, int, bradot_bool*, void> SignalAwaiter_SignalCallback;
        public delegate* unmanaged<IntPtr, void*, bradot_variant**, int, bradot_variant*, void> DelegateUtils_InvokeWithVariantArgs;
        public delegate* unmanaged<IntPtr, IntPtr, bradot_bool> DelegateUtils_DelegateEquals;
        public delegate* unmanaged<IntPtr, int> DelegateUtils_DelegateHash;
        public delegate* unmanaged<IntPtr, bradot_bool*, int> DelegateUtils_GetArgumentCount;
        public delegate* unmanaged<IntPtr, bradot_array*, bradot_bool> DelegateUtils_TrySerializeDelegateWithGCHandle;
        public delegate* unmanaged<bradot_array*, IntPtr*, bradot_bool> DelegateUtils_TryDeserializeDelegateWithGCHandle;
        public delegate* unmanaged<void> ScriptManagerBridge_FrameCallback;
        public delegate* unmanaged<bradot_string_name*, IntPtr, IntPtr> ScriptManagerBridge_CreateManagedForBradotObjectBinding;
        public delegate* unmanaged<IntPtr, IntPtr, bradot_variant**, int, bradot_bool> ScriptManagerBridge_CreateManagedForBradotObjectScriptInstance;
        public delegate* unmanaged<IntPtr, bradot_string_name*, void> ScriptManagerBridge_GetScriptNativeName;
        public delegate* unmanaged<bradot_string*, bradot_string*, bradot_string*, bradot_string*, void> ScriptManagerBridge_GetGlobalClassName;
        public delegate* unmanaged<IntPtr, IntPtr, void> ScriptManagerBridge_SetBradotObjectPtr;
        public delegate* unmanaged<IntPtr, bradot_string_name*, bradot_variant**, int, bradot_bool*, void> ScriptManagerBridge_RaiseEventSignal;
        public delegate* unmanaged<IntPtr, IntPtr, bradot_bool> ScriptManagerBridge_ScriptIsOrInherits;
        public delegate* unmanaged<IntPtr, bradot_string*, bradot_bool> ScriptManagerBridge_AddScriptBridge;
        public delegate* unmanaged<bradot_string*, bradot_ref*, void> ScriptManagerBridge_GetOrCreateScriptBridgeForPath;
        public delegate* unmanaged<IntPtr, void> ScriptManagerBridge_RemoveScriptBridge;
        public delegate* unmanaged<IntPtr, bradot_bool> ScriptManagerBridge_TryReloadRegisteredScriptWithClass;
        public delegate* unmanaged<IntPtr, bradot_csharp_type_info*, bradot_array*, bradot_dictionary*, bradot_dictionary*, bradot_ref*, void> ScriptManagerBridge_UpdateScriptClassInfo;
        public delegate* unmanaged<IntPtr, IntPtr*, bradot_bool, bradot_bool> ScriptManagerBridge_SwapGCHandleForType;
        public delegate* unmanaged<IntPtr, delegate* unmanaged<IntPtr, bradot_string*, void*, int, void>, void> ScriptManagerBridge_GetPropertyInfoList;
        public delegate* unmanaged<IntPtr, delegate* unmanaged<IntPtr, void*, int, void>, void> ScriptManagerBridge_GetPropertyDefaultValues;
        public delegate* unmanaged<IntPtr, bradot_string_name*, bradot_variant**, int, bradot_variant_call_error*, bradot_variant*, bradot_bool> ScriptManagerBridge_CallStatic;
        public delegate* unmanaged<IntPtr, bradot_string_name*, bradot_variant**, int, bradot_variant_call_error*, bradot_variant*, bradot_bool> CSharpInstanceBridge_Call;
        public delegate* unmanaged<IntPtr, bradot_string_name*, bradot_variant*, bradot_bool> CSharpInstanceBridge_Set;
        public delegate* unmanaged<IntPtr, bradot_string_name*, bradot_variant*, bradot_bool> CSharpInstanceBridge_Get;
        public delegate* unmanaged<IntPtr, bradot_bool, void> CSharpInstanceBridge_CallDispose;
        public delegate* unmanaged<IntPtr, bradot_string*, bradot_bool*, void> CSharpInstanceBridge_CallToString;
        public delegate* unmanaged<IntPtr, bradot_string_name*, bradot_bool> CSharpInstanceBridge_HasMethodUnknownParams;
        public delegate* unmanaged<IntPtr, bradot_dictionary*, bradot_dictionary*, void> CSharpInstanceBridge_SerializeState;
        public delegate* unmanaged<IntPtr, bradot_dictionary*, bradot_dictionary*, void> CSharpInstanceBridge_DeserializeState;
        public delegate* unmanaged<IntPtr, void> GCHandleBridge_FreeGCHandle;
        public delegate* unmanaged<IntPtr, bradot_bool> GCHandleBridge_GCHandleIsTargetCollectible;
        public delegate* unmanaged<void*, void> DebuggingUtils_GetCurrentStackInfo;
        public delegate* unmanaged<void> DisposablesTracker_OnBradotShuttingDown;
        public delegate* unmanaged<bradot_bool, void> BR_OnCoreApiAssemblyLoaded;
        // @formatter:on

        public static ManagedCallbacks Create()
        {
            return new()
            {
                // @formatter:off
                SignalAwaiter_SignalCallback = &SignalAwaiter.SignalCallback,
                DelegateUtils_InvokeWithVariantArgs = &DelegateUtils.InvokeWithVariantArgs,
                DelegateUtils_DelegateEquals = &DelegateUtils.DelegateEquals,
                DelegateUtils_DelegateHash = &DelegateUtils.DelegateHash,
                DelegateUtils_GetArgumentCount = &DelegateUtils.GetArgumentCount,
                DelegateUtils_TrySerializeDelegateWithGCHandle = &DelegateUtils.TrySerializeDelegateWithGCHandle,
                DelegateUtils_TryDeserializeDelegateWithGCHandle = &DelegateUtils.TryDeserializeDelegateWithGCHandle,
                ScriptManagerBridge_FrameCallback = &ScriptManagerBridge.FrameCallback,
                ScriptManagerBridge_CreateManagedForBradotObjectBinding = &ScriptManagerBridge.CreateManagedForBradotObjectBinding,
                ScriptManagerBridge_CreateManagedForBradotObjectScriptInstance = &ScriptManagerBridge.CreateManagedForBradotObjectScriptInstance,
                ScriptManagerBridge_GetScriptNativeName = &ScriptManagerBridge.GetScriptNativeName,
                ScriptManagerBridge_GetGlobalClassName = &ScriptManagerBridge.GetGlobalClassName,
                ScriptManagerBridge_SetBradotObjectPtr = &ScriptManagerBridge.SetBradotObjectPtr,
                ScriptManagerBridge_RaiseEventSignal = &ScriptManagerBridge.RaiseEventSignal,
                ScriptManagerBridge_ScriptIsOrInherits = &ScriptManagerBridge.ScriptIsOrInherits,
                ScriptManagerBridge_AddScriptBridge = &ScriptManagerBridge.AddScriptBridge,
                ScriptManagerBridge_GetOrCreateScriptBridgeForPath = &ScriptManagerBridge.GetOrCreateScriptBridgeForPath,
                ScriptManagerBridge_RemoveScriptBridge = &ScriptManagerBridge.RemoveScriptBridge,
                ScriptManagerBridge_TryReloadRegisteredScriptWithClass = &ScriptManagerBridge.TryReloadRegisteredScriptWithClass,
                ScriptManagerBridge_UpdateScriptClassInfo = &ScriptManagerBridge.UpdateScriptClassInfo,
                ScriptManagerBridge_SwapGCHandleForType = &ScriptManagerBridge.SwapGCHandleForType,
                ScriptManagerBridge_GetPropertyInfoList = &ScriptManagerBridge.GetPropertyInfoList,
                ScriptManagerBridge_GetPropertyDefaultValues = &ScriptManagerBridge.GetPropertyDefaultValues,
                ScriptManagerBridge_CallStatic = &ScriptManagerBridge.CallStatic,
                CSharpInstanceBridge_Call = &CSharpInstanceBridge.Call,
                CSharpInstanceBridge_Set = &CSharpInstanceBridge.Set,
                CSharpInstanceBridge_Get = &CSharpInstanceBridge.Get,
                CSharpInstanceBridge_CallDispose = &CSharpInstanceBridge.CallDispose,
                CSharpInstanceBridge_CallToString = &CSharpInstanceBridge.CallToString,
                CSharpInstanceBridge_HasMethodUnknownParams = &CSharpInstanceBridge.HasMethodUnknownParams,
                CSharpInstanceBridge_SerializeState = &CSharpInstanceBridge.SerializeState,
                CSharpInstanceBridge_DeserializeState = &CSharpInstanceBridge.DeserializeState,
                GCHandleBridge_FreeGCHandle = &GCHandleBridge.FreeGCHandle,
                GCHandleBridge_GCHandleIsTargetCollectible = &GCHandleBridge.GCHandleIsTargetCollectible,
                DebuggingUtils_GetCurrentStackInfo = &DebuggingUtils.GetCurrentStackInfo,
                DisposablesTracker_OnBradotShuttingDown = &DisposablesTracker.OnBradotShuttingDown,
                BR_OnCoreApiAssemblyLoaded = &BR.OnCoreApiAssemblyLoaded,
                // @formatter:on
            };
        }

        public static void Create(IntPtr outManagedCallbacks)
            => *(ManagedCallbacks*)outManagedCallbacks = Create();
    }
}
