#pragma once

#include "pch.h"
#include <functional>

class IJob : public enable_shared_from_this<IJob>
{
public:
	uint64 CreatedTime = ::GetTickCount64();
	uint64 Tick;
public:
	bool CanExecute() const { return CreatedTime + Tick < ::GetTickCount64(); }
	virtual void Execute() = 0;
	virtual ~IJob() {}
	bool operator > (const IJob& job) const { return (CreatedTime + Tick) > (job.Tick + job.CreatedTime); }
};

class Job : public IJob {
private:
	function<void()> _func;

public:
	Job(function<void()> func, uint64 tick = 0) { _func = func; Tick = tick; }
	virtual void Execute() { _func(); }
};

template <typename _Ty1>
class Job1 : public IJob {
private:
	_Ty1 _t1;
	function<void(_Ty1)> _func;
public:
	Job1(function<void(_Ty1)> func, _Ty1 t1, uint64 tick = 0) : _func(func), _t1(t1) { Tick = tick; }
	virtual void Execute() { _func(_t1); }
};

template <typename _Ty1, typename _Ty2>
class Job2 : public IJob {
private:
	_Ty1 _t1;
	_Ty2 _t2;
	function<void(_Ty1, _Ty2)> _func;
public:
	Job2(function<void(_Ty1, _Ty2)> func, _Ty1 t1, _Ty2 t2, uint64 tick = 0) : _func(func), _t1(t1), _t2(t2) { Tick = tick; }
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
	Job3(function<void(_Ty1, _Ty2, _Ty3)> func, _Ty1 t1, _Ty2 t2, _Ty3 t3, uint64 tick = 0) : _func(func), _t1(t1), _t2(t2), _t3(t3) { Tick = tick; }
	virtual void Execute() { _func(_t1, _t2, _t3); }
};

template <typename _Ty1, typename _Ty2, typename _Ty3, typename _Ty4>
class Job4 : public IJob {
private:
	_Ty1 _t1;
	_Ty2 _t2;
	_Ty3 _t3;
	_Ty4 _t4;
	function<void(_Ty1, _Ty2, _Ty3, _Ty4)> _func;
public:
	Job4(function<void(_Ty1, _Ty2, _Ty3, _Ty4)> func, _Ty1 t1, _Ty2 t2, _Ty3 t3, _Ty4 t4, uint64 tick = 0) : _func(func), _t1(t1), _t2(t2), _t3(t3), _t4(t4) { Tick = tick; }
	virtual void Execute() { _func(_t1, _t2, _t3, _t4); }
};

template <typename _Ty1, typename _Ty2, typename _Ty3, typename _Ty4, typename _Ty5>
class Job5 : public IJob {
private:
	_Ty1 _t1;
	_Ty2 _t2;
	_Ty3 _t3;
	_Ty4 _t4;
	_Ty5 _t5;
	function<void(_Ty1, _Ty2, _Ty3, _Ty4, _Ty5)> _func;
public:
	Job5(function<void(_Ty1, _Ty2, _Ty3, _Ty4, _Ty5)> func, _Ty1 t1, _Ty2 t2, _Ty3 t3, _Ty4 t4, _Ty5 t5, uint64 tick = 0) : _func(func), _t1(t1), _t2(t2), _t3(t3), _t4(t4), _t5(t5) { Tick = tick; }
	virtual void Execute() { _func(_t1, _t2, _t3, _t4, _t5); }
};