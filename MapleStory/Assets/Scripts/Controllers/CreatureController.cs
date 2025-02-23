using System;
using UnityEngine;

public abstract class CreatureController : BaseController
{
    public UInt64 ID { get; set; }
    public abstract void Destroy();
    private void Start()
    {
        Init();
    }
}
