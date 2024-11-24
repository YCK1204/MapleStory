#include "pch.h"
#include "RecvBuffer.h"

RecvBuffer::~RecvBuffer()
{
}

RecvBuffer::RecvBuffer(uint16 size)
{
	_buffer.assign(size, (byte)0);
}

uint16 RecvBuffer::GetReadSize()
{
	return _writePos - _readPos;
}

uint16 RecvBuffer::GetWriteSize()
{
	return _buffer.size() - _writePos;
}

byte* RecvBuffer::GetReadSegment()
{
	return &_buffer[_readPos];
}

byte* RecvBuffer::GetWriteSegment()
{
	return &_buffer[_writePos];
}

RecvBuffer::operator byte* ()
{
	return _buffer.data();
}

bool RecvBuffer::OnWrite(uint16 numOfBytes)
{
	if (numOfBytes > GetWriteSize())
		return false;
	_writePos += numOfBytes;
	return true;
}

bool RecvBuffer::OnRead(uint16 numOfBytes)
{
	if (numOfBytes > GetReadSize())
		return false;
	_readPos += numOfBytes;
	return true;
}

void RecvBuffer::Clear()
{
	uint16 readSize = GetReadSize();
	if (_readPos == _writePos)
	{
		_readPos = 0;
		_writePos = 0;
	}
	else
	{
		for (uint16 i = 0; i < readSize; i++)
			_buffer[i] = _buffer[i + _readPos];
		_readPos = 0;
		_writePos = readSize;
	}
}

void RecvBuffer::Init()
{
	_readPos = 0;
	_writePos = 0;
	uint32 size = _buffer.size();
	for (uint32 i = 0; i < size; i++)
		_buffer[i] = (byte)0;
}
