//////////////////////////////////////////////////////////////////////////
//
//��ؽ��̺��̵߳Ĵ�����������
//
//
#ifndef _MONITOR2_
#define _MONITOR2_


#include "common.h"


#define PROCESS_TERMINATE         (0x0001) // winnt
#define PROCESS_CREATE_THREAD     (0x0002) // winnt
#define PROCESS_SET_SESSIONID     (0x0004) // winnt
#define PROCESS_VM_OPERATION      (0x0008) // winnt
#define PROCESS_VM_READ           (0x0010) // winnt
#define PROCESS_VM_WRITE          (0x0020) // winnt
// begin_ntddk begin_wdm begin_ntifs
#define PROCESS_DUP_HANDLE        (0x0040) // winnt
// end_ntddk end_wdm end_ntifs
#define PROCESS_CREATE_PROCESS    (0x0080) // winnt
#define PROCESS_SET_QUOTA         (0x0100) // winnt
#define PROCESS_SET_INFORMATION   (0x0200) // winnt
#define PROCESS_QUERY_INFORMATION (0x0400) // winnt
#define PROCESS_SET_PORT          (0x0800)
#define PROCESS_SUSPEND_RESUME    (0x0800) // winnt

#define THREAD_TERMINATE                 (0x0001)  
#define THREAD_SUSPEND_RESUME            (0x0002)  
#define THREAD_GET_CONTEXT               (0x0008)  
#define THREAD_SET_CONTEXT               (0x0010)  
#define THREAD_QUERY_INFORMATION         (0x0040)  
#define THREAD_SET_INFORMATION           (0x0020)  
#define THREAD_SET_THREAD_TOKEN          (0x0080)
#define THREAD_IMPERSONATE               (0x0100)
#define THREAD_DIRECT_IMPERSONATION      (0x0200)


//�µ�ObReferenceObjectByHandle����
NTSTATUS
NewObReferenceObjectByHandle(
							 IN HANDLE  Handle,
							 IN ACCESS_MASK  DesiredAccess,
							 IN POBJECT_TYPE  ObjectType  OPTIONAL,
							 IN KPROCESSOR_MODE  AccessMode,
							 OUT PVOID  *Object,
							 OUT POBJECT_HANDLE_INFORMATION  HandleInformation  OPTIONAL
						  );

//������InlineHook
VOID monitor2_inlineHookObReferenceObjectByHandle();

//�ָ�
VOID monitor2_unInlineHookObReferenceObjectByHandle();


//��DisableCreateProcessInfo�в���Ŀ�����ID
//����
//0��ʾû���ҵ���1��ʾ�ҵ���
ULONG monitor2_findPIDInDisableCreateProcessInfo(ULONG ulPID);


//��DisableCreateProcessInfo��ɾ��Ŀ�����ID
VOID monitor2_deletePIDInDisableCreateProcessInfo(ULONG ulPID);


//��DisableTerminateProcessInfo�в���Ŀ�����ID
//����
//0��ʾû���ҵ���1��ʾ�ҵ���
ULONG monitor2_findPIDInDisableTerminateProcessInfo(ULONG ulPID);


//��DisableTerminateProcessInfo��ɾ��Ŀ�����ID
VOID monitor2_deletePIDInDisableTerminateProcessInfo(ULONG ulPID);

#endif //_MONITOR2_