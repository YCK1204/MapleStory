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
    enum MonsterState
    {
        Move,
        Stand,
        Hit,
        Attack,
        Die,
    }
    MonsterState _state = MonsterState.Stand;
    MonsterState State { 
        get { return _state; }
        set
        {
            _state = value;
            UpdateAnimation();
        }
    }
    public override void Destroy()
    {
        GameObject.Destroy(this);
    }
    protected override void Init()
    {
        Animator = GetComponent<Animator>();
        State = MonsterState.Stand;

    }

    protected override void UpdateAnimation()
    {
        string animString = State.ToString();

        Animator.Play(animString);
    }
    protected override void UpdateController()
    {
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
