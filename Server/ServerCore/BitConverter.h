#pragma once

namespace Utils
{
	class BitConverter
	{
	public:
		static bool TryWriteBytes(byte* destination, uint8 value, uint32 startIndex);
		static bool TryWriteBytes(byte* destination, uint16 value, uint32 startIndex);
		static bool TryWriteBytes(byte* destination, uint32 value, uint32 startIndex);
		static bool TryWriteBytes(byte* destination, uint64 value, uint32 startIndex);
		static bool TryWriteBytes(byte* destination, int8 value, uint32 startIndex);
		static bool TryWriteBytes(byte* destination, int16 value, uint32 startIndex);
		static bool TryWriteBytes(byte* destination, int32 value, uint32 startIndex);
		static bool TryWriteBytes(byte* destination, int64 value, uint32 startIndex);
		static uint8 ToUInt8(byte* arr, int32 startIndex = 0);
		static uint16 ToUInt16(byte* arr, int32 startIndex = 0);
		static uint32 ToUInt32(byte* arr, int32 startIndex = 0);
		static uint64 ToUInt64(byte* arr, int32 startIndex = 0);
		static int8 ToInt8(byte* arr, int32 startIndex = 0);
		static int16 ToInt16(byte* arr, int32 startIndex = 0);
		static int32 ToInt32(byte* arr, int32 startIndex = 0);
		static int64 ToInt64(byte* arr, int32 startIndex = 0);
	};
}
