#include "pch.h"
#include "PacketHandler.h"
#include "format.h"

void PacketHandler::SD_CreateCharacterHandler(PacketSession* session, ByteRef& buffer)
{
	auto pkt = GetRoot<SD_CreateCharacter>(buffer->operator std::byte * ());

	string serverId = to_string(pkt->server_id());
	string name = pkt->name()->str();
	string sessionId = to_string(pkt->session_id());
	string dbId = to_string(pkt->db_id());
	string charType = to_string(pkt->char_type());
	string _str = to_string(pkt->ability()->STR());
	string _dex = to_string(pkt->ability()->DEX());
	string _int = to_string(pkt->ability()->INT());
	string _luk = to_string(pkt->ability()->LUK());

	wstring query = Utils::wformat(
		"call CreateCharacter("
		"{0}, '{1}', {2}, {3}, {4}, {5}, {6}, {7});",
		{ dbId, name, charType, serverId, _str, _dex, _int, _luk });

	try {
		Manager::DB.RequestAsync(query, [sessionId](shared_ptr<DbManager::QueryArgs> result) {
			SQLINTEGER ret = 0;
			SQLLEN retIndicator;
			SQLHSTMT stmt = result->GetStmt();

			SQLBindCol(stmt, 1, SQL_INTEGER, &ret, sizeof(ret), &retIndicator);
			SQLRETURN fetchResult = SQLFetch(stmt);
			CreateCharacterError error;
			if (fetchResult == SQL_ERROR)
			{
				error = CreateCharacterError_UNKNOWN;
				Manager::DB.HandleError(SQL_HANDLE_STMT, (SQLHANDLE*)&stmt);
			}
			else
			{
				if (ret == 1)
					error = CreateCharacterError_SUCCESS;
				else
					error = CreateCharacterError_OVERLAPPED;
			}
			FlatBufferBuilder builder;
			auto data = CreateD_CreateCharacter(builder, ::stoi(sessionId), error);
			auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_D_CreateCharacter);
			Manager::session->Send(pkt);
			});
	}
	catch (exception& e)
	{
		FlatBufferBuilder builder;
		auto data = CreateD_CreateCharacter(builder, ::stoi(sessionId), CreateCharacterError_UNKNOWN);
		auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_D_CreateCharacter);
		Manager::session->Send(pkt);
	}
}

void PacketHandler::SD_CheckNameHandler(PacketSession* session, ByteRef& buffer) {
	auto pkt = GetRoot<SD_CheckName>(buffer->operator std::byte * ());

	string serverId = to_string(pkt->server_id());
	string name = pkt->name()->str();
	auto sessionId = pkt->session_id();
	wstring query = Utils::wformat(
		"select count(*) from character_info "
		"where name='{0}' and server_id={1}",
		{ name, serverId });

	try {
		Manager::DB.RequestAsync(query, [sessionId](shared_ptr<DbManager::QueryArgs> result) {
			SQLINTEGER exist = 0;
			SQLLEN existIndicator;
			SQLHSTMT stmt = result->GetStmt();

			SQLBindCol(stmt, 1, SQL_INTEGER, &exist, sizeof(exist), &existIndicator);
			SQLRETURN fetchResult = SQLFetch(stmt);
			CheckNameError error;
			if (fetchResult == SQL_ERROR)
				error = CheckNameError_UNKNOWN;
			else
			{
				if (exist)
					error = CheckNameError_OVERLAPPED;
				else
					error = CheckNameError_SUCCESS;
			}
			FlatBufferBuilder builder;
			auto data = CreateD_CheckName(builder, sessionId, error);
			auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_D_CheckName);
			Manager::session->Send(pkt);
			});
	}
	catch (exception& e)
	{
		FlatBufferBuilder builder;
		auto data = CreateD_CheckName(builder, sessionId, CheckNameError_UNKNOWN);
		auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_D_CheckName);
		Manager::session->Send(pkt);
	}
}
