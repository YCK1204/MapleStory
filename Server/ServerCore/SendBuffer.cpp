#include "SendBuffer.h"

SendBuffer::SendBuffer(byte* buffer, uint32 size)
{
	_size = size;
	_buffer = buffer;
}

SendBuffer::~SendBuffer()
{
	delete[] _buffer;
}

shared_ptr<SendBuffer> SendBuffer::GetSharedPtr()
{
	return shared_from_this();
}

uint32 SendBuffer::Size()
{
	return _size;
}

SendBuffer::operator byte* ()
{
	return _buffer;
}