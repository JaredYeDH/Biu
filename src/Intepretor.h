#pragma once
#ifndef _INTEPRETOR_H_
#define _INTEPRETOR_H
#include "Insert_Into.h"
#include "Attributes.h"
#include "Tuple.h"
#include "Catalog.h"
#include "Glob_Var.h"
#include "Record.h"
#include "Error.h"
#include "API.h"
#include "Select.h"
#include <vector>
#include <string>
#include <queue>
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
	/*
		����
	*/
	bool Is_CreateTable(vector<string> Input);			//�Ƿ�ΪCreateTable����
	bool Is_CreateIndex(vector<string> Input);			//�Ƿ�ΪCreateIndex����
	bool Is_Select(vector<string> Input);				//�Ƿ�ΪSelect����
	bool Is_Insert(vector<string> Input);				//�Ƿ�ΪInsert����
	bool Is_Quit(vector<string> Input);					//�Ƿ�ΪQuit����
	void CreateTable_command(vector<string >Input);		//CreateTable�������
	void CreateIndex_command(vector<string> Input);		//CreateIndex�������
	void Select_command(vector<string> Input);			//Select�������
	void Insert_command(vector<string> Input);			//Insert�������
	void Quit_command(vector<string> Input);			//Quit�������
	vector<string> Input;//�û����������ļ���
	/*
		����ת��
	*/
	Attributes_Type Trasn2Attributestype(string type);//��stringת��Ϊ�ֶ�����
	static int String2Int(string s);//��string ת��Ϊint
	static const char * String2Char(string s);//��string ת��Ϊ char *
	float String2Float(string s);//��string ת��Ϊfloat
	//�������ѡ��
	static enum Operator_type  Op_Judge(string Op);
};

#endif 