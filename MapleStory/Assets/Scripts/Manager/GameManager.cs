using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameManager : MonoBehaviour
{
    #region Singleton
    SceneManagerEx _scene = new SceneManagerEx();
    NetworkManager _network = new NetworkManager();
    static GameManager _Instance;
    static GameManager Instance { get { return _Instance; } }
    public static SceneManagerEx Scene { get { return Instance._scene; } }
    public static NetworkManager Network { get { return Instance._network; } }
    #endregion
    public static void Init()
    {
        if (_Instance == null)
        {
            GameObject go = new GameObject();
            _Instance = go.AddComponent<GameManager>();
            go.name = "@GameManager";
            DontDestroyOnLoad(go);
        }
    }
}
