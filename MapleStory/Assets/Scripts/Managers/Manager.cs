using System.Collections;
using UnityEngine;

using System.Security.Cryptography;
using System.IO;
using System;

public interface IManager
{
    void Init();
    void Clear();
}
public class Manager : BaseMonobehaviour
{
    #region Singleton
    static Manager _instance;
    SceneManagerEx _scene = new SceneManagerEx();
    NetworkManager _network = new NetworkManager();
    ResourceManager _resource = new ResourceManager();
    PacketManager _packet = new PacketManager();
    public static Manager Instance { get { return _instance; } }
    public static SceneManagerEx Scene { get { return _instance._scene; } }
    public static ResourceManager Resource { get { return _instance._resource; } }
    public static NetworkManager Network { get { return _instance._network; } }
    public static PacketManager Packet { get { return _instance._packet; } }
    #endregion
    void Awake()
    {
        _instance = this;
        DontDestroyOnLoad(gameObject);
        Network.Init();
    }
    void Update()
    {
        Network.Update();
    }
}
