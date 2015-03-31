#include "Buffer.h"
#include <fstream>
#include <sstream>
#include <assert.h>
using namespace std;
/*
	������������
*/
int Buffer_Manager::Write(string & FileName,string & Content,int BlockNum)
{
	string Nouse;
	if (BlockNum != -1)
	{
		if (InBuffer(FileName, BlockNum) == false)//�������Buffer�У��Ͱ��ļ���Ӧ�Ŀ�д��Buffer
			File2Block(FileName, BlockNum, Nouse);
		Write2Block(FileName, BlockNum, Content);
		return BlockNum;
	}
	else
	{
		int num;
		B_File::Write(FileName, Content, num);
		File2Block(FileName, num, Nouse);
		return num;
	}
}
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
	B_Block * cur;
	while ((cur = MemBlock_Used.front())->IsPin())
	{
		MemBlock_Used.pop_front();
		MemBlock_Used.push_back(cur);
	}
	if (cur->IsDirty())
		cur->Write2File();
	string Oldkey = cur->GetKey();
	string Newkey = B_Block::GetKey();
	cur->Update(FileName, BlockNum, Content);
	MemBlock_Map.erase(MemBlock_Map.find(Oldkey));
	MemBlock_Map.insert(make_pair(Newkey, cur));
	return true;
}
/*
	���ļ�д�����
*/
bool Buffer_Manager::File2Block(string& fileName, int blockNum, string& Strout)
{
	char * Empty = new char[Block_Size];//��ȡ�ļ������Ĳ���
	if (B_File::Read(fileName, blockNum, Empty) == false)//��ʧ��(blockNum�����ļ��Ŀ�����ʱ)
	{
		delete[] Empty;
		return false;
	}
	else
	{
		Strout = string(Empty, Block_Size);
		/*
			����������Ѿ����ˣ�
			ʹ��LRU�㷨�����滻
		*/
		if (IsFull())
			Replace(fileName, blockNum, Strout);
		else
			Built_NewBlock(fileName, blockNum, Strout);//����һ���µĿ�
		delete[] Empty;
		return true;
	}
}
//�������Ƿ�����
bool Buffer_Manager::IsFull()
{
	return MemBlock_Used.size() == Block_Size;
}
//�����µĿ�

//�ļ�д��
bool Buffer_Manager::B_File::Write(B_Block * cur)
{
	ofstream Output(cur->FileName, ios::in | ios::out | ios::binary);
	if (File_NotIn())
	{
		B_File::CreateFile(cur->FileName);
		Output.open(cur->FileName, ios::in | ios::out | ios::binary);
	}
	/*
	seekp:��������ļ������ļ���ָ��λ��
	*/
	Output.seekp(Block_Size*cur->Block_Num);//��λд���ļ���ָ��λ��
	Output.write(cur->Block_Content.c_str(), Block_Size);
	Output.clear();
	return true;
}
bool Buffer_Manager::B_File::Write(string &FileName, string &Content, int & Num)
{
	ofstream Output(FileName, ios::in | ios::out | ios::binary);
	if (File_NotIn())
	{
		B_File::CreateFile(FileName);
		Output.open(FileName, ios::in | ios::out | ios::binary);
	}
	Output.seekp(0, ios_base::end);
	long End = Output.tellp();//��ʾ����ָ��ĵ�ǰλ��
	Num = (End / Block_Size);
	string ToWrite(Block_Size, 0);
	ToWrite = Content;
	Output.seekp(Num*Block_Size);
	Output.write(ToWrite.c_str(), Block_Size);
	Output.close();
	return true;
}
//���ؿ��
int Buffer_Manager::ReadLast(string & filename,string & str)
{
	return B_File::ReadLast(filename,str);
}
//���ؿ��
int Buffer_Manager::B_File::ReadLast(string & filename, string &str)
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
	if (filetype == File_Type::Record)
		FileP = fopen(Intepretor::String2Char(tablename + ".bol"), "rb");
	else if (filetype == File_Type::Index)
		FileP = fopen(Intepretor::String2Char(tablename + ".ind"), "rb");
	if (FileP == NULL)
		return false;
	else
		return true;
}