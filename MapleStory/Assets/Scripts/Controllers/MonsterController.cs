using UnityEngine;

public class MonsterController : CreatureController
{
    [HideInInspector]
    public AudioClip AttackSound;
    [HideInInspector]
    public AudioClip DieSound;
    [HideInInspector]
    public AudioClip HitSound;
    public override void Destroy()
    {
        GameObject.Destroy(this);
    }
    protected override void Init()
    {
    }

    protected override void UpdateAnimation()
    {
    }

    protected override void UpdateController()
    {
    }
    void PlaySound(AudioClip audioClip)
    {
        var go = Manager.Resource.Instantiate("prefabs/OneShotPlay");
        var ospc = go.GetComponent<OneShotPlayController>();

        ospc.audioClip = audioClip;
        ospc.Play();
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
