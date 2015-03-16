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
	LESS_THAN,//<
	EQUAL,//=
	MORE_THAN,//>
	MORE_AND_EQUAL,//>=
	LESS_AND_EQUAL,//<=
	NOT_EQUAL,//!=  �� where�Ӿ䲻��ʹ�ã�=�����

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
	ColumnEndComma,//ÿ�����Խ����Ķ���
	PrimaryKey_primary,//Primary Key�е�Primary
	PrimaryKey_key,//Primary Key�е�key
	PrimaryKey_LeftBrackets,//Primary Key��������
	PrimaryKey_RightBrackets,//Primary Key��������
	PrimaryKey_Comma,//Primary Key�и������Էָ��Ķ���
	PrimaryKey_ColumnName,//Լ��ΪPrimary���ֶ�
	EndPrimaryKey,//Primary keyԼ�������ķֺ�
	EndTable,//��������
	Select,//ѡ��
	SelList,//ѡ����б�
	From,//select from �е�from
	FromList,//Select��From���б�
	Where,//Select��Where
	And ,//Select�е�where��And
	EndSelect,//Select�е�End
	Insert,//Insert into �е�Insert
	Into,//Insert into�е� Into
	InsertTable,//Insert into �����ݱ�
	Insert_Value,//Insert into values ��values
	Insert_Rightbracket,//Insert into values ��������
};
//�ֶ�����
enum ColType
{
	Int,
	Char,
	Float,
};
#endif