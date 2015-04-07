#include "Block.h"
Block::Block() :Block_Content(Block_Size, 0)
{
	Block_Dirty = false;
	Block_Num = -1;
}
Block::~Block()
{

}
//��ĸ���
void Block::Block_Update(string &filename, int blocknum, string &content)
{
	Block_Dirty = false;
	Block_Pin = false;
	this->Block_Name = filename;
	this->Block_Num = blocknum;
	this->Block_Content = content;
}
//ͨ���ļ����Ϳ�Ż�ö�Ӧ�ļ�ֵ
string Block::Block_GetKey(string &filename, int blocknum)
{
	stringstream tmp;
	string num;
	tmp << blocknum;
	tmp >> num;
	return (filename + string("@") + num);
}