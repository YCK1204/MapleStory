using System;
using UnityEngine;

public abstract class BaseController : BaseMonobehaviour
{
    public enum ObjectType { Player, Monster, Item }
    public ObjectType Type { get; set; }
    public UInt64 ID { get; set; }
    public 
    void Start()
    {
        Init();
    }
    protected abstract void Init();
    protected abstract void UpdateController();
    protected abstract void UpdateAnimation();
    public abstract void Destroy();
}
