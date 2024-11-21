#pragma once

class BitConverter
{
public:
	static bool TryWriteBytes(Array<byte>& destination, uint8 value, uint32 startIndex);
	static bool TryWriteBytes(Array<byte>& destination, uint16 value, uint32 startIndex);
	static bool TryWriteBytes(Array<byte>& destination, uint32 value, uint32 startIndex);
	static bool TryWriteBytes(Array<byte>& destination, uint64 value, uint32 startIndex);
	static bool TryWriteBytes(Array<byte>& destination, int8 value, uint32 startIndex);
	static bool TryWriteBytes(Array<byte>& destination, int16 value, uint32 startIndex);
	static bool TryWriteBytes(Array<byte>& destination, int32 value, uint32 startIndex);
	static bool TryWriteBytes(Array<byte>& destination, int64 value, uint32 startIndex);
	static uint8 ToUInt8(Array<byte>& arr, int32 startIndex);
	static uint16 ToUInt16(Array<byte>& arr, int32 startIndex);
	static uint32 ToUInt32(Array<byte>& arr, int32 startIndex);
	static uint64 ToUInt64(Array<byte>& arr, int32 startIndex);
	static int8 ToInt8(Array<byte>& arr, int32 startIndex);
	static int16 ToInt16(Array<byte>& arr, int32 startIndex);
	static int32 ToInt32(Array<byte>& arr, int32 startIndex);
	static int64 ToInt64(Array<byte>& arr, int32 startIndex);
};