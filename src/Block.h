#pragma once
#ifndef _BLOCK_H
#define _BLOCK_H
#include "Glob_Var.h"
#include "Buffer.h"
#include <string>
using namespace std;
class Block
{
public:
	Block();
	Block(string blockname, int offset, File_Type filetype);
	~Block();
	void Block_Update(string &filename, int blocknum, string &content);	//��ĸ���
	static string Block_GetKey();										//��ȡ��ļ�ֵ
	string Block_Name;													//�������
	int Block_Num;														//���ƫ����
	bool Block_Pin;														//�Ƿ�Ϊ��פ��
	bool Block_Dirty;													//��� ��־λ
	bool Write2File();													//д���ļ���
	string Block_Content;
};
#endif