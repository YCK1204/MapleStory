using System.Collections;
using UnityEngine;

public class MonsterController : CreatureController
{
    [HideInInspector]
    public AudioClip AttackSound;
    [HideInInspector]
    public AudioClip DieSound;
    [HideInInspector]
    public AudioClip HitSound;
    public Animator Animator;
    float _destPosX;
    public PlayerController Target;
    BoxCollider2D _collider;
    public float DestPosX
    {
        get { return _destPosX; }
        set
        {
            if (Mathf.Abs(transform.position.x - value) < .5f)
                return;
            _destPosX = value;
            Dir = (transform.position.x > _destPosX) ? Dir = MoveDirection.LEFT : MoveDirection.RIGHT;
        }
    }
    MonsterState _state = MonsterState.Stand;
    public MonsterState State
    {
        get { return _state; }
        set
        {
            if (_state == value || _state == MonsterState.Die)
                return;
            _state = value;
            UpdateAnimation();
        }
    }
    Vector2 MoveDir = Vector2.zero;
    MoveDirection _dir = MoveDirection.NONE;
    public MoveDirection Dir
    {
        get { return _dir; }
        set
        {
            if (value == _dir)
                return;
            _dir = value;
            switch (value)
            {
                case MoveDirection.NONE:
                    MoveDir = Vector2.zero;
                    State = MonsterState.Stand;
                    return;
                case MoveDirection.LEFT:
                    transform.localScale = new Vector3(Mathf.Abs(transform.localScale.x), transform.localScale.y, transform.localScale.z);
                    MoveDir = Vector2.left;
                    State = MonsterState.Move;
                    break;
                case MoveDirection.RIGHT:
                    transform.localScale = new Vector3(-Mathf.Abs(transform.localScale.x), transform.localScale.y, transform.localScale.z);
                    MoveDir = Vector2.right;
                    State = MonsterState.Move;
                    break;
            }
        }
    }
    [SerializeField]
    float Speed = 1f;
    private void Update()
    {
        UpdateController();
    }
    public override void Destroy()
    {
        GameObject.Destroy(gameObject);
    }
    protected override void Init()
    {
        Type = ObjectType.Monster;
        Animator = GetComponent<Animator>();
        State = MonsterState.Stand;
        _collider = GetComponent<BoxCollider2D>();
        FadeIn(1f);
    }
    protected override void UpdateAnimation()
    {
        string animString = State.ToString();

        if (Animator == null)
            Animator = GetComponent<Animator>();
        Animator.Play(animString);
    }
    protected override void UpdateController()
    {
        switch (State)
        {
            case MonsterState.Stand:
                UpdateStand();
                break;
            case MonsterState.Move:
                UpdateMove();
                break;
            case MonsterState.Hit:
                UpdateHit();
                break;
            case MonsterState.Die:
                UpdateDie();
                break;
            case MonsterState.Attack:
                UpdateAttack();
                break;
            case MonsterState.Trace:
                UpdateTrace();
                break;
        }
    }
    private void UpdateStand()
    {

    }
    float traceTime = 5f;
    private void UpdateMove()
    {
        var pos = transform.position;
        pos += (Vector3)MoveDir * Speed * Time.deltaTime;
        if (Mathf.Abs(transform.position.x - DestPosX) < .5f)
        {
            Dir = MoveDirection.NONE;
            return;
        }
        var hit = Physics2D.Raycast(new Vector2(_collider.bounds.center.x, _collider.bounds.min.y), Vector2.down, 1f, LayerMask.GetMask("Floor", "FloorBase"));
        if (hit.collider != null)
            transform.position = pos;
    }
    private void UpdateHit()
    {

    }
    private void UpdateDie()
    {

    }
    private void UpdateAttack()
    {

    }
    private void UpdateTrace()
    {
        var pos = transform.position;
        pos += (Vector3)MoveDir * Speed * Time.deltaTime;
        if (Mathf.Abs(transform.position.x - DestPosX) < .5f)
        {
            Dir = MoveDirection.NONE;
            return;
        }
        var hit = Physics2D.Raycast(new Vector2(_collider.bounds.center.x, _collider.bounds.min.y), Vector2.down, 1f, LayerMask.GetMask("Floor", "FloorBase"));
        if (hit.collider != null)
            transform.position = pos;
    }
    void PlaySound(AudioClip audioClip)
    {
        Manager.Audio.OneShotPlay(audioClip);
    }
    public virtual void PlayHitSound()
    {
        PlaySound(HitSound);
    }
    public virtual void PlayDieSound()
    {
        PlaySound(DieSound);
    }
    public virtual void PlayAttackSound()
    {
        PlaySound(AttackSound);
    }
}
