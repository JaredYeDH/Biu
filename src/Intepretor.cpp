#include "Intepretor.h"
#include "Glob_Var.h"
#include <iostream>
using namespace std;

//�õ����������
MSG Getcommand(SQL_Create **)
{
	MSG Commandtype = UNORMAL;//��ʼ��Ϊ���������
	
}

//���������ַ�����Ĺ��캯������������д�������һ�������ַ����ļ���
Deal_Input::Deal_Input()
{
	InitiaInput();//��ʼ�������ַ���
	OutputLink();//�����ַ�������
}
//��ʼ�������ַ���
void Deal_Input::InitiaInput()
{
	this->Head = NULL;
	this->Now = NULL;
	this->NowDealNumber = 0;
}