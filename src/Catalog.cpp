#include "Catalog.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>//����stdlib ofstream����
using namespace std;
//��������
Catalog::~Catalog()
{
	SaveTable2File();//�����ݱ�д���ļ�
}
//�������ݱ�
void Catalog::CatalogCreateTable(string & Tablename,vector<Column_Type> & Attributes)
{
	Table_Type Tabletc;
	Tabletc.Table_Name = Tablename;//���ݱ�����
	Tabletc.Flag = CATALOG_SPACE_USED;
	Tabletc.ColumnNum = Attributes.size();
	short NewTableIndex = -1;//���ݱ�ı�ǩ������
	for (size_t i = 0; i < TableCatalog.size(); i++)
	{
		if (!(TableCatalog[i].Flag&CATALOG_SPACE_USED))
		{
			NewTableIndex = i;
			TableCatalog[i] = Tabletc;
			break;
		}
		if (NewTableIndex == -1)
		{
			TableCatalog.push_back(Tabletc);
			NewTableIndex = TableCatalog.size() - 1;
		}
	}
	Column_Type Kc;
	short KeyIndex = -1, PreviousKeyIndex = 0;
	for (size_t i = 0; i < Attributes.size(); i++)
	{
		Kc.Flag = CATALOG_SPACE_USED;
		if (Attributes[i].IsPrimary)
		{
			Kc.Flag |= CATALOG_IS_PRIMARY_KEY;
			Kc.Flag |= CATALOG_IS_UNIQUE;
			Kc.Flag |= CATALOG_IS_NOT_NULL;
		}
		if (Attributes[i].IsUnique)
			Kc.Flag |= CATALOG_IS_UNIQUE;
		if (Attributes[i].IsNotNull)
			Kc.Flag |= CATALOG_IS_NOT_NULL;
		Kc.Column_TypeName = Attributes[i].Column_TypeName;
		switch (Attributes[i].coltype)
		{
		case Int:
			Kc.coltype = 0;
			Kc.StoredLength = 4;
			break;
		case Char:
			Kc.coltype = 1;
			Kc.StoredLength = Attributes[i].RequestSize;
		case Float:
			Kc.coltype = 2;
			Kc.StoredLength = 4;
		default://Ĭ�Ͽ����ǳ���Ϊ255���ַ���
			Kc.coltype = 1;
			Kc.StoredLength = 255;
			break; 
		}
		Kc.Next = NULL;//��һ����������Ϊ��
		KeyIndex++;
		while (KeyIndex < (int)ColumnCatalog.size() && (ColumnCatalog[KeyIndex].Flag &CATALOG_SPACE_USED))
			KeyIndex++;
		if (KeyIndex >= (int)ColumnCatalog.size())
			//�����������Դ������Ĵ�С ��Ҫ���ٿռ�
			ColumnCatalog.push_back(Kc);
		else
			ColumnCatalog[KeyIndex] = Kc;
		if (i == 0)
			//�����ǰ�ǵ�һ�����ԣ��޸�TableCatalog��FirstKey
			TableCatalog[NewTableIndex].FirstColumn = KeyIndex;
		else
			ColumnCatalog[PreviousKeyIndex].NextColumn = KeyIndex;
		PreviousKeyIndex = KeyIndex;
		//�������������������޸ı�־��Primarykey
		if (Kc.Flag &CATALOG_HAS_PRIMARY_KEY)
		{
			TableCatalog[NewTableIndex].Flag |= CATALOG_HAS_PRIMARY_KEY;
			TableCatalog[NewTableIndex].PrimaryColumn = i;
		}
	}
}
//���ݱ���
void Catalog::CheckTable(string & Tablename, vector<Column_Type> & Attributes)
{
	for (size_t i = 0; i < TableCatalog.size(); i++)
	{
		if ((TableCatalog[i].Flag & CATALOG_SPACE_USED) && !strcmp(TableCatalog[i].Table_Name.c_str(), Tablename.c_str()))
			cout << "Table alredy Exits";
		break;
	}
	//�ж����Ե������Ƿ�Ϸ�
	if (Attributes.size()<1 || Attributes.size()>32)
		throw string("Atttibutes Error:Too few or too many ");
	for (size_t i = 0; i < Attributes.size(); i++)
	{
		//�жϼ�¼�ĳ����Ƿ�Ϸ�
		if (Attributes[i].StoredLength<1 || Attributes[i].StoredLength>255)
			throw string("Attributes Error: Illegal length of the Attribute");
	}
}
//��������Խ��м��
void Catalog::CheckColumn(string & tablename, Record R)
{
	bool TableFind;//�����¼�����ݱ��Ƿ����
	short NumberOfkeys = 0, FirstKeyindex = 0;
	for (size_t i = 0; i < TableCatalog.size(); i++)
	{
		if ((TableCatalog[i].Flag&CATALOG_SPACE_USED) && !strcmp(Intepretor::String2Char(TableCatalog[i].Table_Name), tablename.c_str()))
		{
			TableFind = true;
			NumberOfkeys = TableCatalog[i].NumberColumns;//���ݱ��е����Ե�����
			FirstKeyindex = TableCatalog[i].FirstIndex;
			break;
		}
	}
	if (!TableFind)
		throw Error(1001, "Catalog", "Check_Column", "���ݱ�����");
	if (R.Mem_Element.size() > NumberOfkeys)//�����Ԫ�������������ݱ����ѹ涨��
		throw string("Insert into ValueIllegal: Illegal number of columns");
	short CurrentKeyindex = FirstKeyindex;
	for (size_t i = 0; i < R.Mem_Element.size(); i++)
	{
		switch (ColumnCatalog[i].coltype)
		{
		case 0:
			break;
		case 1:
			if ((int)R.Mem_Element[i].Mem_CharNum>ColumnCatalog[CurrentKeyindex].RequestSize)
				throw string("ValueIllegal: The value to insert is longer than the length of the key.");
			break;
		case 2:
			break;
		default:
			break;
		}
		CurrentKeyindex = ColumnCatalog[CurrentKeyindex].NextColumn;
	}
}
//����Ԫ��
void Catalog::CatalogInsertColumn(string tablename,Record R)
{
	
}
//�õ����ݱ�
Table_Type & Catalog::Get_Table(string table_name)
{
	if (Mem_Table.find(table_name) == Mem_Table.end())
		throw Error(1001, "Catalog", "Insert Column", "�﷨����!");
	return Mem_Table[table_name];
}
//�õ�����
Column_Type & Catalog::Get_Column(string tablename,string columnname)
{
	Table_Type TableInstance;
	TableInstance=Catalog::Get_Table(columnname);
	for (auto i = TableInstance.Table_Column.begin(); i != TableInstance.Table_Column.end(); i++)
	{
		if (i->Column_TypeName == columnname)
			return *i;
	}
}
//��ȡ�ֶεĳ���
size_t Catalog::Table_Size(string &tablename)
{
	size_t length = 0;
	Table_Type t = Get_Table(tablename);
	for (auto Len = t.Table_Column.begin(); Len != t.Table_Column.end(); Len++)
	{
		length += Len->StoredLength;
	}
	return length;
}
//������д���ļ�
void Catalog::SaveTable2File()
{
	ofstream Fout("catalog/table.dat", ios::binary);
	Fout.seekp(0, ios::beg);
	for (size_t i = 0; i < TableCatalog.size(); i++)
	{
		Fout.write(&TableCatalog[i].Flag, 1);
		Fout.write(TableCatalog[i].Table_Name.c_str(), TableCatalog[i].Table_Name.length());
		Fout.write(&TableCatalog[i].NumberColumns, 1);
		Fout.write(&TableCatalog[i].PrimaryColumn, 1);
		Fout.write((char *)&TableCatalog[i].IndexFlags, sizeof(long));
		Fout.write((char *)&TableCatalog[i].FirstColumn, sizeof(short));
		Fout.write((char *)&TableCatalog[i].FirstIndex, sizeof(short));
	}
	Fout.close();
}