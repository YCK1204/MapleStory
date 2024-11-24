#include "pch.h"
#include "PacketHandler.h"

void PacketHandler::C_TestHandler(PacketSession* session, ByteRef& buffer) {
	FlatBufferBuilder b;

	auto c_test = GetRoot<C_Test>(reinterpret_cast<uint8*>(buffer.get()));

	cout << "str : " << c_test->str()->c_str() << ", test : " << c_test->test() << '\n';
}
