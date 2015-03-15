#ifndef _INTEPRETOR_H_
#define _INTEPRETOR_H
#include <string>
#include "Catalog.h"
#include "Glob_Var.h"
#include "Record.h"
#include <vector>
#include <string>
using namespace std;
/*
	�����û������SQL��䣬�������û������SQL��估��ʽ
	��������ȷ�ԡ�ͬʱ�����ϸ�ʽҪ������ת��Ϊ�ڲ���ʽ����
	������ʹ�á����Բ����ϸ�ʽҪ�����䣬���������Ϣ������
	���ο���
*/

struct WhereList//Select �е�where
{
	string Attribute;
	string Where_Operator;
	union
	{
		const char *StrValue;
		int IntValue;
	};
};
class Intepretor
{
public:
	void CommandInput();//��������
	void ParseCommand();//��������
	bool Is_CreateTable(vector<string> Input);//�Ƿ�ΪCreateTable����
	bool Is_Select(vector<string> Input);//�Ƿ�ΪSelect����
	bool Is_Quit(vector<string> Input);//��Quit����
	void CreateTable_command(vector<string >Input);//CreateTable�������
	void Select_command(vector<string> Input);//Select�������
	vector<string> Input;//�û����������
	ColType Trasn2type(string type);//��stringת��Ϊ����
	int String2Int(string s);//��string ת��Ϊint
	const char * String2Char(string s);//��string ת��Ϊ char *
	
};

//Select ����Ľӿ�
class Selection
{
	friend class Intepretor;
public:
	vector<string> SelLists;//��ѡ�������
	vector<string> TableLists;//select ��from���б�
	vector<WhereList> WhereLists;//slect ��where����
	Selection(string *Sel,string *table,WhereList *& wherelist,int wherenum);//��ȡ�û���������Ժ����ݱ��where����
	void Print_SelectHead();//��ӡSelect������ͷ
	vector<Record> Mem_Record;
	vector<Column_Type> Mem_SelectColumn;//����ѡ����Ԫ��
	Table_Type Mem_Table;
};
#endif 