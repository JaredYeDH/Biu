#include "Select.h"
using namespace std;
//�����û���Select����
vector<SelectRecord> & Selection::Select_Parse(queue<string> attributes, queue<string>tablelists, queue<WhereList> wherelists)
{
	if (attributes.front() == "All") //select * from table where ...
	{
		while (tablelists.size() != 0)
		{
			Table table = Catalog::Instance().CatalogGet_Table(TableLists.front());		//�õ����ݱ�
			if (wherelists.empty()) //select * from table .where����Ϊ��
			{
				int table_attributes_size = table.Table_AttributesList.size();
				while (table_attributes_size--)
				{
					SelectRecord selectrecord; //ѡ���һ�м�¼
					selectrecord.AttributesName = table.Table_AttributesList[table_attributes_size].Attributes_Name; //������ݱ��������
					string record;				//������¼ֵ
					int table_blocknum = Buffer_Manager::Instance().Buffer_ManagerReadLastNumber(table.Table_Name, record);	//��ȡ���
					Buffer_Manager::Instance().Buffer_ManagerRead(table.Table_Name, table_blocknum, record);				//�ӻ�������ȡ��¼
				}
			}
		}
	}
}