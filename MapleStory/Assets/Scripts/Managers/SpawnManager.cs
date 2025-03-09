using UnityEngine;

public class SpawnManager : IManager
{
    CreatureSpawner _spawner;
    public void Init()
    {
        _spawner = Manager.Resource.Instantiate("Prefabs/PlayUtil/CreatureSpawner").GetComponent<CreatureSpawner>();
        GameObject.DontDestroyOnLoad(_spawner);
    }
    public void Clear()
    {
    }
    public T SpawnPlayer<T>(PlayerCharacterType type) where T : CreatureController
    {
        return _spawner.SpawnPlayer<T>(type);
    }
    public MonsterController SpawnMonster(MonsterType type)
    {
        return _spawner.SpawnMonster(type);
    }
}