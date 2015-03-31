#include "Buffer.h"
#include <fstream>
#include <sstream>
#include <assert.h>
using namespace std;
/*
	������������
*/

//д�����
bool Buffer_Manager::Write2Block(string & FileName, int Blocknum, string & Content)
{
	assert(InBuffer(FileName, Blocknum) == true);
	string key = B_Block::GetKey();
	if (MemBlock_Map[key]->Content != Content)
	{
		MemBlock_Map[key]->Content = Content;
		MemBlock_Map[key]->Blcok_Ditry = true;
	}
	return true;
}
/*
	�ļ��Ƿ��ڻ�������
*/
bool Buffer_Manager::InBuffer(string FileName, int BlockNum)
{
	string Key = B_Block::GetKey();//����ļ�������Ŷ�Ӧ�ļ�ֵ
	return MemBlock_Map.count(Key) == 1;//�洢���Map�ṹ������оͷ���1

}
/*
	�ڴ��п���û�
	LRU ->������ʹ��ҳ���㷨
*/
bool Buffer_Manager::Replace(string & FileName, int BlockNum, string & Content)
{
	Block * cur;
	while ((cur = MemBlock_Used.front())->IsPin())
	{
		MemBlock_Used.pop_front();
		MemBlock_Used.push_back(cur);
	}
	if (cur->IsDirty())
		cur->Write2File();
	string Oldkey = cur->GetKey();
	string Newkey = Block::GetKey();
	cur->Update(FileName, BlockNum, Content);
	MemBlock_Map.erase(MemBlock_Map.find(Oldkey));
	MemBlock_Map.insert(make_pair(Newkey, cur));
	return true;
}
//�������Ƿ�����
bool Buffer_Manager::IsFull()
{
	return MemBlock_Used.size() == Block_Size;
}
//�ļ�д��
void Buffer_Manager::Write(Block block)
{
	FILE* fp = NULL;
	if (block.Block_Type== File_Type::RECORD)
		fp = fopen(Intepretor::String2Char(block.Block_Name + ".bol"), "rb");
	else if (block.Block_Type == File_Type::INDEX)
		fp = fopen(Intepretor::String2Char(block.Block_Name + ".ind"), "rb");
	if (fp == NULL)
	{
		throw string("block type error");
	}
	fseek(fp, block.Block_Offset * 4096, SEEK_SET);
	fwrite(block.Block_Data, sizeof(Byte), 4096, fp);
	fclose(fp);
}
//���ؿ�
Block Buffer_Manager::ReadLast(string & tablename,File_Type filetype)
{
	B_Block block;
	FILE* fp = NULL;
	if (filetype== File_Type::RECORD)
		fp = fopen(Intepretor::String2Char(tablename + ".bol"), "rb");
	else if (filetype == File_Type::INDEX)
		fp = fopen(Intepretor::String2Char(tablename + ".bol"), "rb");
	if (fp == NULL)
	{
		throw string("no such file");
	}
	fseek(fp, 0, SEEK_END);
	//if (ftell(fp) == 0)
	//	throw error();

	fseek(fp, -4096, SEEK_END);
	int offset = ftell(fp) / 4096;
	fread(block.Block_Data , sizeof(Byte), 4096, fp);
	fclose(fp);
	block.Block_Offset = offset;
	block.Block_Name = tablename;
	block.Block_Type = filetype;
	return block;
}
//���ؿ��
int Buffer_Manager::ReadLastNumber(string & filename, string &str)
{
	ifstream In(filename, ios::binary);
	In.seekg(-4096, ios_base::end);
	const long Target = In.tellg();
	char Dst[Block_Size];
	In.read(Dst, Block_Size);
	str = string(Dst, Block_Size);
	return (Target / Block_Size);
}
//�����ļ��Ƿ����
bool Buffer_Manager::File_Exist(string  &tablename, File_Type filetype)
{
	FILE *FileP = NULL;
	if (filetype == File_Type::RECORD)
		FileP = fopen(Intepretor::String2Char(tablename + ".bol"), "rb");
	else if (filetype == File_Type::INDEX)
		FileP = fopen(Intepretor::String2Char(tablename + ".ind"), "rb");
	if (FileP == NULL)
		return false;
	else
		return true;
}
//�����ļ�
void Buffer_Manager::CreateFile(string & tablename, File_Type filetype)
{
	FILE * FileP = NULL;
	if (filetype == File_Type::RECORD)
		FileP = fopen(Intepretor::String2Char(tablename + ".bol"), "wb");
	else if (filetype == File_Type::INDEX)
		FileP = fopen(Intepretor::String2Char(tablename + ".ind"), "wb");
	if (FileP == NULL)
		throw string("File type error");
	fclose(FileP);
}
//�����µĿ�
int Buffer_Manager::New_Block(string & tablename, File_Type filetype)
{
	FILE *FileP = NULL;
	if (filetype == File_Type::RECORD)
		FileP = fopen(Intepretor::String2Char(tablename + ".bol"), "rb");
	else if (filetype == File_Type::INDEX)
		FileP = fopen(Intepretor::String2Char(tablename + ".ind"), "rb");
		if (FileP== NULL)
		{
			throw string("build new block failness");
		}
		fseek(FileP, 0, SEEK_END);
		int offset = ftell(FileP) / 4096;
		Block block;
		fwrite(block.Block_Data, sizeof(Byte), 4096, FileP);
		fclose(FileP);
		return offset;
}
//��ȡ
Block Buffer_Manager::Read(string &tablename, File_Type filetype, int offset)
{
	Block block;
}