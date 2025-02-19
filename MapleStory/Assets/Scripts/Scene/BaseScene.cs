using Google.FlatBuffers;
using UnityEditor;
using UnityEngine;

public abstract class BaseScene : BaseMonobehaviour
{
    [HideInInspector]
    public GameObject UIControllers;
    [SerializeField]
    protected string NextScene;
    private void Awake()
    {
        Init();
    }
    protected virtual void Init()
    {
        UIControllers = GameObject.Find("@UIControllers");
        Manager.Scene.CurScene = this;
    }

    protected virtual void Clear()
    {

    }
    public abstract void OnRecvPacket<T>(T pkt) where T : struct, IFlatbufferObject;
}
