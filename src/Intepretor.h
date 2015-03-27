#ifndef _INTEPRETOR_H_
#define _INTEPRETOR_H
#include <string>
#include "Catalog.h"
#include "Glob_Var.h"
#include "Record.h"
#include "Error.h"
#include "API.h"
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
	/*
		����
	*/
	bool Is_CreateTable(vector<string> Input);			//�Ƿ�ΪCreateTable����
	bool Is_Select(vector<string> Input);				//�Ƿ�ΪSelect����
	bool Is_Insert(vector<string> Input);				//�Ƿ�ΪInsert����
	bool Is_Quit(vector<string> Input);					//�Ƿ�ΪQuit����
	void CreateTable_command(vector<string >Input);		//CreateTable�������
	void Select_command(vector<string> Input);			//Select�������
	void Insert_command(vector<string> Input);			//Insert�������
	vector<string> Input;//�û����������ļ���
	/*
		����ת��
	*/
	ColType Trasn2type(string type);//��stringת��Ϊ����
	int String2Int(string s);//��string ת��Ϊint
	const char * String2Char(string s);//��string ת��Ϊ char *
	
};
//Select �е�where�ṹ��
struct WhereList
{
	string Attribute;
	string Where_Operator;
	union
	{
		const char *StrValue;
		int IntValue;
	};
};
//Select ����Ľӿ�
class Selection
{
	friend class Intepretor;
public:

	static vector<string> SelLists;//��ѡ�������
	static vector<string> TableLists;//select ��from�����ݱ��б�
	static vector<WhereList> WhereLists;//slect ��where����
	static void SelectionInput(string *Sel,string *table,WhereList *& wherelist,int wherenum);//��ȡ�û���������Ժ����ݱ��where����
	void Selection_Parse();//�����û������Select����
	void Print_SelectHead();//��ӡSelect������ͷ
	vector<Record> Mem_Record;
	vector<Column_Type> Mem_SelectColumn;//����ѡ����Ԫ��
	Table_Type Mem_Table;
};
//Insert_into �е�values�Ľṹ��
struct Insert_IntoStruct
{
	ColType Valuetype;
	string CharValues;
	int  IntValues;
	float FloatValues;
};
class Insert_Into
{
	friend class Intepretor;
public:
	vector<Insert_IntoStruct> InsertIntoValues;//Insert into values �е�values�ļ���
	void Insert_IntoParse();//insert into �������
};
#endif 