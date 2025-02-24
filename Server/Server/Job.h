#pragma once

#include "pch.h"
#include <functional>

class IJob : public enable_shared_from_this<IJob>
{
public:
	virtual void Execute() = 0;
	virtual ~IJob() {}
};

class Job : public IJob {
private:
	function<void()> _func;
public:
	Job(function<void()> func) { _func = func; }
	virtual void Execute() { _func(); }
};

template <typename _Ty1>
class Job1 : public IJob {
private:
	_Ty1 _t1;
	function<void(_Ty1)> _func;
public:
	Job(function<void(_Ty1)> func, _Ty1 t1) : _func(func), _t1(t1) {  }
	virtual void Execute() { _func(_t1); }
};

template <typename _Ty1, typename _Ty2>
class Job2 : public IJob {
private:
	_Ty1 _t1;
	_Ty2 _t2;
	function<void(_Ty1, _Ty2)> _func;
public:
	Job(function<void(_Ty1, _Ty2)> func, _Ty1 t1, _Ty2 t2) : _func(func), _t1(t1), _t2(t2) {}
	virtual void Execute() { _func(_t1, _t2); }
};

template <typename _Ty1, typename _Ty2, typename _Ty3>
class Job3 : public IJob {
private:
	_Ty1 _t1;
	_Ty2 _t2;
	_Ty3 _t3;
	function<void(_Ty1, _Ty2, _Ty3)> _func;
public:
	Job(function<void(_Ty1, _Ty2)> func, _Ty1 t1, _Ty2 t2, _Ty3 t3) : _func(func), _t1(t1), _t2(t2), _t3(t3) {}
	virtual void Execute() { _func(_t1, _t2, _t3); }
};