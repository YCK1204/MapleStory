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
        sp.sprite = MonsterData[(int)type].InitSprite;

        return mc;
    }
    public void SetPosToTilemap(GameObject go)
    {

        RaycastHit2D hit = Physics2D.Raycast(go.transform.position, Vector2.down, 5f, LayerMask.GetMask("Floor", "FloorBase"));
        
        var boxCollider = go.GetComponent<BoxCollider2D>();
        boxCollider.size = go.GetComponent<SpriteRenderer>().bounds.size / 2;

        float halfHeight = boxCollider.size.y * 0.5f * go.transform.localScale.y;
        Vector3 newPosition = new Vector3(go.transform.position.x, hit.point.y + halfHeight, go.transform.position.z);

        go.transform.position = newPosition;
    }
}
