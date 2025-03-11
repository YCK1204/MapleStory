#include "pch.h"
#include "PacketHandler.h"

void PacketHandler::C_AttackHandler(PacketSession* session, ByteRef& buffer)
{
	auto pkt = GetRoot<C_Attack>(buffer->operator std::byte * ());
	ClientSession* client = reinterpret_cast<ClientSession*>(session);
	auto player = client->Player;
	if (client->State != ClientState::INGAME)
	{
		client->Disconnect();
		return;
	}
	if (player->IsInState(PlayerState::ATTACK) == true)
		return;
	player->AddState(PlayerState::ATTACK);
	try {

		player->Room->PushJob<AttackEnum>([player](AttackEnum attackId) {
			FlatBufferBuilder builder;
			auto data = CreateSC_Attack(builder, player->Id, attackId);
			auto packet = Manager::Packet.CreatePacket(data, builder, PacketType::PacketType_SC_Attack);
			player->Room->Broadcast(packet, player);

			}, pkt->attack_id());
		player->Room->PushJob([player]() {
			player->RemoveState(PlayerState::ATTACK);
			}, 350);

	}
	catch (...)
	{

	}
}