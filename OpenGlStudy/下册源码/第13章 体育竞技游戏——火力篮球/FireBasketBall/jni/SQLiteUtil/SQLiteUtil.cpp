#include "SQLiteUtil.h"

#include <vector>

char * SQLiteUtil::pErrMsg = 0;

sqlite3* SQLiteUtil::pDB = 0;

int SQLiteUtil::nRes = 0;
//查询结果储存的向量
std::vector<std::string>* SQLiteUtil::resultVector = new std::vector<std::string>();
SQLiteUtil::SQLiteUtil()
{

}
//打开openSQLiteDB
bool SQLiteUtil::open()
{
	//打开路径采用utf-8编码
	//如果路径中包含中文，需要进行编码转换
	const char* const dbname = "/data/data/com.bn.bullet7111/macydb";
	//打开的错误编号
	nRes = sqlite3_open(dbname, &pDB);

	if(nRes != SQLITE_OK )
	{
		sqlite3_free(pErrMsg);
		//关闭数据库
		sqlite3_close(pDB);
		return false;
	}
	return true;
}
void SQLiteUtil::close()
{
	sqlite3_close(pDB);
}
int SQLiteUtil::doSqlite3_exec(std::string SQLSring)
{
	int result = 0;
	//打开数据库
	open();
	//执行SQL语句
	result = sqlite3_exec(pDB, SQLSring.c_str(), 0, 0, &pErrMsg );
	//关闭数据库
	close();
	return result;
}
std::vector<std::string>* SQLiteUtil::query(std::string sql)
{
	//清空记录
	resultVector->clear();
	int result = 0;
	//打开数据库
	open();
	//查询数据
	result = sqlite3_exec(pDB, sql.c_str(), _sql_callback, 0, &pErrMsg);

	//关闭数据库
	close();
	return resultVector;
}
int SQLiteUtil::_sql_callback(void * notused, int argc, char** argv, char** szColName)
{
	for (int i=0; i < argc; i++ )
	{
		std::string temp = argv[i];
		//记录结果
		resultVector->push_back(std::vector<std::string>::value_type(temp));
	}
	return 0;
}
