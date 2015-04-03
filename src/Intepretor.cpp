#include "Intepretor.h"
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
using namespace std;
//��string���͵�typeת��ΪAttributes_Type
Attributes_Type Intepretor::Trasn2type(string type)
{
	if (type == "int")
		return Attributes_Type::Int;
	if (type == "char")
		return Attributes_Type::Char;
	if (type == "float")
		return Attributes_Type::Float;
	/*
			������������������ͣ���Ҫ�׳�һ������þ�
	*/
	else throw Error(0, "Interpreter", "Type", "���ʹ��󣬲�֧���û����������!");
}
//��String תΪInt
int Intepretor::String2Int(string s)
{
	int n = atoi(s.c_str());
	return n;
}
//��String תΪ Char *
const char * Intepretor::String2Char(string  s)
{
	const char *n = s.c_str();
	return n;
}
//��StringתΪ Float
float Intepretor::String2Float(string s)
{
	float F = atof(s.c_str());
	return F;
}
//�������ѡ���ж�
enum Operator_type  Intepretor::Op_Judge(string Op)
{
	if (Op == "<")
		return EQUAL;
	if (Op == "<")
		return LESS_THAN;//<
	if (Op == "=")
		return EQUAL;//=
	if (Op == ">")
		return MORE_THAN;//>
	if (Op == ">=")
		return MORE_AND_EQUAL;//>=
	if (Op == "<=")
		return LESS_AND_EQUAL;//<=
	if (Op == "!=")
		return NOT_EQUAL;//!=  �� where�Ӿ䲻��ʹ�ã�=�����
	else throw Error(0, "Interpreter", "Operator", "��֧���û�����������!");
}
/*
	����
*/
//������������
void Intepretor::CommandInput()
{
	while (true)
	{
		string command;
		vector<string> Input;
		while (cin >> command)
		{
			//ȡ������;,
			bool End;//�û��Ƿ��������
			auto start = command.begin();
			for (auto i = command.begin(); i != command.end(); i++)
			{
				string s;
				switch (*i)
				{
				case'(':
				case ')':
				case ',':
				case '\'':
				case '=':
					if (start != i)
						Input.push_back(string(start, i));
					s.push_back(*i);
					Input.push_back(s);
					start = i;
					start++;
					break;
				case '<':
				case '>':
					if (start != i)
						Input.push_back(string(start, i));
					s.push_back(*i);
					i++;
					if (i != command.end() && (*i == '=' || *i == '>'))
						s.push_back(*i);
					else
						i--;
					Input.push_back(s);
					start = i;
					start++;
					break;
				case ';':
					if (start != i)
						Input.push_back(string(start, i));
					start = i;
					start++;
					End = true;
					break;

				default:
					break;
				}
				if (End == true)
					break;
			}
			if (Is_Quit(Input))
				exit(0);//�˳�
			else
				ParseCommand();

		}
	}
}
//��������
void Intepretor::ParseCommand()
{
	if (Is_CreateTable(Input))
		CreateTable_command(Input);
	if (Is_Select(Input))
		Select_command(Input);
	if (Is_Insert(Input))
		Insert_command(Input);
}
//�Ƿ�ΪCreate����
bool Intepretor::Is_CreateTable(vector<string> input)
{
	if (input.size() >= 2 && input[0] == "create"&&input[1] == "table")
		return true;
	else
		return false;
}
//�Ƿ�ΪSelect����
bool Intepretor::Is_Select(vector<string> input)
{
	if (input.size() >= 2 && input[0] == "insert"&&input[1] == "into")
		return true;
	else
		return false;
}
//�Ƿ�ΪQuit����
bool Intepretor::Is_Quit(vector<string> input)
{
	if (input.size() >= 2 && input[0] == "quit")
		return true;
	else
		return false;
}
//�Ƿ�ΪInsert����
bool Intepretor::Is_Insert(vector<string> input)
{
	if (input.size() >= 2 && input[0] == "insert"&&input[1] == "into")
		return true;
	else
		return false;
}
//�������ݱ�Create����
void Intepretor::CreateTable_command(vector<string>Input)
{
	Command_State state = Create;
	Table table;//�½��������ݱ�
	vector<Attributes> NewTableAttributes;//�µ����ݱ����ֶ�
	Attributes attributes;
	Attributes attributesprimary;//primary key Լ����Ҫ���ֶ�������
	for (auto i = Input.begin(); i != Input.end(); i++)
	{
		switch (state)
		{
		case Create:
			state = Create_Table; break;
		case Create_Table:
			state = Table_Name; break;
		case Table_Name:
			state = Left_Query;
			table.Table_Name = *i;
			break;
		case Left_Query:
			if (*i != "(")
				throw Error(0, "Interpreter", "Create table", "�﷨����!");
			state = Column_Name;
			break;
		case Column_Name://�ֶ���
			state = Column_type;
			attributes.Attributes_Name= *i;
			break;
		case Column_type://�ֶ�����
			attributes.Attributes_type = Trasn2type(*i);
			if (Trasn2type(*i) == Char)
				state = Char_LeftBrackets;
			else
				state = Unique;
			break;
		case Char_LeftBrackets://������
			if (*i != "(")
				throw Error(0, "Interpreter", "Create table", "�﷨����!");
			state = CharSize;
			break;
		case CharSize://Char���ʹ�С
			attributes.Attributes_Length = String2Int(*i);
			state = Char_LeftBrackets;//char��������
			break;
		case Char_RightBrackets://������
			if (*i != ")")
				throw Error(0, "Interpreter", "Create table", "�﷨����!");
			if (*(++i) == "not")
				state = Not_Null_not;
			break;
		case Not_Null_not://��Ϊ��
			if (*i != "not")
				throw Error(0, "Interpreter", "Create table", "�﷨����!");
			state = Not_Null_null;
			break;
		case Not_Null_null://��Ϊ��
			if (*i != "null")
				throw Error(0, "Interpreter", "Create table", "�﷨����!");
			attributes.Attributes_Null= true;
			if (*(++i) == "unique")
				state = Unique;
			break;
		case Unique://Ψһ����
			if (*i != "unique")
				throw Error(0, "Interpreter", "Create table", "�﷨����!");
			attributes.Attributes_Unique = true;
			if (*(++i) == ",")
			{
				state = ColumnEndComma;
				NewTableAttributes.push_back(attributes);
			}	
			break;
		case ColumnEndComma://һ�����Խ�������һ���µ�����
			state = Column_Name;
			break;
		case PrimaryKey_primary://����
			if (*i != "primary")
				throw Error(0, "Interpreter", "Create table", "�﷨����!");
			state = PrimaryKey_key;
			break;
		case PrimaryKey_key://����
			if (*i != "key")
				throw Error(0, "Interpreter", "Create table", "�﷨����!");
			state = PrimaryKey_LeftBrackets;
			break;
		case PrimaryKey_LeftBrackets://����������
			if (*i != "(")
				throw Error(0, "Interpreter", "Create table", "�﷨����!");
			state = PrimaryKey_ColumnName;
			break;
		case PrimaryKey_ColumnName://����������
			attributesprimary = Catalog::Instance().Get_Attributes(table.Table_Name, *i);
			attributesprimary.Attributes_Primary= true;
			if (*(++i) == ")")
				state = PrimaryKey_RightBrackets;
			if (*(++i) == ",")
				state = PrimaryKey_Comma;
			break;
		case PrimaryKey_Comma://�����������������Ե�״̬
			state = PrimaryKey_ColumnName;
			break;
		case PrimaryKey_RightBrackets://����������
			if (*i != ")")
				throw Error(0, "Interpreter", "Create table", "�﷨����!");
			if (*(++i) == ")")
				state = EndPrimaryKey;
			break;
		case EndPrimaryKey://��������
			if (*i == ")")
				state = Right_Query;
			else
				throw Error(0, "Interpreter", "Create table", "�﷨����!");
			break;
		case Right_Query://��������������û�����Ĳ���
			Catalog::Instance().CatalogCheckTable(table.Table_Name,NewTableAttributes);//����Catalog����Table Check����
			API::Instance().CreateTable(table.Table_Name, NewTableAttributes);//����API���в���
			break;
		default:
			break;
		}
	}
}
//ѡ��Select����
void Intepretor::Select_command(vector<string> Input)
{
	Command_State State = Select;			 //����״̬
	string Attribute[100] = {"$"}; int j = 0;//ѡ�������
	string FromLists[100] = {"$"}; int k = 0;//ѡ������ݱ�
	WhereList WhereLists[100]; int w = 0;	 //where
	for (auto i = Input.begin(); i != Input.end(); i++)
	{
		switch (State)
		{
		case Select:
			State = SelList;
			break;
		case SelList:
			if (*i == "*")
			{
				Attribute[j] = "All";//�����* ��ѡ�����е�����
				break;
			}
			Attribute[j] = *i;
			j++;
			if (*(++i) == "from")
				State = From;
			if (*(++i) == ",")
				State = SelList;
			if (*(++i) == "from")
				State = From;
			break;
		case From:
			if (*i != "from")
				throw Error(0, "Interpreter", "Select", "�﷨����!");
			State = FromList;
			break;
		case FromList:
			FromLists[k] = *i;
			k++;
			if (*(++i) == ",")
				State = FromList;
			if (*(++i) == "where")
				State = Where;
			break;
		case Where:
			WhereLists[w].Attribute = *i;
			i++;
			WhereLists[w].Where_Operator = *i;//�����
			i++;
			if (*i == "'")
				WhereLists[w].StrValue = String2Char(*(++i));
			else
				WhereLists[w].IntValue = String2Int(*(++i));
			if (*i == "and")
				State = And;
			if (*i == ";")
				State = EndSelect;
			break;
		case And:
			State = Where;
			break;
		case EndSelect:
			Selection::Selection();

			break;
		default:
			break; 
		}
	}
}
//Insert_into ����
void Intepretor::Insert_command(vector<string> input)
{
	Tuple tuple;							//��������ļ�¼
	vector<Tuple> Tuple_Lists;				//��β���ļ�¼����
	Command_State state = Insert;			//����״̬
	string Inserttablename;					//��Ҫ��������ݱ������
	for (auto i = input.begin(); i != input.end(); i++)
	{
		switch (state)
		{
		case Insert:
			if (*i == "into")
				state = Into;
			else
				throw Error(0, "Interpreter", "Select", "�﷨����!");
			break;
		case Into:
			state = InsertTable;
			break;
		case InsertTable:
			state = Insert_Values;
			Inserttablename = *i;//��ȡ���ݱ������
			break;
		case Insert_Values:
			state = Insert_Leftbracket;//ֵ�б��������
			break;
		case Insert_Leftbracket://ֵ�б��������
			if (*i != "(")
				throw Error(0, "Interpreter", "Insert into", "�﷨����!");
			state = Insert_Values_Or_Mark;
			break;
		case Insert_Values_Or_Mark://�������ֵ�����ַ���
			if (*i == "\'")
				state = Insert_Value;
			else
			{
				state = Insert_Comma_Or_Bracket;
				tuple.Tuple_content.push_back(*i);
			}
			break;
		case Insert_Value:			//�������ֵ
			tuple.Tuple_content.push_back(*i);
			state = Insert_Right_Mark;
			break;
		case Insert_Right_Mark:		//��������ַ���
			if (*i != "\'")
				throw Error(0, "Interpreter", "Insert into", "�﷨����!");
			state = Insert_Comma_Or_Bracket;
			break;
		case Insert_Comma_Or_Bracket://������ӻ��ǽ����˴β���
			if (*i == ",")
				state = Insert_Values_Or_Mark;
			else if (*i == ")")
				state = Insert_EndInsert;
			break;
		case Insert_Rightbracket://ֵ�б��������
			if (*i == ";")
				state = Insert_EndInsert;
			if (*i == ",")
				state = Insert_Leftbracket;//��Ҫ���������¼
			else 
				throw Error(0, "Interpreter", "Insert into", "�﷨����!");
			Tuple_Lists.push_back(tuple);//��һ�β���ļ�¼�ռ�
			break;
		case Insert_EndInsert://��������
			break;
		default:
			break;
		}
	}
	if (state = Insert_EndInsert)
	{
		Table table = Catalog::Instance().CatalogGet_Table(Inserttablename);
		vector<Attributes> Attributes_List = table.Table_AttributesList;
		if (Attributes_List.size() != tuple.Tuple_content.size())			//��������������Ƿ�����ݱ�ԭ�е���������ƥ��
			throw Error(0, "Interpreter", "Insert into", "�﷨����!");
		API::Instance().Insert_Into(Inserttablename,Tuple_Lists);
}