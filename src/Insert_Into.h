#pragma once
#ifndef _INSERT_INTO_H
#define _INSERT_INTO_H
#include "Catalog.h"
#include <string>
/*
	Insert into ����ӿ�
*/
//Insert_into �е�values�Ľṹ��
struct Insert_IntoStruct
{
	Attributes_Type Valuetype;
	string CharValues;
	int  IntValues;
	float FloatValues;
};
class Insert_Into
{
public:
	vector<Insert_IntoStruct> InsertIntoValues;//Insert into values �е�values�ļ���
	void Insert_IntoParse();//insert into �������
};
#endif