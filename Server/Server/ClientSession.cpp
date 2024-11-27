#include "pch.h"
#include "ClientSession.h"
#include "Manager.h"

void ClientSession::OnConnect()
{
	

	Init();
	cout << "connected!" << endl;
	RegisterRecv();
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
	Manager::Packet.OnRecvPacket(this, data);
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