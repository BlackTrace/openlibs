/*
builder.exe��Դ���룬���ڴ���һ������̨�����ҳ�ʼ��GCC�Լ�DDK�Ļ�����
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <windows.h>

int main(void)
{
	WinExec("cmd /k \"b.bat\" p",SW_SHOWNORMAL);
	WinExec("start cmd /k \"db.bat\"",SW_SHOWNORMAL);
	return 0;
}
