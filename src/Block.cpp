#include "Block.h"
#include <sstream>
#include <strstream>
Block::Block()
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
	//��blocknum int ��תΪstring ��
	stringstream ss;
	string num;
	ss << blocknum;
	ss >> num;
	string Stringblocknum = ss.str();
	return (filename + string("@") + Stringblocknum);
}