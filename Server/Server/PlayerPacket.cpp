#include "pch.h"
#include "PacketHandler.h"
#include "Meso.h"

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
			vector<uint64> diedMonsters;
			vector<shared_ptr<Meso>> mesos;

			hitted.reserve(targets.size());
			diedMonsters.reserve(targets.size());
			mesos.reserve(targets.size());

			for (auto id : targets)
			{
				auto go = room->Find(id);
				if (go == nullptr)
					continue;

				auto enemy = reinterpret_pointer_cast<Monster>(go);
				if (enemy->GetState() == MonsterState::MonsterState_Hit)
					continue;
				enemy->Target = player;
				enemy->DestPosX = player->Pos->X;
				enemy->SetState(MonsterState::MonsterState_Hit);
				auto dmg = 10;
				enemy->TakeDamage(dmg);
				if (enemy->IsAlive() == false)
				{
					enemy->SetState(MonsterState::MonsterState_Die);
					diedMonsters.push_back(id);
					auto meso = make_shared<Meso>();
					meso->Pos->X = enemy->Pos->X;
					meso->Pos->Y = enemy->Pos->Y;
					auto go = reinterpret_pointer_cast<GameObject>(meso);
					room->Push(go);
					mesos.push_back(meso);
				}
				else
				{
					hitted.push_back(id);
				}
			}
			FlatBufferBuilder builder;
			{
				auto targetsVector = builder.CreateVector(hitted);
				auto data = CreateSC_Attack(builder, player->Id, attackId, targetsVector);
				auto packet = Manager::Packet.CreatePacket(data, builder, PacketType::PacketType_SC_Attack);
				room->Broadcast(packet);
			}

			if (diedMonsters.size() > 0)
			{
				builder.Clear();
				vector<Offset<Coin>> coins;
				coins.reserve(diedMonsters.size());
				for (auto meso : mesos)
				{
					auto coin = CreateCoin(builder, meso->Id, meso->Money);
					coins.push_back(coin);
				}

				auto coinVec = builder.CreateVector(coins);

				auto died = builder.CreateVector(diedMonsters);
				auto data = CreateSC_MDespawn(builder, died, coinVec);
				auto packet = Manager::Packet.CreatePacket(data, builder, PacketType::PacketType_SC_MDespawn);
				room->Broadcast(packet);
			}
			}, pkt->attack_id());
		player->Room->PushJob([player]() {
			player->RemoveState(PlayerState::ATTACK);
			}, 350);

	}
	catch (...)
	{

	}
}