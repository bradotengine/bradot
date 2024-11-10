using System.Text;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.Text;

namespace Bradot.SourceGenerators
{
    [Generator]
    public class BradotPluginsInitializerGenerator : ISourceGenerator
    {
        public void Initialize(GeneratorInitializationContext context)
        {
        }

        public void Execute(GeneratorExecutionContext context)
        {
            if (context.IsBradotToolsProject() || context.IsBradotSourceGeneratorDisabled("BradotPluginsInitializer"))
                return;

            string source =
                @"using System;
using System.Runtime.InteropServices;
using Bradot.Bridge;
using Bradot.NativeInterop;

namespace BradotPlugins.Game
{
    internal static partial class Main
    {
        [UnmanagedCallersOnly(EntryPoint = ""bradotsharp_game_main_init"")]
        private static bradot_bool InitializeFromGameProject(IntPtr bradotDllHandle, IntPtr outManagedCallbacks,
            IntPtr unmanagedCallbacks, int unmanagedCallbacksSize)
        {
            try
            {
                DllImportResolver dllImportResolver = new BradotDllImportResolver(bradotDllHandle).OnResolveDllImport;

                var coreApiAssembly = typeof(global::Bradot.BradotObject).Assembly;

                NativeLibrary.SetDllImportResolver(coreApiAssembly, dllImportResolver);

                NativeFuncs.Initialize(unmanagedCallbacks, unmanagedCallbacksSize);

                ManagedCallbacks.Create(outManagedCallbacks);

                ScriptManagerBridge.LookupScriptsInAssembly(typeof(global::BradotPlugins.Game.Main).Assembly);

                return bradot_bool.True;
            }
            catch (Exception e)
            {
                global::System.Console.Error.WriteLine(e);
                return false.ToBradotBool();
            }
        }
    }
}
";

            context.AddSource("BradotPlugins.Game.generated",
                SourceText.From(source, Encoding.UTF8));
        }
    }
}
