#pragma once

#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include <functional>
#include <memory>

class DbContext
{
private:
	class QueryArgs;

	typedef shared_ptr<SQLHSTMT> STMT_REF;
	typedef shared_ptr<HANDLE> HANDLE_REF;
	typedef shared_ptr<QueryArgs> QUERY_REF;

private:
	class QueryArgs {
	private:
		STMT_REF _stmt;
		HANDLE_REF _hEvent;
		function<void()> _callback = nullptr;
	public:
		QueryArgs() = delete;
		QueryArgs(STMT_REF& stmt, HANDLE_REF& hEvent, function<void()> callback = nullptr) : _stmt(stmt), _hEvent(hEvent), _callback(callback) {}
		virtual ~QueryArgs() {}
		const HANDLE GetHandle() const { return *_hEvent.get(); }
		const SQLHSTMT GetStmt() const { return *_stmt.get(); }
		const function<void()> GetCallBack() const { return _callback; }
	};

private:
	USE_LOCK;
	SQLHENV		_henv = nullptr;
	SQLHDBC		_hdbc = nullptr;
	vector<QUERY_REF> _querys;

private:
	void WaitForEvents();

public:
	DbContext(string dbName, string dbPassword);
	~DbContext();
	void RequestAsync(wstring& req, function<void()> callback = nullptr);
	void RequestAsync(wstring& cmd, wstring& table, wstring condition, wstring order, function<void()> callback = nullptr);
};