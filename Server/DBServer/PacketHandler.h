#pragma once
#include "Session.h"

class PacketHandler
{
private:
	static void HandleSignUp(PacketSession* session, const string& id, const uint32& sessionId);
public:
	static void SD_SignUpHandler(PacketSession* session, ByteRef& buffer);
	static void SD_SignInHandler(PacketSession* session, ByteRef& buffer);
	static void SD_CreateCharacterHandler(PacketSession* session, ByteRef& buffer);
	static void SD_CheckNameHandler(PacketSession* session, ByteRef& buffer);
	static void SD_CharacterListHandler(PacketSession* session, ByteRef& buffer);
	static void SD_CharacterDeleteHandler(PacketSession* session, ByteRef& buffer);
	static void SD_CharacterSelectHandler(PacketSession* session, ByteRef& buffer);
};