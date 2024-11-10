using System.Diagnostics.CodeAnalysis;

namespace BradotTools.IdeMessaging
{
    public interface IHandshake
    {
        string GetHandshakeLine(string identity);
        bool IsValidPeerHandshake(string handshake, [NotNullWhen(true)] out string? identity, ILogger logger);
    }
}
