#include "API.h"
using namespace std;
API::API()
{
}
//Slect����
void API::Select(string table_name, vector<Comparison>)
{
	Table_Type table_select = Catalog::Instance().Get_Table(table_name);
}
//Create����
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
//insert into �����¼
void API::Insert_Into(string &tablename, Record R)
{
	Catalog::Instance().CatalogInsertColumn(tablename, R);
	Table_Type table = Catalog::Instance().Get_Table(tablename);
	/*
	Unique�ļ��
	for (auto i = table.Table_Column.begin(); i != table.Table_Column.end(); i++)
	{
		if (i->IsUnique)
		{
			int ColumnIndex=table
		}
	}
	*/
	int Offset = Record_Manager::Instance().Insert_Into(table, R);
	if (Offset=-1)
		throw string("Insert failed: violation of the constraint 'unique'.");
	/*
		��������
	*/
}