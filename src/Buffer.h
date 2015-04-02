#pragma once
#ifndef _BUFFER_H
#define _BUFFER_H
#include "Glob_Var.h"
#include "Intepretor.h"
#include "Block.h"
#include "File.h"
#include <stdlib.h>
#include <string>
#include <map>
#include <list>
using namespace std;
/*
	���������
*/
#define Block_Size 4096	// �� 4KB
#define Block_Tol	  10000	// �������
#define MAX_FILESIZE   256	// �ļ�����󳤶�
class Buffer_Manager
{
public:
	Buffer_Manager();
	~Buffer_Manager();
	static Buffer_Manager & Instance();									  //���������ʵ����
	void Buffer_ManagerWrite(string &filename, string &empty_block);	  //д��һ���տ���

	Block Read(string &tablename, File_Type filetype, int offset);		  //��ȡ
	bool File_Exist(string &tablename, File_Type filetyppe);			  //�ļ��Ƿ����
	Block ReadLast(string & FileName, File_Type filetype);				  //���ؿ�
	bool Write2Block(string& fileName,int blockNum,string& content);	  //д�����
	bool InBuffer(string FileName, int BlockNum);						  //����ڲ��ڻ�������
	map<string, Block*> MemBlock_Map;									  //��¼ʹ�õĿ�
	list<Block *> MemBlock_Used;										  //�Ѿ�ʹ�ù��Ŀ�
	bool Replace(string& FileName,int blockNum,string& content);		  //�滻�㷨
	static void CreateFile(string &tablename, File_Type filetype);        //�����ļ�
	bool IsFull();														  //�������Ƿ�����
	int ReadLastNumber(string & filename, string &str);					  //���ؿ��
	bool File2Block(string& fileName, int blockNum, string& Strout);	  //���ļ�д�����
	bool newBlock(string& fileName,int blockNum,string& content);		  //�����¿�
};
//�����������ʵ����
inline Buffer_Manager & Buffer_Manager::Instance()
{
	static Buffer_Manager Buffer_Instance;
	return Buffer_Instance;
}
#endif