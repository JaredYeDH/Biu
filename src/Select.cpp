#include "Select.h"
using namespace std;
//�����û���Select����
vector<SelectRecord> & Selection::Select_Parse(queue<string> attributes, queue<string>tablelists, queue<string> wherelists)
{

	if (attributes.front() == "All") //select * from table where ...
	{
		while (tablelists.size() != 0)
		{
			string StrRecord;
			vector<string> record; //���ݱ��еļ�¼vector����
			Table table = Catalog::Instance().CatalogGet_Table(TableLists.front());		//�õ����ݱ�
			int table_blocknum = Buffer_Manager::Instance().Buffer_ManagerReadLastNumber(table.Table_Name, StrRecord);	//��ȡ���ݱ�Ŀ��
			Buffer_Manager::Instance().Buffer_ManagerRead(table.Table_Name, table_blocknum, StrRecord);					//�ӻ�������ȡ��¼
			record = Record_Manager::Instance().Record_ManagerString2Tuple(table.Table_AttributesList, StrRecord);		//���ַ����ָ�ΪԪ��,����vector<stirng>
			vector<Tuple> Tuple_Record = Record_Manager::Instance().Record_Manager2Tuple(record, table);				//���vector<tuple>
			if (wherelists.empty()) //select * from table .where����Ϊ��
			{
				int table_attributes_size = table.Table_AttributesList.size(); //���ݱ����Եĸ���
				int i = 0;
				while (i < table_attributes_size)
				{
					SelectRecord selectrecord; //ѡ���һ�м�¼
					selectrecord.AttributesName = table.Table_AttributesList[i].Attributes_Name; //������ݱ��������
					CatalogTable catalogtable = Catalog::Instance().CatalogGet_CatalogTable(tablelists.front());//�õ����ݱ��ģʽ��Ϣ
					int Tableattributesnum = catalogtable.CatalogTable_AttribtuesNum;//�õ����Ե���Ŀ
					for (size_t k = 0; k < record.size(); k++)
					{
						selectrecord.selectrecords.push_back(record[i + k]); //��ȡ��i�����Եļ�¼��
						k += Tableattributesnum;
					}
					SelectRecords.push_back(selectrecord); //�ռ���i�����Եļ�¼��
				}
			}
			else
			{
				while (!wherelists.empty())
				{
					//select * from where���ִ���
					SelectRecord selectrecord; //ѡ���һ�м�¼
					string WhereAttributes = wherelists.front();				//where �������
					wherelists.pop();
					Operator_type Op = Intepretor::Op_Judge(wherelists.front());//where�е����Ժ������
					wherelists.pop();
					string WhereValues = wherelists.front();					//where�е���������ֵ
					int WhereAttributesNumber = Catalog::Instance().Catalog_GetAttributesNumber(WhereAttributes, table); //��ȡ���Ե�����ֵ�����ڼ�������
					int Tuple_Num = Tuple_Record.size();//��¼������
					int Sign = 0;
					CatalogTable catalogtable = Catalog::Instance().CatalogGet_CatalogTable(tablelists.front());//�õ����ݱ��ģʽ��Ϣ
					int Tableattributesnum = catalogtable.CatalogTable_AttribtuesNum;//�õ����Ե���Ŀ
					for (int i = 0; i < Tableattributesnum; i++)
					{
						while (Sign < Tuple_Num)
						{
							switch (Op)
							{
							case LESS_THAN:
								if (Intepretor::String2Int(Tuple_Record[Sign].Tuple_content[WhereAttributesNumber]) < Intepretor::String2Int(WhereValues))
								{
									selectrecord.AttributesName = table.Table_AttributesList[i].Attributes_Name;
									selectrecord.selectrecords.push_back(Tuple_Record[Sign].Tuple_content[i]);
								}
								break;
							case EQUAL:
								break;
							case MORE_THAN:
								break;
							case MORE_AND_EQUAL:
								break;
							case LESS_AND_EQUAL:
								break;
							case NOT_EQUAL:
								break;
							default:
								break;
							}
						}
					}
				}
			}
		}
	}
}