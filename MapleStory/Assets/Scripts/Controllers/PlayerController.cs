using System;
using Unity.VisualScripting;
using UnityEngine;

public class PlayerController : CreatureController
{
    Vector2 MoveDir = Vector2.zero;
    MoveDirection _dir { get; set; } = MoveDirection.NONE;
    public MoveDirection Dir
    {
        get { return _dir; }
        set
        {
            _dir = value;
            switch (value)
            {
                case MoveDirection.NONE:
                    MoveDir = Vector2.zero;
                    State = CreatureState.IDLE;
                    return;
                case MoveDirection.LEFT:
                    MoveDir = Vector2.left;
                    break;
                case MoveDirection.RIGHT:
                    MoveDir = Vector2.right;
                    break;
            }
            State = CreatureState.MOVE;
        }
    }
    public PlayerCharacterType CharacterType { get; set; }
    public ushort Level { get; set; }
    public string Name { get; set; }

    [SerializeField]
    protected float Speed = 3f;
    [SerializeField]
    protected float JumpForce = 3f;
    protected Rigidbody2D rb;
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        rb = GetComponent<Rigidbody2D>();
    }
    private void OnCollisionEnter2D(Collision2D collision)
    {
        State = CreatureState.IDLE;
        rb.angularVelocity = 0;
    }
    protected void Move()
    {
        transform.position += Speed * Time.deltaTime * (Vector3)MoveDir;
    }
    protected void Jump()
    {
        if (State == CreatureState.JUMP || rb.linearVelocity.magnitude > .01f)
            return;
        rb.AddForce(Vector3.up * JumpForce, (ForceMode2D)ForceMode.Impulse);
        State = CreatureState.JUMP;
    }
    public override void Destroy()
    {
    }
    protected override void UpdateController()
    {
        if (Dir != MoveDirection.NONE)
            Move();
    }
    protected override void Init()
    {
    }
    protected override void UpdateAnimation()
    {
    }
    private void Update()
    {
        UpdateController();
    }
}
