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
	string Column_TypeName;//�ֶ���
	ColType coltype;//�ֶ�����
	bool IsPrimary;//�ǲ�������
	bool IsNull;//�Ƿ����Ϊ��
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
	vector<Column_Type> Table_Column;//���ݱ��е�Ԫ���ŵĵط�
	string Table_Name;//����
	int ColumnNum;//���ݱ���е�������Ŀ
	int RecordSize;//��¼�ĳ���
	int PrimaryNum;//�����������ĸ���
	union Table_ptr
	{
		Column_Type * Key;//ָ��������ָ��
	};
	Column_Type GetColumn(string name);//ͨ���ֶε����ֻ���ֶ�
	void InsertColumn(Column_Type column);//���ݱ��������

};
class Key_Type
{
	char Flag;
	string KeyName;
	char KeyType;//�����ͣ�0 int,1 char ,2 float
	char KeyLength;//������
	short NextKey;//�ñ���һ������Ϣ�ı�ţ�����Ϊ-1
};
//���ݿ��е����ݱ�ʵ����
class Catalog
{
public :
	static const char CATALOG_SPACE_USED = 0x80;
	void CatalogCreateTable(string & Tablename,vector<Column_Type> & Attributes);
	vector<Table_Type> TableCatalog;
	static map<string, Table_Type> Mem_Table;
	static Table_Type & Get_Table(string tablename);
	static Column_Type & Get_Column(string columnname);
};
#endif