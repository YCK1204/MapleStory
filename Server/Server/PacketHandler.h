#pragma once
#include "Session.h"

class PacketHandler
{
public:
	static void C_SignUpHandler(PacketSession* session, ByteRef& buffer);
	static void D_SignUpHandler(PacketSession* session, ByteRef& buffer);
	static void C_SignInHandler(PacketSession* session, ByteRef& buffer);
	static void D_SignInHandler(PacketSession* session, ByteRef& buffer);
	static void C_EnterChannelHandler(PacketSession* session, ByteRef& buffer);
	static void C_ChannelInfoHandler(PacketSession* session, ByteRef& buffer);
	static void C_CreateCharacterHandler(PacketSession* session, ByteRef& buffer);
	static void D_CreateCharacterHandler(PacketSession* session, ByteRef& buffer);
	static void C_CharacterListHandler(PacketSession* session, ByteRef& buffer);
	static void D_CharacterListHandler(PacketSession* session, ByteRef& buffer);
	static void C_CharacterDeleteHandler(PacketSession* session, ByteRef& buffer);
	static void D_CharacterDeleteHandler(PacketSession* session, ByteRef& buffer);
	static void C_CheckNameHandler(PacketSession* session, ByteRef& buffer);
	static void D_CheckNameHandler(PacketSession* session, ByteRef& buffer);
	static void C_CharacterSelectHandler(PacketSession* session, ByteRef& buffer);
	static void D_CharacterSelectHandler(PacketSession* session, ByteRef& buffer);
	static void C_DespawnHandler(PacketSession* session, ByteRef& buffer);
	static void C_PortalHandler(PacketSession* session, ByteRef& buffer);
	static void C_EnterGameHandler(PacketSession* session, ByteRef& buffer);
	static void C_CreatureInfosHandler(PacketSession* session, ByteRef& buffer);
	static void C_MoveStartHandler(PacketSession* session, ByteRef& buffer);
	static void C_MoveEndHandler(PacketSession* session, ByteRef& buffer);
	static void C_JumpHandler(PacketSession* session, ByteRef& buffer);
	static void C_ProneStabStartHandler(PacketSession* session, ByteRef& buffer);
	static void C_ProneStabEndHandler(PacketSession* session, ByteRef& buffer);
	static void C_AttackHandler(PacketSession* session, ByteRef& buffer);
	static void C_OnCreatureInfosHandler(PacketSession* session, ByteRef& buffer);
};