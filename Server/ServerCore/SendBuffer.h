#pragma once
#include "pch.h"

class SendBuffer : public enable_shared_from_this<SendBuffer>
{
public:
	SendBuffer(byte* buffer, uint32 size);
	shared_ptr<SendBuffer> GetSharedPtr();
	uint32 Size();
	operator byte* ();

private:
	vector<byte> _buffer;
};