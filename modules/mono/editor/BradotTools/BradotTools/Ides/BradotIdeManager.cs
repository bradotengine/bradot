using System;
using System.IO;
using System.Threading.Tasks;
using Bradot;
using BradotTools.IdeMessaging;
using BradotTools.IdeMessaging.Requests;
using BradotTools.Internals;

namespace BradotTools.Ides
{
    public sealed partial class BradotIdeManager : Node, ISerializationListener
    {
        private MessagingServer? _messagingServer;

        private MonoDevelop.Instance? _monoDevelInstance;
        private MonoDevelop.Instance? _vsForMacInstance;

        private MessagingServer GetRunningOrNewServer()
        {
            if (_messagingServer != null && !_messagingServer.IsDisposed)
                return _messagingServer;

            _messagingServer?.Dispose();
            _messagingServer = new MessagingServer(OS.GetExecutablePath(),
                ProjectSettings.GlobalizePath(BradotSharpDirs.ResMetadataDir), new BradotLogger());

            _ = _messagingServer.Listen();

            return _messagingServer;
        }

        public override void _Ready()
        {
            _ = GetRunningOrNewServer();
        }

        public void OnBeforeSerialize()
        {
        }

        public void OnAfterDeserialize()
        {
            _ = GetRunningOrNewServer();
        }

        protected override void Dispose(bool disposing)
        {
            base.Dispose(disposing);

            if (disposing)
            {
                _messagingServer?.Dispose();
            }
        }

        private string GetExternalEditorIdentity(ExternalEditorId editorId)
        {
            // Manually convert to string to avoid breaking compatibility in case we rename the enum fields.
            switch (editorId)
            {
                case ExternalEditorId.None:
                    return string.Empty;
                case ExternalEditorId.VisualStudio:
                    return "VisualStudio";
                case ExternalEditorId.VsCode:
                    return "VisualStudioCode";
                case ExternalEditorId.Rider:
                    return "Rider";
                case ExternalEditorId.VisualStudioForMac:
                    return "VisualStudioForMac";
                case ExternalEditorId.MonoDevelop:
                    return "MonoDevelop";
                case ExternalEditorId.CustomEditor:
                    return "CustomEditor";
                default:
                    throw new NotImplementedException();
            }
        }

        public async Task<EditorPick?> LaunchIdeAsync(int millisecondsTimeout = 10000)
        {
            var editorSettings = EditorInterface.Singleton.GetEditorSettings();
            var editorId = editorSettings.GetSetting(BradotSharpEditor.Settings.ExternalEditor).As<ExternalEditorId>();
            string editorIdentity = GetExternalEditorIdentity(editorId);

            var runningServer = GetRunningOrNewServer();

            if (runningServer.IsAnyConnected(editorIdentity))
                return new EditorPick(editorIdentity);

            LaunchIde(editorId, editorIdentity);

            var timeoutTask = Task.Delay(millisecondsTimeout);
            var completedTask = await Task.WhenAny(timeoutTask, runningServer.AwaitClientConnected(editorIdentity));

            if (completedTask != timeoutTask)
                return new EditorPick(editorIdentity);

            return null;
        }

        private void LaunchIde(ExternalEditorId editorId, string editorIdentity)
        {
            switch (editorId)
            {
                case ExternalEditorId.None:
                case ExternalEditorId.VisualStudio:
                case ExternalEditorId.VsCode:
                case ExternalEditorId.Rider:
                case ExternalEditorId.CustomEditor:
                    throw new NotSupportedException();
                case ExternalEditorId.VisualStudioForMac:
                    goto case ExternalEditorId.MonoDevelop;
                case ExternalEditorId.MonoDevelop:
                {
                    MonoDevelop.Instance GetMonoDevelopInstance(string solutionPath)
                    {
                        if (Utils.OS.IsMacOS && editorId == ExternalEditorId.VisualStudioForMac)
                        {
                            _vsForMacInstance = (_vsForMacInstance?.IsDisposed ?? true ? null : _vsForMacInstance) ??
                                               new MonoDevelop.Instance(solutionPath, MonoDevelop.EditorId.VisualStudioForMac);
                            return _vsForMacInstance;
                        }

                        _monoDevelInstance = (_monoDevelInstance?.IsDisposed ?? true ? null : _monoDevelInstance) ??
                                            new MonoDevelop.Instance(solutionPath, MonoDevelop.EditorId.MonoDevelop);
                        return _monoDevelInstance;
                    }

                    try
                    {
                        var instance = GetMonoDevelopInstance(BradotSharpDirs.ProjectSlnPath);

                        if (instance.IsRunning && !GetRunningOrNewServer().IsAnyConnected(editorIdentity))
                        {
                            // After launch we wait up to 30 seconds for the IDE to connect to our messaging server.
                            var waitAfterLaunch = TimeSpan.FromSeconds(30);
                            var timeSinceLaunch = DateTime.Now - instance.LaunchTime;
                            if (timeSinceLaunch > waitAfterLaunch)
                            {
                                instance.Dispose();
                                instance.Execute();
                            }
                        }
                        else if (!instance.IsRunning)
                        {
                            instance.Execute();
                        }
                    }
                    catch (FileNotFoundException)
                    {
                        string editorName = editorId == ExternalEditorId.VisualStudioForMac ? "Visual Studio" : "MonoDevelop";
                        BR.PushError($"Cannot find code editor: {editorName}");
                    }

                    break;
                }

                default:
                    throw new ArgumentOutOfRangeException(nameof(editorId));
            }
        }

        public readonly struct EditorPick
        {
            private readonly string _identity;

            public EditorPick(string identity)
            {
                _identity = identity;
            }

            public bool IsAnyConnected() =>
                BradotSharpEditor.Instance.BradotIdeManager.GetRunningOrNewServer().IsAnyConnected(_identity);

            private void SendRequest<TResponse>(Request request)
                where TResponse : Response, new()
            {
                // Logs an error if no client is connected with the specified identity
                BradotSharpEditor.Instance.BradotIdeManager
                    .GetRunningOrNewServer()
                    .BroadcastRequest<TResponse>(_identity, request);
            }

            public void SendOpenFile(string file)
            {
                SendRequest<OpenFileResponse>(new OpenFileRequest { File = file });
            }

            public void SendOpenFile(string file, int line)
            {
                SendRequest<OpenFileResponse>(new OpenFileRequest { File = file, Line = line });
            }

            public void SendOpenFile(string file, int line, int column)
            {
                SendRequest<OpenFileResponse>(new OpenFileRequest { File = file, Line = line, Column = column });
            }
        }

        public EditorPick PickEditor(ExternalEditorId editorId) => new EditorPick(GetExternalEditorIdentity(editorId));

        private class BradotLogger : ILogger
        {
            public void LogDebug(string message)
            {
                if (OS.IsStdOutVerbose())
                    Console.WriteLine(message);
            }

            public void LogInfo(string message)
            {
                if (OS.IsStdOutVerbose())
                    Console.WriteLine(message);
            }

            public void LogWarning(string message)
            {
                BR.PushWarning(message);
            }

            public void LogError(string message)
            {
                BR.PushError(message);
            }

            public void LogError(string message, Exception e)
            {
                BR.PushError(message + "\n" + e);
            }
        }
    }
}