
using System.Net.Sockets;
using System.Net;
using System.Text;
using ServerCore;

namespace DummyClient
{
    public class ServerSession : PacketSession
    {
        public override void OnConnected(EndPoint endPoint)
        {
            Console.WriteLine($"OnConnected {endPoint.ToString()}");
        }

        public override void OnDisconnected(EndPoint endPoint)
        {
            Console.WriteLine($"OnDisconnected {endPoint.ToString()}");
        }

        public override void OnRecvPacket(ArraySegment<byte> data)
        {
            PacketManager manager = new PacketManager();
            manager.OnRecvPacket(this, data);
        }

        public override void OnSend(int numOfBytes)
        {
            Console.WriteLine($"OnSend byte : {numOfBytes}");
        }
    }
    class Program
    {
        static Connector _connector;
        static void Main(string[] args)
        {
            string host = Dns.GetHostName();
            IPHostEntry entry = Dns.GetHostEntry(host);
            IPAddress addr = entry.AddressList[0];
            IPEndPoint endPoint = new IPEndPoint(addr, 8080);

            _connector = new Connector();
            _connector.Init(endPoint, () => { return new ServerSession();});

            while (true) ;
        }
    }
}