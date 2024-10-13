using System.Net;
using System.Net.Sockets;
using System.Runtime.CompilerServices;

namespace ServerCore
{
    public class Listener
    {
        Socket _listenSocket = null;
        Func<PacketSession> _sessionFactory = null;

        public void Init(EndPoint endPoint, Func<PacketSession> sessionFactory, int listenCount = 5)
        {
            _listenSocket = new Socket(endPoint.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

            _listenSocket.Bind(endPoint);
            _listenSocket.Listen(listenCount);
            _sessionFactory = sessionFactory;

            SocketAsyncEventArgs args = new SocketAsyncEventArgs();
            args.Completed += new EventHandler<SocketAsyncEventArgs>(OnAcceptCompleted);
            RegisterAccept(args);
        }

        void RegisterAccept(SocketAsyncEventArgs args)
        {
            args.AcceptSocket = null;
            bool pending = _listenSocket.AcceptAsync(args);

            if (pending == false)
                OnAcceptCompleted(null, args);
        }
        void OnAcceptCompleted(object sender, SocketAsyncEventArgs args)
        {
            if (args.SocketError == SocketError.Success)
            {
                PacketSession session = _sessionFactory.Invoke();
                session.Init(args.AcceptSocket);
                session.OnConnected(args.AcceptSocket.RemoteEndPoint);
            }
            else
            {
                Console.Error.WriteLine($"Listener : SocketError");
                Utils.LogCurrentLocation(printAction: Console.Error.WriteLine);
            }
            RegisterAccept(args);
        }
    }
}