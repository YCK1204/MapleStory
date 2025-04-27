using Google.FlatBuffers;
using ServerCore;
using System;
using Unity.Cinemachine;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.Tilemaps;

public partial class PacketHandler
{
    // todo
    // 맵에 있는 item 정보도 갖고와야함
    static void ChangeSceneHandler(CharacterInfoDetail myPlayerInfo, byte mapId, Position position)
    {
        var prevInfo = myPlayerInfo.PrevInfo.Value;
        var ability = prevInfo.Ability.Value;
        string mapName = $"Map_{mapId.ToString("000")}";
        Manager.Scene.LoadSceneAsync(mapName, () =>
        {
            Manager.Object.Clear();
            GameObject map = GameObject.Find(mapName);
            Manager.Object.NavGrid = Util.FindChild<Tilemap>(map.transform, true, "SpawnGround");
            var pc = HandlePSpawn<MyPlayerContoller>(prevInfo);
            {
                pc.STR = ability.STR;
                pc.DEX = ability.DEX;
                pc.INT = ability.INT;
                pc.LUK = ability.LUK;
                pc.HP = myPlayerInfo.Hp;
                pc.MP = myPlayerInfo.Mp;
                pc.EXP = myPlayerInfo.Exp;
                pc.Money = myPlayerInfo.Money;
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
            if (GameObject.Find("@BGMHandler") == null)
            {
                var bgmHandler = Manager.Resource.Instantiate("Prefabs/PlayUtil/BGMHandler");
                bgmHandler.name = "@BGMHandler";
                GameObject.DontDestroyOnLoad(bgmHandler);
            }

            FlatBufferBuilder builder = new FlatBufferBuilder(1);
            C_RoomObjects.StartC_RoomObjects(builder);
            var data = C_RoomObjects.EndC_RoomObjects(builder);
            var packet = Manager.Packet.CreatePacket(data, builder, PacketType.C_RoomObjects);
            Manager.Network.Send(packet);
        });
    }
    static void ChangeSceneHandler(CharacterInfoDetail myPlayerInfo, byte mapId)
    {
        var prevInfo = myPlayerInfo.PrevInfo.Value;
        var ability = prevInfo.Ability.Value;
        string mapName = $"Map_{mapId.ToString("000")}";
        Manager.Scene.LoadSceneAsync(mapName, () =>
        {
            Manager.Object.Clear();
            GameObject map = GameObject.Find(mapName);
            Manager.Object.NavGrid = Util.FindChild<Tilemap>(map.transform, true, "SpawnGround");
            var pc = HandlePSpawn<MyPlayerContoller>(prevInfo);
            {
                pc.STR = ability.STR;
                pc.DEX = ability.DEX;
                pc.INT = ability.INT;
                pc.LUK = ability.LUK;
                pc.HP = myPlayerInfo.Hp;
                pc.MP = myPlayerInfo.Mp;
                pc.EXP = myPlayerInfo.Exp;
                pc.Money = myPlayerInfo.Money;
            }
            Manager.Object.MyPlayer = pc;
            pc.transform.position = new Vector3(0, 0);
            var cc = Camera.main.gameObject.AddComponent<CameraController>();
            cc.AddComponent<CinemachineBrain>();
            cc.cinemachineCamera = Manager.Resource.Instantiate("Prefabs/PlayUtil/FollowCamera").GetComponent<CinemachineCamera>();
            var border = GameObject.Find("Border").GetComponent<Collider2D>();
            cc.Border = border;
            cc.Target = pc.gameObject;
            Camera.main.cullingMask = ~0;

            FlatBufferBuilder builder = new FlatBufferBuilder(1);
            C_RoomObjects.StartC_RoomObjects(builder);
            var data = C_RoomObjects.EndC_RoomObjects(builder);
            var packet = Manager.Packet.CreatePacket(data, builder, PacketType.C_RoomObjects);
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
    public static void SC_PDespawnHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_PDespawn.GetRootAsSC_PDespawn(buffer);

        var player = Manager.Object.FindPlayer(pkt.Id);
        if (player != null)
            Manager.Object.RemovePlayer(player.ID);
    }
    public static void SC_MDespawnHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_MDespawn.GetRootAsSC_MDespawn(buffer);

        for (var i = 0; i < pkt.IdLength; i++)
            Manager.Object.RemoveMonster(pkt.Id(i), pkt.Coin(i).Value);
    }
    static T HandlePSpawn<T>(CharacterInfo charInfo) where T : PlayerController
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
    public static void SC_RoomObjectsHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_RoomObjects.GetRootAsSC_RoomObjects(buffer);

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
        len = pkt.ItemsLength;
        for (var i = 0; i < len; i++)
        {
            var item = pkt.Items(i).Value;
            CoinController cc = Manager.Resource.Instantiate("prefabs/MapObject/coin").GetComponent<CoinController>();

            cc.transform.position = new Vector2(item.X, item.Y);
            cc.ID = item.Id;
            cc.Money = item.Money;
            Manager.Object.Push(cc);
        }
    }
}
