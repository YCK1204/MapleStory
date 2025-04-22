using System.Collections.Generic;
using System.Net.Sockets;
using System.Net;
using System.Threading;
using System;
namespace ServerCore
{
    public abstract class PacketSession : Session
    {
        private static readonly ushort HeaderSize = 2;

        public sealed override int OnRecv(ArraySegment<byte> buffer)
        {
            int num = 0;
            while (buffer.Count >= HeaderSize)
            {
                ushort num2 = BitConverter.ToUInt16(buffer.Array, buffer.Offset);
                if (num2 < 0 || num2 > buffer.Count)
                {
                    break;
                }

                OnRecvPacket(new ArraySegment<byte>(buffer.Array, buffer.Offset, num2));
                num += num2;
                buffer = new ArraySegment<byte>(buffer.Array, buffer.Offset + num2, buffer.Count - num2);
            }

            return num;
        }

        public abstract void OnRecvPacket(ArraySegment<byte> data);
    }
    public abstract class Session
    {
        private Socket _socket;

        private int _disconnect = 0;

        private object _lock = new object();

        private SocketAsyncEventArgs _sendArgs = new SocketAsyncEventArgs();

        private SocketAsyncEventArgs _recvArgs = new SocketAsyncEventArgs();

        private Queue<byte[]> _sendQueue = new Queue<byte[]>();

        private List<ArraySegment<byte>> _pendingList = new List<ArraySegment<byte>>();

        private RecvBuffer _recvBuffer = new RecvBuffer(UInt16.MaxValue);

        public abstract void OnSend(int numOfBytes);

        public abstract int OnRecv(ArraySegment<byte> bytes);

        public abstract void OnConnect(EndPoint endPoint);

        public abstract void OnDisconnect(EndPoint endPoint);

        public void Start(Socket socket)
        {
            _socket = socket;
            _recvArgs.Completed += OnRecvCompleted;
            _sendArgs.Completed += OnSendCompleted;
            RegisterRecv();
        }

        public void Send(ArraySegment<byte> data)
        {
            Send(data.Array);
        }

        public void Send(byte[] data)
        {
            lock (_lock)
            {
                _sendQueue.Enqueue(data);
                if (_pendingList.Count == 0)
                {
                    RegisterSend();
                }
            }
        }

        private void RegisterSend()
        {
            while (_sendQueue.Count > 0)
            {
                byte[] array = _sendQueue.Dequeue();
                _pendingList.Add(new ArraySegment<byte>(array, 0, array.Length));
            }

            _sendArgs.BufferList = _pendingList;
            if (!_socket.SendAsync(_sendArgs))
            {
                OnSendCompleted(null, _sendArgs);
            }
        }

        private void OnSendCompleted(object sender, SocketAsyncEventArgs args)
        {
            if (args.SocketError == SocketError.Success && args.BytesTransferred > 0)
            {
                _pendingList.Clear();
                OnSend(args.BytesTransferred);
                if (_sendQueue.Count > 0)
                {
                    RegisterSend();
                }
            }
            else
            {
                Console.WriteLine("OnSendCompleted Failed");
            }
        }

        private void RegisterRecv()
        {
            _recvBuffer.Clean();
            ArraySegment<byte> writeSegment = _recvBuffer.WriteSegment;
            _recvArgs.SetBuffer(writeSegment.Array, writeSegment.Offset, writeSegment.Count);
            if (!_socket.ReceiveAsync(_recvArgs))
            {
                OnRecvCompleted(null, _recvArgs);
            }
        }

        private void OnRecvCompleted(object sender, SocketAsyncEventArgs args)
        {
            if (args.SocketError == SocketError.Success && args.BytesTransferred > 0)
            {
                if (!_recvBuffer.OnWrite((ushort)args.BytesTransferred))
                {
                    Disconnect();
                    return;
                }

                int num = OnRecv(_recvBuffer.ReadSegment);
                if (num > _recvBuffer.DataSize)
                {
                    Disconnect();
                }
                else if (!_recvBuffer.OnRead((ushort)num))
                {
                    Disconnect();
                }
                else
                {
                    RegisterRecv();
                }
            }
            else
            {
                Console.WriteLine("OnRecvCompleted Failed");
                Disconnect();
            }
        }

        private void Disconnect()
        {
            if (Interlocked.Exchange(ref _disconnect, 1) != 1)
            {
                OnDisconnect(_socket.RemoteEndPoint);
                _socket.Shutdown(SocketShutdown.Both);
            }
        }
    }
}
