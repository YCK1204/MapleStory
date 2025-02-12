using Google.FlatBuffers;
using ServerCore;
using UnityEngine;

public partial class PacketHandler
{
    public static void SC_CharacterListHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_CharacterList.GetRootAsSC_CharacterList(buffer);

        Manager.Scene.CurScene.OnRecvPacket<SC_CharacterList>(pkt);

    }
    public static void SC_CharacterDeleteHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_CharacterDelete.GetRootAsSC_CharacterDelete(buffer);

        Manager.Scene.CurScene.OnRecvPacket<SC_CharacterDelete>(pkt);
    }

    public static void SC_CharacterSelectHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_CharacterSelect.GetRootAsSC_CharacterSelect(buffer);

        Manager.Scene.CurScene.OnRecvPacket<SC_CharacterSelect>(pkt);
    }
}
