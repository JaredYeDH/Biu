#include "Buffer.h"
void M_Buffer::Start()
{
	if (MEM_PAGENUM < 3) throw 1033;//����Ҫ���� 3 ҳ,2ҳͷ��Ϣ(Ŀ¼��������������һҳ),���� 1 ҳ��¼��ȡ
	this->F_First = NULL;
	this->F_Last = NULL;
	this->F_Current = NULL;
	this->FileCount = 0;
	this->MemPageClock = new M_Clock();
}