#include "MiniSQL.h"
#include "API.h"
#include "Buffer.h"
#include "Catalog.h"
#include <string>
#include  <iostream>
using namespace std;
int main()
{
	Intepretor MiniSQLIntepretor;
	cout << "----------Welcome to MiniSQL----------";
	cout << "|                                    |";
	cout << "|                                    |";
	cout << "|                                    |";
	cout << "|			by:Leviathan1995		  |";
	cout << "--------------------------------------";
	MiniSQLIntepretor.CommandInput();//�����û����������
	MiniSQLIntepretor.ParseCommand();//�����������
	return 0;
}