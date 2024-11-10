using System;
using System.Collections.Generic;
using Bradot;
using BradotTools.Build;
using BradotTools.Utils;

namespace BradotTools.Inspector
{
    public partial class InspectorPlugin : EditorInspectorPlugin
    {
        public override bool _CanHandle(BradotObject godotObject)
        {
            foreach (var script in EnumerateScripts(godotObject))
            {
                if (script is CSharpScript)
                {
                    return true;
                }
            }
            return false;
        }

        public override void _ParseBegin(BradotObject godotObject)
        {
            foreach (var script in EnumerateScripts(godotObject))
            {
                if (script is not CSharpScript)
                    continue;

                string scriptPath = script.ResourcePath;

                if (string.IsNullOrEmpty(scriptPath))
                {
                    // Generic types used empty paths in older versions of Bradot
                    // so we assume your project is out of sync.
                    AddCustomControl(new InspectorOutOfSyncWarning());
                    break;
                }

                if (scriptPath.StartsWith("csharp://"))
                {
                    // This is a virtual path used by generic types, extract the real path.
                    var scriptPathSpan = scriptPath.AsSpan("csharp://".Length);
                    scriptPathSpan = scriptPathSpan[..scriptPathSpan.IndexOf(':')];
                    scriptPath = $"res://{scriptPathSpan}";
                }

                if (File.GetLastWriteTime(scriptPath) > BuildManager.LastValidBuildDateTime)
                {
                    AddCustomControl(new InspectorOutOfSyncWarning());
                    break;
                }
            }
        }

        private static IEnumerable<Script> EnumerateScripts(BradotObject godotObject)
        {
            var script = godotObject.GetScript().As<Script>();
            while (script != null)
            {
                yield return script;
                script = script.GetBaseScript();
            }
        }
    }
}
