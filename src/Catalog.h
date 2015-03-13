#ifndef _CATALOG_H
#define _CATALOG_H
#include "Glob_Var.h"
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
	vector<Column_Type> Table_Column;//���ݱ��е�����
	string Table_Name;//����
	int ColumnNum;//���ݱ���е��ֶ���Ŀ
	int RecordSize;//��¼�ĳ���
	int PrimaryNum;//�����������ĸ���
	union Table_ptr
	{
		Column_Type * Key;//ָ��������ָ��
	};
	Column_Type GetColumn(string name);//ͨ���ֶε����ֻ���ֶ�
	void InsertColumn(Column_Type column);//���ݱ��������

};
class Catalog
{
public :
	map<string, Table_Type> Mem_Table;
	Table_Type & Get_Table(string name);
};
#endif