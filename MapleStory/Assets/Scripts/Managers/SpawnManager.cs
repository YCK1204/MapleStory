using UnityEngine;

public class SpawnManager : IManager
{
    CreatureSpawner _spawner;
    public void Init()
    {
        _spawner = Manager.Resource.Instantiate("Prefabs/CreatureSpawner").GetComponent<CreatureSpawner>();
        GameObject.DontDestroyOnLoad(_spawner);
    }
    public void Clear()
    {
    }
    public T SpawnPlayer<T>(PlayerCharacterType type) where T : CreatureController
    {
        return _spawner.SpawnPlayer<T>(type);
    }
    public T SpawnMonster<T>(MonsterType type) where T : CreatureController
    {
        return _spawner.SpawnMonster<T>(type);
    }
}