#ifndef _MONITOR_
#define _MONITOR_

#pragma once

//��������
typedef struct _MonitorProcessFilterInfo{
	//��������
	WCHAR ProcessName[256];

	//��־
	BOOL bProcessInclude;

	//ָ����һ��
	struct _MonitorProcessFilterInfo* next;
}MONITOR_PROCESS_FILTER_INFO,*PMONITOR_PROCESS_FILTER_INFO;

//��������
typedef struct _MonitorPathFilterInfo{
	//·��
	WCHAR PathName[256];
	//��־
	BOOL bPathInclude;

	//ָ����һ��
	struct _MonitorPathFilterInfo* next;
}MONITOR_PATH_FILTER_INFO,*PMONITOR_PATH_FILTER_INFO;

//��������
typedef struct _MonitorInfo{
	//ʱ��
	WCHAR Time[256];
	//��������
	WCHAR ProcessName[256];
	//
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
	_MonitorInfo* next;
}MONITOR_INFO,*PMONITOR_INFO;

#endif //_MONITOR_