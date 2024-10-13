using ServerCore;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using UnityEngine;

public class ServerSession : PacketSession
{
    public override void OnConnected(EndPoint endPoint)
    {
        Debug.Log("OnConnect");
    }

    public override void OnDisconnected(EndPoint endPoint)
    {
        Debug.Log("OnDisConnect");
    }

    public override void OnRecvPacket(ArraySegment<byte> data)
    {
        Debug.Log("OnRecvPacket");

    }
    public override void OnSend(int numOfBytes)
    {
        Debug.Log("OnSend");
    }
}
