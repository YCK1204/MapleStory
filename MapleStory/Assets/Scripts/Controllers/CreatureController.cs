using System;
using UnityEngine;

public abstract class CreatureController : BaseController
{
    protected virtual void CollisionEnter2D(Collision2D collider) { }
    protected virtual void CollisionExit2D(Collision2D collider) { }
    protected virtual void TriggerEnter2D(Collider2D collider) { }
    protected virtual void TriggerExit2D(Collider2D collider) { }
}
