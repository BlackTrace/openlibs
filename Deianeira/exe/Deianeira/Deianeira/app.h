//app.h
#ifndef _APP_
#define _APP_
#include "..\common\common.h"

typedef enum _HOOK_TYPE{ 
	R_WH_MSGFILTER = -1, 
	R_WH_JOURNALRECORD = 0, 
	R_WH_JOURNALPLAYBACK = 1, 
	R_WH_KEYBOARD = 2, 
	R_WH_GETMESSAGE = 3, 
	R_WH_CALLWNDPROC = 4, 
	R_WH_CBT = 5, 
	R_WH_SYSMSGFILTER = 6, 
	R_WH_MOUSE = 7, 
	R_WH_HARDWARE = 8, 
	R_WH_DEBUG = 9, 
	R_WH_SHELL = 10, 
	R_WH_FOREGROUNDIDLE = 11, 
	R_WH_CALLWNDPROCRET = 12, 
	R_WH_KEYBOARD_LL = 13, 
	R_WH_MOUSE_LL = 14 
}HOOK_TYPE; 

typedef enum _HANDLE_TYPE 
{ 
	TYPE_FREE = 0, 
	TYPE_WINDOW = 1, 
	TYPE_MENU = 2, 
	TYPE_CURSOR = 3, 
	TYPE_SETWINDOWPOS = 4, 
	TYPE_HOOK = 5, 
	TYPE_CLIPDATA = 6, 
	TYPE_CALLPROC = 7, 
	TYPE_ACCELTABLE = 8, 
	TYPE_DDEACCESS = 9, 
	TYPE_DDECONV = 10, 
	TYPE_DDEXACT = 11, 
	TYPE_MONITOR = 12, 
	TYPE_KBDLAYOUT = 13, 
	TYPE_KBDFILE = 14, 
	TYPE_WINEVENTHOOK = 15, 
	TYPE_TIMER = 16, 
	TYPE_INPUTCONTEXT = 17, 
	TYPE_CTYPES = 18, 
	TYPE_GENERIC = 255 
}HANDLE_TYPE; 

typedef struct _HANDLEENTRY{ 
	PVOID phead; 
	ULONG pOwner; 
	BYTE bType; 
	BYTE bFlags; 
	WORD wUniq; 
}HANDLEENTRY,*PHANDLEENTRY; 

typedef struct _SERVERINFO{ 
	WORD wRIPFlags ; 
	WORD wSRVIFlags ; 
	WORD wRIPPID ; 
	WORD wRIPError ; 
	ULONG cHandleEntries; 
}SERVERINFO,*PSERVERINFO; 

typedef struct _SHAREDINFO{ 
	PSERVERINFO psi; 
	PHANDLEENTRY aheList; 
	ULONG pDispInfo; 
	ULONG ulSharedDelta; 
}SHAREDINFO,*PSHAREDINFO; 

typedef struct _HookInfo
{
	HANDLE hHandle;    //���ӵľ��
	DWORD Unknown1;
	PVOID Win32Thread; //һ��ָ�� win32k!_W32THREAD �ṹ���ָ��
	PVOID Unknown2;
	PVOID SelfHook;    //ָ��ṹ����׵�ַ
	PVOID NextHook;    //ָ����һ�����ӽṹ��
	int iHookType;     //���ӵ����ͣ� winuser.h ���ж���
	DWORD OffPfn;      //���Ӻ����ĵ�ַƫ��
	int iHookFlags;
	int iMod;          //���Ӻ�������ģ����������룬ͨ����ѯ WowProcess �ṹ���Եõ�ģ��Ļ���ַ��
	PVOID Win32ThreadHooked; //�������̵߳Ľṹָ��
} HOOK_INFO,*PHOOK_INFO;

typedef enum _SYSDBG_COMMAND 
{ 
	SysDbgQueryModuleInformation, 
	SysDbgQueryTraceInformation, 
	SysDbgSetTracepoint, 
	SysDbgSetSpecialCall, 
	SysDbgClearSpecialCalls, 
	SysDbgQuerySpecialCalls, 
	SysDbgCopyMemoryChunks_0 = 8 
} SYSDBG_COMMAND, *PSYSDBG_COMMAND; 

typedef struct _MEMORY_CHUNKS 
{ 
	ULONG ulAddress; 
	PVOID pBuffer; 
	ULONG ulSize; 
}MEMORY_CHUNKS, *PMEMORY_CHUNKS; 


typedef NTSTATUS (WINAPI *PNtSystemDebugControl)( 
	IN SYSDBG_COMMAND Command, 
	IN PVOID InputBuffer, 
	IN ULONG InputBufferLength, 
	OUT PVOID OutputBuffer, 
	IN ULONG OutputBufferLength, 
	OUT PULONG ReturnLength 
	); 

typedef NTSTATUS (WINAPI *PNtReadVirtualMemory)(
	IN HANDLE ProcessHandle, 
	IN PVOID BaseAddress, 
	OUT PVOID Buffer, 
	IN ULONG NumberOfBytesToRead, 
	OUT PULONG NumberOfBytesRead OPTIONAL
	); 

typedef struct _AppMsgInfo{
	//���
	HANDLE hMsg;
	//����
    WCHAR  Type[20];
	//����
	ULONG  ulFuncAddr;
	//�߳�ID
	ULONG  ulTID;
	//����ID
	ULONG  ulPID;
	//��־�Ƿ�ȫ��
	bool bGlobal;
	//next
	_AppMsgInfo* next;
}APP_MSG_INFO,*PAPP_MSG_INFO;
 
extern PAPP_MSG_INFO pAppMsgInfo,pAppMsgInfoHead;

void app_freeMsgInfo();
bool app_getMsgInfo(bool bGetGlobal);

#endif //_APP_