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
	Table();														//Ĭ�Ϲ��캯��
	Table(string tablename);										
	char Flag;														//���ݱ��־λ
	char Table_Attributesnum;										//���ݱ������Ե�����
	string Table_Name;												//���ݱ������
	vector<Attributes> Table_Attributes;							//���ݱ������Լ���
	Attributes & Get_Attributes(string & attributes_name);			//���ز��ҵ����ݱ�����
	int Get_RecordSize();											//�������Table��һ����¼���ֽڴ�С
	const vector<Attributes> Get_Attributes_List();				    //�������Ե��б�
	short Table_FirstAttributes;									//��һ����¼��������
	char Table_PrimaryAttributes;									//���ݱ��е�����
	unsigned long IndexFlags;										// ��ÿһλ��0 ��ʾ�ü���������1 ��ʾ�ü�������
	short FirstIndex;												// index Ŀ¼�ļ��У��ñ��һ��������Ϣ�ı��

};
#endif