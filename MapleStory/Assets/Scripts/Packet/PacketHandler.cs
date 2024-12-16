using Google.FlatBuffers;
using ServerCore;
using System;
using System.Text;
using UnityEngine;

public class PacketHandler
{
    #region SignIn
    public static void SC_SignInHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_SignIn.GetRootAsSC_SignIn(buffer);
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
    #endregion
    public static void SC_EnterChannelHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_EnterChannel.GetRootAsSC_EnterChannel(buffer);

        Manager.Scene.CurScene.OnRecvPacket<SC_EnterChannel>(pkt);
    }
    public static void SC_ChannelInfoHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_ChannelInfo.GetRootAsSC_ChannelInfo(buffer);

        Manager.Scene.CurScene.OnRecvPacket<SC_ChannelInfo>(pkt);
    }
}
