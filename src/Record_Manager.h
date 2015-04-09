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
	��ÿһ��".table"�ļ��У����һ�ű����Ϣ��֧�ֶ����ļ�¼
	һ��".table"�ļ����Էָ�����ɸ��飬ÿһ����4KB����0��ʼ���
	��֧�ֿ��洢��ÿ����ĩβ�������һ����¼�Ŀռ䱻����������һ���¿鿪ʼ���
*/
class Record_Manager
{
public:
	static Record_Manager & Instance();																	//ʵ����
	bool Record_ManagerCreateTable(string &tablename, const vector<Attributes>& Table_Column);			//Create����
	bool Record_ManagerInsert_Into(Table &table, vector<Tuple> Tuple_Lists);							//insert into �����¼
	bool Record_ManagerHasExisted(Table &table, string &content, int num, int BlockNum);				//Ԫ���¼�Ƿ����
	int Record_ManagerFindDirtyTuple(string &strout, int size);											//���������ƫ����
	vector<Tuple> Record_ManagerSelectTuple(Table & table, int blocknum);								//ѡ��Ԫ��
	vector<string> Record_ManagerString2Tuple(vector<Attributes> attributes, string tuple_str);			//����¼ת��Ϊvector<string> ��ʽ
	vector<Tuple> Record_Manager2Tuple(vector<string> tuple_str,Table &table);							//�ٽ�vector<string>��ʽ�ļ�¼ת��Ϊtuple
};
//ʵ����
inline Record_Manager & Record_Manager::Instance()
{
	static Record_Manager Record_Instance;
	return Record_Instance;
}
#endif