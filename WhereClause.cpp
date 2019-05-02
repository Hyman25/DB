#include "WhereClause.h"

WhereClause::WhereClause(string Sqlexpr)
{
	stringstream ss;
	ss << Sqlexpr;
	string token;
	while (ss >> token)//第一阶段只处理and、or、<、>、=
	{
		Expression expr;
		string tmp = token;
		std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
		if (tmp != "AND" && tmp != "OR")
		{
			expr.name = token;
			ss >> token;
			if (token == "=")
				expr.op = Operator::Equal;
			else if (token == ">")
				expr.op = Operator::Great;
			else if (token == "<")
				expr.op = Operator::Less;
			ss >> token;
			expr.value = token;
		}
		else
		{
			if (tmp == "AND")
				expr.op = Operator::And;
			else if (tmp == "OR")
				expr.op = Operator::Or;
		}
		conditon.emplace_back(expr);
	}
}

bool getAnd(stack<Operator>& opr, stack<bool>& fit)
{
	bool a = fit.top(); fit.pop();
	bool b = fit.top(); fit.pop();
	if (opr.top() == Operator::And)
	{
		fit.push((a && b));
		opr.pop();
	}
	else
		return false;
	return true;
}

bool getOr(stack<Operator>& opr, stack<bool>& fit)
{
	bool a = fit.top(); fit.pop();
	bool b = fit.top(); fit.pop();
	if (opr.top() == Operator::And)
		fit.push((a && b));
	else
		fit.push((a || b));
	opr.pop();
	return true;
}

set<int> WhereClause::Satisfy(map<string, class Column> columns, int cnt)
{
	set <int> result;
	stack<Operator> opr;//储存And/Or
	stack<bool> fit;
	for (int i = 0; i < cnt; i++)
	{
		Column col;
		for (Expression x : conditon)
		{
			switch (x.op)
			{
			case Operator::And://And连接符
				while (!opr.empty())
				{
					if (!getAnd(opr, fit))
						break;
				}
				opr.push(Operator::And);
				break;
			case Operator::Or://Or连接符
				while (!opr.empty())
				{
					if (!getOr(opr, fit))
						break;
				}
				opr.push(Operator::Or);
				break;
			case Operator::Equal://等于号
				if (columns.count(x.name))
					col = columns[x.name];
				else
				{
					cout << "Don't have clomun " << x.name << " in this table" << endl;
					result.clear();
					return result;
				}
				fit.push((x.value == col.getData()[i]));
				break;
			case Operator::Great://大于号
				if (columns.count(x.name))
					col = columns[x.name];
				else
				{
					cout << "Don't have clomun " << x.name << " in this table" << endl;
					result.clear();
					return result;
				}
				if (col.getType() == ColumnType::Char)
					fit.push((x.value > col.getData()[i]));
				else if (col.getType() == ColumnType::Double)
					fit.push(stod(col.getData()[i]) > stod(x.value));
				else
					fit.push(stoi(col.getData()[i]) > stoi(x.value));
				break;
			case Operator::Less://小于号
				if (columns.count(x.name))
					col = columns[x.name];
				else
				{
					cout << "Don't have clomun " << x.name << " in this table" << endl;
					result.clear();
					return result;
				}
				if (col.getType() == ColumnType::Char)
					fit.push((x.value < col.getData()[i]));
				else if (col.getType() == ColumnType::Double)
					fit.push(stod(col.getData()[i]) < stod(x.value));
				else
					fit.push(stoi(col.getData()[i]) < stoi(x.value));
				break;
			}
		}
		while (!opr.empty())
		{
			if (opr.top() == Operator::And)
				getAnd(opr, fit);
			else
				getOr(opr, fit);
		}
		if (fit.top() == true)
			result.insert(i);
	}

	return result;
}
