using System.Net.Sockets;
using System.Net;
using System;

namespace ServerCore
{
    public class Connector
    {
        private Func<Session> _sessionFactory;

        public void Init(IPEndPoint endPoint, Func<Session> sessionFactory)
        {
            Socket userToken = new Socket(endPoint.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
            _sessionFactory = sessionFactory;
            SocketAsyncEventArgs socketAsyncEventArgs = new SocketAsyncEventArgs();
            socketAsyncEventArgs.UserToken = userToken;
            socketAsyncEventArgs.RemoteEndPoint = endPoint;
            socketAsyncEventArgs.Completed += OnConnectCompleted;
            RegisterConnect(socketAsyncEventArgs);
        }

        private void RegisterConnect(SocketAsyncEventArgs args)
        {
            Socket socket = args.UserToken as Socket;
            if (!socket.ConnectAsync(args))
            {
                OnConnectCompleted(null, args);
            }
        }

        private void OnConnectCompleted(object sender, SocketAsyncEventArgs args)
        {
            if (args.SocketError == SocketError.Success)
            {
                Session session = _sessionFactory();
                session.Start(args.ConnectSocket);
                session.OnConnect(args.ConnectSocket.RemoteEndPoint);
            }
            else
            {
                Console.WriteLine("connected failed");
                RegisterConnect(args);
            }
        }
    }
}
