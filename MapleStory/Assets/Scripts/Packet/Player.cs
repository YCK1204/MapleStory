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
        if (pc == null || pc is MyPlayerContoller)
            return;
        pc.tanjiro_Attack = (Tanjiro_Attack)pkt.AttackId;
        pc.State = PlayerController.PlayerState.Attack;
        pc.Attack();
    }
}