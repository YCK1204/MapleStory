#include "pch.h"
//#include "pch.h"
//#include "ClientSession.h"
//
//void ClientSession::OnConnect()
//{
//	Init();
//	cout << "connected!" << endl;
//	RegisterRecv();
//}
//
//void ClientSession::OnDisconnect()
//{
//	GPoolManager->Push<ClientSession>(this);
//}
//
//void ClientSession::OnSend()
//{
//}
//
//void ClientSession::OnRecvPacket(byte* data)
//{
//	Array<byte> arr = Array<byte>(data);
//	uint16 packetSize = BitConverter::ToUInt16(arr, 0);
//	cout << packetSize << endl;
//}