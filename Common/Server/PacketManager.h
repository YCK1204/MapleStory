
#pragma once
#include <functional>
#include <unordered_map>
#include "pch.h"
#include "Session.h"
#include "Array.h"

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
