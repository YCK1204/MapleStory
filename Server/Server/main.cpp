//#include "pch.h"
//#include "ThreadManager.h"
//#include "Listener.h"
//#include "Session.h"
//#include "SocketUtils.h"
//#include "ClientSession.h"
//#include "PoolManager.h"
//
//class A {
//public:
//	A() {}
//	~A() { cout << "�Ҹ���!\n"; }
//};
//
//vector<A*> Temp() {
//	vector<A*> a;
//	A bb;
//	a.push_back(&bb);
//
//	vector<A*> b;
//	b.push_back(a[0]);
//	return b;
//}
//
//int main()
//{
//	//Listener listener;
//	//sockaddr_in addr;
//	//::memset(&addr, 0, sizeof(addr));
//	//addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
//	//addr.sin_port = htons(8080);
//	//addr.sin_family = AF_INET;
//	//GPoolManager->CreatePool<ClientSession>(500);
//	//bool ret = listener.StartAccept(addr, []() { return reinterpret_cast<PacketSession*>(GPoolManager->Pop<ClientSession>()); });
//	//for (int i = 0; i < 2; i++)
//	//{
//	//	GThreadManager->Launch([]() { GIocpCore->Dispatch(); });
//	//}
//	//while (true)
//	//{
//	//	this_thread::yield();
//	//}
//	auto a =Temp();
//}

#include <iostream>
#include "pch.h"
#include <cstddef>
#include <memory>
using namespace std;

template <typename T>
class Array {
protected:
    T* _buffer;

public:
    Array() : _buffer(nullptr) {}
    Array(T* buffer) : _buffer(buffer) {}

    // T* ��ȯ
    virtual operator T* () const {
        return _buffer;
    }


};

class SendBuffer : public Array<byte>, public enable_shared_from_this<SendBuffer> {
public:
    SendBuffer(byte* buffer) : Array<byte>(buffer) {}

    // GetSharedPtr���� shared_ptr ��ȯ
    shared_ptr<SendBuffer> GetSharedPtr() {
        return shared_ptr<SendBuffer>(this);  // 'this'�� shared_ptr�� ��ȯ
    }

    // byte* ��ȯ�� ���� operator �������̵�
    template <typename T = byte>
    operator byte* () const {
        return Array<byte>::operator T * ();  // �θ��� operator byte* ȣ��
    }
};

int main() {
    // SendBuffer�� shared_ptr�� ����
    auto buffer = make_shared<SendBuffer>((byte*)nullptr);

    // GetSharedPtr�� ȣ���� shared_ptr ���
    auto sharedPtr = buffer->GetSharedPtr();


    return 0;
}
