#include "Record_Manager.h"
//����
bool Record_Manager::CreateTable(string tablename, vector<Column_Type>&Table_Column)
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
void Record_Manager::PrintHead()
{

}