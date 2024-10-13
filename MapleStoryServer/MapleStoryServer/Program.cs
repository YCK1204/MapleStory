using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using Google.FlatBuffers;
using MapleStoryServer.Session;
using ServerCore;

namespace MapleStoryServer
{
    class Program
    {
        static Listener _listener = new Listener();
        static void Main(string[] args)
        {
            string host = Dns.GetHostName();
            IPHostEntry entry = Dns.GetHostEntry(host);
            IPAddress addr = entry.AddressList[0];
            IPEndPoint endPoint = new IPEndPoint(addr, 8080);

            ProtocolType type = ProtocolType.Tcp;
            Console.WriteLine($"create socket! addr : {addr}, ProtocolType : {type}");
            _listener.Init(endPoint, () =>
            {
                return new ClientSession();
            });
            while (true) ;
        }
    }
}