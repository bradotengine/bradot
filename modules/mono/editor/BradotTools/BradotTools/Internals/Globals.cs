using Bradot;
using Bradot.NativeInterop;
using System.Diagnostics.CodeAnalysis;
using System.Runtime.CompilerServices;

namespace BradotTools.Internals
{
    public static class Globals
    {
        public static float EditorScale => Internal.bradot_icall_Globals_EditorScale();

        // ReSharper disable once UnusedMethodReturnValue.Global
        public static Variant GlobalDef(string setting, Variant defaultValue, bool restartIfChanged = false)
        {
            using bradot_string settingIn = Marshaling.ConvertStringToNative(setting);
            using bradot_variant defaultValueIn = defaultValue.CopyNativeVariant();
            Internal.bradot_icall_Globals_GlobalDef(settingIn, defaultValueIn, restartIfChanged,
                out bradot_variant result);
            return Variant.CreateTakingOwnershipOfDisposableValue(result);
        }

        // ReSharper disable once UnusedMethodReturnValue.Global
        public static Variant EditorDef(string setting, Variant defaultValue, bool restartIfChanged = false)
        {
            using bradot_string settingIn = Marshaling.ConvertStringToNative(setting);
            using bradot_variant defaultValueIn = defaultValue.CopyNativeVariant();
            Internal.bradot_icall_Globals_EditorDef(settingIn, defaultValueIn, restartIfChanged,
                out bradot_variant result);
            return Variant.CreateTakingOwnershipOfDisposableValue(result);
        }

        public static Shortcut EditorDefShortcut(string setting, string name, Key keycode = Key.None, bool physical = false)
        {
            using bradot_string settingIn = Marshaling.ConvertStringToNative(setting);
            using bradot_string nameIn = Marshaling.ConvertStringToNative(name);
            Internal.bradot_icall_Globals_EditorDefShortcut(settingIn, nameIn, keycode, physical.ToBradotBool(), out bradot_variant result);
            return (Shortcut)Variant.CreateTakingOwnershipOfDisposableValue(result);
        }

        public static Shortcut EditorGetShortcut(string setting)
        {
            using bradot_string settingIn = Marshaling.ConvertStringToNative(setting);
            Internal.bradot_icall_Globals_EditorGetShortcut(settingIn, out bradot_variant result);
            return (Shortcut)Variant.CreateTakingOwnershipOfDisposableValue(result);
        }

        public static void EditorShortcutOverride(string setting, string feature, Key keycode = Key.None, bool physical = false)
        {
            using bradot_string settingIn = Marshaling.ConvertStringToNative(setting);
            using bradot_string featureIn = Marshaling.ConvertStringToNative(feature);
            Internal.bradot_icall_Globals_EditorShortcutOverride(settingIn, featureIn, keycode, physical.ToBradotBool());
        }

        [SuppressMessage("ReSharper", "InconsistentNaming")]
        public static string TTR(this string text)
        {
            using bradot_string textIn = Marshaling.ConvertStringToNative(text);
            Internal.bradot_icall_Globals_TTR(textIn, out bradot_string dest);
            using (dest)
                return Marshaling.ConvertStringToManaged(dest);
        }
    }
}
