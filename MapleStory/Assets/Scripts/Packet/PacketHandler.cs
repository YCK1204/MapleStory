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
    public static void SC_SignUpHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_SignUp.GetRootAsSC_SignUp(buffer);
        Manager.Scene.CurScene.OnRecvPacket<SC_SignUp>(pkt);
    }
    #endregion
    #region WorldSelect
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
    #endregion
    #region CharacterSelect
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
    #endregion
    #region CreateCharacter
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
    #endregion
    public static void SC_EnterMapHandler(PacketSession session, ByteBuffer buffer)
    {

    }

    public static void SC_SpawnHandler(PacketSession session, ByteBuffer buffer)
    {

    }
    public static void SC_DespawnHandler(PacketSession session, ByteBuffer buffer)
    {

    }
}
