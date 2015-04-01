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
	static Record_Manager & Instance();//ʵ����
	bool CreateTable(string &tablename, const vector<Column_Type>& Table_Column);//Create����
	bool Insert_Into(Table_Type &table,Record R);//Insert into �����¼
	void PrintHead();//��ӡ��¼��ͷ��
	void Print();//��ӡ��¼
	void PrintEnd();//��ӡ��¼��β��
	bool HasExisted(Table_Type &table, string content, int num, int BlockNum);
	void WriteRecord2Block(Byte *Positon, Record R);////��һ����¼��˳�򿽱���p��λ��
	Record Copy_block_to_record(Byte* Position, Table_Type table);
};
//ʵ����
inline Record_Manager & Record_Manager::Instance()
{
	static Record_Manager Record_Instance;
	return Record_Instance;
}
#endif