#include "reg.h"


PVOID NtCreateKey = NULL;
PVOID NtSetValueKey = NULL;
PVOID NtDeleteKey = NULL;
PVOID NtDeleteValueKey = NULL;
PVOID NtQueryKey=NULL;
PVOID NtQueryValueKey = NULL;


//�µĴ�����
NTSTATUS NewObReferenceObjectByHandle_forNtCreateKeyAndNtSetValueKey(
	IN HANDLE  Handle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_TYPE  ObjectType  OPTIONAL,
	IN KPROCESSOR_MODE  AccessMode,
	OUT PVOID  *Object,
	OUT POBJECT_HANDLE_INFORMATION  HandleInformation  OPTIONAL
	)
{
	//ȫ��
	if (bGlobal_disableCreateReg)
	{
		//ֱ�ӷ���ʧ��
		return STATUS_ACCESS_DENIED;
	}

	//������ָ��Ŀ����̲���
	if (b_disableCreateReg)
	{
		NTSTATUS status;
		PDISABLE_CREATE_REG p= NULL;
		PEPROCESS pEpr = NULL;

		//���������б�
		for (p=pDisableCreateRegInfo;p;p=p->next)
		{
			status = PsLookupProcessByProcessId((HANDLE)p->ulPID,&pEpr);

			if (NT_SUCCESS(status))
			{
				//���Ŀ����̵�EPROCESS ���� ring3�µ�Ӧ�ó���EPROCESS
				if (PsGetCurrentProcess() == pEpr)
				{
					//���ؾ����Ч
					return STATUS_ACCESS_DENIED;
				}
			}																			
		}		
	}

	return ObReferenceObjectByHandle(Handle,DesiredAccess,ObjectType,AccessMode,Object,HandleInformation);
}

//�µĴ�����
NTSTATUS NewObReferenceObjectByHandle_forNtDeleteKeyAndNtDeleteVauleKey(
	IN HANDLE  Handle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_TYPE  ObjectType  OPTIONAL,
	IN KPROCESSOR_MODE  AccessMode,
	OUT PVOID  *Object,
	OUT POBJECT_HANDLE_INFORMATION  HandleInformation  OPTIONAL
	)
{
	if (bGlobal_disableDeleteReg)
	{
		//ֱ�ӷ���ʧ��
		return STATUS_ACCESS_DENIED;
	}

	//������ָ��Ŀ����̲���
	if (b_disableDeleteReg)
	{
		NTSTATUS status;
		PDISABLE_DELETE_REG p= NULL;
		PEPROCESS pEpr = NULL;

		//���������б�
		for (p=pDisableDeleteRegInfo;p;p=p->next)
		{
			status = PsLookupProcessByProcessId((HANDLE)p->ulPID,&pEpr);

			if (NT_SUCCESS(status))
			{
				//���Ŀ����̵�EPROCESS ���� ring3�µ�Ӧ�ó���EPROCESS
				if (PsGetCurrentProcess() == pEpr)
				{
					//���ؾ����Ч
					return STATUS_ACCESS_DENIED;
				}
			}																			
		}		
	}

	return ObReferenceObjectByHandle(Handle,DesiredAccess,ObjectType,AccessMode,Object,HandleInformation);
}

//�µĴ�����
NTSTATUS NewObReferenceObjectByHandle_forNtQueryKeyAndNtQueryValueKey(
	IN HANDLE  Handle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_TYPE  ObjectType  OPTIONAL,
	IN KPROCESSOR_MODE  AccessMode,
	OUT PVOID  *Object,
	OUT POBJECT_HANDLE_INFORMATION  HandleInformation  OPTIONAL
	)
{
	if (bGlobal_disableOpenReg)
	{
		//ֱ�ӷ���ʧ��
		return STATUS_ACCESS_DENIED;
	}

	//������ָ��Ŀ����̲���
	if (b_disableOpenReg)
	{
		NTSTATUS status;
		PDISABLE_OPEN_REG p= NULL;
		PEPROCESS pEpr = NULL;

		//���������б�
		for (p=pDisableOpenRegInfo;p;p=p->next)
		{
			status = PsLookupProcessByProcessId((HANDLE)p->ulPID,&pEpr);

			if (NT_SUCCESS(status))
			{
				//���Ŀ����̵�EPROCESS ���� ring3�µ�Ӧ�ó���EPROCESS
				if (PsGetCurrentProcess() == pEpr)
				{
					//���ؾ����Ч
					return STATUS_ACCESS_DENIED;
				}
			}																			
		}		
	}

	return ObReferenceObjectByHandle(Handle,DesiredAccess,ObjectType,AccessMode,Object,HandleInformation);
}


