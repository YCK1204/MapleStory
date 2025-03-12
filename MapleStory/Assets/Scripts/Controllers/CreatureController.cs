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
    protected virtual void CollisionEnter2D(Collision2D collider) { }
    protected virtual void CollisionExit2D(Collision2D collider) { }
    protected virtual void TriggerEnter2D(Collider2D collider) { }
    protected virtual void TriggerExit2D(Collider2D collider) { }
}
