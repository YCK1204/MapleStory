using UnityEngine;

public enum PlayerCharacterType { AngelicBuster, Tanjiro }
public enum MonsterType { }

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
    public T SpawnMonster<T>(MonsterType type) where T : CreatureController
    {
        var go = Instantiate(MonsterController);
        var cc = go.AddComponent<T>();

        Animator animator = go.AddComponent<Animator>();
        animator.runtimeAnimatorController = MonsterData[(int)type].AnimatorController;

        return cc;
    }
}
