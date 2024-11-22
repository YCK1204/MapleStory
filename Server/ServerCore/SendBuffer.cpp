#include "SendBuffer.h"

SendBuffer::SendBuffer(byte* buffer, uint32 size)
{
	_buffer.assign(size, (byte)0);
	
	for (uint32 i = 0; i < size; i++)
		_buffer[i] = buffer[i];
}

shared_ptr<SendBuffer> SendBuffer::GetSharedPtr()
{
	return shared_from_this();
}

uint32 SendBuffer::Size()
{
	return _buffer.size();
}

SendBuffer::operator byte* ()
{
	return _buffer.data();
}