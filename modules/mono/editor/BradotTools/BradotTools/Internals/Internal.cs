#pragma warning disable IDE1006 // Naming rule violation
// ReSharper disable InconsistentNaming

using System;
using System.Diagnostics.CodeAnalysis;
using System.Runtime.CompilerServices;
using Bradot;
using Bradot.NativeInterop;
using Bradot.SourceGenerators.Internal;
using BradotTools.IdeMessaging.Requests;

namespace BradotTools.Internals
{
    [GenerateUnmanagedCallbacks(typeof(InternalUnmanagedCallbacks))]
    internal static partial class Internal
    {
        public const string CSharpLanguageType = "CSharpScript";
        public const string CSharpLanguageExtension = ".cs";

        public static string FullExportTemplatesDir
        {
            get
            {
                bradot_icall_Internal_FullExportTemplatesDir(out bradot_string dest);
                using (dest)
                    return Marshaling.ConvertStringToManaged(dest);
            }
        }

        public static string SimplifyBradotPath(this string path) => Bradot.StringExtensions.SimplifyPath(path);

        public static bool IsMacOSAppBundleInstalled(string bundleId)
        {
            using bradot_string bundleIdIn = Marshaling.ConvertStringToNative(bundleId);
            return bradot_icall_Internal_IsMacOSAppBundleInstalled(bundleIdIn);
        }

        public static bool LipOCreateFile(string outputPath, string[] files)
        {
            using bradot_string outputPathIn = Marshaling.ConvertStringToNative(outputPath);
            using bradot_packed_string_array filesIn = Marshaling.ConvertSystemArrayToNativePackedStringArray(files);
            return bradot_icall_Internal_LipOCreateFile(outputPathIn, filesIn);
        }

        public static bool BradotIs32Bits() => bradot_icall_Internal_BradotIs32Bits();

        public static bool BradotIsRealTDouble() => bradot_icall_Internal_BradotIsRealTDouble();

        public static void BradotMainIteration() => bradot_icall_Internal_BradotMainIteration();

        public static bool IsAssembliesReloadingNeeded() => bradot_icall_Internal_IsAssembliesReloadingNeeded();

        public static void ReloadAssemblies(bool softReload) => bradot_icall_Internal_ReloadAssemblies(softReload);

        public static void EditorDebuggerNodeReloadScripts() => bradot_icall_Internal_EditorDebuggerNodeReloadScripts();

        public static bool ScriptEditorEdit(Resource resource, int line, int col, bool grabFocus = true) =>
            bradot_icall_Internal_ScriptEditorEdit(resource.NativeInstance, line, col, grabFocus);

        public static void EditorNodeShowScriptScreen() => bradot_icall_Internal_EditorNodeShowScriptScreen();

        public static void EditorRunPlay() => bradot_icall_Internal_EditorRunPlay();

        public static void EditorRunStop() => bradot_icall_Internal_EditorRunStop();

        public static void EditorPlugin_AddControlToEditorRunBar(Control control) =>
            bradot_icall_Internal_EditorPlugin_AddControlToEditorRunBar(control.NativeInstance);

        public static void ScriptEditorDebugger_ReloadScripts() =>
            bradot_icall_Internal_ScriptEditorDebugger_ReloadScripts();

        public static string[] CodeCompletionRequest(CodeCompletionRequest.CompletionKind kind,
            string scriptFile)
        {
            using bradot_string scriptFileIn = Marshaling.ConvertStringToNative(scriptFile);
            bradot_icall_Internal_CodeCompletionRequest((int)kind, scriptFileIn, out bradot_packed_string_array res);
            using (res)
                return Marshaling.ConvertNativePackedStringArrayToSystemArray(res);
        }

        #region Internal

        private static bool initialized = false;

        // ReSharper disable once ParameterOnlyUsedForPreconditionCheck.Global
        internal static unsafe void Initialize(IntPtr unmanagedCallbacks, int unmanagedCallbacksSize)
        {
            if (initialized)
                throw new InvalidOperationException("Already initialized.");
            initialized = true;

            if (unmanagedCallbacksSize != sizeof(InternalUnmanagedCallbacks))
                throw new ArgumentException("Unmanaged callbacks size mismatch.", nameof(unmanagedCallbacksSize));

            _unmanagedCallbacks = Unsafe.AsRef<InternalUnmanagedCallbacks>((void*)unmanagedCallbacks);
        }

