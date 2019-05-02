#include "SQLparse.h"
using std::vector;
using std::string;
using std::regex;

DataSet dataSet;

vector<string> split(const string &str, const string &sep)
{
	vector<string> tmp;
	string::size_type pos[2] = { 0, str.find(sep) };
	while (string::npos != pos[1])
	{
		tmp.push_back(str.substr(pos[0], pos[1] - pos[0]));
		pos[0] = pos[1] + sep.size();
		pos[1] = str.find(sep, pos[0]);
	}
	if (pos[0] != str.length())
		tmp.push_back(str.substr(pos[0]));

	return tmp;
}

inline string getFirstSubstr(string& str, const char& sep) {
	string::size_type pos = str.find(sep);
	if (pos != string::npos) {
		string tmp = str.substr(0, pos);
		str.erase(0, pos+1);
		return tmp;
	}
	else {
		string tmp = str;
		str.clear();
		return tmp;
	}
}

inline void toUpper(string& str) {
    for (int i = 0; i < str.size(); i ++)
        str[i] = toupper(str[i]);
}

void SqlParse::Statement()
{
	FormatSQL();

	string order = getFirstSubstr(buffer, ' ');
	toUpper(order);

	if (order == "CREATE")
		SqlCreate();
	else if (order == "DROP")
		SqlDrop();
	else if (order == "DELETE")
		SqlDrop();
	else if (order == "SELECT")
		SqlSelect();
	else if (order == "INSERT")
		SqlInsert();
	else if (order == "UPDATE")
		SqlUpdate();
	else if (order == "SHOW")
		SqlShow();
	else if (order == "USE")
		SqlUse();
}

void SqlParse::FormatSQL()
{
	/*换行,tab,",'回车用" "代替*/
	regex reg("[\r\n\t\"']");
	buffer = regex_replace(buffer, reg, " ");
	/*去除分号之后的内容*/
	reg = ";.*$";
	buffer = regex_replace(buffer, reg, "");
	/*去除最前和最后的" "*/
	reg = "(^ +)|( +$)";
	buffer = regex_replace(buffer, reg, "");
	/*去除重复的空格*/
	reg = " +";
	buffer = regex_replace(buffer, reg, " ");
	/*在 = <> < >前后添加空白符" "*/
	reg = " ?(=|(<>)|<|>) ?";
	buffer = regex_replace(buffer, reg, " $1 ");
	/*将, ()左右的空格去掉
	如果未来在Where语句的处理中需要加入括号作为运算符，
	在whereclause类内用正则表达式再次处理即可.*/
	reg = " ?(\\)|\\(|,) ?";
	buffer = regex_replace(buffer, reg, "$1");
	/*去除<> <= >= 两个符号间的空格*/
	reg = "< *>";
	buffer = regex_replace(buffer, reg, "<>");
	reg = "< *=";
	buffer = regex_replace(buffer, reg, "<=");
	reg = "> *=";
	buffer = regex_replace(buffer, reg, ">=");

}

void SqlParse::SqlDrop()
{
	vector<string> orders = split(buffer, " ");
	if (orders.size() == 2) {
		toUpper(orders[0]);
		string name = orders[1];
		if (orders[0] == "DATABASE")
			dataSet.DropDB(name);
		else if (orders[0] == "TABLE") {
			Database& database = dataSet.getDB();
			database.DropTable(name);
		}
	}
}

void SqlParse::SqlDelete()
{
	string FROM = getFirstSubstr(buffer, ' '); toUpper(FROM);
	if (FROM != "FROM") return;  //输入异常

	string tablename = getFirstSubstr(buffer, ' ');

	string WHERE = getFirstSubstr(buffer, ' '); toUpper(WHERE);
	if (WHERE != "WHERE") return;  //输入异常

	WhereClause Where(buffer);

	Table& table = dataSet.getDB().getTable(tablename);
	table.Delete(Where);
}

void SqlParse::SqlSelect()
{
	string AttrName = getFirstSubstr(buffer, ' ');
	vector<string> AttrNames = split(AttrName, ",");

	if (AttrName == "*") AttrNames.clear();

	string FROM = getFirstSubstr(buffer, ' '); toUpper(FROM);
	if (FROM != "FROM") return;  //输入异常

	string tablename = getFirstSubstr(buffer, ' ');

	if (!buffer.empty()) {//如果buffer非空，则有where语句
		string WHERE = getFirstSubstr(buffer, ' '); toUpper(WHERE);
		if (WHERE != "WHERE") return;  //输入异常
	}
	//如果没有where语句，此处会生成一个空的WhereClause
	WhereClause Where(buffer);

	Table& table = dataSet.getDB().getTable(tablename);
	table.Select(AttrNames, Where);
}

