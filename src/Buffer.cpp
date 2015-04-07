#include "Buffer.h"
#include <fstream>
#include <sstream>
#include <assert.h>
using namespace std;
/*
	������������
*/
Buffer_Manager::~Buffer_Manager()
{
	for (auto i = Buffer_ManagerUsedBlock.begin(); i != Buffer_ManagerUsedBlock.end(); i++)
	{
		if ((*i)->Block_Dirty)
		{
			File::Write(*i);
		}
	}
}
//д��һ���տ���
int Buffer_Manager::Buffer_ManagerWrite(string &filename, string &empty_block,int blocknum)
{
	string  Nouse;
	if (blocknum != -1)											//ָ����ŵ�д��
	{
		if (Buffer_ManagerInBuffer(filename, blocknum) == false)//��������ִ�Ŀ���
			Buffer_ManagerFile2Block(filename, blocknum, Nouse);//���ļ���Ӧ�Ŀ���뻺������
		Buffer_ManagerWrite2Block(filename, blocknum, empty_block);
		return blocknum;
		
	}
}
//�ļ��Ƿ��ڻ�������
bool Buffer_Manager::Buffer_ManagerInBuffer(string &fileName, int blocknum)
{
	string Key = Buffer_ManagerGetKey(fileName,blocknum);//����ļ�������Ŷ�Ӧ�ļ�ֵ
	return Buffer_ManagerBlockMap.count(Key) == 1;//�洢���Map�ṹ������оͷ���1

}
//���ļ���ȡ��д�뵽�������Ŀ���
bool Buffer_Manager::Buffer_ManagerFile2Block(string& fileName, const int blockNum, string& strOut)
{
	char *Dst = new char[Block_Size];//����һ�����С���ڴ�
	if (File::Read(fileName, blockNum, Dst) == false)
	{
		delete[] Dst;
		return false;
	}
	else
	{
		strOut = string(Dst, Block_Size);
		if (Buffer_ManagerIsFull())//����������Ѿ�����
			Buffer_ManagerReplace(fileName, blockNum, strOut);//�����滻�㷨 �滻���� 
		else
			Buffer_ManagerNewBlock(fileName, blockNum, strOut);
	}
}
//�����µĿ�
bool Buffer_Manager::Buffer_ManagerNewBlock(string & tablename, int blocknum, string & content)
{
	Block * block = new Block();
	block->Block_Update(tablename, blocknum, content);
	Buffer_ManagerUsedBlock.push_back(block);													//���¿����buffer�еĿ�����
	Buffer_ManagerBlockMap.insert(make_pair(block->Block_GetKey(tablename, blocknum), block));	//ά��Map
	return true;
}
//ֱ��д�����
bool Buffer_Manager::Buffer_ManagerWrite2Block(string & filename, int blocknum, string & Content)
{
	/*
	���ڳ������
	assert()��һ����Ϻ꣬���ڶ�̬��ʶ������߼�������������ԭ���ǣ� void assert(int expression);
	�����Ĳ�����ֵ���Ϊ����ֵ�������κβ�����no action�����������ֵ���ÿ��ַ���ӡ�����Ϣ��Ȼ�����abort()��
	*/
	assert(Buffer_ManagerInBuffer(filename, blocknum) == true);
	string key = Block::Block_GetKey(filename, blocknum);
	if (Buffer_ManagerBlockMap[key]->Block_Content != Content)
	{
		Buffer_ManagerBlockMap[key]->Block_Content;
		Buffer_ManagerBlockMap[key]->Block_Dirty = true;				//�޸�ĳһ���ݣ�������д�ش���
	}
	return true;
}
//�������Ƿ�����
bool Buffer_Manager::Buffer_ManagerIsFull()
{
	return Buffer_ManagerUsedBlock.size() == Block_Size;
}
//���ؿ��
int Buffer_Manager::Buffer_ManagerReadLastNumber(string & filename, string &str)
{
	File::Instance().ReadLastNumber(filename, str);
}
//��ȡ
bool Buffer_Manager::Buffer_ManagerRead(string &filename,int blocknum,string &strout)
{
	if (Buffer_ManagerInBuffer(filename, blocknum)) //����Ƿ��ڻ�������
		return Buffer_ManagerReadBlock(filename,blocknum,strout);//����ڻ������ʹӻ�������ȡ
	else
		return Buffer_ManagerFile2Block(filename,blocknum,strout);//������ļ��ж�ȡ
}
//�ӻ������Ŀ��ж�ȡ,��Buffer_ManagerRead()����
bool Buffer_Manager::Buffer_ManagerReadBlock(string &filename, int blocknum, string &strout)
{
	string Key = Buffer_ManagerGetKey(filename, blocknum);
	if (Buffer_ManagerBlockMap[Key]->Block_Pin == false)						 // �ڸÿ�ǳ�פ��ʱ������LRU����ά��
	{
		Buffer_ManagerUsedBlock.remove(Buffer_ManagerBlockMap[Key]);			//List Buffer_ManagerUsedBlock ����ά��
		Buffer_ManagerUsedBlock.push_back(Buffer_ManagerBlockMap[Key]);
	}
	strout = Buffer_ManagerBlockMap[Key]->Block_Content;
	return true;
}
//���ڻ������ڣ����ļ��ж�ȡ
bool Buffer_Manager::Buffer_ManagerFile2Block(string &filename, int blocknum, string &strout)
{
	char *Dst = new char[Block_Size];
	//������ļ��ж�ȡʧ�ܣ�����False
	if (File::Read(filename, blocknum, Dst) == false)
	{
		delete[] Dst;
		return false;
	}
	else
	{
		strout = string(Dst, Block_Size);
		if (Buffer_ManagerIsFull())
			Buffer_ManagerReplace(filename,blocknum,strout);//����������Ѿ����ˣ��ͽ����滻
		else
			Buffer_ManagerNewBlock(filename, blocknum, strout);//��������һ���µĿ飬���뵽��������
	}
	delete[] Dst;
	return true;
}
//�����������������滻
bool Buffer_Manager::Buffer_ManagerReplace(string &filename,int blocknum,string &strout)
{
	Block *ReplaceBlock;
	while ((ReplaceBlock = Buffer_ManagerUsedBlock.front())->Block_Pin)		//�ҵ��� ��פ��
	{
		Buffer_ManagerUsedBlock.pop_front();
		Buffer_ManagerUsedBlock.push_back(ReplaceBlock);
	}
	if (ReplaceBlock->Block_Dirty)					//��������Ĺ����ͽ���д���ļ���
		ReplaceBlock->Write2File();
	string Key = ReplaceBlock->Block_GetKey(filename,blocknum);
	string NewKey = Buffer_ManagerGetKey(filename, blocknum);
	ReplaceBlock->Block_Update(filename, blocknum, strout);			//�ڻ������ڽ��п�ĸ���
	Buffer_ManagerBlockMap.erase(Buffer_ManagerBlockMap.find(Key));	//�Ƴ��ɵĿ�
	Buffer_ManagerBlockMap.insert(make_pair(NewKey, ReplaceBlock));	//����µĿ�
	return true;
}
