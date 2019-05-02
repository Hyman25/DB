#include "Header.h"
#include "SQLparse.h"
int main()
{
	std::string str;
	while (std::getline(std::cin, str, ';'))
	{
		SqlParse sql(str);
		sql.Statement();
	}
	return 0;
}