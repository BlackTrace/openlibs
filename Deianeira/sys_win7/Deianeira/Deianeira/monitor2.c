#include "monitor2.h"


//����ԭ�ȿ�ͷ��5���Լ�
BYTE OldObAddress[5]={0};
//jmp�µĺ�����ַ
BYTE NewObAddress[5]={0xe9,0,0,0,0};


//������InlineHook
VOID monitor2_inlineHookObReferenceObjectByHandle()
{

	//��ֵǰ�涨�������
	KIRQL Irql;

	//���溯��ǰ����ֽ�����
	RtlCopyMemory(OldObAddress,(PBYTE)ObReferenceObjectByHandle,5);
	//�����º�������ֽ�֮��ƫ��
	*(PULONG)(NewObAddress+1)=(ULONG)NewObReferenceObjectByHandle-((ULONG)ObReferenceObjectByHandle+5);

	//��ʼinline hook
	//�ر��ڴ�д����
	UnProtected();

	//����IRQL�жϼ�
	Irql=KeRaiseIrqlToDpcLevel();
	//������ͷ����ֽ�дJMP 
	RtlCopyMemory((PBYTE)ObReferenceObjectByHandle,NewObAddress,5);
	//�ָ�Irql
	KeLowerIrql(Irql);

	//�����ڴ�д����
	Protected();
}


//�ָ�
VOID monitor2_unInlineHookObReferenceObjectByHandle()
{

	//������ֽ���д�ص�ԭ����
	KIRQL Irql;

	//�ر�д����
	UnProtected();

	//����IRQL��Dpc
	Irql=KeRaiseIrqlToDpcLevel();
	//��ԭ�ȱ���ĵ�ַ����ǰ5���ֽ�
	RtlCopyMemory((PBYTE)ObReferenceObjectByHandle,OldObAddress,5);
	//����IRQL��Dpc
	KeLowerIrql(Irql);

	//����д����
	Protected();
}


//�Լ������ԭʼ����
_declspec(naked) NTSTATUS OldObReferenceObjectByHandle(
	IN HANDLE  Handle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_TYPE  ObjectType  OPTIONAL,
	IN KPROCESSOR_MODE  AccessMode,
	OUT PVOID  *Object,
	OUT POBJECT_HANDLE_INFORMATION  HandleInformation  OPTIONAL
	)
{
	_asm
	{   
		mov edi,edi
			push ebp
			mov ebp,esp
			mov eax,ObReferenceObjectByHandle
			add eax,5
			jmp eax                
	}
}

