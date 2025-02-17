using System;
using System.Collections.Generic;
using UnityEngine;

public class ObjectManager : IManager
{
    Dictionary<UInt64, CreatureController> _objects = new Dictionary<UInt64, CreatureController>();
    GameObject Player;
    GameObject Monster;
    public void Init()
    {

    }
    public void Clear()
    {
        _objects.Clear();
    }
    public void Remove(UInt64 id)
    {
        CreatureController cc = null;
        if (_objects.TryGetValue(id, out cc) == false)
            return;
        cc.Destroy();
        _objects.Remove(id);
    }
    public void Push(CreatureController cc)
    {
        if (_objects.ContainsKey(cc.ID) == true)
            return;
        _objects.Add(cc.ID, cc);
    }
    public PlayerController GeneratePlayer(CharacterPreviewInfo info)
    {
        GameObject go = Manager.Resource.Instantiate(Player);
        var pc = go.GetComponent<PlayerController>();

        return pc;
    }
}