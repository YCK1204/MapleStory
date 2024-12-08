using Google.FlatBuffers;
using ServerCore;
using System;
using System.Text;
using UnityEngine;

public class PacketHandler
{
    public static void SC_SignInHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_SignUp.GetRootAsSC_SignUp(buffer);
        Manager.Scene.CurScene.OnRecvPacket<SC_SignIn>(pkt);
    }
    public static void SC_SignOutHandler(PacketSession session, ByteBuffer buffer)
    {
    }
    public static void SC_SignUpHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_SignUp.GetRootAsSC_SignUp(buffer);
        Manager.Scene.CurScene.OnRecvPacket<SC_SignUp>(pkt);
    }
}
