#ifndef _KERNEL_
#define _KERNEL_

#include "..\common\common.h"

//��⵽��ϵͳ�ص������е���Ϣ
typedef struct _KernelSystemCallbackInfo{
	//�ص�����
	ULONG ulType;
	//����·��
	WCHAR PathName[256];
	//
	ULONG ulEntryAddress;
	//�ص�������ַ
	ULONG ulCallBackAddress;
	//������Ϣ
	ULONG ulAttatch;

	//ָ����һ��
	_KernelSystemCallbackInfo* next;
}KERNEL_SYSTEM_CALLBACK_INFO,*PKERNEL_SYSTEM_CALLBACK_INFO;

//��⵽�Ĺ�����������Ϣ
typedef struct _KernelFilterDriverInfo{
	ULONG ulObjType;
	WCHAR FileName[64];
	WCHAR FilePath[256];
	ULONG ulAttachDevice;
	//����
	WCHAR HostFileName[64];
	//ָ����һ��
	_KernelFilterDriverInfo* next;
}KERNEL_FILTERDRIVER_INFO,*PKERNEL_FILTERDRIVER_INFO;

#endif //_KERNEL_