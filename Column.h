#ifndef COLUMN_H
#define COLUMN_H
#include "Header.h"
using std::vector;
using std::string;
enum class ColumnType//储存的数据类型
{
	Int, Char, Double
};

class ColumnAttribute//列的属性
{
public:
	ColumnAttribute() {};
	ColumnAttribute(string _name, ColumnType _type, bool _not_null, bool _primary = false)
		: name(_name), type(_type), not_null(_not_null), primary(_primary) {};

	string name;//名称
	ColumnType type;//类型
	bool not_null;//非空
	bool primary;//主键
};

class Column
{
public:
	Column() {};
	Column(ColumnAttribute att) :attr(att) {};
	inline string &getName() { return attr.name; }//列名称
	inline const ColumnType &getType() { return attr.type; }//数据类型
	inline bool isPrimary() { return attr.primary; }//是否为主键
	inline bool isNot_Null() { return attr.not_null; }//是否可以为空
	void Add(string str);//添加一条记录
	void Delete(int rownum);//按照行号删去一条记录
	void Update(int rownum, string value);//按照行号更新一条记录
	vector<string> &getData() { return data; }
private:
	ColumnAttribute attr;//属性
	vector <string> data;//数据
};


#endif // !COLUMN_H
