using Google.FlatBuffers;
using ServerCore;
using System;
using Unity.Cinemachine;
using Unity.VisualScripting;
using UnityEngine;

public partial class PacketHandler
{
    // todo
    // 맵에 있는 item 정보도 갖고와야함
    static void ChangeSceneHandler(CharacterTotalInfo myPlayerInfo, byte mapId, Position position)
    {
        Manager.Object.Clear();
        var prevInfo = myPlayerInfo.PrevInfo.Value;
        var ability = prevInfo.Ability.Value;
        Manager.Scene.LoadSceneAsync($"Map_{mapId.ToString("000")}", () =>
        {
            var pc = HandlePSpawn<MyPlayerContoller>(prevInfo);
            {
                pc.STR = ability.STR;
                pc.DEX = ability.DEX;
                pc.INT = ability.INT;
                pc.LUK = ability.LUK;
                pc.HP = myPlayerInfo.Hp;
                pc.MP = myPlayerInfo.Mp;
                pc.EXP = myPlayerInfo.Exp;
            }
            Manager.Object.MyPlayer = pc;
            pc.transform.position = new Vector3(position.X, position.Y);
            var cc = Camera.main.gameObject.AddComponent<CameraController>();
            cc.AddComponent<CinemachineBrain>();
            cc.cinemachineCamera = Manager.Resource.Instantiate("Prefabs/PlayUtil/FollowCamera").GetComponent<CinemachineCamera>();
            var border = GameObject.Find("Border").GetComponent<Collider2D>();
            cc.Border = border;
            cc.Target = pc.gameObject;
            Camera.main.cullingMask = ~0;
            //cc.Init();

            FlatBufferBuilder builder = new FlatBufferBuilder(1);

            C_CreatureInfos.StartC_CreatureInfos(builder);
            var data = C_CreatureInfos.EndC_CreatureInfos(builder);
            var packet = Manager.Packet.CreatePacket(data, builder, PacketType.C_CreatureInfos);
            Manager.Network.Send(packet);
        });
    }
    public static void SC_EnterGameHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_EnterGame.GetRootAsSC_EnterGame(buffer);

        ChangeSceneHandler(pkt.MyPlayerInfo.Value, pkt.MapId, pkt.Position.Value);
    }
    public static void SC_PortalHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_Portal.GetRootAsSC_Portal(buffer);

        ChangeSceneHandler(pkt.MyPlayerInfo.Value, pkt.MapId, pkt.Position.Value);
    }
    public static void SC_DespawnHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_Despawn.GetRootAsSC_Despawn(buffer);

         //Manager.Object.FindPlayer(pkt.Id);
    }
    public static void SC_MSpawnHandler(PacketSession session, ByteBuffer buffer)
    {
    }
    static T HandlePSpawn<T>(CharacterPreviewInfo charInfo) where T : PlayerController
    {
        var pc = Manager.Spawn.SpawnPlayer<T>((PlayerCharacterType)charInfo.CharType);
        {
            pc.ID = charInfo.CharId;
            pc.Name = charInfo.Name;
            pc.Level = charInfo.Level;
            pc.CharacterType = (PlayerCharacterType)charInfo.CharType;
        }
        Manager.Object.Push(pc);
        return pc;
    }
    public static void SC_PSpawnHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_PSpawn.GetRootAsSC_PSpawn(buffer);

        var playerInfo = pkt.PlayerInfo.Value;
        var previewInfo = playerInfo.CharInfo.Value;
        var position = playerInfo.Position.Value;

        if (previewInfo.CharId == Manager.Object.MyPlayer.ID)
            return;

        var pc = HandlePSpawn<PlayerController>(previewInfo);
        pc.transform.position = new Vector3(position.X, position.Y);
    }
    public static void SC_CreatureInfosHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_CreatureInfos.GetRootAsSC_CreatureInfos(buffer);

        var len = pkt.PlayersLength;
        for (var i = 0; i < len; i++)
        {
            var playerInfo = pkt.Players(i).Value;
            var charInfo = playerInfo.CharInfo.Value;
            var position = playerInfo.Position.Value;
            if (charInfo.CharId == Manager.Object.MyPlayer.ID)
                continue;

            var pc = HandlePSpawn<PlayerController>(charInfo);
            pc.transform.position = new Vector3(position.X, position.Y);
        }
        //len = pkt.MonstersLength;
        //for (var i = 0; i < len; ++i)
        //{
        //    var monsterInfo = pkt.Monsters(i).Value;
        //    var position = monsterInfo.Position.Value;

        //    var mc = Manager.Spawn.SpawnMonster<MonsterController>((MonsterType)monsterInfo.Type);
        //    mc.transform.position = new Vector3(position.X, position.Y);
        //    mc.ID = monsterInfo.Id;
        //    Manager.Object.Push(mc);
        //}
    }
}