//������InlineHook,��ֹ������ͼ�ֵ
VOID reg_inlineHook_disableCreate()
{
	__try
	{
		//���ԭ�е�ַ
		NtCreateKey = (PVOID)GetSSDTFuncAddress(ZwCreateKey);
		NtSetValueKey = (PVOID)GetSSDTFuncAddress(ZwSetValueKey);
		//inline hook
		common_callAddrHook(NtCreateKey,ObReferenceObjectByHandle,NewObReferenceObjectByHandle_forNtCreateKeyAndNtSetValueKey,PAGE_SIZE);
		common_callAddrHook(NtSetValueKey,ObReferenceObjectByHandle,NewObReferenceObjectByHandle_forNtCreateKeyAndNtSetValueKey,PAGE_SIZE);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("reg_inlineHook_disableCreate EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//ֹͣ��ֹ������ͼ�ֵ
VOID reg_inlineHook_disableCreate_stop()
{
	__try
	{
		//inline hook
		common_callAddrHook(NtCreateKey,NewObReferenceObjectByHandle_forNtCreateKeyAndNtSetValueKey,ObReferenceObjectByHandle,PAGE_SIZE);
		common_callAddrHook(NtSetValueKey,NewObReferenceObjectByHandle_forNtCreateKeyAndNtSetValueKey,ObReferenceObjectByHandle,PAGE_SIZE);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("reg_inlineHook_disableCreate_stop EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//������InlineHook,��ֹɾ����ͼ�ֵ
VOID reg_inlineHook_disableDelete()
{
	__try
	{
		//���ԭ�е�ַ
		NtDeleteKey = (PVOID)GetSSDTFuncAddress(ZwDeleteKey);
		NtDeleteValueKey = (PVOID)GetSSDTFuncAddress(ZwDeleteValueKey);
		//inline hook
		common_callAddrHook(NtDeleteKey,ObReferenceObjectByHandle,NewObReferenceObjectByHandle_forNtDeleteKeyAndNtDeleteVauleKey,PAGE_SIZE);
		common_callAddrHook(NtDeleteValueKey,ObReferenceObjectByHandle,NewObReferenceObjectByHandle_forNtDeleteKeyAndNtDeleteVauleKey,PAGE_SIZE);
	
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("reg_inlineHook_disableDelete EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//ֹͣ��ֹɾ����ͼ�ֵ
VOID reg_inlineHook_disableDelete_stop()
{
	__try
	{
		//inline hook
		common_callAddrHook(NtDeleteKey,NewObReferenceObjectByHandle_forNtDeleteKeyAndNtDeleteVauleKey,ObReferenceObjectByHandle,PAGE_SIZE);
		common_callAddrHook(NtDeleteValueKey,NewObReferenceObjectByHandle_forNtDeleteKeyAndNtDeleteVauleKey,ObReferenceObjectByHandle,PAGE_SIZE);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("reg_inlineHook_disableDelete_stop EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//������InlineHook,��ֹ��ѯ
VOID reg_inlineHook_disableQuery()
{
	__try
	{
		//���ԭ�е�ַ
		NtQueryValueKey = (PVOID)GetSSDTFuncAddress(ZwQueryValueKey);
		NtQueryKey = (PVOID)GetSSDTFuncAddress(ZwQueryKey);

		common_callAddrHook(NtQueryKey,ObReferenceObjectByHandle,NewObReferenceObjectByHandle_forNtQueryKeyAndNtQueryValueKey,PAGE_SIZE);
		common_callAddrHook(NtQueryValueKey,ObReferenceObjectByHandle,NewObReferenceObjectByHandle_forNtQueryKeyAndNtQueryValueKey,PAGE_SIZE);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("reg_inlineHook_disableQuery EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//ֹͣ��ֹ��ѯ
VOID reg_inlineHook_disableQuery_stop()
{
	__try
	{
		common_callAddrHook(NtQueryKey,NewObReferenceObjectByHandle_forNtQueryKeyAndNtQueryValueKey,ObReferenceObjectByHandle,PAGE_SIZE);
		common_callAddrHook(NtQueryValueKey,NewObReferenceObjectByHandle_forNtQueryKeyAndNtQueryValueKey,ObReferenceObjectByHandle,PAGE_SIZE);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("reg_inlineHook_disableQuery_stop EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//��DisableCreateRegInfo�в���Ŀ�����ID
//����
//0��ʾû���ҵ���1��ʾ�ҵ���
ULONG reg_findPIDInDisableCreateRegInfo(ULONG ulPID)
{
	PDISABLE_CREATE_REG p = NULL;

	__try
	{
		//���������б�
		for (p=pDisableCreateRegInfo;p;p=p->next)
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
		DbgPrint("reg_findPIDInDisableCreateRegInfo EXCEPTION_EXECUTE_HANDLER error!");
		return 0;
	}
}

//��DisableDeleteRegInfo�в���Ŀ�����ID
//����
//0��ʾû���ҵ���1��ʾ�ҵ���
ULONG reg_findPIDInDisableDeleteRegInfo(ULONG ulPID)
{
	PDISABLE_DELETE_REG p = NULL;

	__try
	{
		//���������б�
		for (p=pDisableDeleteRegInfo;p;p=p->next)
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
		DbgPrint("reg_findPIDInDisableDeleteRegInfo EXCEPTION_EXECUTE_HANDLER error!");
		return 0;
	}
}

//��DisableOpenRegInfo�в���Ŀ�����ID
//����
//0��ʾû���ҵ���1��ʾ�ҵ���
ULONG reg_findPIDInDisableOpenRegInfo(ULONG ulPID)
{
	PDISABLE_OPEN_REG p = NULL;

	__try
	{
		//���������б�
		for (p=pDisableOpenRegInfo;p;p=p->next)
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
		DbgPrint("reg_findPIDInDisableOpenRegInfo EXCEPTION_EXECUTE_HANDLER error!");
		return 0;
	}
}


//��DisableCreateRegInfo��ɾ��Ŀ�����ID
VOID reg_deletePIDInDisableCreateRegInfo(ULONG ulPID)
{
	PDISABLE_CREATE_REG p = NULL;
	PDISABLE_CREATE_REG p2 = NULL;

	__try
	{
		for (p2 = p =pDisableCreateRegInfo;p;p2=p,p=p->next)
		{
			if (p->ulPID == ulPID)
			{
				if (pDisableCreateRegInfo==p)
				{
					pDisableCreateRegInfo = pDisableCreateRegInfo->next;
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
		DbgPrint("reg_deletePIDInDisableCreateRegInfo EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//��DisableDeleteRegInfo��ɾ��Ŀ�����ID
VOID reg_deletePIDInDisableDeleteRegInfo(ULONG ulPID)
{
	PDISABLE_DELETE_REG p = NULL;
	PDISABLE_DELETE_REG p2 = NULL;

	__try
	{
		for (p2 = p =pDisableDeleteRegInfo;p;p2=p,p=p->next)
		{
			if (p->ulPID == ulPID)
			{
				if (pDisableDeleteRegInfo==p)
				{
					pDisableDeleteRegInfo = pDisableDeleteRegInfo->next;
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
		DbgPrint("reg_deletePIDInDisableDeleteRegInfo EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//��DisableOpenRegInfo��ɾ��Ŀ�����ID
VOID reg_deletePIDInDisableOpenRegInfo(ULONG ulPID)
{
	PDISABLE_OPEN_REG p = NULL;
	PDISABLE_OPEN_REG p2 = NULL;

	__try
	{
		for (p2 = p =pDisableOpenRegInfo;p;p2=p,p=p->next)
		{
			if (p->ulPID == ulPID)
			{
				if (pDisableOpenRegInfo==p)
				{
					pDisableOpenRegInfo = pDisableOpenRegInfo->next;
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
		DbgPrint("reg_deletePIDInDisableOpenRegInfo EXCEPTION_EXECUTE_HANDLER error!");
	}
}