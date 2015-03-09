#ifndef _BUFFER_H
#define _BUFFER_H
/*
	���������
*/

//��C++������C�����еĺ����ͱ������ڰ���C����ͷ�ļ�ʱ����������д���
extern "C"
{
	#include <io.h>
	#include <fcntl.h>
	#include <stdlib.h>
	#include <string.h>
}
#define Block_Size 4096	// �� 4KB
#define Bolck_Num	  1000	// �������
#ifndef MAX_FILESIZE
#define MAX_FILESIZE   256	// �ļ�����󳤶�
#endif

//ҳͷ��Ϣ����ʶ�ļ�ҳ
typedef struct
{
	unsigned long PageID;		//ҳ���
	bool Mem_Fixed;				//�Ƿ�פ�ڴ�
	void InitPage(unsigned long pageID, bool mem_fixed);//��ʼ������
}Page_Head;

//�ļ��ڴ��ַ
class F_FileAdd
{
public:
	unsigned long FilePageID; //ҳ���
	unsigned int PageOffset;  //ҳ��ƫ����
	void InitiaFile();		  //��ʼ����0.0��
	void *MemAdd() const;	  //����ҳ��ź�ƫ������ȡ����ǰ�ļ��õ�ַ���ڴ��еĵ�ַ,ͬʱ����ҳƫ�����Ƿ����
	void *MemAdd(class M_File &) const;//����ҳ��ź�ƫ������ȡ���ļ��õ�ַ���ڴ��еĵ�ַ,ͬʱ����ҳƫ�����Ƿ����
	void *MemAdd(class M_File *) const;//����ҳ��ź�ƫ������ȡ���ļ��õ�ַ���ڴ��еĵ�ַ,ͬʱ����ҳƫ�����Ƿ����
	bool operator>=(F_FileAdd& other) const; // _F_FileAddr >= ����
	bool operator> (F_FileAdd& other) const; // _F_FileAddr >  ����
	bool operator==(F_FileAdd& other) const; // _F_FileAddr == ����
	bool operator!=(F_FileAdd& other) const; // _F_FileAddr != ����
	bool operator==(const int zero) const;   // _F_FileAddr == ����
	void ShiftOffset(int offset);			 //�ڵ�ǰҳ�л���offset��(�����ɸ�)
};

//�ļ�ͷ��Ϣ
typedef struct
{
	F_FileAdd DelFirst;             // ��һ����ɾ����¼��ַ
	F_FileAdd DelLast;              // ���һ����ɾ����¼��ַ  
	F_FileAdd NewInsert;            // �ļ�ĩβ�ɲ��������ݵĵ�ַ
	unsigned long ulPageTotal;        // Ŀǰ�ļ��й���ҳ��
	void InitialFileCond();           // ��ʼ��
}File_Head;

//�ڴ�ҳ
class M_Page
{
	friend class M_Page_Info;
	friend class M_FileAdd;
	friend class M_Clock;//���á�ʱ�ӡ��Ĳ���
private:
	unsigned int M_FileID;//�����ļ���ţ���̬���䣩
	unsigned long M_FilePageID;//���ļ��е�ҳ��ID
	void *PageStart;//�����ڴ���ҳ�׵�ַ
	Page_Head *PtrHead;//ҳͷ��Ϣ
	M_Page();//��Ա��ʼ���������ڴ�ռ�
	~M_Page();//�ͷ��ڴ�ռ�
	//���ļ�����ҳ�����ٺõ��ڴ�ռ�
	void LoadFromFile(unsigned int fileID, unsigned long filepageID);
	//���ڴ��е�ҳд�ص��ļ���
	void BacktoFile()const;
};

//�ڴ�ҳ������Ϣ
class M_PageInfo
{
	friend class M_Clock;
	friend class F_FileAdd;
	friend class M_Buffer;
	bool IsLastUsed;//���һ�η����ڴ��Ƿ�ʹ�ã�����Clock�㷨
	bool IsModiFied;//�ӵ����ڴ濪ʼ���Ƿ��޸ģ����ھ����Ƿ���Ҫд�ļ�
	class M_Page * PtrtoPage;//��������ڴ�ҳ����
	M_PageInfo();//��Ա��ʼ��
	~M_PageInfo();//����
	void UpdatePageInfo(unsigned int fileID, unsigned long filepageID); //ҳ�滻������
	Page_Head * GetPtrHead() const;//���ҳͷ��Ϣ��ַ
	File_Head * GetPtrFileHead() const;//����ļ�ͷ��Ϣ��ַ
	unsigned int GetFileID() const;//�����������ڴ�ҳĿǰ�����������ļ����
	void SetFileID(unsigned int fileID); //�����µ��ļ���ţ�����ҳʱ��Ϊ0��
	unsigned long GetFilePageID() const;//�����������ڴ�ҳĿǰ�������ļ��е�ҳ���
};

//�ڴ�ҳ�û��㷨 ������Clock�㷨
class M_Clock
{
	friend class M_Buffer;
	friend class M_File;
private:
	unsigned int M_ClockSize;//�ڴ���ҳ������
	unsigned int M_UseingPage;//����ʹ�õ��ڴ�ҳ
	M_Clock();//��Ա��ʼ��
	~M_Clock();//��������
	void SetPageMod();//���õ�ǰҳΪ��ҳ

};

/*
	Bufferģ�������
	һ���ڴ�ҳ����һ���ļ�����
*/
class M_Buffer
{
public :
	void Start();//Buffer��ʼ��
	void End();//����
	unsigned int FileCount;//�򿪵��ļ�����
	M_File * F_First;//��һ���ļ�
	M_File * F_Last;//���һ���ļ�
	M_File * F_Current;//��ǰʹ���ļ�
	class M_Clock * MemPageClock;//Clock�㷨�Ķ���
};
#endif