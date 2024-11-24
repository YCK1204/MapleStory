#include "pch.h"
#include "ClientSession.h"
#include "PacketManager.h"

void ClientSession::OnConnect()
{
	flatbuffers::FlatBufferBuilder builder;
	auto str = builder.CreateString("test string");
	auto test = CreateS_Test(builder, 32, str);
	auto size = builder.GetSize();
	//auto* data = builder.GetBufferPointer();
	Init();
	cout << "connected!" << endl;
	RegisterRecv();

	auto data = PacketManager::CreatePacket(test, builder, PacketType_S_Test);
	Send(data);
}

void ClientSession::OnDisconnect()
{
	GPoolManager->Push<ClientSession>(this);
	cout << "disconnected!\n";
}

void ClientSession::OnSend()
{
}

void ClientSession::OnRecvPacket(int32 size, byte* data)
{
	PacketManager::OnRecvPacket(this, data);
	/*vector<byte> arr;
	arr.assign(size, (byte)0);
	for (int32 i = 0; i < size; i++)
		arr[i] = data[i];
	uint16 packetSize = BitConverter::ToUInt16(arr);
	data += 2;
	char* str = (char*)data;
	cout << packetSize << endl;
	cout << str << endl;*/
}