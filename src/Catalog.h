#pragma once
#ifndef _CATALOG_H
#define _CATALOG_H
#include "Glob_Var.h"
#include "Error.h"
#include <string>
#include <vector>
#include <map>
using namespace std;

/*
	ģʽ��Ϣ�������ݱ����Ϣ
*/
/*
	һ����˵ size_t ��һ���޷����������ͣ���ȷ�е���Ϣ�� size_t �� sizeof() ���ص����͡�
	���ڶ���� unsigned int ���� unsigned long ���Ǳ��ʲô���޹ؽ�Ҫ�ˡ�
	ʹ�� size_t ��������ڴ��뼶��Ŀ���ֲ��
*/
//�ֶ�
struct Column_Type
{
	int ID;
	char Flag;//��־λ
	string Column_TypeName;//�ֶ���
	char coltype;//�ֶ�����,0 ��ʾ int��1 ��ʾ char(n)��2 ��ʾ float
	bool IsPrimary;//�ǲ�������
	bool IsNotNull;//�Ƿ����Ϊ��
	bool IsUnique;//�Ƿ�ΪUnique
	int RequestSize;//�û�����ĳ���
	int StoredLength;//ʵ�ʴ洢����
	Column_Type * Next;//��һ���ֶ�
	short NextKey;//�ñ���һ��������Ϣ����û����Ϊ-1
};
//���ݱ�
class Table_Type
{
public:
	Table_Type();//�������ݱ�
	char Flag;//��־λ
	char PrimaryKey;//���>32��ʾ������
	char NumberKeys;//<=32
	vector<Column_Type> Table_Column;//���ݱ��е�Ԫ���ŵĵط�
	string Table_Name;//����
	int ColumnNum;//���ݱ���е�������Ŀ
	int RecordSize;//��¼�ĳ���
	int PrimaryNum;//�����������ĸ���
	unsigned long IndexFlags; // ��ÿһλ��0 ��ʾ�ü���������1 ��ʾ�ü�������
	unsigned short FirstKey; // key Ŀ¼�ļ��У��ñ��һ������Ϣ�ı��
	short FirstIndex; // index Ŀ¼�ļ��У��ñ��һ��������Ϣ�ı��
	union Table_ptr
	{
		Column_Type * Key;//ָ��������ָ��
	};
	Column_Type GetColumn(string name);//ͨ���ֶε����ֻ���ֶ�
	void InsertColumn(Column_Type column);//���ݱ��������

};
//���ݿ��е����ݱ�ʵ����
/*
	Catalog�����ļ��Ĵ�ȡ��ͨ��Buffer �����Ƕ�����Catalog file����ά��
	�ڹ��캯���ж�ȡ��ϢȻ����뻺�棬������������Ϣд���ļ���
*/
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
	static const char CATALOG_SPACE_USED = 0x80;
	static const char CATALOG_HAS_PRIMARY_KEY = 0x40;
	static const char CATALOG_HAS_INDEX = 0x20;
	static const char CATALOG_IS_PRIMARY_KEY = 0x10;
	static const char CATALOG_IS_UNIQUE = 0x08;
	static const char CATALOG_IS_NOT_NULL = 0x04;
	static const char CATALOG_IS_INDEX = 0x02;
	static const char CATALOG_HAS_NEXT = 0x01;
	//��������
	void CatalogCreateTable(string & Tablename, vector<Column_Type> & Attributes);//�������ݱ�
	vector<Table_Type> TableCatalog;//���ݱ�Ĵ��
	vector<Column_Type> ColumnCatalog;//���ԵĴ��
	static map<string, Table_Type> Mem_Table;
	static Table_Type & Get_Table(string tablename);//�õ� ���ݱ�
	static Column_Type & Get_Column(string tablename,string columnname);//�õ� ���ݱ��е�����
	void CheckTable(string &Tablename ,vector<Column_Type> & Attributes);//���ݱ�ļ��
	//��������
	~Catalog();
	void SaveTable2File();//������д���ļ�����������������
};
#endif