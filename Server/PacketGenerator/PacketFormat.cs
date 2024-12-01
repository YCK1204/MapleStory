using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PacketGenerator
{
    public class PacketFormat_CSharp
    {
        // 0 ServerSession or ClientSession
        // 1 Dict변수.Add
        public static string PMTotal = @"
using Google.FlatBuffers;
using ServerCore;
using System;
using System.Collections.Generic;

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
            Array.Copy(buffer.Array, buffer.Offset + count, data, 0, size - count);
            ByteBuffer bb = new ByteBuffer(data);
            action.Invoke(session, bb);
        }}
    }}
    ushort PacketHederSize = 4;
    public byte[] CreatePacket<T>(Offset<T> data, FlatBufferBuilder builder, PacketType id) where T : struct
    {{
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
    }}
}}
";
        // 0 패킷 테이블 이름
        public static string PMRegister = @"_handler.Add((ushort)PacketType.{0}, PacketHandler.{0}Handler);";
    }
    public class PacketFormat_CPP
    {
        public class Header
        {
            public static string HTotal = @"
#pragma once
#include <functional>
#include <unordered_map>
#include ""pch.h""
#include ""Session.h""
#include ""Array.h""

class PacketManager
{
private:
    static PacketManager* _instance;
    unordered_map<int, function<void(PacketSession*, ByteRef&)> > _handler;
    void Register();
    PacketManager();

public:
    static PacketManager& Instance();
    ~PacketManager();
    void OnRecvPacket(PacketSession* session, byte* buffer);
    template <typename T>
    SendBufferRef CreatePacket(Offset<T>& data, FlatBufferBuilder& builder, PacketType id);
};

template<typename T>
inline SendBufferRef PacketManager::CreatePacket(Offset<T>& data, FlatBufferBuilder& builder, PacketType id)
{
    builder.Finish(data);
    auto size = builder.GetSize() + 4;
    byte* bb = new byte[size];
    BitConverter::TryWriteBytes(bb, size, 0);
    BitConverter::TryWriteBytes(bb, id, 2);
    
    auto* pkt = builder.GetBufferPointer();
    Utils::Array::Copy((byte*)pkt, 0, bb, 4, size - 4);
    return make_shared<SendBuffer>(bb, size);
}
";
        }
        public class Source
        {
            public static string STotal = @"
#include ""pch.h""
#include ""PacketManager.h""
#include ""PacketHandler.h""
#include ""BitConverter.h""

PacketManager* PacketManager::_instance = nullptr;

PacketManager& PacketManager::Instance()
{{
	if (_instance == nullptr)
		_instance = new PacketManager();
	return *_instance;
}}

PacketManager::PacketManager()
{{
	Register();
}}

PacketManager::~PacketManager()
{{
}}

void PacketManager::Register()
{{
    {0}
}}

void PacketManager::OnRecvPacket(PacketSession* session, byte* buffer)
{{
	uint16 count = 0;

	uint16 size = BitConverter::ToUInt16(buffer, count);
	count += sizeof(uint16);
	uint16 id = BitConverter::ToUInt16(buffer, count);
	count += sizeof(uint16);

	auto func = _handler.find(id);
	if (func != _handler.end())
	{{
		size -= count;
		ByteRef buf = make_shared<BaseRef<byte>>();

		buf->Reserve(size);
		buf->Copy(buffer + count, size);
		func->second(session, buf);
	}}
}}
";
        }
        // 0 패킷 테이블 이름
        public static string PMRegister = @"_handler[PacketType::PacketType_{0}] = PacketHandler::{0}Handler;";
    }
}
