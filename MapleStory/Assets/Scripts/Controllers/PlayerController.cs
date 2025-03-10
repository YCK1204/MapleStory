using System;
using System.Collections;
using UnityEngine;

public class PlayerController : CreatureController
{
    public enum PlayerState : byte
    {
        Jump,
        Ladder,
        ProneStab,
        ProneStabAttack,
        rope,
        Stand01,
        SwingT2,
        SwingT3,
        Walk01,
    }
    PlayerState _state { get; set; } = PlayerState.Stand01;
    BoxCollider2D boxCollider;
    public PlayerState State
    {
        get { return _state; }
        set
        {
            _state = value;
            UpdateAnimation();
        }
    }
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
                    break;
                case MoveDirection.LEFT:
                    MoveDir = Vector2.left;
                    transform.localScale = new Vector3(Mathf.Abs(transform.localScale.x), transform.localScale.y, transform.localScale.z);
                    break;
                case MoveDirection.RIGHT:
                    MoveDir = Vector2.right;
                    transform.localScale = new Vector3(-Mathf.Abs(transform.localScale.x), transform.localScale.y, transform.localScale.z);
                    break;
            }
            if (State == PlayerState.Jump)
                return;
            State = MoveDir == Vector2.zero ? PlayerState.Stand01 : PlayerState.Walk01;
        }
    }
    public PlayerCharacterType CharacterType { get; set; }
    public ushort Level { get; set; }
    public string Name { get; set; }

    [SerializeField]
    protected float Speed = 5f;
    [SerializeField]
    protected float JumpForce = 7f;
    protected Rigidbody2D rb;
    Animator anim;
    Vector2 bottomOffset = new Vector2(0, -.5f);
    private void OnCollisionEnter2D(Collision2D collision)
    {
        //bool isGrounded = Physics2D.Raycast(transform.position, Vector2.down, 1f, "");
        int target = LayerMask.GetMask("Floor", "FloorBase", "Stair");
        var hit = Physics2D.Raycast((Vector2)transform.position + bottomOffset, Vector2.down, .2f, target);

        if (hit.collider == null || hit.collider.gameObject == null)
            return;
        if (hit.collider.gameObject != collision.gameObject)
            return;

        rb.angularVelocity = 0;
        //if (rb.linearVelocityY >= 0)
        //    return;
        if (Dir != MoveDirection.NONE)
        {
            State = PlayerState.Walk01;
        }
        else
        {
            State = PlayerState.Stand01;
        }
    }
    protected void Move()
    {
        transform.position += Speed * Time.deltaTime * (Vector3)MoveDir;
    }
    private IEnumerator JumpDown()
    {
        State = PlayerState.Stand01;
        float jumpForceOriginal = JumpForce;
        JumpForce = .1f;
        boxCollider.enabled = false;
        Jump();
        yield return new WaitForSeconds(.2f);
        boxCollider.enabled = true;
        JumpForce = jumpForceOriginal;
        StopCoroutine(JumpDown());
    }
    protected bool CanJump()
    {
        if (State == PlayerState.ProneStab)
        {
            int target = LayerMask.GetMask("Floor");
            var hit = Physics2D.Raycast((Vector2)transform.position + bottomOffset, Vector2.down, .2f, target);

            if (hit.collider == null)
                return false;
            return true;
        }
        if (State == PlayerState.Jump || rb.linearVelocity.magnitude > .01f)
            return false;
        if (State == PlayerState.Stand01 || State == PlayerState.Walk01)
            return true;
        return false;
    }
    public void Jump()
    {
        if (CanJump())
        {
            if (State == PlayerState.ProneStab)
            {
                StartCoroutine(JumpDown());
            }
            else
            {
                rb.AddForce(Vector3.up * JumpForce, (ForceMode2D)ForceMode.Impulse);
                State = PlayerState.Jump;
            }
        }
    }
    public override void Destroy()
    {
        GameObject.Destroy(gameObject);
    }
    protected override void UpdateController()
    {
        if (Dir != MoveDirection.NONE)
            Move();
    }
    protected override void Init()
    {
        rb = GetComponent<Rigidbody2D>();
        anim = GetComponent<Animator>();
        boxCollider = GetComponent<BoxCollider2D>();
    }
    protected override void UpdateAnimation()
    {
        string animationStr = State.ToString();

        anim.Play(animationStr);
    }
    private void Update()
    {
        UpdateController();
    }
}
