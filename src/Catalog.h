#ifndef _CATALOG_H
#define _CATALOG_H
#include <string>
using namespace std;

/*
	ģʽ��Ϣ�������ݱ����Ϣ
*/
//�ֶ�
struct Column_Type
{
	int ID;
	string Column_TypeName;//�ֶ���
	bool IsPrimary;//�ǲ�������
	bool IsNull;//�Ƿ����Ϊ��
	int RequestSize;//�û�����ĳ���
	int StoredLength;//ʵ�ʴ洢����
	union Column_ptr
	{
		Column_Type * Next;//��һ���ֶ�
	};
};
//���ݱ�
struct Table_Type
{
	string Table_Name;//����
	int ColumnNum;//���ݱ���е��ֶ���Ŀ
	int RecordSize;//��¼�ĳ���
	int PrimaryNum;//�����������ĸ���
	union Table_ptr
	{
		Column_Type * Key;//ָ��������ָ��
	};

};
class Catalog
{
public :
	
};
#endif