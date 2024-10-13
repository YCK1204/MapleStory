using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace ServerCore
{
    public class Connector
    {
        Func<PacketSession> _sessionFactory;
        public void Init(EndPoint endPoint, Func<PacketSession> sessionFactory)
        {
            Socket sock = new Socket(endPoint.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

            _sessionFactory = sessionFactory;
            SocketAsyncEventArgs args = new SocketAsyncEventArgs();
            args.UserToken = sock;
            args.Completed += new EventHandler<SocketAsyncEventArgs>(OnConnectCompleted);
            args.RemoteEndPoint = endPoint;
            RegisterConnect(args);
        }

        void RegisterConnect(SocketAsyncEventArgs args)
        {
            Socket sock = args.UserToken as Socket;
            if (sock == null)
            {
                Console.Error.WriteLine("RegisterConnect Error");
                Utils.LogCurrentLocation(printAction: Console.Error.WriteLine);
                return;
            }
            bool pending = sock.ConnectAsync(args);
            if (pending == false)
                OnConnectCompleted(null, args);
        }

        void OnConnectCompleted(object sender, SocketAsyncEventArgs args)
        {
            if (args.SocketError == SocketError.Success)
            {
                PacketSession session = _sessionFactory.Invoke();
                session.Init(args.ConnectSocket);
                session.OnConnected(args.ConnectSocket.RemoteEndPoint);
            }
            else
            {
                Console.Error.WriteLine($"OnConnectCompleted Failed");
                Utils.LogCurrentLocation(printAction:Console.Error.WriteLine);
            }
        }
    }
}
