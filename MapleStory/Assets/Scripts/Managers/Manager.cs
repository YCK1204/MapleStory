using UnityEngine;

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
    public static Manager Instance { get { return _instance; } }
    public static SceneManagerEx Scene { get { return _instance._scene; } }
    public static ResourceManager Resource { get { return _instance._resource; } }
    public static NetworkManager Network { get { return _instance._network; } }
    #endregion
    void Start()
    {
        _instance = this;
        DontDestroyOnLoad(gameObject);
    }
    void Update()
    {
        Network.Update();
    }
}
