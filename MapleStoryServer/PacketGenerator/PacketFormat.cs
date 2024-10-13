using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PacketGenerator
{
    public class PacketFormat
    {
        // 0 ServerSession or ClientSession
        // 1 Dict변수.Add
        public static string PMTotal = @"
using Google.FlatBuffers;
using ServerCore;

public class PacketManager
{{
    Dictionary<ushort, Action<PacketSession, ByteBuffer>> _handler = new Dictionary<ushort, Action<PacketSession, ByteBuffer>>();
    public PacketManager()
    {{
        Register();
    }}
    void Register()
    {{
        {0}
    }}
    public void OnRecvPacket(PacketSession session, ArraySegment<byte> buffer)
    {{
        ushort count = 0;

        ushort size = BitConverter.ToUInt16(buffer.Array, buffer.Offset);
        count += sizeof(ushort);
        ushort id = BitConverter.ToUInt16(buffer.Array, buffer.Offset + count);
        count += sizeof(ushort);

        Action<PacketSession, ByteBuffer> action = null;
        if (_handler.TryGetValue(id, out action))
        {{
            byte[] data = new byte[size - count];
            Array.Copy(buffer.Array, count, data, 0, size - count);
            ByteBuffer bb = new ByteBuffer(data);
            action.Invoke(session, bb);
        }}
    }}
    ushort PacketHederSize = 4;
    public byte[] CreatePacket(byte[] data, ushort id)
    {{
        ushort size = (ushort)(data.Length + PacketHederSize);
        ArraySegment<byte> segment = new ArraySegment<byte>(new byte[size]);

        bool success = true;
        success &= BitConverter.TryWriteBytes(new Span<byte>(segment.Array, 0, 2), size);
        success &= BitConverter.TryWriteBytes(new Span<byte>(segment.Array, 2, 4), id);
        Buffer.BlockCopy(data, 0, segment.Array, 4, data.Length);

        if (success == false)
            return null;
        return segment.Array;
    }}
}}
";
        // 0 패킷 테이블 이름
        public static string PMRegister = @"_handler.Add((ushort)Equipment.{0}, PacketHandler.{0}Handler);";
    }
}
