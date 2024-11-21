#include "pch.h"
#include "BitConverter.h"
#include "Array.h"

template <typename T>
bool WriteBytes(T value, Array<byte>& bytes, uint32 startIndex) {
	if (bytes.Size() - startIndex < sizeof(value))
		return false;
	::memcpy(&bytes[startIndex], &value, sizeof(value));
	return true;
}

template <typename T>
T ToInt(Array<byte>& bytes, uint32 startIndex) {
	T value;
	::memcpy(&value, &bytes[startIndex], sizeof(value));
	return value;
}

bool BitConverter::TryWriteBytes(Array<byte>& destination, uint8 value, uint32 startIndex)
{
	return WriteBytes(value, destination, startIndex);
}

bool BitConverter::TryWriteBytes(Array<byte>& destination, uint16 value, uint32 startIndex)
{
	return WriteBytes(value, destination, startIndex);
}

bool BitConverter::TryWriteBytes(Array<byte>& destination, uint32 value, uint32 startIndex)
{
	return WriteBytes(value, destination, startIndex);
}

bool BitConverter::TryWriteBytes(Array<byte>& destination, uint64 value, uint32 startIndex)
{
	return WriteBytes(value, destination, startIndex);
}

bool BitConverter::TryWriteBytes(Array<byte>& destination, int8 value, uint32 startIndex)
{
	return WriteBytes(value, destination, startIndex);
}

bool BitConverter::TryWriteBytes(Array<byte>& destination, int16 value, uint32 startIndex)
{
	return WriteBytes(value, destination, startIndex);
}

bool BitConverter::TryWriteBytes(Array<byte>& destination, int32 value, uint32 startIndex)
{
	return WriteBytes(value, destination, startIndex);
}

bool BitConverter::TryWriteBytes(Array<byte>& destination, int64 value, uint32 startIndex)
{
	return WriteBytes(value, destination, startIndex);
}

uint8 BitConverter::ToUInt8(Array<byte>& arr, int32 startIndex)
{
	return ToInt<uint8>(arr, startIndex);
}

uint16 BitConverter::ToUInt16(Array<byte>& arr, int32 startIndex)
{
	return ToInt<uint16>(arr, startIndex);
}

uint32 BitConverter::ToUInt32(Array<byte>& arr, int32 startIndex)
{
	return ToInt<uint32>(arr, startIndex);
}

uint64 BitConverter::ToUInt64(Array<byte>& arr, int32 startIndex)
{
	return ToInt<uint64>(arr, startIndex);
}

int8 BitConverter::ToInt8(Array<byte>& arr, int32 startIndex)
{
	return ToInt<int8>(arr, startIndex);
}

int16 BitConverter::ToInt16(Array<byte>& arr, int32 startIndex)
{
	return ToInt<int16>(arr, startIndex);
}

int32 BitConverter::ToInt32(Array<byte>& arr, int32 startIndex)
{
	return ToInt<int32>(arr, startIndex);
}

int64 BitConverter::ToInt64(Array<byte>& arr, int32 startIndex)
{
	return ToInt<int64>(arr, startIndex);
}
