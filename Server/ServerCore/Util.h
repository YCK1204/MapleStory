#pragma once

template <typename T1, typename T2>
void WriteBytes(T1 value, T2 bytes) {
	std::memcpy(bytes, &value, sizeof(value));
}

int bytesToInt(const unsigned char* bytes) {
	int value;
	std::memcpy(&value, bytes, sizeof(value));
	return value;
}