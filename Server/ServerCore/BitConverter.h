#pragma once

class BitConverter
{
public:
	static bool TryWriteBytes(vector<byte>& destination, uint8 value, uint32 startIndex);
	static bool TryWriteBytes(vector<byte>& destination, uint16 value, uint32 startIndex);
	static bool TryWriteBytes(vector<byte>& destination, uint32 value, uint32 startIndex);
	static bool TryWriteBytes(vector<byte>& destination, uint64 value, uint32 startIndex);
	static bool TryWriteBytes(vector<byte>& destination, int8 value, uint32 startIndex);
	static bool TryWriteBytes(vector<byte>& destination, int16 value, uint32 startIndex);
	static bool TryWriteBytes(vector<byte>& destination, int32 value, uint32 startIndex);
	static bool TryWriteBytes(vector<byte>& destination, int64 value, uint32 startIndex);
	static uint8 ToUInt8(vector<byte>& arr, int32 startIndex = 0);
	static uint16 ToUInt16(vector<byte>& arr, int32 startIndex = 0);
	static uint32 ToUInt32(vector<byte>& arr, int32 startIndex = 0);
	static uint64 ToUInt64(vector<byte>& arr, int32 startIndex = 0);
	static int8 ToInt8(vector<byte>& arr, int32 startIndex = 0);
	static int16 ToInt16(vector<byte>& arr, int32 startIndex = 0);
	static int32 ToInt32(vector<byte>& arr, int32 startIndex = 0);
	static int64 ToInt64(vector<byte>& arr, int32 startIndex = 0);
};