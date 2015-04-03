#include "Record_Manager.h"
//Create����
bool Record_Manager::Record_ManagerCreateTable(string &tablename, const vector<Attributes>&Table_Column)
{
	string FileName = tablename + ".table";
	string Empty_Block(Block_Size, 0);						//����һ���µĿ�
	int Tuple_size = 0;										//���ݱ���Ԫ����Ҫ������ڴ��С
	for (int i = 0; i < Table_Column.size(); i++)			//Table_Column.size()���ݱ�����Ը���
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
bool Record_Manager::Record_ManagerInsert_Into(Table &table, vector<Tuple> Tuple_Lists)
{
	string filename = table.Table_Name + ".table";
	char Dirty = '0';
	int Lenght = table.Table_Length + 1;
	string InsertContent = "";
	string str;
	int blocknum = Buffer_Manager::Instance().Buffer_ManagerReadLastNumber(table.Table_Name, str);
	for (int i = 0; i < Tuple_Lists.size();i++)
}
//�Ѽ�¼д�����
void Record_Manager::WriteRecord2Block(Byte * Position,Record R)
{
	for (vector<Element>::iterator I = R.Mem_Element.begin(); i != R.Mem_Element.end(); i++)
	{
		switch (I->Mem_ColType)
		{
		case Char:
			memcpy(Position, I->Mem_Char.c_str(), I->Mem_CharNum);
			Position += I->Mem_CharNum;
			break;
		case Float:
			memcpy(Position, &I->Mem_Float, sizeof(float));
			Position += sizeof(float);
			break;
		case Int:
			memcpy(Position, &I->Mem_Int, sizeof(int));
			Position += sizeof(int);
			break;
		default:
			break;
		}
	}
}
//���ؿ��еļ�¼
Record Copy_block_to_record(Byte* Position, Table_Type table)
{
	vector<Column_Type> Attributes = table.Table_Column;
	Record R;
	int Target_Int;
	float Target_Float;
	char Target_Char[255];
	for (vector<Column_Type>::iterator i = Attributes.begin(); i != Attributes.end(); i++)
	{
		Element e;
		Target_Int = *((int*)p);
		Target_Float = *((float*)p);
		memset(Target_Char, 0, 255);
		//�ֶ�����,0 ��ʾ int��1 ��ʾ char(n)��2 ��ʾ float
		switch (i->coltype)
		{
		case 0:
			e.Mem_ColType =Int;
			e.Mem_Int = Target_Int;
			Position += 4;
			break;
		case 1:
			memcpy(Target_Char, Position, i->M_Char_Num);
			e.Mem_ColType = Float;
			Position += 4;
			break;
		case 2:
			e.Mem_ColType = Float;
			e.Mem_Float = Target_Float;
			Position += 4;
			break;
		default:
			break;
		}
		R.Mem_Element.push_back(e);
	}
	return R;

}
void Record_Manager::PrintHead()
{

}