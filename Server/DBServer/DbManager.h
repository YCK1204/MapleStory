#pragma once

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include <functional>
#include <memory>

#define SYNC

class DbManager
{
public:
	friend class QueryArgs;
#define OPERATION_NUMBER 5
	class QueryArgs;
	typedef shared_ptr<SQLHSTMT> STMT_REF;
	typedef shared_ptr<HANDLE> HANDLE_REF;
	typedef shared_ptr<QueryArgs> QUERY_REF;

public:
	class QueryArgs {
	private:
		STMT_REF _stmt;
		HANDLE_REF _hEvent;
		HANDLE* _hdbc;
		function<void(QUERY_REF)> _callback;
	public:
		SQLRETURN _ret;
		QueryArgs() = delete;
		//QueryArgs(STMT_REF& stmt, HANDLE_REF& hEvent, HANDLE* hdbc, function<void(QUERY_REF)> callback = nullptr) : _stmt(stmt), _hEvent(hEvent), _hdbc(hdbc), _callback(move(callback)) {}
		QueryArgs(STMT_REF stmt, HANDLE_REF hEvent, HANDLE* hdbc, function<void(QUERY_REF)> callback);
		virtual ~QueryArgs() { GPoolManager->Push<HANDLE>(_hdbc); }
		const HANDLE& GetHandle() const { return *_hEvent.get(); }
		const SQLHSTMT& GetStmt() const { return *_stmt.get(); }
		function<void(QUERY_REF)> GetCallBack() { return _callback; }
	};

private:
	USE_LOCK;
	SQLHENV		_henv;
	vector<QUERY_REF> _querys;
	queue<function<void()>> _jobQueue;

private:
	static DbManager* _instance;
public:
	static DbManager& Instance();

private:
	DbManager();
	void WaitForEvents();
	void HandleError(SQLHDBC* dbc);

public:
	void Init(json& j);
	void RequestAsync(wstring req, function<void(QUERY_REF)> callback);
	void RequestAsync(wstring cmd, wstring table, wstring condition, wstring order, function<void(QUERY_REF)> callback);
};