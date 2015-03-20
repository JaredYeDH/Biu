#ifndef _CATALOG_H
#define _CATALOG_H
#include "Glob_Var.h"
#include "Intepretor.h"
#include <string>
#include <vector>
#include <map>
using namespace std;

/*
	ģʽ��Ϣ�������ݱ����Ϣ
*/
//�ֶ�
class Column_Type
{
public:
	int ID;
	char Flag;//��־λ
	string Column_TypeName;//�ֶ���
	char coltype;//�ֶ�����,0 ��ʾ int��1 ��ʾ char(n)��2 ��ʾ float
	bool IsPrimary;//�ǲ�������
	bool IsNotNull;//�Ƿ����Ϊ��
	bool IsUnique;//�Ƿ�ΪUnique
	int RequestSize;//�û�����ĳ���
	int StoredLength;//ʵ�ʴ洢����
	union Column_ptr
	{
		Column_Type * Next;//��һ���ֶ�
	};
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
	unsigned long indexFlags; // ��ÿһλ��0 ��ʾ�ü���������1 ��ʾ�ü�������
	unsigned short firstKey; // key Ŀ¼�ļ��У��ñ��һ������Ϣ�ı��
	short firstIndex; // index Ŀ¼�ļ��У��ñ��һ��������Ϣ�ı��
	union Table_ptr
	{
		Column_Type * Key;//ָ��������ָ��
	};
	Column_Type GetColumn(string name);//ͨ���ֶε����ֻ���ֶ�
	void InsertColumn(Column_Type column);//���ݱ��������

};
//���ݿ��е����ݱ�ʵ����
class Catalog
{
public :
	// ��־λ���
	static const char CATALOG_SPACE_USED = 0x80;
	static const char CATALOG_HAS_PRIMARY_KEY = 0x40;
	static const char CATALOG_HAS_INDEX = 0x20;
	static const char CATALOG_IS_PRIMARY_KEY = 0x10;
	static const char CATALOG_IS_UNIQUE = 0x08;
	static const char CATALOG_IS_NOT_NULL = 0x04;
	static const char CATALOG_IS_INDEX = 0x02;
	static const char CATALOG_HAS_NEXT = 0x01;
	void CatalogCreateTable(string & Tablename,const vector<Column_Type> & Attributes);
	vector<Table_Type> TableCatalog;
	static map<string, Table_Type> Mem_Table;
	static Table_Type & Get_Table(string tablename);
	static Column_Type & Get_Column(string columnname);
	void SaveTable2File();
};
#endif