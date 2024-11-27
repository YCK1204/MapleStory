#include <format>
#include "pch.h"
#include "DbContext.h"

void DbContext::WaitForEvents()
{
	while (true)
	{
		this_thread::sleep_for(0.1s);
		vector<HANDLE> handles;
		vector<QueryArgs*> querys;

		{
			READ_LOCK;
			for (auto& query : _querys)
				querys.push_back(query.get());
		}

		DWORD waitResult = WaitForMultipleObjects(
			handles.size(),
			handles.data(),
			false,
			1000
		);

		if (waitResult == WAIT_TIMEOUT)
			continue;
		if (waitResult == WAIT_FAILED)
		{
			cerr << "WaitForMultipleObjects Failed\n";

			// todo log

			continue;
		}
		QueryArgs* completedQuery = querys[waitResult - WAIT_OBJECT_0];
		RETCODE retCode;
		auto ret = SQLCompleteAsync(SQL_HANDLE_STMT, completedQuery->GetStmt(), &retCode);
		if (SQL_SUCCEEDED(ret))
		{
			if (SQL_SUCCEEDED(retCode))
			{
				auto func = completedQuery->GetCallBack();
				if (func != nullptr)
					func();
			}
			else
			{
				goto ERR;
			}
		}
		else
		{
			goto ERR;
		}

		{
			WRITE_LOCK;

			auto it = find_if(_querys.begin(), _querys.end(), [&](QUERY_REF& q) {
				return q->GetStmt() == completedQuery->GetStmt(); });
			if (it != _querys.end())
				_querys.erase(it);
		}

	ERR:
		// todo
		cerr << "에러남 레전드 상황 발생\n";
	}
}

DbContext::DbContext(string dbName, string dbPassword)
{
	auto ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_henv);
	ASSERT_CRASH(SQL_SUCCEEDED(ret));
	ret = SQLSetEnvAttr(_henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3_80, SQL_IS_INTEGER);
	ASSERT_CRASH(SQL_SUCCEEDED(ret));

	string connectionString = format("DRIVER={SQL Server};SERVER=localhost;DATABASE={0};UID=root;PWD={your_password={1};", dbName, dbPassword);
	wstring connectionStringW;
	connectionStringW.assign(connectionString.begin(), connectionString.end());
	const wchar_t* str = connectionStringW.data();

	// 환경 핸들 설정 및 sql 서버 접속
	{
		ret = SQLAllocHandle(SQL_HANDLE_DBC, _henv, &_hdbc);
		ASSERT_CRASH(SQL_SUCCEEDED(ret));
		ret = SQLSetConnectAttr(_hdbc, SQL_ATTR_ASYNC_DBC_FUNCTIONS_ENABLE, (SQLPOINTER)SQL_ASYNC_DBC_ENABLE_ON, SQL_IS_INTEGER);
		ASSERT_CRASH(SQL_SUCCEEDED(ret));
		SQLDriverConnect(_hdbc, NULL, (SQLTCHAR*)str, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);
	}
}

DbContext::~DbContext()
{
	if (_hdbc)
		SQLFreeHandle(SQL_HANDLE_STMT, _hdbc);
	if (_henv)
		SQLFreeHandle(SQL_HANDLE_ENV, _henv);
}

void DbContext::RequestAsync(wstring& req, function<void()> callback)
{
	STMT_REF stmt(new SQLHSTMT(),
		[](SQLHSTMT* stmt) {
			SQLFreeHandle(SQL_HANDLE_STMT, *stmt);
			delete stmt;
		});

	auto ret = SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, stmt.get());
	if (SQL_SUCCEEDED(ret) == false)
	{
		cerr << "Sql Query Gen Failed\n";
		return;
	}

	HANDLE_REF hEvent(new HANDLE(),
		[](HANDLE* hEvent) {
			CloseHandle(*hEvent);
			delete hEvent;
		});

	*hEvent.get() = CreateEvent(nullptr, true, false, nullptr);
	if (*hEvent.get() == nullptr)
	{
		cerr << "Gen Handle Failed\n";
		return;
	}

	ret = SQLSetStmtAttr(*stmt, SQL_ATTR_ASYNC_ENABLE, (SQLPOINTER)SQL_ASYNC_ENABLE_ON, SQL_IS_INTEGER);
	if (SQL_SUCCEEDED(ret) == false)
	{
		cerr << "SQLSetStmtAttr Failed\n";
		return;
	}
	ret = SQLSetStmtAttr(*stmt, SQL_ATTR_ASYNC_STMT_EVENT, *hEvent.get(), 0);
	if (SQL_SUCCEEDED(ret) == false)
	{
		cerr << "SQLSetStmtAttr Failed\n";
		return;
	}
	ret = SQLExecDirect(*stmt, (SQLWCHAR*)req.c_str(), SQL_NTS);
	if (ret != SQL_STILL_EXECUTING) {
		cerr << "SQLExecDirect Failed\n";
		return;
	}

	WRITE_LOCK;
	{
		_querys.push_back(make_shared<QueryArgs>(stmt, hEvent, callback));
	}
}

void DbContext::RequestAsync(wstring& cmd, wstring& table, wstring condition, wstring order, function<void()> callback)
{
	wstring req = cmd + L" " + table + L" " + condition + L" " + order + L";";
	RequestAsync(req, callback);
}
