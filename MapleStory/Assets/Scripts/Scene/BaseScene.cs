using Google.FlatBuffers;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UIElements;

public abstract class BaseScene : BaseMonobehaviour
{
    protected GameObject UIControllers;

    private void Start()
    {
        Init();
    }
    protected virtual void Init()
    {
        Manager.Scene.CurScene = this;
        UIControllers = GameObject.Find("@UIControllers");
    }

    protected virtual void Clear()
    {

    }
    public abstract void OnRecvPacket<T>(T pkt) where T : struct, IFlatbufferObject;
}
