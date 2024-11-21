#include "SendBuffer.h"
#include "Array.h"

SendBuffer::SendBuffer(byte* buffer, uint32 size) : Array<byte>(size, buffer)
{
}

shared_ptr<SendBuffer> SendBuffer::GetSharedPtr()
{
	return shared_from_this();
}
