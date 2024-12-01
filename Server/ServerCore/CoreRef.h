#pragma once

#include "pch.h"
#include "Array.h"

template <class _Ty>
class BaseRef : public enable_shared_from_this<BaseRef<_Ty>> {
private:
	vector<_Ty> _data;

public:
	virtual ~BaseRef() {}

public:
	const uint32 Size() const { return _data.size(); }

public:
	void Reserve(uint32 size) { Assign(size); }
	void Assign(uint32 size, _Ty defaultVal = (_Ty)0) { _data.assign(size, defaultVal); }
	void Copy(vector<_Ty>& target) { Copy(target.data(), target.size()); }
	void Copy(_Ty* data, uint32 size) {
		_data.clear();
		Assign(size);
		Utils::Array::Copy(data, _data.data(), size);
	}
	shared_ptr<BaseRef<_Ty>> GetPtr() { return this->shared_from_this(); }

public:
	operator const _Ty* () { return _data.data(); }
	_Ty& operator[] (const uint32 i) { return _data[i]; }
};