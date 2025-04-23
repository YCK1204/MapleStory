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
		vector<uint64> targets(pkt->targets()->begin(), pkt->targets()->end());

		player->Room->PushJob<AttackEnum>([player, targets](AttackEnum attackId) {
			auto room = player->Room;

			vector<uint64> hitted;
			hitted.reserve(targets.size());
			for (auto id : targets)
			{
				auto go = room->Find(id);
				if (go == nullptr)
					continue;

				auto enemy = reinterpret_cast<Monster*>(go);
				enemy->Target = player;
				enemy->DestPosX = player->Pos->X;
				enemy->SetState(MonsterState::MonsterState_Trace);
				//auto dmg = 10;
				//enemy->TakeDamage(dmg);
				//if (enemy->IsAlive() == false)
				hitted.push_back(id);
			}
			FlatBufferBuilder builder;
			auto targetsVector = builder.CreateVector(hitted);
			auto data = CreateSC_Attack(builder, player->Id, attackId, targetsVector);
			auto packet = Manager::Packet.CreatePacket(data, builder, PacketType::PacketType_SC_Attack);
			room->Broadcast(packet);
			}, pkt->attack_id());
		player->Room->PushJob([player]() {
			player->RemoveState(PlayerState::ATTACK);
			}, 350);

	}
	catch (...)
	{

	}
}