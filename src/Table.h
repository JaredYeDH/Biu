#pragma once
#ifndef _TABLE_H
#define _TABLE_H
#include <string>
#include "Attributes.h"
#include <vector>
using namespace std;
class Table
{
public:
	Table(){}
	string Table_Name;								//���ݱ������
	vector<Attributes> Table_AttributesList;		//���Եļ���
	size_t Table_Length();							//���ݱ�ĳ���
	Attributes GetAttributes(string &attributesname);//�õ�����
	size_t GetAttributesBegin(string &attributesname);//�õ����������ģʽ��Ϣ��������ƫ����
	size_t GetAttributesEnd(string &attributesname);//�õ����������ģʽ��Ϣ������յ�ƫ����
};
#endif