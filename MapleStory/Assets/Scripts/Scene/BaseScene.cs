using Google.FlatBuffers;
using System.Collections.Generic;
using UnityEditor;
using UnityEditor.SearchService;
using UnityEngine;
using UnityEngine.UIElements;

public abstract class BaseScene : BaseMonobehaviour
{
    [HideInInspector]
    public GameObject UIControllers;
    [SerializeField]
    protected SceneAsset NextScene;
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
