#include "pch.h"
#include "PacketHandler.h"

void PacketHandler::C_CollectCoinHandler(PacketSession* session, ByteRef& buffer)
{
	auto client = Manager::Session.Find(session->GetSessionId());
	if (client->State != ClientState::INGAME)
	{
		client->Disconnect();
		return;
	}
	auto player = client->Player;
	if (player->IsInState(PlayerState::MOVE))
		return;

	auto room = player->Room;
	if (room == nullptr)
		return;

	auto pkt = GetRoot<C_CollectCoin>(buffer->operator std::byte * ());
	try {
		auto targetId = pkt->id();

		room->PushJob([player, targetId]() {
			auto room = player->Room;

			auto go = room->Find(targetId);
			if (go == nullptr)
				return;
			auto session = player->Session.lock();
			if (session == nullptr)
				return;
			if (go->Type != ObjectType::ITEM)
			{
				session->Disconnect();
				return;
			}
			auto item = reinterpret_pointer_cast<Item>(go);
			if (item->ItemType != ItemType::COIN)
			{
				session->Disconnect();
				return;
			}
			if (session == nullptr)
				return;
			auto meso = reinterpret_pointer_cast<Meso>(item);
			player->CollectCoin(meso);
			room->RemoveObject(targetId);
			FlatBufferBuilder builder;
			{
				auto data = CreateSC_CollectCoin(builder, targetId, player->Id);
				auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_SC_CollectCoin);
				room->Broadcast(pkt);
			}
			{
				builder.Clear();
				auto data = CreateSD_CollectionCoin(builder, player->GetCharId(), player->GetMoey());
				auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_SD_CollectionCoin);
				Manager::Session.dbSession->Send(pkt);
			}
			});
	}
	catch (exception& e) {

	}
}