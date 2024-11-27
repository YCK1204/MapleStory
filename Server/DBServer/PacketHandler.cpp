#include "pch.h"
#include "PacketHandler.h"

void PacketHandler::S_LoginHandler(PacketSession* session, ByteRef& buffer) {
	FlatBufferBuilder builder;

	auto pkt = GetRoot<S_Login>(reinterpret_cast<uint8*>(buffer.get()));

	cout << "S_LoginHandler id : " << pkt->id()->str() << pkt->password()->str() << '\n';
	auto ansPkt = CreateD_Login(builder, true);
	auto data = Manager::Packet.CreatePacket(ansPkt, builder, PacketType_D_Login);
	session->Send(data);
}
