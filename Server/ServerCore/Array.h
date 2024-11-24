#pragma once

#include "Types.h"

namespace Utils
{
	class Array
	{
	public:
		template <typename T>
		static void Copy(T* src, uint32 srcIndex, T* destination, uint32 destIndex, uint32 length) {
			for (uint32 i = 0; i < length; i++)
				destination[i + destIndex] = src[i + srcIndex];
		}
		template <typename T>
		static void Copy(T* src, uint32 srcIndex, T* destination, uint32 length) {
			Copy(src, srcIndex, destination, 0, length);
		}
		template <typename T>
		static void Copy(T* src, T* destination, uint32 destIndex, uint32 length) {
			Copy(src, 0, destination, destIndex, length);
		}
		template <typename T>
		static void Copy(T* src, T* destination, uint32 length) {
			Copy(src, 0, destination, 0, length);
		}
	};
}
