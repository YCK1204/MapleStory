using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Define;
using UnityEngine.UIElements;

public abstract class BaseScene : MonoBehaviour
{
    [HideInInspector]
    public SceneType SceneType = SceneType.None;
    protected VisualElement root;
    private void Awake()
    {
        GameManager.Init();
        Init();
    }
    protected virtual void Init()
    {
        GameManager.Scene.CurScene = this;
    }
    protected abstract void Clear();
}
