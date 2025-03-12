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
    public static void SC_JumpHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_Jump.GetRootAsSC_Jump(buffer);

        var id = pkt.Id;
        var pc = Manager.Object.FindPlayer(id);
        if (pc == null || pc is MyPlayerContoller)
            return;

        pc.Jump();
    }

    public static void SC_ProneStabEndHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_ProneStabEnd.GetRootAsSC_ProneStabEnd(buffer);

        var id = pkt.Id;
        var pc = Manager.Object.FindPlayer(id);
        if (pc == null || pc is MyPlayerContoller)
            return;

        pc.AddState(PlayerController.PlayerState.ProneStab);
    }

    public static void SC_ProneStabStartHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_ProneStabStart.GetRootAsSC_ProneStabStart(buffer);

        var id = pkt.Id;
        var pc = Manager.Object.FindPlayer(id);
        if (pc == null || pc is MyPlayerContoller)
            return;

        pc.RemoveState(PlayerController.PlayerState.ProneStab);
    }
    public static void SC_LadderUpStartHandler(PacketSession session, ByteBuffer buffer)
    {

    }
    public static void SC_LadderUpEndHandler(PacketSession session, ByteBuffer buffer)
    {

    }
    public static void SC_LadderDownStartHandler(PacketSession session, ByteBuffer buffer)
    {

    }
    public static void SC_LadderDownEndHandler(PacketSession session, ByteBuffer buffer)
    {

    }
}
