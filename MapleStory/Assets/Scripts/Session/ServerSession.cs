using Google.FlatBuffers;
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
        Debug.Log($"OnSend {numOfBytes}");
    }
}