
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
    _handler[PacketType::PacketType_SD_SignUp] = PacketHandler::SD_SignUpHandler;
		_handler[PacketType::PacketType_SD_SignIn] = PacketHandler::SD_SignInHandler;
		_handler[PacketType::PacketType_SD_CharacterList] = PacketHandler::SD_CharacterListHandler;
		_handler[PacketType::PacketType_SD_CharacterDelete] = PacketHandler::SD_CharacterDeleteHandler;
		_handler[PacketType::PacketType_SD_CheckName] = PacketHandler::SD_CheckNameHandler;
		_handler[PacketType::PacketType_SD_CreateCharacter] = PacketHandler::SD_CreateCharacterHandler;
		
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
		ByteRef buf = make_shared<BaseRef<byte>>();

		buf->Reserve(size);
		buf->Copy(buffer + count, size);
		func->second(session, buf);
	}
}
