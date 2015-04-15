#pragma once
#ifndef _INDEX_H
#define _INDEX_H
#include <string>
#include <vector>
#include "Attributes.h"
#include "Table.h"
using namespace std;

class Index_Manager
{
public:
	Index_Manager(){}
	Index_Manager(Index_Manager &);
	~Index_Manager(){}
	static Index_Manager & Instance(); //ʵ����
	void Index_CreateIndex(string & tablename,vector<Attributes> & attributes); //��API::CreateTable ����
	void Index_CreataIndex(string & tablename, Attributes & attributes);//��API::CreateIndex ����
	void Index_DropIndex(string &tablename,vector<string> & AttributesName);//��API:;DropTable ����
	void Index_DropIndex(string & tablename,string & attritbuesname);//��API::DropIndex����
	void Index_InsertIndex(string &tablename, Attributes & attributes, vector< pair<string, int>>&Info);//��API::InsertTuple����
	void Index_DeleteIndex(string &tablename, Attributes &attributes, vector<pair<string, int>> & Info);//��API::DeleteTuple����
	void Index_InsertCheck(Table &table, vector<string> & StrTuple);//��API::InsertTuple

};
inline Index_Manager & Instance()
{
	Index_Manager instance;
	return instance;
}
#endif