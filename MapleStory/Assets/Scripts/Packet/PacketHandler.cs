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
    #region WorldSelect
    public static void SC_EnterChannelHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_EnterChannel.GetRootAsSC_EnterChannel(buffer);

        if (pkt.Ok == EnterChannelError.SUCCESS)
        {
            FlatBufferBuilder builder = new FlatBufferBuilder(1);
            
            C_CharacterList.StartC_CharacterList(builder);
            var data = C_CharacterList.EndC_CharacterList(builder);
            var packet = Manager.Packet.CreatePacket(data, builder, PacketType.C_CharacterList);
            Manager.Network.Send(packet);
        }
        else
        {
            Manager.Scene.CurScene.OnRecvPacket<SC_EnterChannel>(pkt);
        }
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
    #endregion
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

    public static void SC_CharacterDeleteHandler(PacketSession session, ByteBuffer buffer)
    {
        var pkt = SC_CharacterDelete.GetRootAsSC_CharacterDelete(buffer);

        Manager.Scene.CurScene.OnRecvPacket<SC_CharacterDelete>(pkt);
    }
}
