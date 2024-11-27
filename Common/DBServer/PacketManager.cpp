
#include "pch.h"
#include "PacketManager.h"
#include "PacketHandler.h"
#include "BitConverter.h"

PacketManager* PacketManager::_instance = nullptr;

PacketManager& PacketManager::Instance()
{
	if (_instance == nullptr)
		_instance = new PacketManager();
	return *_instance;
}

PacketManager::PacketManager()
{
	Register();
}

PacketManager::~PacketManager()
{
}

void PacketManager::Register()
{
    _handler[PacketType::PacketType_S_Login] = PacketHandler::S_LoginHandler;
		
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
