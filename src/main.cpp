#include "MiniSQL.h"
#include <string>
#include  <iostream>
using namespace std;

void Set_MiniSQL();//��ʼ�����ݿ�
void Run_MiniSQL();//ִ�����ݿ�
void Exit_MiniSQL();//�˳����ݿ�

extern M_Buffer Buffer;

int main()
{
	Set_MiniSQL();
	Run_MiniSQL();
	Exit_MiniSQL();
	return 0;
}
void Set_MiniSQL()
{
	Buffer.Start();
}
void Exit_MiniSQL()
{
	Buffer.End();
}
void Run_MiniSQL()
{
	Command_lin msg = Default;
	while (msg != Quit)
	{
		try
		{
			SQL_Create *CreaetInfo;
			SQL_Insert *InsertInfo;
			SQL_Select *SelectInfo;
			SQL_Update *UpdateInfo;
			SQL_Delete *DeleteInfo;
		}
		switch (msg)
		{
		case Create: Create(*CreateInfo); break;
		case Insert: Insert(*InsertInfo); break;
		case Select: Select(*SelectInfo); break;
		case Update: Update(*UpdateInfo); break;
		case Delete: Delete(*DeleteInfo); break;
		case Quit:						  break;
		default:
			break;
		}
	}
	catch (int Error_type)
	{
		SQL_Error(Error_type);
	}
}