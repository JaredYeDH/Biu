#include "Record_Manager.h"
//Create����
bool Record_Manager::CreateTable(string &tablename, const vector<Column_Type>&Table_Column)
{
	string FileName = tablename + ".table";
	string Empty_Block(Block_Size, 0);//����һ���µĿ�
	int tuple_size = 0;//���ݱ���Ԫ����Ҫ������ڴ��С
	for (int i = 0; i < Table_Column.size(); i++)//Table_Column.size()���ݱ�����Ը���
		tuple_size += Table_Column[i].RequestSize;
	tuple_size++;
	int Offset = 0;//ƫ����
	while ((Offset + tuple_size) < Block_Size)
	{
		Empty_Block.replace(Offset + tuple_size - 1, 1, "1");//�滻
		Offset += tuple_size;
	}
	Buffer_Manager::Instance().Write(FileName,Empty_Block);
	return true;
}
//Insert into �����¼
bool Record_Manager::Insert_Into(Table_Type &table,Record R)
{
	string FileName = table.Table_Name + ".table";
	char Dirty = 0;
	int Size = Catalog::Instance().Table_Size(table.Table_Name) + 1;
	string Inserted = "";
	string str;
	int BlockNum = Buffer_Manager::Instance().ReadLast(table.Table_Name, str);
	for (int i = 0; i < R.Mem_Element.size(); i++)
	{
		if (table.Table_Column[i].IsUnique)
		{
			if ()
		}
	}
}
void Record_Manager::PrintHead()
{

}