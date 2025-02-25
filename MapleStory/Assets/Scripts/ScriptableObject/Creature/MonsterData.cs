using UnityEngine;

[CreateAssetMenu(fileName = "Monster Data", menuName = "Scriptable Object/Monster Data", order = 2)]
public class MonsterData : ScriptableObject
{
    [SerializeField]
    public RuntimeAnimatorController AnimatorController;
    [SerializeField]
    public AudioClip AttackSound;
    [SerializeField]
    public AudioClip DieSound;
    [SerializeField]
    public AudioClip HitSound;
}
