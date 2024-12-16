#include "pch.h"
#include "PacketHandler.h"
#include "format.h"

void PacketHandler::SD_SignUpHandler(PacketSession* session, ByteRef& buffer)
{
	auto pkt = GetRoot<SD_SignUp>(reinterpret_cast<uint8*>(buffer->operator byte * ()));
	string id = pkt->user_id()->str();
	string password = pkt->passowrd()->str();
	uint32 sessionId = pkt->session_id();

	string query = Utils::format("insert into user_account(user_id, password) values('{0}', '{1}');", { id, password });
	wstring wQuery = wstring(query.begin(), query.end());

	try {
		Manager::DB.RequestAsync(wQuery, [sessionId](shared_ptr<DbManager::QueryArgs> result) {
			SignUpError error;
			if (SQL_SUCCEEDED(result->_ret))
			{
				SQLHSTMT stmt = result->GetStmt();
				HANDLE handle = result->GetHandle();

				SQLRETURN fetchResult = SQLFetch(stmt);

				SQLINTEGER count = 0;
				SQLLEN indicator;
				SQLGetData(stmt, 1, SQL_C_LONG, &count, sizeof(count), &indicator);

				error = SignUpError_SUCCESS;
			}
			else
			{
				error = SignUpError_OVERLAPPED_USERID;
			}
			FlatBufferBuilder builder;
			auto data = CreateD_SignUp(builder, sessionId, error);
			auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_D_SignUp);
			Manager::session->Send(pkt);
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

	string query = Utils::format("with data as "
		"(select * from user_account where user_id='{0}') "
		"select id, case "
			"when user_id is null then 1 "	// 1 == INVALID_ID
			"when password != '{1}' then 2 "// 2 == INVALID_PW
			"else 0 "						// 0 == SUCCESS
		"end as result "
		"from data", {id, password});
	wstring wQuery = wstring(query.begin(), query.end());

	try {
		Manager::DB.RequestAsync(wQuery, [sessionId](shared_ptr<DbManager::QueryArgs> result) {
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
