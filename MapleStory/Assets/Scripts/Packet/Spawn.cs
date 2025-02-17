using Google.FlatBuffers;
using ServerCore;
using System;
using UnityEngine;

public partial class PacketHandler
{
    // todo
    // 맵에 있는 item 정보도 갖고와야함
    public static void SC_EnterMapHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_EnterMap.GetRootAsSC_EnterMap(buffer);

        Manager.Scene.LoadScene($"Map_{pkt.MapId.ToString("000")}");

        int playerCount = pkt.PlayersLength;
        int monsterCount = pkt.MonstersLength;

        Manager.Object.Clear();
        for (int i = 0; i < playerCount; i++) 
        {
            var playerInfo = pkt.Players(i).Value;
            var position = playerInfo.Position.Value;
            var charInfo = playerInfo.CharInfo.Value;

            var player = Manager.Spawn.SpawnPlayer<PlayerController>((PlayerCharacterType)charInfo.CharType);

            player.ID = charInfo.CharId;
            player.Level = charInfo.Level;
            player.Name = charInfo.Name;
            player.CharacterType = (PlayerCharacterType)charInfo.CharType;

            player.transform.position = new Vector3(position.X, position.Y);

            Manager.Object.Push(player);
        }
        for (int i = 0; i < monsterCount; i++)
        {
            var monsterInfo = pkt.Monsters(i).Value;
            var position = monsterInfo.Position.Value;

            var monster = Manager.Spawn.SpawnMonster<MonsterController>((MonsterType)monsterInfo.Type);

            monster.ID = monsterInfo.Id;
            monster.transform.position = new Vector3(position.X, position.Y);
            Manager.Object.Push(monster);
        }
    }
    public static void SC_DespawnHandler(PacketSession session, ByteBuffer buffer)
    {
    }
    public static void SC_MSpawnHandler(PacketSession session, ByteBuffer buffer)
    {
    }
    public static void SC_PSpawnHandler(PacketSession session, ByteBuffer buffer)
    {
    }
}
