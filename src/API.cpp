#include "API.h"
using namespace std;
API::API()
{

}
//Create����
void API::CreateTable(string & tablename, vector<Attributes> &Attributes)
{
	try
	{
		Catalog::Instance().CatalogCreateTable(tablename, Attributes);//ֱ�Ӵ���ģʽ��Ϣ�ഴ����
	}
	catch (string errsrt)
	{
		throw;
	}
	Record_Manager::Instance().Record_ManagerCreateTable(tablename, Attributes);//�������̹���ϵͳ������
}
//Insert into �����¼
void API::Insert_Into(string & tablename, vector<Tuple> Tuple_Lists)
{
	Catalog::Instance().CatalogCheckTuple(tablename, Tuple_Lists);			//�Բ���ļ�¼���м��
	Table table = Catalog::Instance().CatalogGet_Table(tablename);
	int Offset = Record_Manager::Instance().
}

//Slect����
void API::Select(string table_name, vector<Comparison>)
{
	Table_Type table_select = Catalog::Instance().Get_Table(table_name);
}
