using Google.FlatBuffers;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UIElements;

public abstract class BaseScene : BaseMonobehaviour
{
    public GameObject UIControllers;
    public string PrevSceneName;
    private void Awake()
    {
        UIControllers = GameObject.Find("@UIControllers");
        Manager.Scene.CurScene = this;
    }
    private void Start()
    {
        Init();
    }
    protected virtual void Init()
    {
    }

    protected virtual void Clear()
    {

    }
    public abstract void OnRecvPacket<T>(T pkt) where T : struct, IFlatbufferObject;
}