void SqlParse::SqlInsert()
{
	string INTO = getFirstSubstr(buffer, ' '); toUpper(INTO);
	if (INTO != "INTO") return;  //输入异常

	string tablename = getFirstSubstr(buffer, '(');

	vector<string> ColumnNames = split(getFirstSubstr(buffer, ')'), ",");

	string VALUES = getFirstSubstr(buffer, '('); toUpper(VALUES);
	if (VALUES != "VALUES") return; //输入异常

	vector<string> values = split(getFirstSubstr(buffer, ')'), ",");

	Table& table = dataSet.getDB().getTable(tablename);
	table.Insert(ColumnNames, values);
}

void SqlParse::SqlUpdate()
{
	string tablename = getFirstSubstr(buffer, ' ');

	string SET = getFirstSubstr(buffer, ' '); toUpper(SET);
	if (SET != "SET") return;  //输入异常

	vector<string> ColumnNames;
	vector<string> values;

	regex tmp(" ?= ?");
	buffer = regex_replace(buffer, tmp, "=");

	vector<string> UpdateAttr = split(getFirstSubstr(buffer, ' '), ",");

	for (int i = 0; i < UpdateAttr.size(); ++i) {
		vector<string> Attr_tmp = split(UpdateAttr[i], "=");
		if (Attr_tmp.size() != 2) return; //输入异常

		ColumnNames.push_back(Attr_tmp[0]);
		values.push_back(Attr_tmp[1]);
	}

	string WHERE = getFirstSubstr(buffer, ' '); toUpper(WHERE);
	if (WHERE != "WHERE") return;  //输入异常

	tmp = "=";
	buffer = regex_replace(buffer, tmp, " = ");

	WhereClause Where(buffer);

	Table& table = dataSet.getDB().getTable(tablename);
	table.Update(ColumnNames, values, Where);
}

void SqlParse::SqlShow()
{
	vector<string> orders = split(buffer, " ");
	if (orders.size() == 1) {
		toUpper(orders[0]);
		if (orders[0] == "DATABASES")
			dataSet.ShowDB();
        else if (orders[0] == "TABLES") {
			Database& database = dataSet.getDB();
			database.ShowTable();
		}
	}
	else if (orders.size() == 3) {
        toUpper(orders[0]);
        toUpper(orders[1]);
		if (orders[0] + " " + orders[1] == "COLUMNS FROM") {
			Table& table = dataSet.getDB().getTable(orders[2]);
			table.ShowColumn();
		}
	}
}

void SqlParse::SqlUse()
{
	string dbname = getFirstSubstr(buffer, ' ');
    dataSet.UseDB(dbname);
}

void SqlParse::SqlCreate()
{
	string order = getFirstSubstr(buffer, ' ');
    toUpper(order);

	if (order == "DATABASE") {
		string dbname = getFirstSubstr(buffer, ' ');
		dataSet.CreateDB(dbname);

    }
	else if (order == "TABLE") {
		string tableName = getFirstSubstr(buffer, '(');

		//将( )转换成空格
		regex reg("(\\(|\\))");
		buffer = regex_replace(buffer, reg, " ");
		//去除首尾的" "
		reg = "(^ +)|( +$)";
		buffer = regex_replace(buffer, reg, "");
		//去除,周围的" "
		reg = " ?, ?";
		buffer = regex_replace(buffer, reg, ",");

		vector<string> TableAttr = split(buffer, ",");
		vector<ColumnAttribute> attr;
		std::set<string> PrimaryKey;

		for (int i = 0; i < (int)TableAttr.size(); i++) {
			vector<string> ColumnAttr = split(TableAttr[i], " ");
			string PRIMARY = ColumnAttr[0], KEY = ColumnAttr[1];
            toUpper(PRIMARY);
            toUpper(KEY);
			if (PRIMARY == "PRIMARY" && KEY == "KEY") {
				for (int i = 2; i < (int)ColumnAttr.size(); i++)
					PrimaryKey.insert(ColumnAttr[i]);
				continue;
			}

			string& name = ColumnAttr[0], type_str = ColumnAttr[1];
			toUpper(type_str);
			ColumnType type;
			if (type_str == "INT") type = ColumnType::Int;
			else if (type_str == "DOUBLE") type = ColumnType::Double;
			else if (type_str == "CHAR") type = ColumnType::Char;
			else return;

			bool not_null = ((ColumnAttr.size() == 4) && (ColumnAttr[2] + " " + ColumnAttr[3] == "NOT NULL"));

			attr.push_back(ColumnAttribute(name, type, not_null));
		}

		for (int i = 0; i < (int)attr.size(); i++)
			if (PrimaryKey.find(attr[i].name) != PrimaryKey.end())
				attr[i].primary = true;

		Database& database = dataSet.getDB();
		database.AddTable(tableName, attr);
	}
}
