#include "Catalog.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>//����stdlib ofstream����
using namespace std;
//��������
Catalog::~Catalog()
{
	WriteTable2File();//�����ݱ�д���ļ�
	WriteAttributes2File();//����������д���ļ�
}
//�����ݱ�д���ļ�
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
		Fout.write((char *)&TableCatalog[i].CatalogTable_FirstAttributesIndex, sizeof(short));					//��һ��������"attritbutes.dat"�е��±�
		Fout.write((char *)&TableCatalog[i].CatablogTable_FirstIndex, sizeof(short));							//�ñ��һ��������Ϣ��"index.dat"���
	}
	Fout.close();
}
//��������д���ļ�
void Catalog::WriteAttributes2File()
{
	ofstream Fout("catalog/attributes.dat", ios::binary);
	Fout.seekp(0, ios::beg);
	for (size_t i = 0; i < AttributesCatalog.size(); i++)
	{

		Fout.write(&AttributesCatalog[i].CatalogAttributes_Flag, 1);		//���Եı�־λ
		Fout.write(AttributesCatalog[i].CatalogAttributes_Name.c_str(), AttributesCatalog[i].CatalogAttributes_Name.length());//���Ե�����
		Fout.write(&AttributesCatalog[i].CatalogAttributes_Type, 1);		//���Ե�����
		Fout.write((char *)&AttributesCatalog[i].CatalogAttributes_Length, 1);	//���Եĳ���
		Fout.write((char *)&AttributesCatalog[i].CatalogAttributes_NextAttributes, sizeof(short));		//��һ�������ڸñ���±�
	}
}
//���ݱ���
void Catalog::CatalogCheckCreateTable(string & Tablename, vector<Attributes> & Attributes)
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
		if (attributes[i].Attributes_Unique())
			NewTableAttributes.CatalogAttributes_Flag|= CATALOG_IS_UNIQUE;
		if (attributes[i].Attributes_Null())
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
		NewTableAttributes.CatalogAttributes_NextAttributes = NULL;//��һ��������Ϊ��
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
			AttributesCatalog[PreviousAttributesIndex].CatalogAttributes_NextAttributes = AttributesIndex;// �����޸Ĵ˱���һ�����Ե�NextAttributes;
		PreviousAttributesIndex = AttributesIndex;
		//�������������������޸ı�־��Primarykey
		if (NewTableAttributes.CatalogAttributes_Flag &CATALOG_HAS_PRIMARY_KEY)
		{
			TableCatalog[NewTableIndex].CatalogTable_Flag |= CATALOG_HAS_PRIMARY_KEY;
			TableCatalog[NewTableIndex].CatalogTable_PrimaryAttributes= i;
		}
	}
}
//�õ����ݱ��ģʽ��Ϣ
CatalogTable &Catalog::CatalogGet_CatalogTable(string tablename)
{
	for (auto i = TableCatalog.begin(); i != TableCatalog.end(); i++)
	{
		if (i->CatalogTable_Name == tablename)
			return *i;
	}
	
}
//�õ����ݱ�
Table & Catalog::CatalogGet_Table(string tablename)
{
	Table * table = new Table;
	table->Table_Name = tablename;
	//���ұ�
	short FirstAttribtuesIndex = 0;
	unsigned long IndexFlag = 0;
	bool Find = false;
	for (int i = 0; i < TableCatalog.size(); i++)
	{
		if (TableCatalog[i].CatalogTable_Flag&CATALOG_SPACE_USED&&!strcmp(Intepretor::String2Char(TableCatalog[i].CatalogTable_Name), tablename.c_str()))
		{
			Find = true;
			IndexFlag = TableCatalog[i].CatalogTable_IndexFlag;							//��øñ��������־
			FirstAttribtuesIndex = TableCatalog[i].CatalogTable_FirstAttributesIndex;	//��һ�����Ե�����
		}
	}
	if (!Find)
		throw Error(0, "Interpreter", "Insert into", "�﷨����!");
	//��øñ����������
	short CurrentAttributesIndex = FirstAttribtuesIndex;
	while (CurrentAttributesIndex != -1)
	{
		//��õ������Ե���Ϣ
		Attributes attritbues;
		attritbues.Attributes_Name = AttributesCatalog[CurrentAttributesIndex].CatalogAttributes_Name;
		switch (AttributesCatalog[CurrentAttributesIndex].CatalogAttributes_Type)
		{
		case Int:
			attritbues.Attributes_type = Int;
			break;
		case Char:
			attritbues.Attributes_type = Char;
			break;
		case Float:
			attritbues.Attributes_type = Float;
			break;
		default:
			break;
		}
		attritbues.Attributes_Length = AttributesCatalog[CurrentAttributesIndex].CatalogAttributes_Length;
		attritbues.Attributes_Flag = 0;
		attritbues.Attributes_Flag |= (AttributesCatalog[CurrentAttributesIndex].CatalogAttributes_Flag&	CATALOG_IS_PRIMARY_KEY) ? 1 : 0;
		attritbues.Attributes_Flag |= (AttributesCatalog[CurrentAttributesIndex].CatalogAttributes_Flag&	CATALOG_IS_UNIQUE) ? 2 : 0;
		attritbues.Attributes_Flag |= (AttributesCatalog[CurrentAttributesIndex].CatalogAttributes_Flag& CATALOG_IS_NOT_NULL) ? 4 : 0;
		attritbues.Attributes_Flag |= (AttributesCatalog[CurrentAttributesIndex].CatalogAttributes_Flag& CATALOG_IS_INDEX) ? 8 : 0;
		table->Table_AttributesList.push_back(attritbues);
		CurrentAttributesIndex = AttributesCatalog[CurrentAttributesIndex].CatalogAttributes_NextAttributes;
	}
	return *table;
}
//�õ� ���ݱ��е����Ե��кţ�Ҳ���ǵڼ�������,���û��������Ծͷ���-1
int Catalog::Catalog_GetAttributesNumber(string & attributesname, Table & table)
{
	int Number = 0;
	for (auto i = table.Table_AttributesList.begin(); i != table.Table_AttributesList.end(); i++)
	{
		if (i->Attributes_Name == attributesname)
		{
			Number++;
			return Number;
		}
		Number++;
	}
	return -1;
}
//����ļ�¼���м��
void Catalog::CatalogCheckInsertTuple(string & tablename, vector<Tuple> Tuple_Lists)
{
	Table table = Catalog::CatalogGet_Table(tablename);
	bool TableFind;//�����¼�����ݱ��Ƿ����
	short NumberOfAttributes = 0, FirstAttributexindex = 0;
	for (int i = 0; i < TableCatalog.size(); i++)
	{
		if ((TableCatalog[i].CatalogTable_Flag&CATALOG_SPACE_USED) && !strcmp(Intepretor::String2Char(TableCatalog[i].CatalogTable_Name), tablename.c_str()))
		{
			TableFind = true;
			NumberOfAttributes = TableCatalog[i].CatalogTable_AttribtuesNum;			//���ݱ��е����Ե�����
			FirstAttributexindex = TableCatalog[i].CatalogTable_FirstAttributesIndex;	//��һ�����Ե�����
			break;
		}
	}
	if (!TableFind)
		throw Error(1001, "Catalog", "Check_Attributes", "���ݱ�����");
	short CurrentKeyindex =FirstAttributexindex;
	for (int  i = 0; i < Tuple_Lists.size(); i++)
	{
		Tuple tuple = Tuple_Lists[i];
		for (int j = 0; j < tuple.Tuple_content.size();j++)
		switch (table.Table_AttributesList[j].Attributes_type)
		{
		case Int:
			break;
		case Char:
			if (tuple.Tuple_content[j].length()>table.Table_AttributesList[j].Attributes_Length)	//�����û�����ĳ���
				throw string("ValueIllegal: The value to insert is longer than the length of the key.");
			break;
		case Float:
			break;
		default:
			break;
		}
	}
}
//��Select ��Ԫ������Խ��м��
void Catalog::CatalogCheckSelectTuple(queue<string> attributes, queue<string>tablelists)
{
	bool Find = false;
	int size = tablelists.size();
	for (int j = 0; j < size; j++)
	{
		string tablename = tablelists.front();
		for (auto i = TableCatalog.begin(); i != TableCatalog.end(); i++)
		{
			if (tablename == i->CatalogTable_Name)
			{
				break;
				Find = true;
			}
		}
		if (!Find)
			throw Error(1001, "Catalog", "Select tuple", "No such table");
	}

	Find = false;
	size = attributes.size();
	for (int j = 0; j < size; j++)
	{
		string tablename = attributes.front();
		for (auto i = AttributesCatalog.begin(); i != AttributesCatalog.end(); i++)
		{
			if (tablename == i->CatalogAttributes_Name)
			{
				break;
				Find = true;
			}
		}
		if (!Find)
			throw Error(1001, "Catalog", "Select tuple", "No such Attribtues");
	}

}
//����û�׼������������
void Catalog::CatalogCheckCreateIndex(string &indexname, string &tablename, string &attributesname)
{
	short FirstAttributesIndex = 0, FirstIndexIndex = 0;
	unsigned long IndexFlags = 0;
	bool TableFind = false, HasIndex = false;
	for (size_t i = 0; i < TableCatalog.size(); i++)
	{
		if ((TableCatalog[i].CatalogTable_Flag & CATALOG_SPACE_USED) && !strcmp(Intepretor::String2Char(TableCatalog[i].CatalogTable_Name), tablename.c_str()))
		{
			TableFind = true;
			//��øñ��������־λ
			IndexFlags = TableCatalog[i].CatalogTable_IndexFlag;
			//��øñ��һ�����ԣ�����������У���λ��
			FirstAttributesIndex = TableCatalog[i].CatalogTable_FirstAttributesIndex;
			if (TableCatalog[i].CatalogTable_Flag &CATALOG_HAS_INDEX)
			{
				HasIndex = true;
				FirstIndexIndex = TableCatalog[i].CatablogTable_FirstIndex;
			}
			break;
		}
	}
	if (!TableFind)
		throw Error(0, "Catalog", "Create index", "no such table");
	//��������Ƿ���ڣ������Ƿ���Unique
	bool AttributesFind = false;
	short CurrentAttributesIndex = FirstAttributesIndex, AttribtuesNumber = -1;
	while (CurrentAttributesIndex != -1 && !TableFind)
	{
		AttribtuesNumber++;
		if (!strcmp(Intepretor::String2Char(AttributesCatalog[CurrentAttributesIndex].CatalogAttributes_Name), attributesname.c_str()))
		{
			AttributesFind = true;
			if (!(AttributesCatalog[CurrentAttributesIndex].CatalogAttributes_Flag &CATALOG_IS_UNIQUE))
				throw Error(0, "Catalog", "Create index", "Attribtues is not unique");
			break;
		}
		CurrentAttributesIndex = AttributesCatalog[CurrentAttributesIndex].CatalogAttributes_NextAttributes;
	}
	if (!AttributesFind)
		throw Error(0, "Catalog", "Create index", "no such attribtues");
	//��������Ƿ��ͻ
	if ((IndexFlags >> AttribtuesNumber) & 1)
		throw Error(0, "Catalog", "Create index", "The index is already exist");
	//���indexname
	for (size_t i = 0; i < IndexCatalog.size(); i++)
	{
		if ((IndexCatalog[i].CatalogIndex_Flag & CATALOG_SPACE_USED) && !strcmp(Intepretor::String2Char(IndexCatalog[i].CatalogIndex_Name), indexname.c_str()))
		{
			throw Error(0, "Catalog", "Create index", "An index of the same name already exists.");
		}
	}
}
