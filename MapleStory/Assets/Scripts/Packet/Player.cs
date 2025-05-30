using Google.FlatBuffers;
using ServerCore;
using System;
using UnityEngine;
using System.Collections;

public partial class PacketHandler
{
    public static void SC_AttackHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_Attack.GetRootAsSC_Attack(buffer);

        var id = pkt.Id;

        var pc = Manager.Object.FindPlayer(id);
        if (pc == null)
            return;
        if (pc is not MyPlayerContoller)
        {
            pc.Attack();
            pc.tanjiro_Attack = (Tanjiro_Attack)pkt.AttackId;
            var len = pkt.TargetsLength;
            for (var i = 0; i < len; i++)
            {
                var monster = Manager.Object.FindMonster(pkt.Targets(i));
                if (monster != null)
                {
                    monster.State = MonsterState.Hit;
                    monster.Invoke(() =>
                    {
                        monster.State = MonsterState.Move;
                    }, 1f);
                }
            }
        }
    }
    public static void SC_HitByMonsterHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_HitByMonster.GetRootAsSC_HitByMonster(buffer);

    }
    public static void SC_DieHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_Die.GetRootAsSC_Die(buffer);

        ChangeSceneHandler(pkt.MyPlayerInfo.Value, 1);
    }
}