#include "HyColorText.h"

//��ӡ��ɫ�ı�,�ɹ�����true�����򷵻�false
bool HyPrintColorText(const char *pText,USHORT color)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	//DWORD CharsWritten;
	if(!pText) return false;
	if(!SetConsoleTextAttribute(hOut,color)) return false;
	//if(!WriteConsole(hOut,pText,strlen(pText)+1,&CharsWritten,NULL)) return false;
	//����WriteConsole��ô�鷳����C�Ĵ�ӡ�����Ϳ����ˣ�
	printf(pText);
	//�ָ���ɫ���ֺͺ�ɫ����
	return SetConsoleTextAttribute(hOut,CLR_TEXT_WHITE);
}
