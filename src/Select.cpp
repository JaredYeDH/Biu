#include "Select.h"
using namespace std;
//��ȡ�û�������
void Selection::SelectionInput(string *Sel, string *table, WhereList *& wherelist, int wherenum)
{
	int i = 0;
	while (Sel[i] != "$")
	{
		SelLists.push_back(Sel[i]);
		i++;
	}
	i = 0;
	while (table[i] != "$")
	{
		TableLists.push_back(table[i]);
		i++;
	}
	for (i = 0; i < wherenum; i++)
		WhereLists.push_back(wherelist[i]);
}
//����Select����
void Selection::Selection_Parse()
{
	Table_Type SelectTable;
	string WhereAttributesName;//where��ѡ�����Ե�����
	Column_Type WhereAttributes;// where��ѡ������
	vector<Record> SelectRecord;//Select�жϺ�ļ�¼
	Record WhereRecord;//select����where���־�ѡ���ļ�¼
	while (!TableLists.empty())//TableListsΪ�û�ѡ������ݱ��ϣ���From
	{
		SelectTable = Catalog::Get_Table(TableLists.front());//��ȡ�û�ѡ������ݱ�
		while (!SelLists.empty())//SelListsΪ�û�ѡ������Լ���
		{
			Column_Type SelectColumn;
			SelectColumn = Catalog::Get_Column(TableLists.front(), SelLists.front());//�û�ѡ�������
			Operator_type Select_Operator = Intepretor::Op_Judge(WhereLists.front().Where_Operator);//ȡ��where�������
			switch (Select_Operator)
			{
			case EQUAL:
				WhereAttributesName = WhereLists.front().Attribute;//where��ѡ�����Ե�����
				WhereAttributes = Catalog::Get_Column(SelectTable.Table_Name, WhereAttributesName);// where��ѡ������
			case LESS_THAN:
			case MORE_THAN://>
			case MORE_AND_EQUAL://>=
			case LESS_AND_EQUAL://<=
			}
		}
	}
}
//��ӡSelect������ͷ
void Selection::Print_SelectHead()
{
	/*
	ֻ��ӡSelection�е�vector<string> SelLists;
	*/
}