using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;

public class ObjectManager : IManager
{
    Dictionary<UInt64, BaseController> _objects = new Dictionary<UInt64, BaseController>();
    Dictionary<UInt64, PlayerController> _players = new Dictionary<UInt64, PlayerController>();
    public MyPlayerContoller MyPlayer { get; set; } = null;
    public Tilemap NavGrid = null;
    public void Init()
    {

    }
    public void Clear()
    {
        _objects.Clear();
        _players.Clear();
    }
    public void RemoveMonster(UInt64 id, Coin coin)
    {
        BaseController bc = null;
        if (_objects.TryGetValue(id, out bc) == false)
            return;
        CoinController cc = Manager.Resource.Instantiate("prefabs/MapObject/coin").GetComponent<CoinController>();
        cc.transform.position = bc.transform.position;
        cc.ID = coin.Id;
        cc.Money = coin.Money;
        MonsterController mc = bc as MonsterController;
        mc.State = MonsterState.Die;
        _objects.Remove(id);
        Push(cc);
    }
    public void RemoveObject(UInt64 id)
    {
        BaseController bc = null;
        if (_objects.TryGetValue(id, out bc) == false)
            return;
        bc.Destroy();
        _objects.Remove(id);
    }
    public void RemovePlayer(UInt64 id)
    {
        PlayerController pc = null;
        if (_players.TryGetValue(id, out pc) == false)
            return;
        _players.Remove(id);
        pc.Destroy();
    }
    public void Push(BaseController go)
    {
        if (_objects.ContainsKey(go.ID) == true)
            return;
        _objects.Add(go.ID, go);
    }
    public void Push(PlayerController pc)
    {
        if (_players.ContainsKey(pc.ID) == true)
            return;
        _players.Add(pc.ID, pc);
    }
    public PlayerController FindPlayer(UInt64 id)
    {
        PlayerController pc = null;
        _players.TryGetValue(id, out pc);
        return pc;
    }
    public MonsterController FindMonster(UInt64 id)
    {
        BaseController bc = null;
        if (_objects.TryGetValue(id, out bc) == true)
            return bc as MonsterController;
        return null;
    }
}