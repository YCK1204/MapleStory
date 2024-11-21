#pragma once
#include "pch.h"

template <typename T>
class Array
{
private:
	T* _buffer;
	uint32 _size = 0;
public:
	Array() : _buffer(nullptr) {}
	Array(uint32 size, T defaultVal = (T)0) {
		_buffer = new T[size];
		_size = size;
		for (uint32 i = 0; i < size; i++)
			_buffer[i] = defaultVal;
	}
	Array(uint32 size, T* buffer) {
		_buffer = buffer;
		_size = size;
	}
	operator T* () const {
		return _buffer;
	}
	void Reserve(uint32 size) {
		_size = size;
		_buffer = new T[size];
	}
	virtual ~Array() { delete _buffer; }
	const T& operator[](uint32 pos) const {
		return _buffer.get()[pos];
	}
	T& operator=(Array<T>& src) {
		_buffer = src._buffer;
		_size = src.Size();
	}
	T& operator[](uint32 pos) {
		return _buffer[pos];
	}
	const uint32 Size() const {
		return _size;
	}

public:
	static void Copy(Array<T>& dest, uint32 destStartIndex, Array<T>& src, uint32 srcStartIndex, uint32 length) {
		for (uint32 i = 0; i < length; i++)
			dest[i + destStartIndex] = src[i + srcStartIndex];
	}
	static void Copy(Array<T>& dest, uint32 destStartIndex, Array<T>& src, uint32 length) {
		Copy(dest, destStartIndex, src, 0, length);
	}
};