        private partial struct InternalUnmanagedCallbacks
        {
        }

        /*
         * IMPORTANT:
         * The order of the methods defined in NativeFuncs must match the order
         * in the array defined at the bottom of 'editor/editor_internal_calls.cpp'.
         */

        public static partial void bradot_icall_BradotSharpDirs_ResMetadataDir(out bradot_string r_dest);

        public static partial void bradot_icall_BradotSharpDirs_MonoUserDir(out bradot_string r_dest);

        public static partial void bradot_icall_BradotSharpDirs_BuildLogsDirs(out bradot_string r_dest);

        public static partial void bradot_icall_BradotSharpDirs_DataEditorToolsDir(out bradot_string r_dest);

        public static partial void bradot_icall_BradotSharpDirs_CSharpProjectName(out bradot_string r_dest);

        public static partial void bradot_icall_EditorProgress_Create(in bradot_string task, in bradot_string label,
            int amount, bool canCancel);

        public static partial void bradot_icall_EditorProgress_Dispose(in bradot_string task);

        public static partial bool bradot_icall_EditorProgress_Step(in bradot_string task, in bradot_string state,
            int step,
            bool forceRefresh);

        private static partial void bradot_icall_Internal_FullExportTemplatesDir(out bradot_string dest);

        private static partial bool bradot_icall_Internal_IsMacOSAppBundleInstalled(in bradot_string bundleId);

        private static partial bool bradot_icall_Internal_LipOCreateFile(in bradot_string outputPath, in bradot_packed_string_array files);

        private static partial bool bradot_icall_Internal_BradotIs32Bits();

        private static partial bool bradot_icall_Internal_BradotIsRealTDouble();

        private static partial void bradot_icall_Internal_BradotMainIteration();

        private static partial bool bradot_icall_Internal_IsAssembliesReloadingNeeded();

        private static partial void bradot_icall_Internal_ReloadAssemblies(bool softReload);

        private static partial void bradot_icall_Internal_EditorDebuggerNodeReloadScripts();

        private static partial bool bradot_icall_Internal_ScriptEditorEdit(IntPtr resource, int line, int col,
            bool grabFocus);

        private static partial void bradot_icall_Internal_EditorNodeShowScriptScreen();

        private static partial void bradot_icall_Internal_EditorRunPlay();

        private static partial void bradot_icall_Internal_EditorRunStop();

        private static partial void bradot_icall_Internal_EditorPlugin_AddControlToEditorRunBar(IntPtr p_control);

        private static partial void bradot_icall_Internal_ScriptEditorDebugger_ReloadScripts();

        private static partial void bradot_icall_Internal_CodeCompletionRequest(int kind, in bradot_string scriptFile,
            out bradot_packed_string_array res);

        public static partial float bradot_icall_Globals_EditorScale();

        public static partial void bradot_icall_Globals_GlobalDef(in bradot_string setting, in bradot_variant defaultValue,
            bool restartIfChanged, out bradot_variant result);

        public static partial void bradot_icall_Globals_EditorDef(in bradot_string setting, in bradot_variant defaultValue,
            bool restartIfChanged, out bradot_variant result);

        public static partial void
            bradot_icall_Globals_EditorDefShortcut(in bradot_string setting, in bradot_string name, Key keycode, bradot_bool physical, out bradot_variant result);

        public static partial void
            bradot_icall_Globals_EditorGetShortcut(in bradot_string setting, out bradot_variant result);

        public static partial void
            bradot_icall_Globals_EditorShortcutOverride(in bradot_string setting, in bradot_string feature, Key keycode, bradot_bool physical);

        public static partial void bradot_icall_Globals_TTR(in bradot_string text, out bradot_string dest);

        public static partial void bradot_icall_Utils_OS_GetPlatformName(out bradot_string dest);

        public static partial bool bradot_icall_Utils_OS_UnixFileHasExecutableAccess(in bradot_string filePath);

        #endregion
    }
}
