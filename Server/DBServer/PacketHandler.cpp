#include "pch.h"
#include "PacketHandler.h"

void PacketHandler::S_LoginHandler(PacketSession* session, ByteRef& buffer) {
	FlatBufferBuilder builder;

	auto pkt = GetRoot<S_Login>(reinterpret_cast<uint8*>(buffer.get()));

	Manager::DB.RequestAsync(L"insert into test values(3000)", [&](shared_ptr<DbManager::QueryArgs> result) {
		if (SQL_SUCCEEDED(result->_ret))
		{
			SQLHSTMT stmt = result->GetStmt();
			HANDLE handle = result->GetHandle();

			SQLRETURN fetchResult = SQLFetch(stmt);
			while (fetchResult == SQL_SUCCESS)
			{
				char buf[256] = {};
				SQLLEN indicator;
				SQLGetData(stmt, 1, SQL_C_CHAR, buf, sizeof(buf), &indicator);

				cout << "buf : " << buf << endl;
				fetchResult = SQLFetch(stmt);
			}
		}
		else
		{
			// todo
		}
		});

	cout << "S_LoginHandler id : " << pkt->id()->str() << pkt->password()->str() << '\n';
	auto ansPkt = CreateD_Login(builder, true);
	auto data = Manager::Packet.CreatePacket(ansPkt, builder, PacketType_D_Login);
	session->Send(data);
}
