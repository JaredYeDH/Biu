#pragma once
#ifndef _FILE_H
#define _FILE_H
#include <string>
#include "Glob_Var.h"
#include "Catalog.h"
using namespace std;
class File
{
public:
	static File & Instance();										//�ļ�ʵ����
	static bool Read(string &filename,int blocknum,char *& dst);	//��ȡ�ļ�
	static bool Write(string & filename,string & content,int & num);//д���ļ���
	static bool Write(Block * block);								//д���ļ���
	static void CreateFile(string &filename);						//�����ļ�
	static int ReadLast();
};
//ʵ����
File &File::Instance()
{
	static File File_Instance;
	return File_Instance;
}
#endif