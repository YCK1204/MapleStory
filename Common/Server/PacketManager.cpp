
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
		_handler[PacketType::PacketType_C_EnterChannel] = PacketHandler::C_EnterChannelHandler;
		_handler[PacketType::PacketType_C_ChannelInfo] = PacketHandler::C_ChannelInfoHandler;
		_handler[PacketType::PacketType_C_CharacterList] = PacketHandler::C_CharacterListHandler;
		_handler[PacketType::PacketType_D_CharacterList] = PacketHandler::D_CharacterListHandler;
		_handler[PacketType::PacketType_C_CharacterDelete] = PacketHandler::C_CharacterDeleteHandler;
		_handler[PacketType::PacketType_D_CharacterDelete] = PacketHandler::D_CharacterDeleteHandler;
		_handler[PacketType::PacketType_C_CharacterSelect] = PacketHandler::C_CharacterSelectHandler;
		_handler[PacketType::PacketType_D_CharacterSelect] = PacketHandler::D_CharacterSelectHandler;
		_handler[PacketType::PacketType_C_CheckName] = PacketHandler::C_CheckNameHandler;
		_handler[PacketType::PacketType_D_CheckName] = PacketHandler::D_CheckNameHandler;
		_handler[PacketType::PacketType_C_CreateCharacter] = PacketHandler::C_CreateCharacterHandler;
		_handler[PacketType::PacketType_D_CreateCharacter] = PacketHandler::D_CreateCharacterHandler;
		_handler[PacketType::PacketType_C_Portal] = PacketHandler::C_PortalHandler;
		_handler[PacketType::PacketType_C_EnterGame] = PacketHandler::C_EnterGameHandler;
		_handler[PacketType::PacketType_C_Despawn] = PacketHandler::C_DespawnHandler;
		_handler[PacketType::PacketType_C_CreatureInfos] = PacketHandler::C_CreatureInfosHandler;
		_handler[PacketType::PacketType_C_MoveStart] = PacketHandler::C_MoveStartHandler;
		_handler[PacketType::PacketType_C_MoveEnd] = PacketHandler::C_MoveEndHandler;
		_handler[PacketType::PacketType_C_Jump] = PacketHandler::C_JumpHandler;
		_handler[PacketType::PacketType_C_ProneStabStart] = PacketHandler::C_ProneStabStartHandler;
		_handler[PacketType::PacketType_C_ProneStabEnd] = PacketHandler::C_ProneStabEndHandler;
		
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
