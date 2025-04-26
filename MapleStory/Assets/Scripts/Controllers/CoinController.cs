using System;
using UnityEngine;

public class CoinController : BaseController
{
    Rigidbody2D rb;
    public UInt16 Money { get; set; }

    public override void Destroy()
    {
        rb.AddForce(Vector2.up * 300f);
        FadeOut(1f);
        Invoke(() => { GameObject.Destroy(gameObject); }, 1f);
    }
    protected override void Init()
    {
        Type = ObjectType.Item;
        rb = GetComponent<Rigidbody2D>();
        rb.AddForce(Vector2.up * 150f);
    }

    protected override void UpdateAnimation()
    {
    }
    protected override void UpdateController()
    {
    }
}