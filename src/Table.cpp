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
//�õ����������ģʽ��Ϣ��������ƫ����
size_t Table::GetAttributesBegin(string &attributesname)
{
	size_t Begin = 0;
	for (auto i = Table_AttributesList.begin(); i != Table_AttributesList.end(); i++)
	{
		if (i->Attributes_Name == attributesname)
			return Begin;
		Begin += i->Attributes_Length;
	}
	return 0;
}
//�õ����������ģʽ��Ϣ������յ�ƫ����
size_t Table::GetAttributesEnd(string &attributesname)
{
	size_t End = 0;
	for (auto i = Table_AttributesList.begin(); i != Table_AttributesList.end(); i++)
	{
		if (i->Attributes_Name == attributesname)
			return End+=i->Attributes_Length;
		End+= i->Attributes_Length;
	}
	return 0;
}