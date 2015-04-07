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
#include "Block.h"
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
	list<Block *> Buffer_ManagerUsedBlock;													//buffer��ʹ�õĿ�
	map<string, Block*>Buffer_ManagerBlockMap;												//buffer�еĿ��Ӧ�ֵ�
	static Buffer_Manager & Instance();														//���������ʵ����
	int Buffer_ManagerWrite(string &filename, string &empty_block,int blocknum=-1);			//д�룬����д��Ŀ��
	bool Buffer_ManagerInBuffer(string & fileame, int blocknum);							//����ڲ��ڻ�������
	string Buffer_ManagerGetKey(string &filename, int blocknum);							//ͨ���ļ����Ϳ�Ż�����ֵ
	bool Buffer_ManagerFile2Block(string& fileName, int blocknum, string& Strout);		    //���ļ����뵽����������
	bool Buffer_ManagerNewBlock(string& fileName, int blocknum, string& content);			//�����¿�
	bool Buffer_ManagerWrite2Block(string& fileName, int blockNum, string& content);		//д�뵽�������Ŀ���
	int Buffer_ManagerReadLastNumber(string & filename, string &str);						//���ؿ��
	bool Buffer_ManagerRead(string &filename, int blocknum, string &strout);				//��ȡ
	bool Buffer_ManagerIsFull();															//�������Ŀ��Ƿ�����
	bool Buffer_ManagerReadBlock(string &filename,int blocknum,string &strout);				//�ӻ������Ŀ��ж�ȡ
	bool Buffer_ManagerReplace(string &filename, int blocknum, string &strout);				//���������������滻
};
//�����������ʵ����
inline Buffer_Manager & Buffer_Manager::Instance()
{
	static Buffer_Manager Buffer_Instance;
	return Buffer_Instance;
}
#endif