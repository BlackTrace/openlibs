#include "common.h"
#include "monitor2.h"

#define NtSuspendProcess_XP 253
#define NtSuspendThread_XP 254
#define NtResumeProcess_XP 205
#define NtResumeThread_XP 206
#define NtSuspendProcess_WIN7 366
#define NtSuspendThread_WIN7 367
#define NtResumeProcess_WIN7 303
#define NtResumeThread_WIN7 304

//�߳���Ϣ�����б�
typedef struct _ProcessThreadInfo
{   
	//�߳�ID
	ULONG ulTID;
	//PETHREAD��ַ
	ULONG ulPEThread;
	//TEB
	ULONG ulTEB;
	//��ַ
	ULONG ulBaseAddress;
	//�����б�ָ��
	struct PROCESS_THREAD_INFO* next;
}PROCESS_THREAD_INFO, *PPROCESS_THREAD_INFO; 


//���̾����Ϣ
typedef struct _ProcessHandleInfo
{   
	//�������
	WCHAR wstrHandleTypeName[25];
    //�������
	WCHAR wstrHandleName[256];
    //���
	ULONG ulHandle;
    //�������
	ULONG ulObject;
    //�����������
	ULONG ulTypeIndex;
    //���ü���
	ULONG ulReferenceCount;

	//�����б�ָ��
	struct PROCESS_HANDLE_INFO* next;
}PROCESS_HANDLE_INFO, *PPROCESS_HANDLE_INFO; 


//
PPROCESS_HANDLE_INFO pProcessHandleInfo,pProcessHandleInfoHead;


typedef NTSTATUS (*PPspTerminateThreadByPointer)(
	__in PETHREAD pEThread, 
	__in NTSTATUS status
	);


typedef NTSTATUS (*PNtUnmapViewOfSection)(
	IN HANDLE ProcessHandle,
	IN PVOID BaseAddress
	);


typedef NTSTATUS (*PMiUnmapViewOfSection)(
					 __in PEPROCESS  Process,
					 __in PVOID BaseAddress,
					 __in ULONG ulUnknown
					 );


typedef NTSTATUS (*PPsSuspendProcess) (
									   IN PEPROCESS Process
									   );

typedef NTSTATUS (*PPsResumeProcess) (
									  IN PEPROCESS Process
									  );

typedef ULONG (*PKeSuspendThread)(
								  IN PKTHREAD
								  );

typedef ULONG (*PKeResumeThread)(
								  IN PKTHREAD
								  );

//��������
PPspTerminateThreadByPointer PspTerminateThreadByPointer;
PMiUnmapViewOfSection MiUnmapViewOfSection;
PKeSuspendThread KeSuspendThread;
PKeResumeThread KeResumeThread;
PPsSuspendProcess PsSuspendProcess;
PPsResumeProcess PsResumeProcess;


//�������
BOOLEAN process_suspendProcess(ULONG ulPID);

//�ָ�����
BOOLEAN process_resumeProcess(ULONG ulPID);

//�ڴ����㣬ǿ�ƽ�������
BOOLEAN process_killProcess(ULONG ulPID);

//���ؽ���
BOOLEAN process_hideProcess(ULONG ulPID);

//����MiUnmapViewOfSection�ĵ�ַ
ULONG process_getMiUnmapViewOfSectionAddress();


//�õ�KeSuspendThread�ĵ�ַ
ULONG process_getKeSuspendThreadAddress();

//
BOOLEAN process_suspendThread(ULONG ulTID);

//
BOOLEAN process_resumeThread(ULONG ulTID);

//��������
BOOLEAN process_killProcessByPspTerminateThreadByPointer(ULONG ulPID);

//
BOOLEAN process_killThreadByPspTerminateThreadByPointer(ULONG ulPID,ULONG ulTID);


//ж��ģ��
//ulPID:����ID
//ulModuleBase:ģ���ַ
BOOLEAN process_unloadModule(ULONG ulPID,ULONG ulModuleBase);

//����ģ��
BOOLEAN process_hideModule(ULONG ulPID);


//���Ŀ����̵��߳���Ϣ
PPROCESS_THREAD_INFO process_getProcessThreadInfo(ULONG ulPID);

//����̵߳Ļ�ַ 
ULONG process_getThreadBaseAddr(ULONG ulPID,ULONG ulTID);

//��ý��̾����Ϣ
NTSTATUS process_getProcessHandleInfo(ULONG ulPID);

//��ô�С
ULONG process_getProcessHandleInfoCount();

//�������ݵ�ring3
NTSTATUS process_getProcessHandleInfo_send(PPROCESS_HANDLE_INFO pBuffer);

