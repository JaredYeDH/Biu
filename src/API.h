#ifndef _API_H
#define _API_H
#include "Intepretor.h"
#include "Glob_Var.h"
#include "Record_Manager.h"
#include "Buffer.h"
#include "Catalog.h"
#include <string>
#include <vector>
using namespace std;

/*
	������ȸ���ģ��.
	�����û������룬����Interpreter����ͨ��������������
	����ģ��Ĺ���ʵ���û�������.
*/
class API
{
public:
	API();
	static API & Instance();	//ʵ����
	//�������
	void CreateTable(string & Tablename,vector<Column_Type> & Attributes);
	Selection Select(string table_name, vector<Comparison>);//ѡ������
};
inline static API & Instance()
{
	static API API_Instance;
	return API_Instance;
}
#endif