#pragma once
#ifndef _ATTRIBUTES_H
#define _ATTRIBUTES_H
#include <string>
#include "Glob_Var.h"
using namespace std;
/*
	���ݱ��е�����
*/
class Attributes
{
public:
	Attributes();						//Ĭ�Ϲ��캯��
	Attributes(string attributes_name, Attributes_Type attributes_type, int attributes_charnum, bool primary, bool unique);
	Attributes(string attributes_name, Attributes_Type attributes_type, bool primary, bool unique);
	~Attributes();
	char Flag;							//Ԫ��ı�־λ
	string Attributes_name;				//Ԫ�������
	Attributes_Type Attributes_type;	//Ԫ�������
	int Attributes_charnum;				//�����char ���ͣ��ַ�������
	bool Attributes_primary;			//�Ƿ�ΪPrimary
	bool Attributes_unique;				//�Ƿ�ΪUnique
	bool Attributes_null;				//�Ƿ�ΪNull
	int RequestSize;					//�û�����ĳ���
	int StoredLength;					//ʵ�ʴ洢����
	short NextAttributes;				//��һ�����Ե�������
};
#endif