#pragma once
#ifndef _RECORD_MANAGER_H
#define _RECORD_MANAGER_H
#include "Catalog.h"
#include "Buffer.h"
#include "Record.h"
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
	static Record_Manager & Instance();																	//ʵ����
	bool Record_ManagerCreateTable(string &tablename, const vector<Attributes>& Table_Column);			//Create����
	bool Record_ManagerInsert_Into(Table &table, vector<Tuple> Tuple_Lists);							//insert into �����¼
	bool Record_ManagerHasExisted(Table &table, string &content, int num, int BlockNum);				//Ԫ���¼�Ƿ����
	int Record_ManagerFindDirtyTuple(string &strout, int size);											//���������ƫ����
};
//ʵ����
inline Record_Manager & Record_Manager::Instance()
{
	static Record_Manager Record_Instance;
	return Record_Instance;
}
#endif