#ifndef _GLOB_VAR_H_
#define _GLOB_VAR_H_
/*
	ȫ�ֱ���
*/
enum MSG {
	CREATE, SELECT, INSERT, UPDATE, DELETE, DROP, Quit,
	SHOWDB, SHOWTABLE, USE, HELP, UNORMAL, NEW, Default, DROPDB
};
enum Operator_type
{
	B,//>
	BE,//>=
	L,//<
	LE,//<=
	NE,//!=
	Between,//between
	ALL//and
};
enum Command_State
{
	Create,
	Table,
	Table_Name,
	Left_Query,
	Right_Query,
	Column_Name,
	Column_type,
	Char_LeftBrackets,//char��������
	Char_RightBrackets,//char��������
	Unique,//����Լ��
	Not_Null_not,//��ֹ��ֵ�е�not
	Not_Null_null,//��ֹ��ֵ�е�null
	CharSize,//�ֶ���Char���ԵĴ�С
	EndComma,//ÿ�����Խ����Ķ���
	PrimaryKey_primary,//Primary Key�е�Primary
	PrimaryKey_key,//Primary Key�е�key
	PrimaryKey_LeftBrackets,//Primary Key��������
	PrimaryKey_RightBrackets,//Primary Key��������
	PrimaryKey_ColumnName,//Լ��ΪPrimary���ֶ�
	EndTable,//��������
};
enum ColType
{
	Int,
	Char,
	Float,
};
#endif