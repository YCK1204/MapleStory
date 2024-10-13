using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace ServerCore
{
    public abstract class PacketSession : Session
    {
        static readonly ushort HeaderSize = 2;
        public sealed override int OnRecv(ArraySegment<byte> buffer)
        {
            int processLen = 0;
            while (true)
            {
                if (buffer.Count < HeaderSize)
                    break;
                ushort dataSize = BitConverter.ToUInt16(buffer.Array, buffer.Offset);
                if (dataSize > buffer.Count || dataSize < HeaderSize)
                    break;
                OnRecvPacket(new ArraySegment<byte>(buffer.Array, buffer.Offset, dataSize));

                processLen += dataSize;
                buffer = new ArraySegment<byte>(buffer.Array, buffer.Offset + dataSize, buffer.Count - dataSize);
            }
            return processLen;
        }
        public abstract void OnRecvPacket(ArraySegment<byte> data);
    }

    public abstract class Session
    {
        Socket _socket;
        int _disconnected = 0;
        SocketAsyncEventArgs _sendArgs = new SocketAsyncEventArgs();
        SocketAsyncEventArgs _recvArgs = new SocketAsyncEventArgs();
        Queue<byte[]> _sendQueue = new Queue<byte[]>();
        object _lock = new object();
        List<ArraySegment<byte>> _pendingList = new List<ArraySegment<byte>>();
        RecvBuffer _recvBuffer = new RecvBuffer(65535);

        public abstract void OnConnected(EndPoint endPoint);
        public abstract void OnDisconnected(EndPoint endPoint);
        public abstract int OnRecv(ArraySegment<byte> data);
        public abstract void OnSend(int numOfBytes);

        public void Init(Socket socket)
        {
            _socket = socket;
            _recvArgs.Completed += new EventHandler<SocketAsyncEventArgs>(OnRecvCompleted);
            _sendArgs.Completed += new EventHandler<SocketAsyncEventArgs>(OnSendCompleted);
            RegisterRecv();
        }

        public void Disconnect()
        {
            if (Interlocked.Exchange(ref _disconnected, 1) == 1)
                return;
            OnDisconnected(_socket.RemoteEndPoint);
            _socket.Shutdown(SocketShutdown.Both);
            _socket.Close();
        }

        public void Send(Byte[] data)
        {
            lock (_lock)
            {
                _sendQueue.Enqueue(data);
                if (_pendingList.Count == 0)
                    RegisterSend();
            }
        }
        #region Network
        void RegisterSend()
        {
            while (_sendQueue.Count > 0)
            {
                byte[] data = _sendQueue.Dequeue();
                _pendingList.Add(data);
            }

            _sendArgs.BufferList = _pendingList;
            bool pending = _socket.SendAsync(_sendArgs);

            if (pending == false)
                OnSendCompleted(null, _sendArgs);
        }

        void OnSendCompleted(object sender, SocketAsyncEventArgs args)
        {
            lock (_lock)
            {
                if (args.BytesTransferred > 0 && args.SocketError == SocketError.Success)
                {
                    _pendingList.Clear();
                    OnSend(args.BytesTransferred);
                    if (_sendQueue.Count > 0)
                        RegisterSend();
                }
                else
                {
                    Console.Error.WriteLine($"Session : Send Error");
                    Utils.LogCurrentLocation(printAction: Console.Error.WriteLine);
                }
            }
        }
        void RegisterRecv()
        {
            _recvBuffer.Clear();
            ArraySegment<byte> segment = _recvBuffer.WriteSegment;
            _recvArgs.SetBuffer(segment.Array, segment.Offset, segment.Count);

            bool pending = _socket.ReceiveAsync(_recvArgs);
            if (pending == false)
                OnRecvCompleted(null, _recvArgs);
        }

        void OnRecvCompleted(object sender, SocketAsyncEventArgs args)
        {
            if (args.SocketError == SocketError.Success && args.BytesTransferred > 0)
            {
                if (_recvBuffer.OnWrite(args.BytesTransferred) == false)
                {
                    Disconnect();
                    return;
                }
                int processLen = OnRecv(_recvBuffer.ReadSegment);
                if (processLen > _recvBuffer.ReadSize)
                {
                    Disconnect();
                    return;
                }
                if (_recvBuffer.OnRead(processLen) == false)
                {
                    Disconnect();
                    return;
                }
                RegisterRecv();
            }
            else
            {
                Console.Error.WriteLine($"Session : Recv Error");
                Utils.LogCurrentLocation(printAction: Console.Error.WriteLine);
                Disconnect();
            }
        }
    }
    #endregion
}