using System;
using UnityEditor.Animations;
using UnityEngine;

public abstract class CreatureController : BaseController
{
    public UInt64 ID { get; set; }
    protected override void Init()
    {
        base.Init();
    }
    protected override void UpdateController()
    {
        base.UpdateController();
    }
    public abstract void Destroy();
}
