#include "pch.h"
#include "PacketHandler.h"

// [ 패킷 크기 ushort 2byte][ 패킷 종류 ushort  ] [uint64][uint64][ushort]

void PacketHandler::C_EnterChannelHandler(PacketSession* session, ByteRef& buffer)
{
	FlatBufferBuilder builder;
	ClientSession* client = reinterpret_cast<ClientSession*>(session);

	EnterChannelError error = EnterChannelError::EnterChannelError_SUCCESS;
	try {
		auto pkt = GetRoot<C_EnterChannel>(buffer->operator std::byte * ());
		uint8 channelId = pkt->channel_index();
		uint8 serverId = pkt->server_index();

		Server* server = Manager::Server.Find(serverId);
		if (server == nullptr)
		{
			session->Disconnect();
			return;
		}
		Channel* channel = server->FindChannel(channelId);
		if (channel == nullptr)
		{
			session->Disconnect();
			return;
		}

		if (channel->GetUserCount() >= server->GetMaxUserCount())
			error = EnterChannelError::EnterChannelError_FULL;
		if (error == EnterChannelError_SUCCESS)
		{
			client->ServerId = serverId;
			client->ChannelId = channelId;
		}
	}
	catch (exception& e)
	{
		cerr << e.what() << endl;
		error = EnterChannelError::EnterChannelError_UNKNOWN;
	}

	auto data = CreateSC_EnterChannel(builder, error);
	auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SC_EnterChannel);
	client->Send(packet);
}

void PacketHandler::C_ChannelInfoHandler(PacketSession* session, ByteRef& buffer)
{
	ClientRef client = Manager::Session.Find(session->GetSessionId());
	if (client == nullptr)
	{
		session->Disconnect();
		return;
	}
	FlatBufferBuilder builder;

	try {
		auto pkt = GetRoot<C_ChannelInfo>(buffer->operator std::byte * ());
		uint8 serverId = pkt->server_id();

		Server* server = Manager::Server.Find(serverId);
		if (server == nullptr)
			client->Disconnect();
		else
		{
			auto data = Manager::Server.GetChannelInfo(server, builder);
			auto packet = Manager::Packet.CreatePacket(data, builder, PacketType::PacketType_SC_ChannelInfo);
			client->Send(packet);
		}
	}
	catch (exception& e)
	{
		cerr << e.what() << endl;
	}
}
