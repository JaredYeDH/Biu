#include "File.h"
#include <fstream>
//д���ļ�
bool Write(string & filename, string & content, int & num)
{
	ofstream Out(filename, ios::in | ios::out | ios::binary);	//  д׷�ӵ��ļ�β
	if (Out == 0)//����ļ�������
	{
		//�ļ������ڣ��������ļ�
		File::CreateFile(filename);
		Out.open(filename, ios::in | ios::out | ios::binary);
	}
	Out.seekp(0, ios_base::end);
	long End = Out.tellp();
	num = (End / Block_Size);                                                                // ��(��)β����Ϊ��������
	string toBeWrite(Block_Size, 0);                                                     // ensure the rest of the 'toBeWrite' is 0
	toBeWrite = content;
	Out.seekp(num * Block_Size);
	Out.write(toBeWrite.c_str(), Block_Size);
	Out.close();

	return true;
}
int File::ReadLastNumber(string &filename,string &str)
{
	ifstream In(filename, ios::binary);
	In.seekg(-4096, ios_base::end);
	const long Target = In.tellg();
	char Dst[Block_Size];
	In.read(Dst, Block_Size);
	str = string(Dst, Block_Size);
	return (Target / Block_Size);
}