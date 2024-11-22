#pragma once

class RecvBuffer
{
public:
	RecvBuffer() = delete;
	virtual ~RecvBuffer();
	RecvBuffer(uint16 size);
	uint16 GetReadSize();
	uint16 GetWriteSize();
	byte* GetReadSegment();
	byte* GetWriteSegment();
	operator vector<byte>&();

public:
	bool OnWrite(uint16 numOfBytes);
	bool OnRead(uint16 numOfBytes);
	void Clear();
	void Init();

private:
	vector<byte> _buffer;
	uint16 _readPos = 0;
	uint16 _writePos = 0;
};