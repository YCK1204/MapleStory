using Google.FlatBuffers;
using ServerCore;
using System;
using UnityEngine;

public partial class PacketHandler
{
    public static void SC_MoveStartHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_MoveStart.GetRootAsSC_MoveStart(buffer);

        var id = pkt.Id;

        var pc =  Manager.Object.FindPlayer(id);
        if (pc == null || pc is MyPlayerContoller)
            return;
        var dir = pkt.Dir;
        var x = pkt.X;
        var y = pkt.Y;
        pc.Dir = dir;
        pc.gameObject.transform.position = new Vector3(x, y);
    }
    public static void SC_MoveEndHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_MoveEnd.GetRootAsSC_MoveEnd(buffer);

        var id = pkt.Id;

        var pc = Manager.Object.FindPlayer(id);
        if (pc == null || pc is MyPlayerContoller)
            return;
        var x = pkt.X;
        var y = pkt.Y;
        pc.Dir = MoveDirection.NONE;
        pc.gameObject.transform.position = new Vector3(x, y);
    }
}
