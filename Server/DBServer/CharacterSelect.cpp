#include "pch.h"
#include "PacketHandler.h"
#include "format.h"

void PacketHandler::SD_CharacterListHandler(PacketSession* session, ByteRef& buffer) {
	auto pkt = GetRoot<SD_CharacterList>(buffer->operator byte * ());
	uint64 sessionId = pkt->session_id();

	wstring query = Utils::wformat(
		"with data as "
		"(select T.char_id from character_info as T "
		"where server_id = {0} and char_id in "
		"(select U.char_id from user_character as U where owner = {1})) "

		"select A.char_id, A.name, B.level, A.char_type, B._str, B._dex, B._int, B._luk "
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
			SQLINTEGER lastPos = 0;
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
