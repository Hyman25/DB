#ifndef SQLPARSE_H
#define SQLPARSE_H
#include "Header.h"
#include "Column.h"
#include "Table.h"
#include "Database.h"
#include "Dataset.h"
#include "WhereClause.h"
using std::vector;
using std::string;
class SqlParse
{
public:
	SqlParse(string str) :buffer(str) {};
	void Statement(); //解析sql语句类型

private:
	void FormatSQL();
	void SqlCreate();//创建
	void SqlDrop();//删除数据库、表
	void SqlDelete();//删除某几行数据
	void SqlSelect();//筛选
	void SqlInsert();//插入
	void SqlUpdate();//更新
	void SqlShow();//显示
	void SqlUse();//使用

	friend vector<string> split(const string & str, const string & sep);
	friend inline void toUpper(string & str);
	friend inline void toUpper(string & input, string & output);
	friend inline string getFirstSubstr(string & str, const char& sep);

	string buffer;//当前行sql语句
};
#endif // !SQLPARSE_H
