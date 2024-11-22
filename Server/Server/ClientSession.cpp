#include "pch.h"
#include "ClientSession.h"

void ClientSession::OnConnect()
{
	flatbuffers::FlatBufferBuilder builder;
	auto str = builder.CreateString("test string");
	auto test = CreateTest(builder, 32, str);
	builder.Finish(test);
	auto size = builder.GetSize();
	auto* data = builder.GetBufferPointer();
	Init();
	cout << "connected!" << endl;
	RegisterRecv();
	
	//Send(reinterpret_cast<byte*>(data), size);
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
	vector<byte> arr;
	arr.assign(size, (byte)0);
	for (int32 i = 0; i < size; i++)
		arr[i] = data[i];
	uint16 packetSize = BitConverter::ToUInt16(arr);
	data += 2;
	char* str = (char*)data;
	cout << packetSize << endl;
	cout << str << endl;
}