#include "MiniSQL.h"
#include "API.h"
#include "Buffer.h"
#include "Catalog.h"
#include <string>
#include  <iostream>
using namespace std;
int main()
{
	API MiniSQLApi;
	Catalog CatalogManagent;//ģʽ��Ϣ����Ķ���
	MiniSQLApi.MiniSQLIntepretor.CommandInput();//�����û����������
	MiniSQLApi.MiniSQLIntepretor.ParseCommand();//�����������
	return 0;
}