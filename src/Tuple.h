#pragma once
#ifndef _TUPLE_H
#define _TUPLE_H
#include "Table.h"
#include <string>
#include <vector>
using namespace std;
/*
		���ݱ��еļ�¼
*/
class Tuple
{
public:
	Tuple(){}
	Tuple(Table & table, string &content, int begin);
	Tuple(vector<string> & tuplecontent);
	vector<string> Tuple_content;							//Ԫ�������
};
#endif