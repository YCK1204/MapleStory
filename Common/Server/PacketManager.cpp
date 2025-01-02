
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
    _handler[PacketType::PacketType_C_SignUp] = PacketHandler::C_SignUpHandler;
		_handler[PacketType::PacketType_D_SignUp] = PacketHandler::D_SignUpHandler;
		_handler[PacketType::PacketType_C_SignIn] = PacketHandler::C_SignInHandler;
		_handler[PacketType::PacketType_D_SignIn] = PacketHandler::D_SignInHandler;
		_handler[PacketType::PacketType_C_SignOut] = PacketHandler::C_SignOutHandler;
		_handler[PacketType::PacketType_C_EnterChannel] = PacketHandler::C_EnterChannelHandler;
		_handler[PacketType::PacketType_C_ChannelInfo] = PacketHandler::C_ChannelInfoHandler;
		_handler[PacketType::PacketType_C_CreateCharacter] = PacketHandler::C_CreateCharacterHandler;
		_handler[PacketType::PacketType_D_CreateCharacter] = PacketHandler::D_CreateCharacterHandler;
		
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
