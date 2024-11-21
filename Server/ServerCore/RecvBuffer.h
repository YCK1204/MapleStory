#pragma once
#include "Array.h"

class RecvBuffer : public Array<byte>
{
public:
	RecvBuffer() = delete;
	virtual ~RecvBuffer();
	RecvBuffer(uint16 size);
	uint16 GetReadSize();
	uint16 GetWriteSize();
	byte* GetReadSegment();
	byte* GetWriteSegment();

public:
	bool OnWrite(uint16 numOfBytes);
	bool OnRead(uint16 numOfBytes);
	void Clear();
	void Init();

private:
	uint16 _readPos = 0;
	uint16 _writePos = 0;
};