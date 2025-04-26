#include "pch.h"
#include "PacketHandler.h"
#include "format.h"

void PacketHandler::SD_CollectionCoinHandler(PacketSession* session, ByteRef& buffer) {
	auto pkt = GetRoot<SD_CollectionCoin>(buffer->operator byte * ());
	wstring query = Utils::wformat(
		"update character_status set money = {0} where char_id = {1};"
		, initializer_list<uint64>{ static_cast<uint64>(pkt->money()), pkt->char_id() });

	try {
		Manager::DB.RequestAsync(query, [](shared_ptr<DbManager::QueryArgs> result) {

			if (result->_ret == SQL_ERROR)
				throw - 1;
			});
	}
	catch (...)
	{
	}
}