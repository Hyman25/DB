#ifndef COLUMN_H
#define COLUMN_H
#include "Header.h"
using std::vector;
using std::string;
enum class ColumnType//�������������
{
	Int, Char, Double
};

class ColumnAttribute//�е�����
{
public:
	ColumnAttribute() {};
	ColumnAttribute(string _name, ColumnType _type, bool _not_null, bool _primary = false)
		: name(_name), type(_type), not_null(_not_null), primary(_primary) {};

	string name;//����
	ColumnType type;//����
	bool not_null;//�ǿ�
	bool primary;//����
};

class Column
{
public:
	Column() {};
	Column(ColumnAttribute att) :attr(att) {};
	inline string &getName() { return attr.name; }//������
	inline const ColumnType &getType() { return attr.type; }//��������
	inline bool isPrimary() { return attr.primary; }//�Ƿ�Ϊ����
	inline bool isNot_Null() { return attr.not_null; }//�Ƿ����Ϊ��
	void Add(string str);//���һ����¼
	void Delete(int rownum);//�����к�ɾȥһ����¼
	void Update(int rownum, string value);//�����кŸ���һ����¼
	vector<string> &getData() { return data; }
private:
	ColumnAttribute attr;//����
	vector <string> data;//����
};


#endif // !COLUMN_H
