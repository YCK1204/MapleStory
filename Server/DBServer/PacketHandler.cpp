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
	auto pkt = GetRoot<SD_SignUp>(buffer->operator byte * ());
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
	auto pkt = GetRoot<SD_SignIn>(buffer->operator byte * ());
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
	auto pkt = GetRoot<SD_CharacterList>(buffer->operator byte * ());
	uint64 sessionId = pkt->session_id();

	wstring query = Utils::wformat(
		"with data as "
		"(select T.char_id from character_info as T "
		"where server_id = {0} and char_id in "
		"(select U.char_id from user_character as U where owner = {1})) "

		"select A.char_id, A.name, A.level, A.char_type, B._str, B._dex, B._int, B._luk "
		"from character_info as A "
		"inner join "
		"character_status as B "
		"on A.char_id = B.char_id "
		"where A.char_id in (select D.char_id from data as D);"
		, initializer_list<uint64>{ pkt->server_id(), pkt->db_id()});

	try {
		Manager::DB.RequestAsync(query, [sessionId](shared_ptr<DbManager::QueryArgs> result) {

			if (result->_ret == SQL_ERROR)
				throw - 1;

			FlatBufferBuilder builder;
			SQLHSTMT stmt = result->GetStmt();
			HANDLE handle = result->GetHandle();

			SQLINTEGER charId = 0;
			SQLCHAR name[20] = {};
			SQLINTEGER level = 0;
			SQLINTEGER charType = 0;
			SQLINTEGER _str = 0;
			SQLINTEGER _dex = 0;
			SQLINTEGER _int = 0;
			SQLINTEGER _luk = 0;
			SQLLEN charIdIndicator;
			SQLLEN nameIndicator;
			SQLLEN levelIndicator;
			SQLLEN charTypeIndicator;
			SQLLEN strIndicator;
			SQLLEN dexIndicator;
			SQLLEN intIndicator;
			SQLLEN lukIndicator;

			SQLBindCol(stmt, 1, SQL_C_LONG, &charId, sizeof(charId), &charIdIndicator);
			SQLBindCol(stmt, 2, SQL_C_CHAR, name, sizeof(name), &nameIndicator);
			SQLBindCol(stmt, 3, SQL_C_LONG, &level, sizeof(level), &levelIndicator);
			SQLBindCol(stmt, 4, SQL_C_LONG, &charType, sizeof(charType), &charTypeIndicator);
			SQLBindCol(stmt, 5, SQL_C_LONG, &_str, sizeof(_str), &strIndicator);
			SQLBindCol(stmt, 6, SQL_C_LONG, &_dex, sizeof(_dex), &dexIndicator);
			SQLBindCol(stmt, 7, SQL_C_LONG, &_int, sizeof(_int), &intIndicator);
			SQLBindCol(stmt, 8, SQL_C_LONG, &_luk, sizeof(_luk), &lukIndicator);

			vector<Offset<CharacterPreviewInfo>> charInfos;
			while (true)
			{
				SQLRETURN fetchResult = SQLFetch(stmt);
				if (fetchResult == SQL_NO_DATA || fetchResult != SQL_SUCCESS)
					break;

				string str;
				str.reserve(nameIndicator);
				for (auto i = 0; i < nameIndicator; i++)
					str.push_back(name[i]);
				auto ability = CreateCharacterAbility(builder, _str, _dex, _int, _luk);
				auto info = CreateCharacterPreviewInfo(
					builder,
					charId,
					static_cast<uint8>(charType),
					static_cast<uint16>(level),
					builder.CreateString(str),
					ability);
				charInfos.push_back(info);
			}

			auto infos = builder.CreateVector(charInfos);
			auto data = CreateD_CharacterList(builder, CharacterListError_SUCCESS, sessionId, infos);
			auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_D_CharacterList);
			Manager::session->Send(pkt);
			});
	}
	catch (...)
	{
		FlatBufferBuilder builder;
		auto data = CreateD_CharacterList(builder, CharacterListError_UNKNOWN);
		auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_D_CharacterList);
		Manager::session->Send(pkt);
	}
}
void PacketHandler::SD_CharacterDeleteHandler(PacketSession* session, ByteRef& buffer)
{
	auto pkt = GetRoot< SD_CharacterDelete>(buffer->operator std::byte * ());

	try {
		auto owner = pkt->db_id();
		auto charId = pkt->char_id();
		auto sessionId = pkt->session_id();

		wstring query = Utils::wformat(
			"call DeleteCharacter({0}, {1});"
			, initializer_list<uint64>{owner, charId});

		Manager::DB.RequestAsync(query, [sessionId](shared_ptr<DbManager::QueryArgs> result) {
			if (result->_ret == SQL_ERROR)
				throw - 1;
			SQLHSTMT stmt = result->GetStmt();

			SQLINTEGER ret = 0;
			SQLLEN retIndicator;
			SQLBindCol(stmt, 1, SQL_C_LONG, &ret, sizeof(ret), &retIndicator);
			SQLRETURN fetchResult = SQLFetch(stmt);
			if (fetchResult == SQL_NO_DATA || fetchResult != SQL_SUCCESS || ret == 0)
				throw - 1;
			FlatBufferBuilder builder;
			auto data = CreateD_CharacterDelete(builder, sessionId, CharacterDeleteError_SUCCESS);
			auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_D_CharacterDelete);
			Manager::session->Send(pkt);
			});
	}
	catch (...)
	{
		FlatBufferBuilder builder;
		auto data = CreateD_CharacterDelete(builder, pkt->session_id(), CharacterDeleteError_UNKNOWN);
		auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_D_CharacterDelete);
		Manager::session->Send(packet);
	}
}
void PacketHandler::SD_CharacterSelectHandler(PacketSession* session, ByteRef& buffer)
{
	auto pkt = GetRoot<SD_CharacterSelect>(buffer->operator std::byte * ());

	auto sessionId = pkt->session_id();
	auto dbId = pkt->db_id();
	auto charId = pkt->char_id();

	wstring query = Utils::wformat(
		"select A.level, A.exp, A.hp, A.mp, A.last_pos, A._str, A._dex, A._int, A._luk, B.name, B.char_type " 
		"from character_status as A "
		"inner join " 
		"character_info as B " 
		"on A.char_id = B.char_id " 
		"where(select owner from user_character where owner = {0} and char_id = {1}) is not null;"
		, initializer_list<uint64>{ dbId, charId }
	);

	try {
		Manager::DB.RequestAsync(query, [sessionId, charId](shared_ptr<DbManager::QueryArgs> result) {
			FlatBufferBuilder builder;

			SQLINTEGER level;
			SQLINTEGER exp;
			SQLINTEGER hp;
			SQLINTEGER mp;
			SQLINTEGER lastPos;
			SQLINTEGER _str;
			SQLINTEGER _dex;
			SQLINTEGER _int;
			SQLINTEGER _luk;
			SQLCHAR name[20] = {};
			SQLINTEGER charType;

			SQLLEN levelIndicator;
			SQLLEN expIndicator;
			SQLLEN hpIndicator;
			SQLLEN mpIndicator;
			SQLLEN lastPosIndicator;
			SQLLEN _strIndicator;
			SQLLEN _dexIndicator;
			SQLLEN _intIndicator;
			SQLLEN _lukIndicator;
			SQLLEN nameIndicator;
			SQLLEN charTypeIndicator;

			SQLHSTMT stmt = result->GetStmt();

			if (result->_ret == SQL_ERROR)
				throw - 1;
			SQLBindCol(stmt, 1, SQL_INTEGER, &level, sizeof(level), &levelIndicator);
			SQLBindCol(stmt, 2, SQL_INTEGER, &exp, sizeof(exp), &expIndicator);
			SQLBindCol(stmt, 3, SQL_INTEGER, &hp, sizeof(hp), &hpIndicator);
			SQLBindCol(stmt, 4, SQL_INTEGER, &mp, sizeof(mp), &mpIndicator);
			SQLBindCol(stmt, 5, SQL_INTEGER, &lastPos, sizeof(lastPos), &lastPosIndicator);
			SQLBindCol(stmt, 6, SQL_INTEGER, &_str, sizeof(_str), &_strIndicator);
			SQLBindCol(stmt, 7, SQL_INTEGER, &_dex, sizeof(_dex), &_dexIndicator);
			SQLBindCol(stmt, 8, SQL_INTEGER, &_int, sizeof(_int), &_intIndicator);
			SQLBindCol(stmt, 9, SQL_INTEGER, &_luk, sizeof(_luk), &_lukIndicator);
			SQLBindCol(stmt, 10, SQL_CHAR, name, sizeof(name), &nameIndicator);
			SQLBindCol(stmt, 11, SQL_INTEGER, &charType, sizeof(charType), &charTypeIndicator);
			SQLRETURN fetchResult = SQLFetch(stmt);
			if (fetchResult != SQL_SUCCESS || fetchResult == SQL_NO_DATA)
				throw - 1;

			string n;
			for (SQLLEN i = 0; i < nameIndicator; i++)
				n += name[i];

			auto nameStr = builder.CreateString(n);

			auto ability = CreateCharacterAbility(builder, _str, _dex, _int, _luk);
			auto prevInfo = CreateCharacterPreviewInfo(builder, charId, charType, level, nameStr, ability);
			auto totalInfo = CreateCharacterTotalInfo(builder, prevInfo, lastPos, hp, mp, exp);
			auto data = CreateD_CharacterSelect(builder, CharacterSelectError_SUCCESS, sessionId, totalInfo);
			auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_D_CharacterSelect);
			Manager::session->Send(pkt);
			});
	}
	catch (...)
	{
		FlatBufferBuilder builder;
		auto data = CreateD_CharacterSelect(builder, CharacterSelectError_UNKNOWN, sessionId);
		auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_D_CharacterSelect);
		session->Send(pkt);
	}
}
#pragma endregion
#pragma region CreateCharacter
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
#pragma endregion
