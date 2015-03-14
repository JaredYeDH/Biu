#ifndef _BUFFER_H
#define _BUFFER_H
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
	class B_Block;
	class B_File;
public:
	Buffer_Manager();
	~Buffer_Manager();
	Buffer_Manager & Instance();//���������ʵ����
	int  Write(string & FileName, string & Content, int BolckNum = -1);
	bool Write2Block(string& fileName,int blockNum,string& content);
	bool InBuffer(string FileName, int BlockNum);//����ڲ��ڻ�������
	map<string, B_Block*> MemBlock_Map;//��¼ʹ�õĿ�
	list<B_Block *> MemBlock_Used;//�Ѿ�ʹ�ù��Ŀ�
	bool Built_NewBlock(string FileName,int BlockNum,string & str);//�����µĿ�
	bool File2Block(string& fileName, int blockNum, string& strOut);
	bool IsFull();
	bool Replace(string& FileName,int blockNum,string& content);//�滻�㷨
	class B_Block
	{
	public:
		string Block_Content;//���Ŀ¼
		/*
		�ļ�ϵͳ��黺������Ƿ��и��ļ��еĿ飬
		��������Щ���Ƿ�д����д����Щ�鼴Ϊ dirty ��
		*/
		bool Block_Pin;
		bool Blcok_Ditry;
		int Block_Num;
		string FileName;
		string Content;
		B_Block();
		static string GetKey();
		bool IsPin();
		bool IsDirty();
		bool Write2File();
		void Update(string & FileName, int Block_Num, string & Content);
	};
	class B_File
	{
	public:
		static bool Read(string & FileName, int Block_Num, char *& Dst);
		static bool Write(B_Block *Current);
		static bool Write(string &FileName, string &Content, int & Num);
		static void CreateFile(string FileName);
		static bool File_NotIn();//�ļ��Ѿ�������

	};
};
//�����������ʵ����
inline Buffer_Manager & Buffer_Manager::Instance()
{
	Buffer_Manager Buffer_Instance;
	return Buffer_Instance;
}
#endif