//�µ�ObReferenceObjectByHandle����
NTSTATUS
NewObReferenceObjectByHandle(
							 IN HANDLE  Handle,
							 IN ACCESS_MASK  DesiredAccess,
							 IN POBJECT_TYPE  ObjectType  OPTIONAL,
							 IN KPROCESSOR_MODE  AccessMode,
							 OUT PVOID  *Object,
							 OUT POBJECT_HANDLE_INFORMATION  HandleInformation  OPTIONAL
						  )
{
	//Drivers can use the following flags for handles to processes:
	//
	//Flag Allowed operations 
	//PROCESS_CREATE_PROCESS Create a new child process of the process. 
	//PROCESS_CREATE_THREAD Create a new thread in the context of the process. 
	//PROCESS_DUP_HANDLE Duplicate handles to or from the context of the process, such as by calling the user-mode DuplicateHandle routine. 
	//PROCESS_SET_QUOTA Set the working set size for the process, such as by calling the user-mode SetProcessWorkingSetSize routine. 
	//PROCESS_SET_INFORMATION Modify process settings, such as by calling the user-mode SetPriorityClass routine. 
	//PROCESS_SUSPEND_RESUME Suspend or resume the process. 
	//PROCESS_TERMINATE Terminate the process, such as by calling the user-mode TerminateProcess routine.. 
	//PROCESS_VM_OPERATIONS Modify the address space of the process, such as by calling the user-mode WriteProcessMemory and VirtualProtectEx routines. 
	//PROCESS_VM_WRITE Write to the address space of the process, such as by calling the user-mode WriteProcessMemory routine. 
	//
	//Drivers can use the following flags for handles to threads:
	//
	//Flag Allowed operations 
	//THREAD_DIRECT_IMPERSONATION Enable a server thread to impersonate one of its clients. 
	//THREAD_IMPERSONATE Impersonate the operating system's anonymous logon token, such as by calling the user-mode ImpersonateAnonymousToken routine. 
	//THREAD_SET_CONTEXT Modify the thread's execution context, such as by calling the user-mode SetThreadContext routine. 
	//THREAD_SET_INFORMATION Modify thread settings, such as by calling the user-mode SetThreadIdealProcessor routine. The operations that are permitted by this access right are a superset of those that are permitted by the THREAD_SET_LIMITED_INFORMATION access right. 
	//THREAD_SET_LIMITED_INFORMATION Modify a limited set of thread settings, such as by calling the user-mode SetThreadAffinityMask and SetThreadPriorityBoost routines. 
	//THREAD_SET_THREAD_TOKEN Modify properties of the thread's impersonation token, such as by calling the user-mode SetTokenInformation routine. 
	//THREAD_SUSPEND_RESUME Suspend or resume the thread, such as by calling the user-mode SuspendThread and ResumeThread routines. 
	//THREAD_TERMINATE Terminate the thread, such as by calling the user-mode TerminateThread routine. 

	//ObReferenceObjectByHandle returns an NTSTATUS value. The possible return values include:

	//STATUS_SUCCESS
	//STATUS_OBJECT_TYPE_MISMATCH
	//STATUS_ACCESS_DENIED
	//STATUS_INVALID_HANDLE

	NTSTATUS status,status2;

	//����ԭ����
	status=OldObReferenceObjectByHandle(Handle,DesiredAccess,ObjectType,AccessMode,Object,HandleInformation);

	//����ɹ�
	if (NT_SUCCESS(status))
	{
		if(ObjectType == *PsProcessType)
		{
			switch(DesiredAccess)
			{
				//��������
			case PROCESS_CREATE_PROCESS:
				{
					//�������ֹ��������
					if (bGlobal_disableCreateProcess)
					{
						ObDereferenceObject(*Object);
						//���ؾ����Ч
						return STATUS_ACCESS_DENIED;
					}

					//������ָ��Ŀ����̲���
					if (b_disableCreateProcess)
					{
						PDISABLE_CREATE_PROCESS p = NULL;
						PEPROCESS pEpr = NULL;

						//���������б�
						for (p=pDisableCreateProcessInfo;p;p=p->next)
						{
							status2 = PsLookupProcessByProcessId((HANDLE)p->ulPID,&pEpr);

							if (NT_SUCCESS(status2))
							{
								//���Ŀ����̵�EPROCESS ���� ring3�µ�Ӧ�ó���EPROCESS
								if (PsGetCurrentProcess() == pEpr)
								{
									ObDereferenceObject(*Object);
									//���ؾ����Ч
									return STATUS_ACCESS_DENIED;
								}
							}																			
						}		
					}
				}
				//�����߳�
			case PROCESS_CREATE_THREAD:
				{
					//�������ֹ�����߳�
					if (bGlobal_disableCreateThread)
					{
						ObDereferenceObject(*Object);
						//���ؾ����Ч
						return STATUS_ACCESS_DENIED;
					}
				}
			case PROCESS_DUP_HANDLE:
			case PROCESS_SET_QUOTA:
			case PROCESS_SET_INFORMATION:
			case PROCESS_SUSPEND_RESUME:
				//��������
			case PROCESS_TERMINATE:
				{
					//�������ֹ��������
					if (bGlobal_disableTerminateProcess)
					{
						ObDereferenceObject(*Object);
						//���ؾ����Ч
						return STATUS_ACCESS_DENIED;
					}
					//������ָ��Ŀ����̲���
					if (b_disableTerminateProcess)
					{
						PDISABLE_TERMINATE_PROCESS p = NULL;
						PEPROCESS pEpr = NULL;

						//���������б�
						for (p=pDisableTerminateProcessInfo;p;p=p->next)
						{
							status2 = PsLookupProcessByProcessId((HANDLE)p->ulPID,&pEpr);

							if (NT_SUCCESS(status2))
							{
								//���Ŀ����̵�EPROCESS ���� ring3�µ�Ӧ�ó���EPROCESS
								if ((PEPROCESS)(*Object) == pEpr)
								{
									ObDereferenceObject(*Object);
									//���ؾ����Ч
									return STATUS_ACCESS_DENIED;
								}
							}																			
						}		
					}
				}
			case PROCESS_VM_OPERATION:
			case PROCESS_VM_WRITE:
				{
					if(b_startSelfProtectionByObReferenceObjectByHandle)
					{
						//����ǽ����Լ������Լ��Ļ�
						if(PsGetCurrentProcessId() == ulProcessID)
						{
							return status;
						}

						//��������ڲ����Լ��Ľ��̾���Ļ�
						//���Ŀ����̵�EPROCESS ���� ring3�µ�Ӧ�ó���EPROCESS
						if ((PEPROCESS)(*Object) == pEProcess)
						{
							ObDereferenceObject(*Object);
							//���ؾ����Ч
							return STATUS_ACCESS_DENIED;
						}
					}
				}
				break;
			}
		}
		else if(Object == *PsThreadType)
		{
			switch(DesiredAccess)
			{
				  //�߳̽���
			case THREAD_TERMINATE:
				  {
					  //�������ֹ�����߳�
					  if (bGlobal_disableTerminateThread)
					  {
						  ObDereferenceObject(*Object);
						  //���ؾ����Ч
						  return STATUS_ACCESS_DENIED;
					  }
				  }
				  break;
			}
		}
	}

	return status;
}

