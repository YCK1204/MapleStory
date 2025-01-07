#include "pch.h"
#include "PacketHandler.h"
#include "format.h"

#pragma region Login
void PacketHandler::HandleSignUp(PacketSession* session, const string& id, const uint32& sessionId)
{
	wstring query = Utils::wformat("select id from user_account where user_id='{0}'", { id });

	Manager::DB.RequestAsync(query, [sessionId](shared_ptr<DbManager::QueryArgs> result) {
		FlatBufferBuilder builder;
		SQLINTEGER dbId = 0;
		SQLHSTMT stmt = result->GetStmt();
		HANDLE handle = result->GetHandle();

		SQLINTEGER ret = 0;
		SQLLEN dbIdIndicator;
		SQLBindCol(stmt, 1, SQL_C_LONG, &dbId, sizeof(dbId), &dbIdIndicator);

		SQLRETURN fetchResult = SQLFetch(stmt);
		auto data = CreateD_SignUp(builder, dbId, sessionId, SignUpError_SUCCESS);
		auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_D_SignUp);
		Manager::session->Send(pkt);
		});
}

void PacketHandler::SD_SignUpHandler(PacketSession* session, ByteRef& buffer)
{
	auto pkt = GetRoot<SD_SignUp>(reinterpret_cast<uint8*>(buffer->operator byte * ()));
	string id = pkt->user_id()->str();
	string password = pkt->passowrd()->str();
	uint32 sessionId = pkt->session_id();

	wstring query = Utils::wformat("insert into user_account(user_id, password) values('{0}', '{1}');", { id, password });

	try {
		Manager::DB.RequestAsync(query, [id, sessionId, session](shared_ptr<DbManager::QueryArgs> result) {
			FlatBufferBuilder builder;
			if (SQL_SUCCEEDED(result->_ret))
			{
				HandleSignUp(session, id, sessionId);
			}
			else
			{
				auto data = CreateD_SignUp(builder, 0, sessionId, SignUpError_OVERLAPPED_USERID);
				auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_D_SignUp);
				Manager::session->Send(pkt);
			}
			});
	}
	catch (exception& e)
	{
		cerr << e.what() << endl;
		FlatBufferBuilder builder;
		auto data = CreateD_SignUp(builder, pkt->session_id(), SignUpError_UNKNOWN);
		auto bytes = Manager::Packet.CreatePacket(data, builder, PacketType_D_SignUp);
		Manager::session->Send(bytes);
	}
}

