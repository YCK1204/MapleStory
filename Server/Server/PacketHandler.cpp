#include "pch.h"
#include "PacketHandler.h"

void PacketHandler::D_LoginHandler(PacketSession* session, ByteRef& buffer)
{
	FlatBufferBuilder b;

	auto pkt = GetRoot<D_Login>(reinterpret_cast<uint8*>(buffer.get()));

	if (pkt->ok() == true)
		cout << "D_LoginHandler : true\n";
	else
		cout << "D_LoginHandler : false\n";
}
