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
				int wherelistsNum = 0;
				for (int i = 0; i < wherelists.size();i++)
				{
					SelectRecord selectrecord; //ѡ���һ�м�¼
					string WhereAttributes = wherelists.front();			//where �������
					wherelists.pop();
					wherelists.push(WhereAttributes);
					Operator_type Op = Intepretor::Op_Judge(wherelists.front());//where�е����Ժ������
					wherelists.push(wherelists.front());
					wherelists.pop();
					string WhereValues = wherelists.front();					//where�е���������ֵ
					wherelists.pop();
					wherelists.push(WhereValues);
					i += 3;
					int WhereAttributesNumber = Catalog::Instance().Catalog_GetAttributesNumber(WhereAttributes, table); //��ȡ���Ե�����ֵ�����ڼ�������
					if (WhereAttributesNumber != -1) //���ݱ������������
					{
						int Tuple_Num = Tuple_Record.size();//��¼������
						int Sign = 0;
						CatalogTable catalogtable = Catalog::Instance().CatalogGet_CatalogTable(tablelists.front());//�õ����ݱ��ģʽ��Ϣ
						int Tableattributesnum = catalogtable.CatalogTable_AttribtuesNum;//�õ����Ե���Ŀ
						for (int i = 0; i < Tableattributesnum; i++)
						{
							while (Sign < Tuple_Num)
							{
								selectrecord.AttributesName = table.Table_AttributesList[i].Attributes_Name;
								switch (Op)
								{
								case LESS_THAN:
									if (Intepretor::String2Int(Tuple_Record[Sign].Tuple_content[WhereAttributesNumber]) < Intepretor::String2Int(WhereValues))
										selectrecord.selectrecords.push_back(Tuple_Record[Sign].Tuple_content[i]);
									Sign++;
									break;
								case EQUAL:
									if (Intepretor::String2Int(Tuple_Record[Sign].Tuple_content[WhereAttributesNumber]) == Intepretor::String2Int(WhereValues))
										selectrecord.selectrecords.push_back(Tuple_Record[Sign].Tuple_content[i]);								
									Sign++;
									break;
								case MORE_THAN:
									if (Intepretor::String2Int(Tuple_Record[Sign].Tuple_content[WhereAttributesNumber]) > Intepretor::String2Int(WhereValues))
										selectrecord.selectrecords.push_back(Tuple_Record[Sign].Tuple_content[i]);
									Sign++;
									break;
								case MORE_AND_EQUAL:
									if (Intepretor::String2Int(Tuple_Record[Sign].Tuple_content[WhereAttributesNumber]) <= Intepretor::String2Int(WhereValues))
										selectrecord.selectrecords.push_back(Tuple_Record[Sign].Tuple_content[i]);
									
									Sign++;
									break;
								case LESS_AND_EQUAL:
									if (Intepretor::String2Int(Tuple_Record[Sign].Tuple_content[WhereAttributesNumber]) >= Intepretor::String2Int(WhereValues))
										selectrecord.selectrecords.push_back(Tuple_Record[Sign].Tuple_content[i]);									
									Sign++;
									break;
								default:
									break;
								}
							}
							SelectRecords.push_back(selectrecord);
						}
					}
					WhereAttributesNumber++;
				}
			}
			tablelists.pop();
		}
	}
	else //�� select * form...
	{
		int SelectAttributesnumber = attributes.size(); //Select���Եĸ���
		int SelectTablenumber = tablelists.size();  //Select���ݱ�ĸ���
		int TableNum = 0;
		int AttributesNum = 0;
		string StrRecord;
		vector<string> record; //���ݱ��еļ�¼vector����
		Table table = Catalog::Instance().CatalogGet_Table(TableLists.front());		//�õ����ݱ�
		int table_blocknum = Buffer_Manager::Instance().Buffer_ManagerReadLastNumber(table.Table_Name, StrRecord);	//��ȡ���ݱ�Ŀ��
		Buffer_Manager::Instance().Buffer_ManagerRead(table.Table_Name, table_blocknum, StrRecord);					//�ӻ�������ȡ��¼
		record = Record_Manager::Instance().Record_ManagerString2Tuple(table.Table_AttributesList, StrRecord);		//���ַ����ָ�ΪԪ��,����vector<stirng>
		vector<Tuple> Tuple_Record = Record_Manager::Instance().Record_Manager2Tuple(record, table);				//���vector<tuple>
		while (TableNum<SelectTablenumber)
		{
			Table table = Catalog::Instance().CatalogGet_Table(tablelists.front());		//�õ����ݱ�
			SelectRecord selectrecord; //ѡ���һ�м�¼
			selectrecord.AttributesName = attributes.front(); //�õ��û�select���Ե�����
			string WhereAttributes = wherelists.front();			//where �������
			wherelists.pop();
			wherelists.push(WhereAttributes);
			Operator_type Op = Intepretor::Op_Judge(wherelists.front());//where�е����Ժ������
			wherelists.push(wherelists.front());
			wherelists.pop();
			string WhereValues = wherelists.front();					//where�е���������ֵ
			wherelists.pop();
			wherelists.push(WhereValues);
			while (AttributesNum < attributes.size())
			{
				int WhereAttributesNumber = Catalog::Instance().Catalog_GetAttributesNumber(WhereAttributes, table); //��ȡ���Ե�����ֵ�����ڼ�������
				int SelectAttributesNumber = Catalog::Instance().Catalog_GetAttributesNumber(attributes.front(), table);//�����Ҫ�����Ե�����ֵ
				if (WhereAttributesNumber != -1)
				{
					int Tuple_Num = Tuple_Record.size();//��¼������
					int Sign = 0;
					CatalogTable catalogtable = Catalog::Instance().CatalogGet_CatalogTable(tablelists.front());//�õ����ݱ��ģʽ��Ϣ
					int Tableattributesnum = catalogtable.CatalogTable_AttribtuesNum;//�õ����Ե���Ŀ
					for (int i = SelectAttributesNumber; i < Tableattributesnum; i++)
					{
						switch (Op)
						{
						case LESS_THAN:
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