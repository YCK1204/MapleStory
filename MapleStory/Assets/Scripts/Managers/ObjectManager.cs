using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;

public class ObjectManager : IManager
{
    Dictionary<UInt64, MonsterController> _monsters = new Dictionary<UInt64, MonsterController>();
    Dictionary<UInt64, PlayerController> _players = new Dictionary<UInt64, PlayerController>();
    public  MyPlayerContoller MyPlayer { get; set; } = null;
    public Tilemap NavGrid = null;
    public void Init()
    {

    }
    public void Clear()
    {
        _monsters.Clear();
        _players.Clear();
    }
    public void RemoveMonster(UInt64 id)
    {
        MonsterController mc = null;
        if (_monsters.TryGetValue(id, out mc) == false)
            return;
        mc.Destroy();
        _monsters.Remove(id);
    }
    public void RemovePlayer(UInt64 id)
    {
        PlayerController pc = null;
        if (_players.TryGetValue(id, out pc) == false)
            return;
        _players.Remove(id);
        pc.Destroy();
    }
    public void Push(MonsterController mc)
    {
        if (_monsters.ContainsKey(mc.ID) == true)
            return;
        _monsters.Add(mc.ID, mc);
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
        MonsterController mc = null;
        _monsters.TryGetValue (id, out mc);
        return mc;
    }
}