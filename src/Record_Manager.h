#ifndef _RECORD_MANAGER_H
#define _RECORD_MANAGER_H
#include "Catalog.h"
#include "Buffer.h"
#include <string>
#include <vector>
using namespace std;
/*
	ʵ�������ļ��Ĵ�����ɾ��
	��¼�Ĳ���
	��¼��ɾ��
	��¼�Ĳ���
	�����ļ���һ����������ɣ����С�뻺������С��ͬ��
	һ�������һ���������¼��
	ֻ�洢�����ļ�¼����֧�ֿ��洢
*/
class Record_Manager
{
public:
	bool CreateTable(string tablename, vector<Column_Type>& Table_Column);//����
};
#endif