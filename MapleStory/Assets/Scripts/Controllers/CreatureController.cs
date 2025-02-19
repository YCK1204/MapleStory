using System;

public abstract class CreatureController : BaseController
{
    public UInt64 ID { get; set; }
    protected override void Init()
    {
    }
    protected override void UpdateController()
    {
    }
    public abstract void Destroy();
}
