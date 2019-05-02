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
	void Statement(); //����sql�������

private:
	void FormatSQL();
	void SqlCreate();//����
	void SqlDrop();//ɾ�����ݿ⡢��
	void SqlDelete();//ɾ��ĳ��������
	void SqlSelect();//ɸѡ
	void SqlInsert();//����
	void SqlUpdate();//����
	void SqlShow();//��ʾ
	void SqlUse();//ʹ��

	friend vector<string> split(const string & str, const string & sep);
	friend inline void toUpper(string & str);
	friend inline void toUpper(string & input, string & output);
	friend inline string getFirstSubstr(string & str, const char& sep);

	string buffer;//��ǰ��sql���
};
#endif // !SQLPARSE_H
