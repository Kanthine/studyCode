#ifndef _SQLiteUtil_H_
#define _SQLiteUtil_H_

using namespace std;
#include "SQLiteUtil/sqlite3.h"

using namespace std;
#include <string>
#include <vector>
class  SQLiteUtil
{
public:
	SQLiteUtil();
	~SQLiteUtil(){}

	//打开openSQLiteDB
	static bool open();
	static void close();

	static int doSqlite3_exec(std::string SQLSring);

	static int _sql_callback(void * notused, int argc, char ** argv, char ** szColName);
	static std::vector<std::string>* query(std::string sql);
public:
	static char * pErrMsg;
	static sqlite3* pDB;
	//打开的编号
	static int nRes;
	//查询结果储存的向量
	static std::vector<std::string>* resultVector;
};

#endif
