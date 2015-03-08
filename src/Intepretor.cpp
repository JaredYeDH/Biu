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
int Intepretor::String2Int(string s)
{
	int n = atoi(s.c_str());
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
		CreateTable(Input);
}
//�������ݱ�
void Intepretor::CreateTable(vector<string>Input)
{
	Command_State state = Create;
	Table_Type table;
	Column_Type column;
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
				state = EndComma;
			break;
		case EndComma:
			if (*i != ",")
				throw Error();
			if (*(++i) == "primary")
				state = PrimaryKey_primary;
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
			Column_Type columnprimary = table.GetColumn(*i);
			columnprimary.IsPrimary = true;
			if (*(++i) == ")")
				state = PrimaryKey_RightBrackets;
			else
				state = PrimaryKey_ColumnName;
			break;
		case PrimaryKey_RightBrackets:
			if (*i != ")")
				throw Error();
			if (*(++i) == ";")
				state=
			break;
		default:
			break;
		}
	}
}