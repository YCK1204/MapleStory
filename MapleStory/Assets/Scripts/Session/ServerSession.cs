using ServerCore;
using System;
using System.Net;
using System.Text;
using UnityEngine;

public class ServerSession : PacketSession
{
    public override void OnConnect(EndPoint endPoint)
    {
        Debug.Log($"OnConnect");
        string a = "hello world!";
        byte[] bytes = Encoding.UTF8.GetBytes(a);
        ushort size = (ushort)((bytes.Length) + sizeof(ushort));
        var pkt = new ArraySegment<byte>(new byte[size]);
        BitConverter.TryWriteBytes(new Span<byte>(pkt.Array, pkt.Offset, pkt.Count), size);
        Array.Copy(bytes, 0, pkt.Array, 2, bytes.Length);
        Send(pkt);
    }

    public override void OnDisconnect(EndPoint endPoint)
    {
        Debug.Log($"OnDisconnect");
    }

    public override void OnRecvPacket(ArraySegment<byte> data)
    {
        Debug.Log($"OnRecvPacket");
        Manager.Network.Push(data);
    }

    public override void OnSend(int numOfBytes)
    {
        Debug.Log($"OnSend");
    }
}
