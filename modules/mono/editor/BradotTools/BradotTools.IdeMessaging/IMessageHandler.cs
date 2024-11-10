using System.Threading.Tasks;

namespace BradotTools.IdeMessaging
{
    public interface IMessageHandler
    {
        Task<MessageContent> HandleRequest(Peer peer, string id, MessageContent content, ILogger logger);
    }
}
