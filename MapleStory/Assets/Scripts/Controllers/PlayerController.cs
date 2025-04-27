using System;
using System.Collections;
using UnityEngine;
using static UnityEditor.PlayerSettings;

public class PlayerController : CreatureController
{
    #region State
    public enum PlayerState : byte
    {
        None,
        Jump,
        Ladder,
        ProneStab,
        ProneStabAttack,
        rope,
        Stand,
        Attack,
        Walk,
    }
    int _state = 0;
    private int State
    {
        get { return _state; }
        set
        {
            _state = value;
            UpdateAnimation();
        }
    }
    public void AddState(PlayerState state)
    {
        State |= (1 << (int)state);

        if (state == PlayerState.Attack)
        {
            var clip = Manager.Audio.FindAudioClip("Tanjiro_DefaultAttack");
            Manager.Audio.OneShotPlay(clip);
            if (tanjiro_Attack == Tanjiro_Attack.Swing)
                Manager.Audio.OneShotPlay(Manager.Audio.FindAudioClip("Tanjiro_Swing"));
            else if (tanjiro_Attack == Tanjiro_Attack.SwingT3)
                Manager.Audio.OneShotPlay(Manager.Audio.FindAudioClip("Tanjiro_SwingT3"));
            var effactorAnim = Util.FindChild<Animator>(gameObject.transform, false, "Effector");
            effactorAnim.Play($"Attack{(int)tanjiro_Attack + 1}");
        }
        else if (state == PlayerState.Ladder || state == PlayerState.rope)
            rb.gravityScale = 0;
    }
    public void RemoveState(PlayerState state)
    {
        State &= ~(1 << (int)state);
        if (state == PlayerState.Ladder || state == PlayerState.rope)
            rb.gravityScale = GravitiyScale;
    }
    public bool HasState(PlayerState state) { return (State & (1 << (int)state)) != 0; }
    #endregion
    [SerializeField]
    protected float Speed = 5f;
    [SerializeField]
    protected float JumpForce = 7f;
    protected float GravitiyScale = 2;
    public ushort Level { get; set; }
    public string Name { get; set; }

    public Tanjiro_Attack tanjiro_Attack;

    protected UInt64 AttackCount = 0;
    protected Coroutine _coAttack = null;

    public Vector2 MoveDir = Vector2.zero;
    MoveDirection _dir = MoveDirection.NONE;
    public MoveDirection Dir
    {
        get { return _dir; }
        set
        {
            _dir = value;
            switch (value)
            {
                case MoveDirection.LEFT:
                    MoveDir = Vector2.left;
                    transform.FlipXLeft();
                    break;
                case MoveDirection.RIGHT:
                    MoveDir = Vector2.right;
                    transform.FlipXRight();
                    break;
                case MoveDirection.UP:
                    MoveDir = Vector2.up;
                    break;
                case MoveDirection.DOWN:
                    MoveDir = Vector2.down;
                    break;
                case MoveDirection.NONE:
                    MoveDir = Vector2.zero;
                    break;
            }
            if (value == MoveDirection.NONE)
                RemoveState(PlayerState.Walk);
            else if (value == MoveDirection.LEFT || value == MoveDirection.RIGHT)
                AddState(PlayerState.Walk);
            else
                AddState(PlayerState.Ladder);
        }
    }
    public PlayerCharacterType CharacterType { get; set; }
    #region Components Init
    Animator anim;
    protected Rigidbody2D rb;
    protected BoxCollider2D boxCollider;
    protected override void Init()
    {
        Type = ObjectType.Player;
        rb = GetComponent<Rigidbody2D>();
        anim = GetComponent<Animator>();
        boxCollider = GetComponent<BoxCollider2D>();
        AddState(PlayerState.Stand);
        AddState(PlayerState.Jump);
    }
    #endregion
    protected void Move() { transform.position += Speed * Time.deltaTime * (Vector3)MoveDir; }
    #region Jump
    private IEnumerator JumpDown()
    {
        float jumpForceOriginal = JumpForce;
        JumpForce = .1f;
        boxCollider.enabled = false;
        yield return new WaitForSeconds(.2f);
        boxCollider.enabled = true;
        JumpForce = jumpForceOriginal;
        StopCoroutine(JumpDown());
    }
    protected bool CanJump()
    {
        if (boxCollider.IsTouchingLayers(LayerMask.GetMask("Floor", "Stair", "FloorBase")))
        {
            if (HasState(PlayerState.ProneStab) && boxCollider.IsTouchingLayers(LayerMask.GetMask("FloorBase")))
                return false;
            if (rb.linearVelocity.magnitude > .01f)
                return false;
            return true;
        }
        return false;
    }
    public void Jump()
    {
        if (CanJump())
        {
            AddState(PlayerState.Jump);
            if (HasState(PlayerState.ProneStab))
                StartCoroutine(JumpDown());
            else
                rb.AddForce(Vector3.up * JumpForce, (ForceMode2D)ForceMode.Impulse);
        }
    }
    #endregion
    public override void Destroy()
    {
        GameObject.Destroy(gameObject);
    }
    protected override void UpdateController()
    {
        if (Dir != MoveDirection.NONE)
            Move();
    }
    protected override void UpdateAnimation()
    {
        string animationStr = null;
        if (HasState(PlayerState.Attack))
            animationStr = tanjiro_Attack.ToString();
        else
        {
            if (HasState(PlayerState.Ladder))
                animationStr = "Ladder";
            else if (HasState(PlayerState.rope))
                animationStr = "rope";
            else if (HasState(PlayerState.Jump))
                animationStr = "Jump";
            else if (HasState(PlayerState.Walk))
                animationStr = "Walk";
            else if (HasState(PlayerState.ProneStab))
                animationStr = "ProneStab";
            else if (HasState(PlayerState.Stand))
                animationStr = "Stand";
        }

        if (animationStr != null)
            anim.Play(animationStr);
    }
    private void Update()
    {
        UpdateController();
    }
    IEnumerator CoAttack(float time)
    {
        yield return new WaitForSeconds(time);
        RemoveState(PlayerState.Attack);
        StopCoroutine(_coAttack);
        _coAttack = null;
    }
    public void Attack()
    {
        tanjiro_Attack = (Tanjiro_Attack)(AttackCount++ % 4);
        AddState(PlayerState.Attack);
        _coAttack = StartCoroutine(CoAttack(.5f));
    }
    protected override void TriggerExit2D(Collider2D collider)
    {
        if (collider.gameObject.HasLayer("Ladder") && HasState(PlayerState.Ladder))
            MoveDir = Vector2.zero;
    }
    protected override void CollisionExit2D(Collision2D collider)
    {
        if ((collider.gameObject.HasLayer("Floor", "FloorBase", "Stair")))
        {
            AddState(PlayerState.Jump);
        }
    }
    private void OnCollisionExit2D(Collision2D collision) { CollisionExit2D(collision); }
    protected override void CollisionEnter2D(Collision2D collider)
    {
        if ((collider.gameObject.HasLayer("Floor", "FloorBase", "Stair")))
        {
            var hit = Physics2D.Raycast(new Vector2(boxCollider.bounds.center.x, boxCollider.bounds.min.y - .1f), Vector2.down, .1f, LayerMask.GetMask("Floor", "FloorBase", "Stair"));
            if (hit.collider == null)
                return;

            if (MoveDir == Vector2.zero)
                AddState(PlayerState.Stand);
            else
                AddState(PlayerState.Walk);
            rb.angularVelocity = 0;
            RemoveState(PlayerState.Jump);
        }
    }
    private void OnCollisionEnter2D(Collision2D collision) { CollisionEnter2D(collision); }
}
