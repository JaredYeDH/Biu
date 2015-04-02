#include "Catalog.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>//����stdlib ofstream����
using namespace std;
//��������
Catalog::~Catalog()
{
	WriteTable2File();//�����ݱ�д���ļ�
}
//���ݱ���
void Catalog::CatalogCheckTable(string & Tablename, vector<Attributes> & Attributes)
{
	for (size_t i = 0; i < TableCatalog.size(); i++)
	{
		if ((TableCatalog[i].CatalogTable_Flag & CATALOG_SPACE_USED) && !strcmp(TableCatalog[i].CatalogTable_Name.c_str(), Tablename.c_str()))
			cout << "Table alredy Exits";
		break;
	}
	//�ж����Ե������Ƿ�Ϸ�
	if (Attributes.size()<1 || Attributes.size()>32)
		throw string("Atttibutes Error:Too few or too many ");
	for (size_t i = 0; i < Attributes.size(); i++)
	{
		//�жϼ�¼�����볤���Ƿ�Ϸ�
		if (Attributes[i].Attributes_Length<1 || Attributes[i].Attributes_Length>255)
			throw string("Attributes Error: Illegal length of the Attribute");
	}
}
//�������ݱ�
void Catalog::CatalogCreateTable(string & Tablename,vector<Attributes> & attributes)
{
	CatalogTable Tabletc;
	Tabletc.CatalogTable_Name = Tablename;						//���ݱ�����
	Tabletc.CatalogTable_Flag = CATALOG_SPACE_USED;				//�趨��־λ
	Tabletc.CatalogTable_AttribtuesNum = attributes.size();		//���Ե�����
	short NewTableIndex = -1;									//���ݱ��������
	for (int i = 0; i < TableCatalog.size(); i++)				//�����е���������Ѱ
	{
		if (!(TableCatalog[i].CatalogTable_Flag&CATALOG_SPACE_USED))	//���ݱ�Ϊ��
		{
			NewTableIndex = i;
			TableCatalog[i] = Tabletc;
			break;
		}
		if (NewTableIndex == -1)						//û�п����ݱ���ʹ��
		{
			TableCatalog.push_back(Tabletc);
			NewTableIndex = TableCatalog.size() - 1;	//�µ����ݱ��������
		}
	}
	CatalogAttributes NewTableAttributes;
	short AttributesIndex = -1, PreviousAttributesIndex = 0;
	for (int i = 0; i < attributes.size(); i++)
	{
		NewTableAttributes.CatalogAttributes_Flag = CATALOG_SPACE_USED;
		if (attributes[i].Attributes_Primary())
		{
			NewTableAttributes.CatalogAttributes_Flag|= CATALOG_IS_PRIMARY_KEY;
			NewTableAttributes.CatalogAttributes_Flag |= CATALOG_IS_UNIQUE;
			NewTableAttributes.CatalogAttributes_Flag |= CATALOG_IS_NOT_NULL;
		}
		if (attributes[i].Attributes_Unique)
			NewTableAttributes.CatalogAttributes_Flag|= CATALOG_IS_UNIQUE;
		if (attributes[i].Attributes_Null)
			NewTableAttributes.CatalogAttributes_Flag|= CATALOG_IS_NOT_NULL;
		NewTableAttributes.CatalogAttributes_Name = attributes[i].Attributes_Name;
		switch (attributes[i].Attributes_type)
		{
		case Int:
			NewTableAttributes.CatalogAttributes_Type=Int;
			NewTableAttributes.CatalogAttributes_Length = 4;
			break;
		case Char:
			NewTableAttributes.CatalogAttributes_Type = Char;
			NewTableAttributes.CatalogAttributes_Length = attributes[i].Attributes_Length;
		case Float:
			NewTableAttributes.CatalogAttributes_Type = Float;
			NewTableAttributes.CatalogAttributes_Length = 4;
		default:
			break; 
		}
		NewTableAttributes.CatalogAttributes_Next = NULL;//��һ��������Ϊ��
		AttributesIndex++;
		while (AttributesIndex < (int)AttributesCatalog.size() && (AttributesCatalog[AttributesIndex].CatalogAttributes_Flag &CATALOG_SPACE_USED))//��Ѱ����û�пյļ�¼�ռ�
				AttributesIndex++;
		if (AttributesIndex >= (int)AttributesCatalog.size())		//�����������Դ������Ĵ�С ��Ҫ���ٿռ�
			AttributesCatalog.push_back(NewTableAttributes);
		else
			AttributesCatalog[AttributesIndex] = NewTableAttributes;//����Ѽ�¼����յļ�¼�ռ�
		if (i == 0)													//�����ǰ�ǵ�һ�����ԣ��޸�TableCatalog��FirstKey
			TableCatalog[NewTableIndex].CatablogTable_FirstIndex = AttributesIndex;
		else
			AttributesCatalog[PreviousAttributesIndex].CatalogAttributes_NextKey = AttributesIndex;// �����޸Ĵ˱���һ�����Ե�NextAttributes;
		PreviousAttributesIndex = AttributesIndex;
		//�������������������޸ı�־��Primarykey
		if (NewTableAttributes.CatalogAttributes_Flag &CATALOG_HAS_PRIMARY_KEY)
		{
			TableCatalog[NewTableIndex].CatalogTable_Flag |= CATALOG_HAS_PRIMARY_KEY;
			TableCatalog[NewTableIndex].CatalogTable_PrimaryAttributes= i;
		}
	}
}
//������д���ļ�
void Catalog::WriteTable2File()
{
	ofstream Fout("catalog/table.dat", ios::binary);
	Fout.seekp(0, ios::beg);
	for (int i = 0; i < TableCatalog.size(); i++)
	{
		Fout.write(&TableCatalog[i].CatalogTable_Flag, 1);														//��д���־λ
		Fout.write(TableCatalog[i].CatalogTable_Name.c_str(), TableCatalog[i].CatalogTable_Name.length());		//��д�����ݱ�����
		Fout.write(&TableCatalog[i].CatalogTable_AttribtuesNum, 1);												//���ݱ������Ե�����		
		Fout.write(&TableCatalog[i].CatalogTable_PrimaryAttributes, 1);											//�����е�����
		Fout.write((char *)&TableCatalog[i].CatalogTable_IndexFlag, sizeof(long));								//�����ı�־λ
		Fout.write((char *)&TableCatalog[i].CatalogTable_FirstAttributes, sizeof(short));						//��һ������
		Fout.write((char *)&TableCatalog[i].CatablogTable_FirstIndex, sizeof(short));								//�ñ��һ��������Ϣ�ı��
	}
	Fout.close();
}





//��������Խ��м��
void Catalog::CatalogCheckColumn(string & tablename, Record R)
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
