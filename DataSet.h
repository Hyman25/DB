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
	void DropDB(string DBname);//ɾ�����ݿ�
	void ShowDB();//��ʾ���ݿ�
	void UseDB(string dbname);//ʹ�����ݿ�
	void CreateDB(string dbname);
	class Database& getDB() { return databases[used_db]; }

private:
	map<string, class Database> databases;//���е����ݿ�
	string used_db;//�������ʹ�õ����ݿ�
};

#endif // !DATASET_H
