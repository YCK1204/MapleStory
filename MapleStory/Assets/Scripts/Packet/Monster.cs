using Google.FlatBuffers;
using ServerCore;
using UnityEngine;

public partial class PacketHandler
{
    public static void SC_MonsterInfosHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_MonsterInfos.GetRootAsSC_MonsterInfos(buffer);

        var len = pkt.MonsterInfosLength;
        for (var i = 0; i < len; ++i)
        {
            var monsterInfo = pkt.MonsterInfos(i).Value;
            var baseInfo = monsterInfo.BaseInfo.Value;
            var monster = Manager.Object.FindMonster(baseInfo.Id);

            if (monster == null)
            {
                var position = monsterInfo.Position.Value;
                var mc = Manager.Spawn.SpawnMonster((MonsterType)monsterInfo.Type);
                mc.transform.position = new Vector3(position.X, position.Y + 2f);
                mc.ID = baseInfo.Id;
                mc.name = $"{mc.name}_{i}";
                mc.Invoke(() => { mc.DestPosX = baseInfo.DestX; });
                Manager.Spawn.InitMonsterPosition(mc.gameObject);
                Manager.Object.Push(mc);
            }
            else
            {
                monster.DestPosX = baseInfo.DestX;
                monster.State = baseInfo.State;
            }
        }
    }
}
