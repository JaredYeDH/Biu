#include "API.h"
using namespace std;
API::API()
{
}
Selection API::Select(string table_name, vector<Comparison>)
{
	Table_Type table_select = Catalog::Instance().Get_Table(table_name);
}
//����
void API::CreateTable(string & tablename,vector<Column_Type> &Attributes)
{
	try
	{
		Catalog::Instance().CatalogCreateTable(tablename, Attributes);//ֱ�Ӵ���ģʽ��Ϣ�ഴ����
	}
	catch (string errsrt)
	{
		throw;
	}
	Record_Manager::Instance().CreateTable(tablename, Attributes);//�������̹���ϵͳ������
}