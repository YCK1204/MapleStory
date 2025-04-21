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
            pc.Attack();
        //    return;
        pc.tanjiro_Attack = (Tanjiro_Attack)pkt.AttackId;
        //pc.State = PlayerController.PlayerState.Attack;
        var len = pkt.TargetsLength;
        for (var i = 0; i < len; i++)
        {
            var monster = Manager.Object.FindMonster(pkt.Targets(i));
            if (monster == null)
                Debug.Log("null");
            else
                Debug.Log(monster.name);
        }
    }
}