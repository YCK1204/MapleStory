#pragma once
#include "pch.h"

class SendBuffer : public Array<byte>, public enable_shared_from_this<SendBuffer>
{
public:
	SendBuffer(byte* buffer, uint32 size);
	shared_ptr<SendBuffer> GetSharedPtr();

	operator byte* () const {
		return ::Array<byte>::operator byte*();
	}
};