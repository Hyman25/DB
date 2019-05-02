#ifndef DATASET_H
#define DATASET_H
#include "Header.h"
#include "Column.h"
#include "Table.h"
#include "Database.h"
using std::map;
using std::vector;
using std::string;
class DataSet
{
public:
	DataSet() {};
	DataSet(const DataSet&&) = delete;
	void DropDB(string DBname);//删除数据库
	void ShowDB();//显示数据库
	void UseDB(string dbname);//使用数据库
	void CreateDB(string dbname);
	class Database& getDB() { return databases[used_db]; }

private:
	map<string, class Database> databases;//所有的数据库
	string used_db;//标记正在使用的数据库
};

#endif // !DATASET_H
