#ifndef _API_H
#define _API_H
#include "Glob_Var.h"
#include "Intepretor.h"
#include "Record_Manager.h"
#include "Buffer.h"
#include <string>
using namespace std;

/*
	������ȸ���ģ��.
	�����û������룬����Interpreter����ͨ��������������
	����ģ��Ĺ���ʵ���û�������.
*/
class API
{
	friend class Intepretor;
public:
	API();
	Intepretor MiniSQLIntepretor;//Intepretor����
	Catalog * Mem_Catalog;//ʵ���������ݱ���Ϣ
	//�������
	void CreateTable(string & tablename, vector<Column_Type> & Attributes);
	Selection Select(string table_name, vector<Comparison>);//ѡ������
};
#endif