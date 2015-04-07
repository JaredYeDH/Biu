#include "Record_Manager.h"
//Create����
bool Record_Manager::Record_ManagerCreateTable(string &tablename, const vector<Attributes>&Table_Column)
{
	string FileName = tablename + ".table";
	string Empty_Block(Block_Size, 0);						//����һ���µĿ�
	int Tuple_size = 0;										//���ݱ���Ԫ����Ҫ������ڴ��С
	for (size_t i = 0; i < Table_Column.size(); i++)			//Table_Column.size()���ݱ�����Ը���
		Tuple_size += Table_Column[i].Attributes_Length;
	Tuple_size++;
	int Offset = 0;//ƫ����
	while ((Offset + Tuple_size) < Block_Size)
	{
		Empty_Block.replace(Offset + Tuple_size - 1, 1, "1");//�滻
		Offset += Tuple_size;
	}
	Buffer_Manager::Instance().Buffer_ManagerWrite(FileName,Empty_Block);			//ͨ������������д���ļ�
	return true;
}
//Insert into �����¼
bool  Record_Manager::Record_ManagerInsert_Into(Table &table, vector<Tuple> Tuple_Lists)
{
	vector<pair<Tuple, int>> insert_tuple;
	int i, j, k;
	string filename = table.Table_Name + ".table";
	char Dirty = '0';
	int Lengh = table.Table_Length() + 1;
	string InsertContent = "";						//�û�����ļ�¼�ַ���
	string str;
	int blocknum = Buffer_Manager::Instance().Buffer_ManagerReadLastNumber(table.Table_Name, str);
	for (size_t i = 0; i < Tuple_Lists.size(); i++)
	{
		Tuple Insert_Tuple = Tuple_Lists[i];
		for (size_t j = 0; j < Insert_Tuple.Tuple_content.size(); i++)
		{
			if (table.Table_AttributesList[i].Attributes_Unique()== true)
			{
				if (Record_ManagerHasExisted(table, Insert_Tuple.Tuple_content[i], i, blocknum) == true)
					return false;
				Insert_Tuple.Tuple_content[i].resize(table.Table_AttributesList[i].Attributes_Length, 0);
				InsertContent += Insert_Tuple.Tuple_content[i];
			}
		}
		InsertContent += Dirty;
		for (k = 0; k <= blocknum; k++) //�����ִ���еĿռ�
		{
			string Strout;
			Buffer_Manager::Instance().Buffer_ManagerRead(filename, i, Strout);	//��ȡ���е�����
			int Offset = Record_ManagerFindDirtyTuple(Strout, Lengh);			//�ҵ����п���ռ��ƫ����
			if (Offset != -1)
			{
				Strout.replace(Offset, Lengh, InsertContent);
				Buffer_Manager::Instance().Buffer_ManagerWrite(filename, Strout, i);
				insert_tuple.push_back(make_pair(Insert_Tuple, i*Block_Size + Offset));
			}
			else
			{
				continue;//���Offset=-1,˵���ÿ��в������Ѿ�ɾ���ļ�¼����������һ�����
			}
		}
		if (i > blocknum) //�������Ŀ�����
		{
			InsertContent.resize(Block_Size, 0);
			Buffer_Manager::Instance().Buffer_ManagerWrite(filename, InsertContent);
			insert_tuple.push_back(make_pair(Insert_Tuple,blocknum*Block_Size));
		}
	}
	API::Instance().FixIndex(table, insert_tuple);				//�޸�����
	return true;
	
}
//Ԫ���Ƿ��Ѿ�����
bool Record_Manager::Record_ManagerHasExisted(Table &table, string &content, int num, int blocknum)
{

}
//
int Record_Manager::Record_ManagerFindDirtyTuple(string &strout, int size)
{
	int Offset = 0;
	while ((Offset + size) < strout.size())
	{
		string str = strout.substr(Offset, size);	
		string Dirty = str.substr(size - 1, 1);		//size-1 ��ȫ���滻Ϊ1
		if (Dirty == "1")							//����ҵ����޸ĵ�λ��				
			return Offset;							//����ƫ����
		Offset += size;
	}
	return -1;
}