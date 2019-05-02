#ifndef DATABASE_H
#define DATABASE_H
#include "Header.h"
#include "Column.h"
#include "Table.h"
using std::map;
using std::vector;
using std::string;
class Database
{
public:
	Database() {};
	Database(string dbname) : name(dbname) { tables.clear(); }
	void AddTable(string tablename, vector <class ColumnAttribute> attr);//建新表
	void DropTable(string tablename);//删除表
	void ShowTable();//显示包含的表
	class Table& getTable(string tablename) { return tables[tablename]; }
private:
	string name;
	map<string, class Table> tables;
};

#endif // !DATABASE_H

