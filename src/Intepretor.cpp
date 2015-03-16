#include "Intepretor.h"
#include "Glob_Var.h"
#include "Catalog.h"
#include "Error.h"
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>

using namespace std;
//��string���͵�typeת��ΪColType
ColType Intepretor::Trasn2type(string type)
{
	if (type == "int")
		return ColType::Int;
	if (type == "char")
		return ColType::Char;
	if (type == "float")
		return ColType::Float;
	/*
			������������������ͣ���Ҫ�׳�һ������þ�
	*/
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
//������������
void Intepretor::CommandInput()
{
	while (true)
	{
		string command;
		while (cin >> command)
		{
			//ȥ��������;
			bool End;//�û��Ƿ��������
			auto start = command.begin();
			for (auto i = command.begin(); i != command.end(); i++)
			{
				switch (*i)
				{
				case'(':
				case ')':
				case ';':
					if (start != i)
						Input.push_back(string(start, i));
					start = i;
					start++;
					End = true;
				case ',':

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
//�Ƿ�Ϊ�������ݱ�����
bool Intepretor::Is_CreateTable(vector<string> input)
{
	if (input.size() >= 2 && input[0] == "create"&&input[1] == "table")
		return true;
	else
		return false;
}
bool Intepretor::Is_Select(vector<string> input)
{
	if (input.size() >= 2 && input[0] == "insert"&&input[1] == "into")
		return true;
	else
		return false;
}
//�������ݱ�
void Intepretor::CreateTable_command(vector<string>Input)
{
	Command_State state = Create;
	Table_Type table;
	Column_Type column;
	Column_Type columnprimary;//primary key Լ����Ҫ���ֶ�������
	for (auto i = Input.begin(); i != Input.end(); i++)
	{
		switch (state)
		{
		case Create:
			state = Table; break;
		case Table:
			state = Table_Name; break;
		case Table_Name:
			state = Left_Query;
			table.Table_Name = *i;
			break;
		case Left_Query:
			if (*i != "(")
				throw Error();
			state = Column_Name;
			break;
		case Column_Name://�ֶ���
			state = Column_type;
			column.Column_TypeName = *i;
			break;
		case Column_type://�ֶ�����
			column.coltype = Trasn2type(*i);
			if (Trasn2type(*i) == Char)
				state = Char_LeftBrackets;
			else
				state = Unique;
			break;
		case Char_LeftBrackets://������
			if (*i != "(")
				throw Error();
			state = CharSize;
			break;
		case CharSize://Char���ʹ�С
			column.RequestSize = String2Int(*i);
			state = Char_LeftBrackets;//char��������
			break;
		case Char_RightBrackets:
			if (*i != ")")
				throw Error();
			if (*(++i) == "not")
				state = Not_Null_not;
			break;
		case Not_Null_not:
			if (*i != "not")
				throw Error();
			state = Not_Null_null;
			break;
		case Not_Null_null:
			if (*i != "null")
				throw Error();
			column.IsNull = true;
			if (*(++i) == "unique")
				state = Unique;
			break;
		case Unique:
			if (*i != "unique")
				throw Error();
			column.IsUnique = true;
			if (*(++i) == ",")
				state = ColumnEndComma;
			break;
		case ColumnEndComma:
			state = Column_Name;
			table.InsertColumn(column);
			break;
		case PrimaryKey_primary:
			if (*i != "primary")
				throw Error();
			state = PrimaryKey_key;
			break;
		case PrimaryKey_key:
			if (*i != "key")
				throw Error();
			state = PrimaryKey_LeftBrackets;
			break;
		case PrimaryKey_LeftBrackets:
			if (*i != "(")
				throw Error();
			state = PrimaryKey_ColumnName;
			break;
		case PrimaryKey_ColumnName:
			columnprimary = table.GetColumn(*i);
			columnprimary.IsPrimary = true;
			if (*(++i) == ")")
				state = PrimaryKey_RightBrackets;
			if (*(++i) == ",")
				state = PrimaryKey_Comma;
			break;
		case PrimaryKey_Comma:
			state = PrimaryKey_ColumnName;
			break;
		case PrimaryKey_RightBrackets:
			if (*i != ")")
				throw Error();
			if (*(++i) == ";")
				state = EndPrimaryKey;
			break;
		case EndPrimaryKey:
			if (*(++i) == ")")
				state = Right_Query;
			else
				throw Error();
			break;
		case Right_Query:
			break;
		default:
			break;
		}
	}
}
/*
	ѡ������ӿڵĹ��캯��
*/
//ѡ��Select����
void Intepretor::Select_command(vector<string> Input)
{
	Command_State State = Select;
	string Attribute[100] = {"$"}; int j = 0;//ѡ�������
	string FromLists[100] = {"$"}; int k = 0;//ѡ������ݱ�
	WhereList WhereLists[100]; int w = 0;//where
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
				throw Error();
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
			

			break;
		default:
			break; 
		}
	}
}
//��ȡ�û�������
Selection::Selection(string *Sel, string *table,WhereList *& wherelist,int wherenum)
{
	int i = 0;
	while (Sel[i]!="$")
	{
		SelLists.push_back(Sel[i]);
		i++;
	}
	i = 0;
	while (table[i] != "$")
	{
		TableLists.push_back(table[i]);
		i++;
	}
	for (i = 0; i < wherenum; i++)
		WhereLists.push_back(wherelist[i]);
}
//����Select����
void Selection::Selection_Parse()
{
	Table_Type SelectTable;
	while (!TableLists.empty())
	{
		SelectTable = Catalog::Get_Table(TableLists.front());
		while (!SelLists.empty())
		{
			Column_Type SelectColumn;
			SelectColumn = SelLists.front();
			switch (Operator_type)
			{
			case LESS_THAN:
				break;
			case EQUAL:
				break;
			case MORE_THAN:
				break;
			case MORE_AND_EQUAL:
				break;
			case LESS_AND_EQUAL:
				break;
			case NOT_EQUAL:
				break;
			default:
				break;
			}
		}
	}
}
//��ӡSelect������ͷ
void Selection::Print_SelectHead()
{
	/*
		ֻ��ӡSelection�е�vector<string> SelLists;
	*/
}
/*
	Insert_into �������
*/
void Insert_command(vector<string> input)
{
	Command_State state = Insert;
	string Inserttable;
	for (auto i = input.begin(); i != input.end(); i++)
	{
		switch (state)
		{
		case Insert:
			state = Into;
			break;
		case Into:
			state = InsertTable;
			break;
		case InsertTable:
			state = Insert_Value;
			Inserttable = *i;
			break;
		case Insert_Value:
			state = Insert_Rightbracket;
			break;
		case Insert_Rightbracket:
			if (*i != "(")
				throw Error();
			state

		}
	}
}