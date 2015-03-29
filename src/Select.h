#pragma once
#ifndef _SELECT_H
#define _SELECT_H
#include "Catalog.h"
#include "Intepretor.h"
#include <vector>
#include <string>
//Select �е�where�ṹ��
struct WhereList
{
	string Attribute;//ѡ�������
	string Where_Operator;//�������
	union
	{
		const char *StrValue;
		int IntValue;
	};
};
//Select ����Ľӿ�
class Selection
{
public:

	static vector<string> SelLists;//��ѡ�������
	static vector<string> TableLists;//select ��from�����ݱ��б�
	static vector<WhereList> WhereLists;//slect ��where����
	static void SelectionInput(string *Sel, string *table, WhereList *& wherelist, int wherenum);//��ȡ�û���������Ժ����ݱ��where����
	void Selection_Parse();//�����û������Select����
	void Print_SelectHead();//��ӡSelect������ͷ
	vector<Record> Mem_Record;
	vector<Column_Type> Mem_SelectColumn;//����ѡ����Ԫ��
};
#endif