#include "Catalog.h"
#include "Error.h"
void Table_Type::InsertColumn(Column_Type column)
{
	for (auto i = Table_Column.begin(); i != Table_Column.end(); i++)
	{
		if (i->Column_TypeName == column.Column_TypeName)
			throw Error();//�׳����󣬼������Ѿ�����
	}
	Table_Column.push_back(column);
}