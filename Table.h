#ifndef TABLE_H
#define TABLE_H
#include "Header.h"
#include "Column.h"
#include "WhereClause.h"
using std::vector;
using std::string;
using std::map;
using std::set;
class Table
{
public:
	Table() {};
	Table(string tablename, vector <class ColumnAttribute> attr);//构造新表
	string &getName() { return name; }//返回表的名称
	void ShowColumn();//显示所有列
	void Delete(class WhereClause Where);//删除某些记录
	void Insert(vector<string> TargetColumn, vector<string> value);//插入记录
	void Update(vector<string> TargetColumn, vector<string> value, class WhereClause Where);//更新记录
	void Select(vector<string> TargetColumn, class WhereClause Where);//从某些列筛选记录
private:
	vector<string> columnnames;
	map<string, class Column> columns;//储存每一列的记录
	string name;//表名
	int cnt = 0;//记录的条数
};
#endif // !TABLE_H

