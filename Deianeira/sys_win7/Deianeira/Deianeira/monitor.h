
//��ֹ�ض���
#ifndef _MONITOR_
#define _MONITOR_

#include "common.h"

	
//��������
typedef struct _MonitorProcessFilterInfo{
	//��������
	WCHAR ProcessName[256];

	//��־
	BOOL bProcessInclude;

	//ָ����һ��
	struct MONITOR_PROCESS_FILTER_INFO* next;
}MONITOR_PROCESS_FILTER_INFO,*PMONITOR_PROCESS_FILTER_INFO;

//��������
typedef struct _MonitorPathFilterInfo{
	//·��
	WCHAR PathName[256];
	//��־
	BOOL bPathInclude;

	//ָ����һ��
	struct MONITOR_PATH_FILTER_INFO* next;
}MONITOR_PATH_FILTER_INFO,*PMONITOR_PATH_FILTER_INFO;


//��������
typedef struct _MonitorInfo{
	//ʱ��
	WCHAR Time[256];
	//��������
	WCHAR ProcessName[256];
	//����·��
	WCHAR ProcessPath[256];
	//����ID
	ULONG ulPID;
	//����
	WCHAR Action[256];
	//·��
	WCHAR PathName[256];
	//���
	WCHAR Result[256];
	//��ϸ
	WCHAR Other[256];

	//ָ����һ��
	struct MONITOR_INFO* next;
}MONITOR_INFO,*PMONITOR_INFO;

PMONITOR_PROCESS_FILTER_INFO pMonitorProcessFilterInfo,pMonitorProcessFilterInfoHead;
PMONITOR_PATH_FILTER_INFO pMonitorPathFilterInfo,pMonitorPathFilterInfoHead;

PMONITOR_INFO pMonitorInfo,pMonitorInfoHead;
MUTEX_TYPE       MKMutex;


// ���̻ص�����
VOID ProcessCallbackFunc(IN HANDLE  hParentId, IN HANDLE  hProcessId, IN BOOLEAN bCreate);

//�̻߳ص�����
VOID ThreadCallbackFunc(IN HANDLE  hProcessId, IN HANDLE  hThreadId, IN BOOLEAN bCreate);

// LoadImage�ص�����
VOID LoadImageCallbackFunc(IN PUNICODE_STRING  pUniImageName, IN HANDLE  hProcessId, IN PIMAGE_INFO pImageInfo);


NTSTATUS monitor_updateProcessMonitorFilerInfo(PMONITOR_PROCESS_FILTER_INFO p);

NTSTATUS monitor_updatePathMonitorFilerInfo(PMONITOR_PATH_FILTER_INFO p);

void monitor_freeMonitorFilerInfo();

//�������
NTSTATUS monitor_start();

//ֹͣ���
NTSTATUS monitor_stop();

//�ͷ��ڴ�
VOID monitor_free();

//��ô�С
ULONG monitor_getCount();

//�������ݵ�ring3
NTSTATUS monitor_getInfo(PMONITOR_INFO pBuffer);

#endif //_MONITOR_