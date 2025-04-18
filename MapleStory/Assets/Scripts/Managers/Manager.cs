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
    ObjectManager _object = new ObjectManager();
    SpawnManager _spawn = new SpawnManager();
    AudioManager _audio;
    public static Manager Instance
    {
        get
        {
            if (_instance == null)
            {
                GameObject go = new GameObject();
                _instance = go.AddComponent<Manager>();
                _instance.Init();
            }
            return _instance;
        }
    }
    public static AudioManager Audio { get { return _instance._audio; } }
    public static SceneManagerEx Scene { get { return Instance._scene; } }
    public static ResourceManager Resource { get { return Instance._resource; } }
    public static NetworkManager Network { get { return Instance._network; } }
    public static PacketManager Packet { get { return Instance._packet; } }
    public static ObjectManager Object { get { return Instance._object; } }
    public static SpawnManager Spawn { get { return Instance._spawn; } }
    #endregion
    void Init()
    {
        Screen.SetResolution(1920, 1080, false);
        Application.runInBackground = true;

        DontDestroyOnLoad(_instance.gameObject);
        _audio = GameObject.Find("AudioManager").GetComponent<AudioManager>();
        DontDestroyOnLoad(Audio);
        Network.Init();
        Spawn.Init();
    }
    void Update()
    {
        Network.Update();
    }
}
