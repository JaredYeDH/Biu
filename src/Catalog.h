#pragma once
#ifndef _CATALOG_H
#define _CATALOG_H
#include "Glob_Var.h"
#include "Error.h"
#include "Record.h"
#include "Intepretor.h"
#include "Attributes.h"
#include "Table.h"
#include "Tuple.h"
#include <string>
#include <vector>
#include <queue>
#include <map>
using namespace std;

/*
	ģʽ��Ϣ�������ݱ����Ϣ
	һ����˵ size_t ��һ���޷����������ͣ���ȷ�е���Ϣ�� size_t �� sizeof() ���ص����͡�
	���ڶ���� unsigned int ���� unsigned long ���Ǳ��ʲô���޹ؽ�Ҫ�ˡ�
	ʹ�� size_t ��������ڴ��뼶��Ŀ���ֲ��

	Catalog�����ļ��Ĵ�ȡ��ͨ��Buffer �����Ƕ�����Catalog file����ά��
	�ڹ��캯���ж�ȡ��ϢȻ����뻺�棬������������Ϣд���ļ���
*/
struct CatalogTable
{
	char CatalogTable_Flag;							 //��־λ
	string CatalogTable_Name;						 //���ݱ�����
	char CatalogTable_AttribtuesNum;				 //���Ե���Ŀ
	char CatalogTable_PrimaryAttributes;			 //�����е�����
	unsigned long CatalogTable_IndexFlag;			 // ��ÿһλ��0 ��ʾ�ü���������1 ��ʾ�ü�������
	unsigned short CatalogTable_FirstAttributesIndex;// key Ŀ¼�ļ��У��ñ��һ��������Ϣ�ı��
	short CatablogTable_FirstIndex;					 // index Ŀ¼�ļ��У��ñ��һ��������Ϣ�ı��
};
struct CatalogAttributes 
{
	char CatalogAttributes_Flag;			//��־λ
	string CatalogAttributes_Name;			//���Ե�����
	char CatalogAttributes_Type;			//�����ͣ�0 ��ʾ int��1 ��ʾ char(n)��2 ��ʾ float
	char CatalogAttributes_Length;			//������
	short CatalogAttributes_NextAttributes;	//�ñ���һ������Ϣ�ı�ţ��������� -1
	bool CatalogAttributes_Primary;			//�Ƿ�Ϊ����
	bool CatalogAttributes_Null;			//�Ƿ����Ϊ��
	bool CatalogAttributes_Unique;			//�Ƿ�ΪUnique
};
struct CatalogIndex
{
	char CatalogIndex_Flag;					//��־λ
	string CatalogIndex_Name;				//����������
	unsigned short CatalogIndex_InTable;	//��������������TableĿ¼�ļ��еı��
	char CatalogIndex_Key;					//��������������ı�ļ�
	short CatalogIndex_Next;				//�ñ���һ��������Ϣ�ı�ţ�û��Ϊ-1
};
//���ݿ��е����ݱ�ʵ����
class Catalog
{
public :
	//ʵ����
	static Catalog & Instance()
	{
		static Catalog CatalogManagent;
		return CatalogManagent;
	}
	// ��־λ���
	static const unsigned char CATALOG_SPACE_USED = 0x80;
	static const unsigned char CATALOG_HAS_PRIMARY_KEY = 0x40;
	static const unsigned char CATALOG_HAS_INDEX = 0x20;
	static const unsigned char CATALOG_IS_PRIMARY_KEY = 0x10;
	static const unsigned char CATALOG_IS_UNIQUE = 0x08;
	static const unsigned char CATALOG_IS_NOT_NULL = 0x04;
	static const unsigned char CATALOG_IS_INDEX = 0x02;
	static const unsigned char CATALOG_HAS_NEXT = 0x01;
	size_t Table_Size(string &tablename);
	//��������
	void CatalogCreateTable(string & tablename, vector<Attributes> & catalogattributes);			//�������ݱ��ģʽ��Ϣ�ļ�
	void CatalogCheckCreateTable(string &tablename, vector<Attributes> &	catalogattributes);		//Create���ݱ�ļ��
	void CatalogCheckInsertTuple(string &tablename, vector<Tuple> Tuple_Lists);						//��������Ƿ���ȷ
	void CatalogInsertTuple(string &tableneame, vector<Tuple> Tuple_Lists);							//���ݱ��������
	void CatalogCheckSelectTuple(queue<string> attributes, queue<string>tablelists);				//Select ��Ԫ��ļ��

	/*
		�ļ����
		�Ѵ��̶�ȡ��Ŀ¼�ļ�����˴���vector�����У���Ŀ¼���޸Ĳ���ֱ����Դ˴�����
	*/
	static vector<CatalogTable> TableCatalog;														//���ݱ�������
	static vector<CatalogAttributes>AttributesCatalog;												//���ԵĴ��
	/*
		��ȡ����
	*/
	static Table & CatalogGet_Table(string tablename);												//�õ� ���ݱ�
	static CatalogTable & CatalogGet_CatalogTable(string tablename);								//�õ� ���ݱ��ģʽ��Ϣ
	static CatalogAttributes & CatalogGet_Attributes(string tablename,string attributesname);		//�õ� ���ݱ��е�����
	/*
		��������
	*/
	~Catalog();
	void WriteTable2File();															//�����ݱ�д���ļ�����������������
	void WriteAttributes2File();													//����������д���ļ�����������������
};

#endif