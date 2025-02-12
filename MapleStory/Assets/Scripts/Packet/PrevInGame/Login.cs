using Google.FlatBuffers;
using ServerCore;
using UnityEngine;

public partial class PacketHandler
{
    public static void SC_SignInHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_SignIn.GetRootAsSC_SignIn(buffer);
        Manager.Scene.CurScene.OnRecvPacket<SC_SignIn>(pkt);
    }
    public static void SC_SignUpHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_SignUp.GetRootAsSC_SignUp(buffer);
        Manager.Scene.CurScene.OnRecvPacket<SC_SignUp>(pkt);
    }
}
