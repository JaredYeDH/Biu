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
	while (!TableLists.empty())
	{
		SelectTable = Catalog::Get_Table(TableLists.front());
		while (!SelLists.empty())
		{
			Column_Type SelectColumn;
			SelectColumn = Catalog::Get_Column(TableLists.front(), SelLists.front());
			Operator_type Select_Operator = Intepretor::Op_Judge(WhereLists.front().Where_Operator);
			switch (Select_Operator)
			{

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