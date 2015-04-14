#include "API.h"
using namespace std;
API::API()
{

}
//Create table����
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
//Create index ������
void API::CreateIndex(string & indexname, string &tablename, string & attributesname)
{
	Catalog::Instance().CatalogCheckCreateIndex(indexname,tablename,attributesname);
	Table table = Catalog::Instance().CatalogGet_Table(tablename);
	Attributes attributes = table.GetAttributes(attributesname);
	Catalog::Instance().CatalogCreateIndex(indexname, tablename, attributesname);
	size_t BlockNum = 0;
	vector<pair<string, int>> Info;
	while (true)
	{
		Info.clear();
		Info = Record_Manager::Instance().
	}
}
//Insert into �����¼
void API::Insert_Into(string & tablename, vector<Tuple> Tuple_Lists)
{
	Catalog::Instance().CatalogCheckInsertTuple(tablename, Tuple_Lists);			//�Բ���ļ�¼���м��
	Table table = Catalog::Instance().CatalogGet_Table(tablename);
	int Offset = Record_Manager::Instance().Record_ManagerInsert_Into(table, Tuple_Lists);
	if (Offset == false)	//δ����ɹ�
		throw Error(0, "API ", "Insert_Into", "Insert failed : violation of the constraint 'unique'");
}
//Select ѡ��Ԫ��
void API::Select(queue<string> attributes, queue<string>tablelists, queue<string> wherelists)
{
	Catalog::Instance().CatalogCheckSelectTuple(attributes, tablelists);		//������Catalog ���Select�Ƿ���ȷ
	vector<SelectRecord> SelectRecords;
	SelectRecords = Selection::Select_Parse(attributes, tablelists, wherelists);
}
