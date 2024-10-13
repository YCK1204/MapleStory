using ServerCore;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using UnityEngine;

public class NetworkManager : IManager
{
    Connector _connector = new Connector();
    public NetworkManager() { Init(); }
    public void Clear()
    {
    }

    public void Init()
    {
        string host = Dns.GetHostName();
        IPHostEntry entry = Dns.GetHostEntry(host);
        IPAddress addr = entry.AddressList[0];
        IPEndPoint endPoint = new IPEndPoint(addr, 8080);

        _connector.Init(endPoint, () => { return new ServerSession(); });
    }
}
