#include "Buffer.h"
#include <fstream>
#include <sstream>
#include <assert.h>
using namespace std;
/*
	������������
*/
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
	return MemBlock_Map.count(Key) == 1;//�洢���Map�ṹ������оͷ���1

}
//���ļ���ȡ���������Ŀ���
bool Buffer_Manager::File2Block(string& fileName, const int blockNum, string& strOut)
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
		if (IsFull())//����������Ѿ�����
			Replace(fileName, blockNum, strOut);//�����滻�㷨 �滻���� 
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
bool Buffer_Manager::IsFull()
{
	return Buffer_ManagerUsedBlock.size() == Block_Size;
}


{
	FILE *FileP = NULL;
	if (filetype == File_Type::RECORD)
		FileP = fopen(Intepretor::String2Char(tablename + ".bol"), "rb");
	else if (filetype == File_Type::INDEX)
		FileP = fopen(Intepretor::String2Char(tablename + ".ind"), "rb");
	if (FileP == NULL)
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

//�ļ�д��
int Buffer_Manager::Write(string& filename,string& content,int blocknum = -1)
{
	string Nouse;
	if (blocknum != -1)//ָ����ŵ�д��
	{
		if (InBuffer(filename, blocknum) == false)
			File2Block(filename, blocknum, Nouse);	// ����buffer�У����ļ��ж�Ӧ�Ŀ����buffer (�����noUse��Ϊ��read�ӿڼ���)
		Write2Block(filename, blocknum, content);	// ֱ�ӽ�contentд��buffer�е�ָ����blocknum��
		return blocknum;
	}
	else
	{
		int num;
		File::Write(filename,content,num);//��д���ļ���
		File2Block();//�ٰ��ļ�д�뻺�����Ŀ���
		return num;
	}
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

//��ȡ
Block Buffer_Manager::Read(string &tablename, File_Type filetype, int offset)
{
	if (InBuffer(tablename, offset))
		return ReadBlock();//��buffer�У���buffer��
	else
		return File2Block();//����buffer�У����ļ��ж�


}

bool Buffer::newBlock(const std::string& fileName, const int blockNum, const std::string& content);