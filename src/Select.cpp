#include "Select.h"
using namespace std;
//�����û���Select����
vector<SelectRecord> & Selection::Select_Parse(queue<string> attributes, queue<string>tablelists, queue<string> wherelists)
{
	if (attributes.front() == "All") //select * from table where ...
	{
		while (tablelists.size()!=0)
		{
			string StrRecord;
			vector<string> record; //���ݱ��еļ�¼��̬����
			Table table = Catalog::Instance().CatalogGet_Table(TableLists.front());		//�õ����ݱ�
			int table_blocknum = Buffer_Manager::Instance().Buffer_ManagerReadLastNumber(table.Table_Name, StrRecord);	//��ȡ���ݱ�Ŀ��
			Buffer_Manager::Instance().Buffer_ManagerRead(table.Table_Name, table_blocknum, StrRecord);				//�ӻ�������ȡ��¼
			record=Record_Manager::Instance().Record_ManagerString2Tuple(table.Table_AttributesList, StrRecord);				//���ַ����ָ�ΪԪ��,����vector<stirng>
			if (wherelists.empty()) //select * from table .where����Ϊ��
			{
				int table_attributes_size = table.Table_AttributesList.size(); //���ݱ����Եĸ���
				int i = 0;
				while (i<table_attributes_size)
				{
					SelectRecord selectrecord; //ѡ���һ�м�¼
					selectrecord.AttributesName = table.Table_AttributesList[i].Attributes_Name; //������ݱ��������
					CatalogTable catalogtable = Catalog::Instance().CatalogGet_CatalogTable(tablelists.front());//�õ����ݱ��ģʽ��Ϣ
					int Tableattributesnum = catalogtable.CatalogTable_AttribtuesNum;//�õ����Ե���Ŀ
					for (int )
				}
			}
		}
	}
}