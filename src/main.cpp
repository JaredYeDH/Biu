#include "MiniSQL.h"
#include "API.h"
#include "Buffer.h"
#include <string>
#include  <iostream>
using namespace std;
int main()
{
	API MiniSQLApi;
	Buffer_Manager & Instance();//���������ʵ����
	MiniSQLApi.MiniSQLIntepretor.CommandInput();//�����û����������
	MiniSQLApi.MiniSQLIntepretor.ParseCommand();//�����������
	return 0;
}