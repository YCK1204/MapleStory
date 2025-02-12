using Google.FlatBuffers;
using ServerCore;
using UnityEngine;

public partial class PacketHandler
{
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
