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
	Table(string tablename, vector <class ColumnAttribute> attr);//�����±�
	string &getName() { return name; }//���ر������
	void ShowColumn();//��ʾ������
	void Delete(class WhereClause Where);//ɾ��ĳЩ��¼
	void Insert(vector<string> TargetColumn, vector<string> value);//�����¼
	void Update(vector<string> TargetColumn, vector<string> value, class WhereClause Where);//���¼�¼
	void Select(vector<string> TargetColumn, class WhereClause Where);//��ĳЩ��ɸѡ��¼
private:
	vector<string> columnnames;
	map<string, class Column> columns;//����ÿһ�еļ�¼
	string name;//����
	int cnt = 0;//��¼������
};
#endif // !TABLE_H

