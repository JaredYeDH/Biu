#ifndef _INTEPRETOR_H_
#define _INTEPRETOR_H
#endif
//��ʾCreate������һ����Ϣ�Ľṹ��
struct Create_information
{
	char ColumnName[32];
};
//Create����
class SQL_Create
{
public:
	int TotalColumn;
	char Tablename[32];

};