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
	Unique,
	CharSize,//�ֶ���Char���ԵĴ�С
};
enum ColType
{
	Int,
	Char,
	Float,
};
#endif