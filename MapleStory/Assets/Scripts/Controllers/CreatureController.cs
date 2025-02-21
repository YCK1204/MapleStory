using System;
using UnityEngine;

public abstract class CreatureController : BaseController
{
    protected enum CreatureState : byte
    {
        None = 0,
        IDLE = 1,
        MOVE = 2,
        JUMP = 3,
    }
    CreatureState _state { get; set; }
    protected CreatureState State
    {
        get { return _state; }
        set 
        { 
            _state = value;
            //UpdateController();
        }
    }
    public UInt64 ID { get; set; }
    public abstract void Destroy();
}
