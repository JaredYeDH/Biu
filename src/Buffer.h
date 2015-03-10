#ifndef _BUFFER_H
#define _BUFFER_H
#include <string>
using namespace std;
/*
	���������
*/
#define Block_Size 4096	// �� 4KB
#define Bolck_Num	  10000	// �������
#define MAX_FILESIZE   256	// �ļ�����󳤶�

class Buffer_Manager
{
public:
	Buffer_Manager();
	~Buffer_Manager();
	Buffer_Manager & Instance();//���������ʵ����
};
class B_Block
{
public:
	bool Blcok_Ditry;
	int Block_Num;
	string FileName;
	string Content;
	B_Block();
	bool Dirty();
	bool Write2File();
	string GetKey();
	void Update(string & FileName, int Block_Num, string & Content);
};
class B_File
{
public:
	bool Read(string & FileName, int Block_Num, char *& Dst);
	bool Write(B_Block *Current);
};
inline Buffer_Manager & Buffer_Manager::Instance()
{
	Buffer_Manager Buffer_Instance;
	return Buffer_Instance;
}
#endif