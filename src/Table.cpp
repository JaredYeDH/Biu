#include "Table.h"
//�������ݱ�ģʽ��Ϣ���� 
size_t Table::Table_Length()
{
	size_t Length = 0;
	for (auto i = Table_AttributesList.begin(); i != Table_AttributesList.end(); i++)
		Length += i->Attributes_Length;
	return Length;
}
//�õ����ݱ��е�ָ������
Attributes Table::GetAttributes(string & attribtuesname)
{
	for (auto i = Table_AttributesList.begin(); i != Table_AttributesList.end(); i++)
	{
		if (i->Attributes_Name == attribtuesname)
			return *i;
	}
}