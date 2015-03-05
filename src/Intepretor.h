#ifndef _INTEPRETOR_H_
#define _INTEPRETOR_H
#include <string>
#include "Catalog.h"
#include "Glob_Var.h"
#endif
//��ʾCreate������Ϣ�Ľṹ��
struct Create_information
{
	char ColumnName[32];	//�ֶ���.������Ҫ�ö�����䳤���ֽ����б�ʾ���������ֶΡ�
	Byte_type Coltype;		//�ֶ�����
	int length;				//�ֶγ���
	Operator_type OperType; //��ϵ�����
	bool Isnull;			//��û�в�ΪNULL��Ҫ��
	bool Isprimary;			//�ǲ���Primary key
	SQL_Create *next;		//��һ���ֶ�
};
//Create����Ŀ�ܽṹ
class SQL_Create
{
public:
	int TotalColumn;
	Create_information *Head;//�ֶ�ͷ
	char Tablename[32];
	SQL_Create(int, Create_information, char *);//���캯��������ʼ�������������е���Ϣ���ϳ�һ�ű�
	~SQL_Create()
	{
		delete Head;
	}
};
