#include "Record_Manager.h"
//Create����
bool Record_Manager::Record_ManagerCreateTable(string &tablename, const vector<Attributes>&Table_Column)
{
	string FileName = tablename + ".table";
	string Empty_Block(Block_Size, 0);						//����һ���µĿ�
	int Tuple_size = 0;										//���ݱ���Ԫ����Ҫ������ڴ��С
	for (size_t i = 0; i < Table_Column.size(); i++)			//Table_Column.size()���ݱ�����Ը���
		Tuple_size += Table_Column[i].Attributes_Length;
	Tuple_size++;
	int Offset = 0;//ƫ����
	while ((Offset + Tuple_size) < Block_Size)
	{
		Empty_Block.replace(Offset + Tuple_size - 1, 1, "1");//�滻
		Offset += Tuple_size;
	}
	Buffer_Manager::Instance().Buffer_ManagerWrite(FileName,Empty_Block);			//ͨ������������д���ļ�
	return true;
}
//Insert into �����¼
bool  Record_Manager::Record_ManagerInsert_Into(Table &table, vector<Tuple> Tuple_Lists)
{
	vector<pair<Tuple, int>> insert_tuple;
	string filename = table.Table_Name + ".table";
	char Dirty = '0';
	int Lengh = table.Table_Length() + 1;           //һ����¼�ĳ���
	string InsertContent = "";						//�û�����ļ�¼�ַ���
	string str;
	int blocknum = Buffer_Manager::Instance().Buffer_ManagerReadLastNumber(table.Table_Name, str); //��ÿ��
	for (size_t i = 0; i < Tuple_Lists.size(); i++)
	{
		Tuple Insert_Tuple = Tuple_Lists[i];     //һ����¼
		for (size_t j = 0; j < Insert_Tuple.Tuple_content.size(); i++) //һ����¼�е�һ���ֶ�
		{
			if (table.Table_AttributesList[j].Attributes_Unique()== true)//�������ֶε�������Unique
			{
				if (Record_ManagerHasExisted(table, Insert_Tuple.Tuple_content[j], j, blocknum) == true) //���ݵĲ���Ϊ���ݱ� table,�ֶε����ݣ��ڼ������ԣ����
					return false;
				Insert_Tuple.Tuple_content[j].resize(table.Table_AttributesList[j].Attributes_Length, 0);
				InsertContent += Insert_Tuple.Tuple_content[j];
			}
		}
		InsertContent += Dirty;
		for (int k = 0; k <= blocknum; k++) //�����ִ���еĿռ�
		{
			string Strout;
			Buffer_Manager::Instance().Buffer_ManagerRead(filename, i, Strout);	//��ȡ���е�����
			int Offset = Record_ManagerFindDirtyTuple(Strout, Lengh);			//�ҵ����п���ռ��ƫ����
			if (Offset != -1)
			{
				Strout.replace(Offset, Lengh, InsertContent);
				Buffer_Manager::Instance().Buffer_ManagerWrite(filename, Strout, i);
				insert_tuple.push_back(make_pair(Insert_Tuple, i*Block_Size + Offset));
			}
			else
			{
				continue;//���Offset=-1,˵���ÿ��в������Ѿ�ɾ���ļ�¼����������һ�����
			}
		}
		int i;
		if (i > blocknum) //�������Ŀ�����
		{
			InsertContent.resize(Block_Size, 0);
			Buffer_Manager::Instance().Buffer_ManagerWrite(filename, InsertContent);
			insert_tuple.push_back(make_pair(Insert_Tuple,blocknum*Block_Size));
		}
	}
	API::Instance().FixIndex(table, insert_tuple);				//�޸�����
	return true;
	
}
//Ԫ���Ƿ��Ѿ�����,num����ڼ������ԣ�contentΪ�ֶ�����
bool Record_Manager::Record_ManagerHasExisted(Table &table, string &content, int num, int blocknum)
{
	bool Has = false;
	vector<Tuple> SearchTuple = Record_ManagerSelectTuple(table,blocknum);//������Ѱ��Ԫ��
	for (int i = 0; i <= SearchTuple.size(); i++)
	{
		if (SearchTuple[i].Tuple_content[num] == content)
		{
			Has = true;
			break;
		}
	}

}
//���ѡ���Ԫ��
vector<Tuple> Record_Manager::Record_ManagerSelectTuple(Table & table,int blocknum)
{
	string filename = table.Table_Name + ".table";
	vector<Tuple> Selected;
	size_t size = table.Table_Length() + 1; //���ݱ�һ����¼�ĳ���,��1�ǰ���dirty��־λ
	for (int i = 0; i < blocknum; i++)//�������п�
	{
		string strout;
		Buffer_Manager::Instance().Buffer_ManagerRead(filename, i, strout); //����Block������,stroutΪ�����������
		size_t Pointer = 0;
		while ((Pointer + size) < strout.size()) //һ����¼һ����¼�Ļ�ȡ
		{
			string substring = strout.substr(Pointer, size); //��Pointer��ʼ������size��С���ַ���
			vector<string> vec = Record_ManagerString2Tuple(table.Table_AttributesList, substring);//���ַ����ָ�ΪԪ��
			Tuple * NewTuple = new Tuple(vec);  //��Vector<string> ����ΪVector<Tuple>����
			Selected.push_back(*NewTuple);
			Pointer += size;
		}
	}
	return Selected;
}
//��stringתΪTuple �������ʽ��ȻΪvector<string>
vector<string> Record_Manager::Record_ManagerString2Tuple(vector<Attributes> attribtues, string tuple_str)
{
	vector<Attributes>::iterator i;//���ݱ�Ԫ�ص�����ָ��
	vector<string> result;
	int ptr = 0;
	for (i = attribtues.begin(); i != attribtues.end(); i++)
	{
		string tmp = tuple_str.substr(ptr, (*i).Attributes_Length);
		ptr += (*i).Attributes_Length;
		result.push_back(tmp);
	}
	return result;
}
//
int Record_Manager::Record_ManagerFindDirtyTuple(string &strout, int size)
{
	int Offset = 0;
	while ((size_t)(Offset + size) < strout.size())
	{
		string str = strout.substr(Offset, size);	
		string Dirty = str.substr(size - 1, 1);		//size-1 ��ȫ���滻Ϊ1
		if (Dirty == "1")							//����ҵ����޸ĵ�λ��				
			return Offset;							//����ƫ����
		Offset += size;
	}
	return -1;
}