//��DisableCreateProcessInfo�в���Ŀ�����ID
//����
//0��ʾû���ҵ���1��ʾ�ҵ���
ULONG monitor2_findPIDInDisableCreateProcessInfo(ULONG ulPID)
{
	PDISABLE_CREATE_PROCESS p = NULL;

	__try
	{
		//���������б�
		for (p=pDisableCreateProcessInfo;p;p=p->next)
		{
			if (p->ulPID == ulPID)
			{
				//��ʾ�ҵ���
				return 1;
			}
		}

		//û�ҵ��Ļ�
		return 0;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("monitor2_findPIDInDisableCreateProcessInfo EXCEPTION_EXECUTE_HANDLER error!");
		return 0;
	}
}

//��DisableCreateProcessInfo��ɾ��Ŀ�����ID
VOID monitor2_deletePIDInDisableCreateProcessInfo(ULONG ulPID)
{
	PDISABLE_CREATE_PROCESS p = NULL;
	PDISABLE_CREATE_PROCESS p2 = NULL;

	__try
	{
		for (p2 = p =pDisableCreateProcessInfo;p;p2=p,p=p->next)
		{
			if (p->ulPID == ulPID)
			{
				if (pDisableCreateProcessInfo==p)
				{
					pDisableCreateProcessInfo = pDisableCreateProcessInfo->next;
				}
				else
				{
					p2->next=p->next;
				}
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("monitor2_deletePIDInDisableCreateProcessInfo EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//��DisableTerminateProcessInfo�в���Ŀ�����ID
//����
//0��ʾû���ҵ���1��ʾ�ҵ���
ULONG monitor2_findPIDInDisableTerminateProcessInfo(ULONG ulPID)
{
	PDISABLE_TERMINATE_PROCESS p = NULL;

	//���������б�
	for (p=pDisableTerminateProcessInfo;p;p=p->next)
	{
		if (p->ulPID == ulPID)
		{
			//��ʾ�ҵ���
			return 1;
		}
	}

	//û�ҵ��Ļ�
	return 0;
}

//��DisableTerminateProcessInfo��ɾ��Ŀ�����ID
VOID monitor2_deletePIDInDisableTerminateProcessInfo(ULONG ulPID)
{
	PDISABLE_TERMINATE_PROCESS p = NULL;
	PDISABLE_TERMINATE_PROCESS p2 = NULL;

	__try
	{
		for (p2 = p =pDisableTerminateProcessInfo;p;p2=p,p=p->next)
		{
			if (p->ulPID == ulPID)
			{
				if (pDisableTerminateProcessInfo==p)
				{
					pDisableTerminateProcessInfo = pDisableTerminateProcessInfo->next;
				}
				else
				{
					p2->next=p->next;
				}
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("monitor2_deletePIDInDisableTerminateProcessInfo EXCEPTION_EXECUTE_HANDLER error!");
	}
}