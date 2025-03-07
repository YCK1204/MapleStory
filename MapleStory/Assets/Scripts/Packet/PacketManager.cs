
using Google.FlatBuffers;
using ServerCore;
using System;
using System.Collections.Generic;

public class PacketManager
{
    Dictionary<ushort, Action<PacketSession, ByteBuffer>> _handler = new Dictionary<ushort, Action<PacketSession, ByteBuffer>>();
    public PacketManager()
    {
        Register();
    }
    void Register()
    {
        _handler.Add((ushort)PacketType.SC_SignUp, PacketHandler.SC_SignUpHandler);
		_handler.Add((ushort)PacketType.SC_SignIn, PacketHandler.SC_SignInHandler);
		_handler.Add((ushort)PacketType.SC_EnterChannel, PacketHandler.SC_EnterChannelHandler);
		_handler.Add((ushort)PacketType.SC_ChannelInfo, PacketHandler.SC_ChannelInfoHandler);
		_handler.Add((ushort)PacketType.SC_CharacterList, PacketHandler.SC_CharacterListHandler);
		_handler.Add((ushort)PacketType.SC_CharacterDelete, PacketHandler.SC_CharacterDeleteHandler);
		_handler.Add((ushort)PacketType.SC_CharacterSelect, PacketHandler.SC_CharacterSelectHandler);
		_handler.Add((ushort)PacketType.SC_CheckName, PacketHandler.SC_CheckNameHandler);
		_handler.Add((ushort)PacketType.SC_CreateCharacter, PacketHandler.SC_CreateCharacterHandler);
		_handler.Add((ushort)PacketType.SC_Portal, PacketHandler.SC_PortalHandler);
		_handler.Add((ushort)PacketType.SC_EnterGame, PacketHandler.SC_EnterGameHandler);
		_handler.Add((ushort)PacketType.SC_PSpawn, PacketHandler.SC_PSpawnHandler);
		_handler.Add((ushort)PacketType.SC_MSpawn, PacketHandler.SC_MSpawnHandler);
		_handler.Add((ushort)PacketType.SC_Despawn, PacketHandler.SC_DespawnHandler);
		_handler.Add((ushort)PacketType.SC_CreatureInfos, PacketHandler.SC_CreatureInfosHandler);
		_handler.Add((ushort)PacketType.SC_MoveStart, PacketHandler.SC_MoveStartHandler);
		_handler.Add((ushort)PacketType.SC_MoveEnd, PacketHandler.SC_MoveEndHandler);
		_handler.Add((ushort)PacketType.SC_Jump, PacketHandler.SC_JumpHandler);
		_handler.Add((ushort)PacketType.SC_ProneStabStart, PacketHandler.SC_ProneStabStartHandler);
		_handler.Add((ushort)PacketType.SC_ProneStabEnd, PacketHandler.SC_ProneStabEndHandler);
		
    }
    public void OnRecvPacket(PacketSession session, ArraySegment<byte> buffer)
    {
        ushort count = 0;

        ushort size = BitConverter.ToUInt16(buffer.Array, buffer.Offset);
        count += sizeof(ushort);
        ushort id = BitConverter.ToUInt16(buffer.Array, buffer.Offset + count);
        count += sizeof(ushort);

        Action<PacketSession, ByteBuffer> action = null;
        if (_handler.TryGetValue(id, out action))
        {
            byte[] data = new byte[size - count];
            Array.Copy(buffer.Array, buffer.Offset + count, data, 0, size - count);
            ByteBuffer bb = new ByteBuffer(data);
            action.Invoke(session, bb);
        }
    }
    ushort PacketHederSize = 4;
    public byte[] CreatePacket<T>(Offset<T> data, FlatBufferBuilder builder, PacketType id) where T : struct
    {
        builder.Finish(data.Value);
        var bytes = builder.SizedByteArray();
        ushort size = (ushort)(bytes.Length + PacketHederSize);
        ArraySegment<byte> segment = new ArraySegment<byte>(new byte[size]);

        bool success = true;
        success &= BitConverter.TryWriteBytes(new Span<byte>(segment.Array, 0, 2), size);
        success &= BitConverter.TryWriteBytes(new Span<byte>(segment.Array, 2, 4), (ushort)id);
        Buffer.BlockCopy(bytes, 0, segment.Array, 4, bytes.Length);

        if (success == false)
            return null;
        return segment.Array;
    }
}
