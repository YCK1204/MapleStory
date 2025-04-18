using Google.FlatBuffers;
using ServerCore;
using UnityEngine;

public partial class PacketHandler
{
    public static void SC_MonsterMoveHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_MonsterMove.GetRootAsSC_MonsterMove(buffer);

        var len = pkt.MoveInfosLength;
        for (var i = 0; i < len; i++)
        {
            var monsterInfo = pkt.MoveInfos(i).Value;

            var monster = Manager.Object.FindMonster(monsterInfo.Id);
            if (monster == null)
                continue;

            monster.DestPosX = monsterInfo.DestX;
        }
    }
}
