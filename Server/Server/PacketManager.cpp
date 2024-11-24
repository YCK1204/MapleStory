
#include "pch.h"
#include "PacketManager.h"
#include "PacketHandler.h"
#include "BitConverter.h"

unordered_map<int, function<void(PacketSession*, ByteRef&)> > PacketManager::_handler;

PacketManager::PacketManager()
{
	Register();
}

PacketManager::~PacketManager()
{
}

void PacketManager::Register()
{
    _handler[PacketType::PacketType_C_Test] = PacketHandler::C_TestHandler;
		
}

void PacketManager::OnRecvPacket(PacketSession* session, byte* buffer)
{
	uint16 count = 0;

	uint16 size = BitConverter::ToUInt16(buffer, count);
	count += sizeof(uint16);
	uint16 id = BitConverter::ToUInt16(buffer, count);
	count += sizeof(uint16);

	auto func = _handler.find(id);
	if (func != _handler.end())
	{
		size -= count;
		ByteRef buf = std::shared_ptr<std::byte[]>(
			new std::byte[size],
			std::default_delete<std::byte[]>()
		);
		Utils::Array::Copy(buffer, count, buf.get(), 0, size);
		func->second(session, buf);
	}
}
