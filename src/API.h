#pragma once
#ifndef _API_H
#define _API_H
#include "Glob_Var.h"
#include "Intepretor.h"
#include "Record_Manager.h"
#include "Attributes.h"
#include "Buffer.h"
#include "Catalog.h"
#include "Select.h"
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
	/*	
		�������
	*/
	void CreateTable(string & Tablename,vector<Attributes> & Attributes);		//Create		�������ݱ�
	void Insert_Into(string &Tablename, vector<Tuple> Tuple_Lists);				//Insert into	����Ԫ��
	//void Select(string table_name, vector<Comparison> Select_Attributes);//ѡ������
};
inline static API & Instance()
{
	static API API_Instance;
	return API_Instance;
}
#endif