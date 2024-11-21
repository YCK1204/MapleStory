#include "pch.h"
#include "RecvBuffer.h"

RecvBuffer::~RecvBuffer()
{
}

RecvBuffer::RecvBuffer(uint16 size) : Array<byte>(size)
{
}

uint16 RecvBuffer::GetReadSize()
{
	return _writePos - _readPos;
}

uint16 RecvBuffer::GetWriteSize()
{
	return Size() - _writePos;
}

byte* RecvBuffer::GetReadSegment()
{
	return &this->Array<byte>::operator[](_readPos);
}

byte* RecvBuffer::GetWriteSegment()
{
	return &this->Array<byte>::operator[](_writePos);
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
			this->Array<byte>::operator[](i) = this->Array<byte>::operator[](i + _readPos);
		_readPos = 0;
		_writePos = readSize;
	}
}

void RecvBuffer::Init()
{
	_readPos = 0;
	_writePos = 0;
	uint32 size = Size();
	for (uint32 i = 0; i < size; i++)
		this->Array<byte>::operator[](i) = (byte)0;
}