void PacketHandler::SD_SignInHandler(PacketSession* session, ByteRef& buffer)
{
	auto pkt = GetRoot<SD_SignIn>(reinterpret_cast<uint8*>(buffer->operator byte * ()));
	string id = pkt->user_id()->str();
	string password = pkt->password()->str();
	uint32 sessionId = pkt->session_id();

	wstring query = Utils::wformat(
		"with data as "
		"(select * from user_account where user_id='{0}') "
		"select id, case "
		"when user_id is null then 1 "	// 1 == INVALID_ID
		"when password != '{1}' then 2 "// 2 == INVALID_PW
		"else 0 "						// 0 == SUCCESS
		"end as result "
		"from data", { id, password });

	try {
		Manager::DB.RequestAsync(query, [sessionId](shared_ptr<DbManager::QueryArgs> result) {
			SignInError error;
			SQLINTEGER dbId = 0;

			SQLHSTMT stmt = result->GetStmt();
			if (SQL_SUCCEEDED(result->_ret))
			{
				HANDLE handle = result->GetHandle();

				SQLINTEGER ret = 0;
				SQLLEN dbIdIndicator;
				SQLLEN retIndicator;
				SQLBindCol(stmt, 1, SQL_C_LONG, &dbId, sizeof(dbId), &dbIdIndicator);
				SQLBindCol(stmt, 2, SQL_C_LONG, &ret, sizeof(ret), &retIndicator);

				SQLRETURN fetchResult = SQLFetch(stmt);
				if (fetchResult == SQL_NO_DATA)
					error = SignInError_INVALID_ID;
				else if (fetchResult == SQL_SUCCESS)
					error = (SignInError)ret;
			}
			else
			{
				error = SignInError_UNKNOWN;
				Manager::DB.HandleError(SQL_HANDLE_STMT, (SQLHANDLE*)&stmt);
			}
			FlatBufferBuilder builder;
			auto data = CreateD_SignIn(builder, dbId, sessionId, error);
			auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_D_SignIn);
			Manager::session->Send(pkt);
			});
	}
	catch (exception& e)
	{
		cerr << e.what() << endl;
		FlatBufferBuilder builder;
		auto data = CreateD_SignIn(builder, pkt->session_id(), SignInError_UNKNOWN);
		auto bytes = Manager::Packet.CreatePacket(data, builder, PacketType_D_SignIn);
		Manager::session->Send(bytes);
	}
}
#pragma endregion
#pragma region CharacterSelect
void PacketHandler::SD_CharacterListHandler(PacketSession* session, ByteRef& buffer) {
	auto pkt = GetRoot<SD_CharacterList>(reinterpret_cast<uint8*>(buffer->operator byte * ()));
	uint64 sessionId = pkt->session_id();

	wstring query = Utils::wformat(
		"select * from character_info "
		"where char_id in "
		"(select char_id from user_character where owner = {0}) "
		"and "
		"server_id = {1};"
		, initializer_list<uint64>{ pkt->db_id(), pkt->server_id() });

	try {
		Manager::DB.RequestAsync(query, [sessionId](shared_ptr<DbManager::QueryArgs> result) {

			if (result->_ret == SQL_ERROR)
				throw;

			FlatBufferBuilder builder;
			SQLHSTMT stmt = result->GetStmt();
			HANDLE handle = result->GetHandle();

			SQLCHAR name[50] = {};
			SQLINTEGER level = 0;
			SQLINTEGER charType = 0;
			SQLLEN nameIndicator;
			SQLLEN levelIndicator;
			SQLLEN charTypeIndicator;
			SQLBindCol(stmt, 2, SQL_C_CHAR, name, sizeof(name), &nameIndicator);
			SQLBindCol(stmt, 3, SQL_C_LONG, &level, sizeof(level), &levelIndicator);
			SQLBindCol(stmt, 4, SQL_C_LONG, &charType, sizeof(charType), &charTypeIndicator);

			vector<Offset<CharacterInfo>> charInfos;
			while (true)
			{
				SQLRETURN fetchResult = SQLFetch(stmt);
				if (fetchResult == SQL_NO_DATA || fetchResult != SQL_SUCCESS)
					break;

				string str;
				str.reserve(nameIndicator);
				for (auto i = 0; i < nameIndicator; i++)
					str.push_back(name[i]);
				auto info = CreateCharacterInfo(
					builder,
					static_cast<uint8>(charType),
					static_cast<uint16>(level),
					builder.CreateString(str));
				charInfos.push_back(info);
			}

			auto infos = builder.CreateVector(charInfos);
			auto data = CreateD_CharacterList(builder, CharacterListError_SUCCESS, sessionId, infos);
			auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_D_CharacterList);
			Manager::session->Send(pkt);
			});
	}
	catch (exception& e)
	{
		FlatBufferBuilder builder;
		auto data = CreateD_CharacterList(builder, CharacterListError_UNKNOWN);
		auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_D_CharacterList);
		Manager::session->Send(pkt);
	}
}
#pragma endregion
#pragma region CreateCharacter
void PacketHandler::SD_CreateCharacterHandler(PacketSession* session, ByteRef& buffer)
{

}

void PacketHandler::SD_CheckNameHandler(PacketSession* session, ByteRef& buffer) {

}
#pragma endregion