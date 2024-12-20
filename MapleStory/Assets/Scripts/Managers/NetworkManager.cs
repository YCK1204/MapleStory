using System;
using System.Collections.Generic;
using UnityEngine;
using ServerCore;
using System.Net;
using Google.FlatBuffers;
using System.Text;
using System.Collections;
using Newtonsoft.Json.Linq;
using System.IO;
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
        TextAsset PortJson = Manager.Resource.Load<TextAsset>("Json/Port");

        JObject json = JObject.Parse(PortJson.ToString());
        int serverPort = json["client_to_server"].Value<int>();
        IPEndPoint endPoint = new IPEndPoint(IPAddress.Loopback, serverPort);

        _conenctor.Init(endPoint, () => { return _session; });
    }
    public void Update()
    {
        List<ArraySegment<byte>> list = new List<ArraySegment<byte>>();
        lock (_lock)
        {
            while (_queue.Count > 0)
            {
                Manager.Packet.OnRecvPacket(_session, _queue.Dequeue());
            }
        }
    }
}
