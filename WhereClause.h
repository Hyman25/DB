#ifndef WHERECLAUSE_H
#define WHERECLAUSE_H
#include "Header.h"
#include "Column.h"
using std::vector;
using std::string;
using std::set;
using std::stack;
using std::map;
enum class Operator
{
	And, Or, Less, Equal, Great
};

class Expression
{
public:
	Expression() {};
	Expression(Operator _op, string _name = "", string _value = "")
		: op(_op), name(_name), value(_value) {};
	string name;
	Operator op;
	string value;
};

class WhereClause
{
public:
	WhereClause(string Sqlexpr);
	set<int> Satisfy(map<string, class Column> columns, int cnt);//找出table里符合条件的行
private:
	vector<class Expression> conditon;//条件集
};

#endif // !WHERECLAUSE_H
