using Google.FlatBuffers;
using ServerCore;
using UnityEngine;

public partial class PacketHandler
{
    public static void SC_CreateCharacterHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_CreateCharacter.GetRootAsSC_CreateCharacter(buffer);

        Manager.Scene.CurScene.OnRecvPacket<SC_CreateCharacter>(pkt);
    }
    public static void SC_CheckNameHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_CheckName.GetRootAsSC_CheckName(buffer);

        Manager.Scene.CurScene.OnRecvPacket<SC_CheckName>(pkt);
    }
}
