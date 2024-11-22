using System;
using System.Collections.Generic;
using UnityEngine;
using ServerCore;
using System.Net;
using Google.FlatBuffers;
using System.Text;
using System.Collections;
public class NetworkManager : IManager
{
    Queue<ArraySegment<byte>> _queue = new Queue<ArraySegment<byte>>();
    object _lock = new object();
    ServerSession _session = new ServerSession();
    Connector _conenctor = new Connector();
    public void Send(byte[] packet)
    {
        _session.Send(packet);
    }
    public void Push(ArraySegment<byte> segment)
    {
        lock (_lock)
        {
            _queue.Enqueue(segment);
        }
    }
    public void Clear()
    {
    }
    public void Init()
    {
        IPEndPoint endPoint = new IPEndPoint(IPAddress.Loopback, 8080);
        _conenctor.Init(endPoint, () => { return _session; });
    }
    public void COco()
    {
        string a = "hello world!";
        byte[] bytes = Encoding.UTF8.GetBytes(a);
        ushort size = (ushort)((bytes.Length) + sizeof(ushort));
        var pkt = new ArraySegment<byte>(new byte[size]);
        BitConverter.TryWriteBytes(new Span<byte>(pkt.Array, pkt.Offset, pkt.Count), size);
        Array.Copy(bytes, 0, pkt.Array, 2, bytes.Length);
        Send(pkt.Array);
    }
    public void Update()
    {


        List<ArraySegment<byte>> list = new List<ArraySegment<byte>>();
        lock (_lock)
        {
            while (_queue.Count > 0)
            {
                list.Add(_queue.Dequeue());
            }
        }
        FlatBufferBuilder builder = new FlatBufferBuilder(10);
        foreach (ArraySegment<byte> segment in list)
        {
            var size = BitConverter.ToUInt16(segment.Array, 0);
            Debug.Log(size);
            byte[] b = new byte[size - 2];
            Array.Copy(segment.Array, 2, b, 0, size - 2);
            ByteBuffer bb = new ByteBuffer(b);
            var test = Test.GetRootAsTest(bb);
            Debug.Log($"recved test str : {test.Str}, int : {test.Test_}");
        }
        //    Manager.Packet.OnRecvPacket(_session, segment);
    }
}
