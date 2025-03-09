using UnityEngine;

public enum PlayerCharacterType { AngelicBuster, Tanjiro }
public enum MonsterType { Hourglass, GreenMask, BlueMask, RedMask }

public class CreatureSpawner : MonoBehaviour
{
    [SerializeField]
    PlayerData[] PlayerCharacterData;
    [SerializeField]
    MonsterData[] MonsterData;
    [SerializeField]
    GameObject PlayerCharacter;
    [SerializeField]
    GameObject MonsterController;
    public T SpawnPlayer<T>(PlayerCharacterType type) where T : CreatureController
    {
        var go = Instantiate(PlayerCharacter);
        var cc = go.AddComponent<T>();

        Animator animator = go.GetComponent<Animator>();
        animator.runtimeAnimatorController = PlayerCharacterData[(int)type].AnimatorController;

        return cc;
    }
    public MonsterController SpawnMonster(MonsterType type)
    {
        var go = Instantiate(MonsterController);

        Animator animator = go.GetComponent<Animator>();
        animator.runtimeAnimatorController = MonsterData[(int)type].AnimatorController;

        var mc = go.GetComponent<MonsterController>();
        mc.AttackSound = MonsterData[(int)type].AttackSound;
        mc.DieSound = MonsterData[(int)type].DieSound;
        mc.HitSound = MonsterData[(int)type].HitSound;
        var sp = go.GetComponent<SpriteRenderer>();
        sp.sortingOrder = MonsterData[(int)type].SortingLayerNum;

        return mc;
    }
}
