#pragma once
#ifndef _SELECT_H
#define _SELECT_H
#include "Catalog.h"
#include "Intepretor.h"
#include <vector>
#include <string>
//Select �е�where�ṹ��
struct SelectRecord			//Select��ļ�¼
{
	string AttributesName;		//������
	vector<string> selectrecords;//����ֵ
};
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

	static queue<string> SelLists;																//��ѡ�������
	static queue<string> TableLists;															//select ��from�����ݱ��б�
	static queue<WhereList> WhereLists;															//slect ��where����
	static vector<SelectRecord> SelectRecords;													//����Select���Ԫ��
	static vector<SelectRecord> & Select_Parse(queue<string> attributes,queue<string> tableLists,queue<WhereList> whereLists);//�����û������Select ���
};
#endif