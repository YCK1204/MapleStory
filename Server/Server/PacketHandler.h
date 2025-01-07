#pragma once
#include "Session.h"

class PacketHandler
{
public:
	static void C_SignUpHandler(PacketSession* session, ByteRef& buffer);
	static void D_SignUpHandler(PacketSession* session, ByteRef& buffer);
	static void C_SignInHandler(PacketSession* session, ByteRef& buffer);
	static void D_SignInHandler(PacketSession* session, ByteRef& buffer);
	static void C_SignOutHandler(PacketSession* session, ByteRef& buffer);
	static void C_EnterChannelHandler(PacketSession* session, ByteRef& buffer);
	static void C_ChannelInfoHandler(PacketSession* session, ByteRef& buffer);
	static void C_CreateCharacterHandler(PacketSession* session, ByteRef& buffer);
	static void D_CreateCharacterHandler(PacketSession* session, ByteRef& buffer);
	static void C_CharacterListHandler(PacketSession* session, ByteRef& buffer);
	static void D_CharacterListHandler(PacketSession* session, ByteRef& buffer);
	static void C_CheckNameHandler(PacketSession* session, ByteRef& buffer);
	static void D_CheckNameHandler(PacketSession* session, ByteRef& buffer);
};