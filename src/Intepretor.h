#ifndef _INTEPRETOR_H_
#define _INTEPRETOR_H
#include <string>
#include "Catalog.h"
#include "Glob_Var.h"
#include <vector>
#include <string>
using namespace std;
/*
	�����û������SQL��䣬�������û������SQL��估��ʽ
	��������ȷ�ԡ�ͬʱ�����ϸ�ʽҪ������ת��Ϊ�ڲ���ʽ����
	������ʹ�á����Բ����ϸ�ʽҪ�����䣬���������Ϣ������
	���ο���
*/
class Intepretor
{
public:
	void CommandInput();//��������
	void ParseCommand();//��������
	bool Is_CreateTable(vector<string> Input);//��CreateTable����
	bool Is_Quit(vector<string> Input);//��Quit����
	void CreateTable(vector<string >Input);//CreateTable�������
	vector<string> Input;//�û����������
};
#endif 