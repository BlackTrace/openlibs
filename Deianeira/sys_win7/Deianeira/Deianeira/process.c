#include "process.h"



//�õ�PsSuspendProcess�ĵ�ַ
ULONG process_getPsSuspendProcessAddress()
{
	UCHAR *cPtr, *pOpcode;
	ULONG ulLength;
	ULONG ulCallCount=1;

	ULONG ulNtSuspendProcess,ulPsSuspendProcessAddr;

	__try
	{
		if (ulOSVersion == OS_WINDOWS_XP)
		{
			//���ulNtSuspendProcess�ĵ�ַ
			ulNtSuspendProcess = *((PULONG)(KeServiceDescriptorTable->ServiceTableBase) + NtSuspendProcess_XP);
		}
		else if(ulOSVersion == OS_WINDOWS_7_2008R2)
		{
			//���ulNtSuspendProcess�ĵ�ַ
			ulNtSuspendProcess = *((PULONG)(KeServiceDescriptorTable->ServiceTableBase) + NtSuspendProcess_WIN7);
		}

		//���PsSuspendProcess�ĵ�ַ
		for (cPtr = (PUCHAR)ulNtSuspendProcess; cPtr < (PUCHAR)ulNtSuspendProcess + PAGE_SIZE;) 
		{
			//��ñ��������ֽ���
			ulLength = SizeOfCode(cPtr, &pOpcode);

			//��������ʧ�ܵĻ�
			if (ulLength==0) 
			{
				return NULL;
			}

			//E8��ʾcall
			if (*pOpcode == 0xE8)
			{
				if (ulCallCount==2)
				{
					ulPsSuspendProcessAddr = (*(PULONG)(pOpcode + 1) + (ULONG)cPtr + 5);
					break;
				}
				
				ulCallCount++;
			}

			//����
			cPtr = cPtr + ulLength;
		}

		//����
		DbgPrint("process_getPsSuspendProcessAddress ulPsSuspendProcessAddr :%08X",ulPsSuspendProcessAddr);

		//���ص�ַ
		return ulPsSuspendProcessAddr;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("process_getPsSuspendProcessAddress EXCEPTION_EXECUTE_HANDLER error!");
		return NULL;
	}
}

//�õ�PsResumeProcess�ĵ�ַ
ULONG process_getPsResumeProcessAddress()
{
	UCHAR *cPtr, *pOpcode;
	ULONG ulLength;
	ULONG ulCallCount=1;

	ULONG ulNtResumeProcess,ulPsResumeProcessAddr;

	__try
	{
		if (ulOSVersion == OS_WINDOWS_XP)
		{
			//���ulNtSuspendProcess�ĵ�ַ
			ulNtResumeProcess = *((PULONG)(KeServiceDescriptorTable->ServiceTableBase) + NtResumeProcess_XP);
		}
		else if(ulOSVersion == OS_WINDOWS_7_2008R2)
		{
			//���ulNtSuspendProcess�ĵ�ַ
			ulNtResumeProcess = *((PULONG)(KeServiceDescriptorTable->ServiceTableBase) + NtResumeProcess_WIN7);
		}

		//���PsResumeProcess�ĵ�ַ
		for (cPtr = (PUCHAR)ulNtResumeProcess; cPtr < (PUCHAR)ulNtResumeProcess + PAGE_SIZE;) 
		{
			//��ñ��������ֽ���
			ulLength = SizeOfCode(cPtr, &pOpcode);

			//��������ʧ�ܵĻ�
			if (ulLength==0) 
			{
				return NULL;
			}

			//E8��ʾcall
			if (*pOpcode == 0xE8)
			{
				if (ulCallCount==2)
				{
					ulPsResumeProcessAddr = (*(PULONG)(pOpcode + 1) + (ULONG)cPtr + 5);
					break;
				}

				ulCallCount++;
			}

			//����
			cPtr = cPtr + ulLength;
		}

		//����
		DbgPrint("process_getPsResumeProcessAddress ulPsResumeProcessAddr :%08X",ulPsResumeProcessAddr);

		//���ص�ַ
		return ulPsResumeProcessAddr;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("process_getPsResumeProcessAddress EXCEPTION_EXECUTE_HANDLER error!");
		return NULL;
	}
}

//�������
BOOLEAN process_suspendProcess(ULONG ulPID){
	NTSTATUS status;
    PEPROCESS pEpr;
	
	__try
	{
		PsSuspendProcess = (PPsSuspendProcess)process_getPsSuspendProcessAddress();

		if (!PsSuspendProcess)
		{
			DbgPrint("process_suspendProcess process_getPsSuspendProcessAddress error!");
			return FALSE;
		}

		status = PsLookupProcessByProcessId((HANDLE)ulPID,&pEpr);

		if (!NT_SUCCESS(status))
		{
			DbgPrint("process_suspendProcess PsLookupProcessByProcessId error!");
			return FALSE;
		}

		status = PsSuspendProcess(pEpr);

		if (!NT_SUCCESS(status))
		{
			DbgPrint("process_suspendProcess PsSuspendProcess error!");
			return FALSE;
		}

		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("process_suspendProcess EXCEPTION_EXECUTE_HANDLER error!");
		return FALSE;
	}
}

//�ָ�����
BOOLEAN process_resumeProcess(ULONG ulPID){
	NTSTATUS status;
	PEPROCESS pEpr;

	__try
	{
		PsResumeProcess = (PPsResumeProcess)process_getPsResumeProcessAddress();

		if (!PsResumeProcess)
		{
			DbgPrint("process_resumeProcess process_getPsResumeProcessAddress error!");
			return FALSE;
		}

		status = PsLookupProcessByProcessId((HANDLE)ulPID,&pEpr);

		if (!NT_SUCCESS(status))
		{
			DbgPrint("process_resumeProcess PsLookupProcessByProcessId error!");
			return FALSE;
		}

		status = PsResumeProcess(pEpr);

		if (!NT_SUCCESS(status))
		{
			DbgPrint("process_resumeProcess PsResumeProcess error!");
			return FALSE;
		}

		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("process_resumeProcess EXCEPTION_EXECUTE_HANDLER error!");
		return FALSE;
	}
}

//�ڴ����㣬ǿ�ƽ�������
BOOLEAN process_killProcess(ULONG ulPID)
{
	NTSTATUS     status;
	PEPROCESS    m_process;
	KAPC_STATE   m_kapc_state;
	ULONG        m_index;
	BOOLEAN      m_is_valid;    
	PVOID        m_base_address=NULL;

	__try
	{
		//����һ��ֻ�����͵ģ���СΪ0x1000���ڴ�����
		//����ֵΪ�����ڴ��ָ��
		m_base_address=ExAllocatePool(NonPagedPool,0x1000);  
		//��ʼ������ڴ棬δ��ʼ���ı����ᱻϵͳ����ֵΪ0xCC
		RtlZeroMemory(m_base_address,0xcc,0x1000);
		//��������ڴ�ʧ��
		if(m_base_address==NULL)
		{
			DbgPrint("process_killProcess ExAllocatePool error!");
			return FALSE;
		}
		//�õ�ָ������ID�Ľ��̻�����
		status=PsLookupProcessByProcessId((HANDLE)ulPID,&m_process);
		//����ʧ��
		if(!NT_SUCCESS(status)) 
		{
			DbgPrint("process_killProcess  PsLookupProcessByProcessId error!");
			return FALSE;
		}
		//���ӵ�ǰ�̵߳�Ŀ����̿ռ���
		KeStackAttachProcess (m_process,&m_kapc_state); 
		//ѭ������
		for(m_index=0;m_index<0x80000000;m_index+=0x1000)
		{
			//���ָ���ڴ��С�ռ��ܶ���д�Ļ�
			if(MmIsAddressValid((PVOID)(m_index)))
			{
				__try
				{       
					//����m_base_address�ڴ�ռ����ݵ�m_index��С�Ŀռ�
					RtlCopyMemory((PVOID)(m_index),m_base_address,0x1000);  
				}
				__except(1)
				{continue;}
			}
			else
			{
				if(m_index>0x1000000)
					break;
			}
		}
		//�������
		KeUnstackDetachProcess(&m_kapc_state);
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("process_killProcess  EXCEPTION_EXECUTE_HANDLER error!");
		return FALSE;
	}
}

//���ؽ���
BOOLEAN process_hideProcess(ULONG ulPID)
{
	NTSTATUS status;
	//ƫ��
	ULONG ulOffset;
	PLIST_ENTRY pList_entry;
	PEPROCESS pEproOld,pEproCurrent,pEprProcess;

	__try
	{
		//��õ�ǰ���̵�EPROCESS
		pEproOld  = pEproCurrent = PsGetCurrentProcess();

		//���Ŀ����̵�eprocess
		status = PsLookupProcessByProcessId((HANDLE)ulPID,&pEprProcess);

		//�����ȡʧ��
		if (!NT_SUCCESS(status))
		{
			DbgPrint("process_hideProcess PsLookupProcessByProcessId error!");
			return FALSE;
		}

		if (ulOSVersion == OS_WINDOWS_XP)
		{
			//xp sp3
			//kd> dt _eprocess
			//	nt!_EPROCESS
			//	+0x000 Pcb              : _KPROCESS
			//	+0x06c ProcessLock      : _EX_PUSH_LOCK
			//	+0x070 CreateTime       : _LARGE_INTEGER
			//	+0x078 ExitTime         : _LARGE_INTEGER
			//	+0x080 RundownProtect   : _EX_RUNDOWN_REF
			//	+0x084 UniqueProcessId  : Ptr32 Void
			//	+0x088 ActiveProcessLinks : _LIST_ENTRY

			ulOffset = 0x088;
		}
		else if (ulOSVersion == OS_WINDOWS_7_2008R2)
		{
			//win7 sp1
			ulOffset = 0x0b8;
		}

		//�����б�
		do 
		{
			//win7 sp1ϵͳ��EPROCESS�е�imageNameƫ��Ϊ0x16c
			//PUCHAR pName = (PUCHAR)((ULONG)pEproCurrent + 0x16c);
			//uniqueprocessID��ƫ��Ϊ0x0b4
			//DbgPrint("process name:%s,process id:%d",pName,*(PULONG)((ULONG)pEproCurrent + 0x0b4));

			//���������ͬ��EPROCESS
			if(pEprProcess == pEproCurrent)
			{
				//��WIN7 SP1 ϵͳ�� EPROCESS �� ActiveProcessLinks ��ƫ��Ϊ0x0b8
				pList_entry = (PLIST_ENTRY)((ULONG)pEproCurrent + ulOffset);

				//��������ж���
				((PLIST_ENTRY)(pList_entry->Blink))->Flink = pList_entry->Flink;
				((PLIST_ENTRY)(pList_entry->Flink))->Blink = pList_entry->Blink;

				return TRUE;
			}

			//�¸�PLIST_ENTRY
			pList_entry = (PLIST_ENTRY)((ULONG)pEproCurrent + ulOffset);

			//������֮����Ҫ���¼�ȥƫ�ƣ�����Ϊ�¸�Plist_entry���������¸�EPROCESS��ƫ��0x0b8��������Ҫ��ȥƫ�ƺ�����¸�EPROCESS
			pEproCurrent = (PEPROCESS)((ULONG)pList_entry->Flink - ulOffset);

		} while (pEproOld!=pEproCurrent);

		return FALSE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("process_hideProcess  EXCEPTION_EXECUTE_HANDLER error!");
		return FALSE;
	}
}

//����MiUnmapViewOfSection�ĵ�ַ
ULONG process_getMiUnmapViewOfSectionAddress()
{
	UCHAR *cPtr, *pOpcode;
	ULONG ulLength;
	ULONG ulCallCount=1;
	ULONG ulNtUnmapViewOfSectionAddr;
	ULONG ulMiUnmapViewOfSectionAddr;

	__try
	{
		//���Nt������ַ
		ulNtUnmapViewOfSectionAddr = (ULONG)GetSSDTFuncAddress(ZwUnmapViewOfSection);

		//Ȼ�����PsTerminateSystemThread�ĵ�һ��call
		for (cPtr = (PUCHAR)ulNtUnmapViewOfSectionAddr; cPtr < (PUCHAR)ulNtUnmapViewOfSectionAddr + PAGE_SIZE;) 
		{
			//��ñ��������ֽ���
			ulLength = SizeOfCode(cPtr, &pOpcode);

			//��������ʧ�ܵĻ�
			if (ulLength==0) 
			{
				return NULL;
			}

			//E8��ʾcall
			if (*pOpcode == 0xE8)
			{
				if (ulCallCount==2)
				{
					//CALL�������̵��ã���ʵ����push �������ص�ַ������push���ָ��һ���ֽڣ��������ص�ַ�ĸ��ֽڣ����Ծ���5���ֽ�
					//��E8 ��������ľ������ƫ�Ƶ�ַ
					//(*(PULONG)(pOpcode + 1)�õ����ƫ�Ƶ�ַ + ��ǰ��ַ(ULONG)cPtr �ڼ��Ϻ������ص�ַ�Rջ���õ�5���ֽ� = call ����ĺ�����ַ
					ulMiUnmapViewOfSectionAddr = (*(PULONG)(pOpcode + 1) + (ULONG)cPtr + 5);

					DbgPrint("process_getMiUnmapViewOfSectionAddress MiUnmapViewOfSection Address :%08X",ulMiUnmapViewOfSectionAddr);
					return ulMiUnmapViewOfSectionAddr;
				}
				ulCallCount++;
			}

			//����
			cPtr = cPtr + ulLength;
		}

		return NULL;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("process_getMiUnmapViewOfSectionAddress EXCEPTION_EXECUTE_HANDLER error!");
		return NULL;
	}
}

//ж��ģ��
//ulPID:����ID
//ulModuleBase:ģ���ַ
BOOLEAN process_unloadModule(ULONG ulPID,ULONG ulModuleBase)
{
	__try
	{
		NTSTATUS status;
		BOOLEAN bRet = FALSE;
		PEPROCESS pEProcess=NULL;
        ULONG ulMiUnmapViewOfSectionAddr;

		status = PsLookupProcessByProcessId((HANDLE)ulPID,&pEProcess);

		if (!NT_SUCCESS (status)) {

			DbgPrint("process_unloadModule PsLookupProcessByProcessId error!");
			return bRet;
		}

		//���MiUnmapViewOfSection��ַ
        ulMiUnmapViewOfSectionAddr = process_getMiUnmapViewOfSectionAddress();

		//�����õ�ַ�ɹ��Ļ�
		if (ulMiUnmapViewOfSectionAddr)
		{
			MiUnmapViewOfSection = (PMiUnmapViewOfSection)ulMiUnmapViewOfSectionAddr;
		}
		else
		{
			DbgPrint("process_unloadModule ulMiUnmapViewOfSectionAddr error!");
			return bRet;
		}

		//������ɾ������ɾ��ģ��
		status = MiUnmapViewOfSection(pEProcess,(PVOID)ulModuleBase,0);

		if (NT_SUCCESS(status))
		{
			bRet = TRUE;
		}

		return	bRet;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("process_unloadModule EXCEPTION_EXECUTE_HANDLER error!");
		return FALSE;
	}
}

//�õ�PspTerminateThreadByPointer�ĵ�ַ
ULONG process_getPspTerminateThreadByPointerAddress()
{
	UCHAR *cPtr, *pOpcode;
	ULONG ulLength;
	ULONG ulCallCount=1;

	ULONG urlPsTerminateSystemThreadAddr,urlPspTerminateThreadByPointerAddr;

	__try
	{
	//���PsTerminateSystemThread�ĵ�ַ
	urlPsTerminateSystemThreadAddr = GetExportedFuncAddress(L"PsTerminateSystemThread");

	/*
	kd> uf PsTerminateSystemThread
	nt!PsTerminateSystemThread:
	805c9f36 8bff            mov     edi,edi
	805c9f38 55              push    ebp
	805c9f39 8bec            mov     ebp,esp
	805c9f3b 64a124010000    mov     eax,dword ptr fs:[00000124h]
	805c9f41 f6804802000010  test    byte ptr [eax+248h],10h
	805c9f48 7507            jne     nt!PsTerminateSystemThread+0x1b (805c9f51)

	nt!PsTerminateSystemThread+0x14:
	805c9f4a b80d0000c0      mov     eax,0C000000Dh
	805c9f4f eb09            jmp     nt!PsTerminateSystemThread+0x24 (805c9f5a)

	nt!PsTerminateSystemThread+0x1b:
	805c9f51 ff7508          push    dword ptr [ebp+8]
	805c9f54 50              push    eax
	805c9f55 e828fcffff      call    nt!PspTerminateThreadByPointer (805c9b82)
	*/

	//Ȼ�����PsTerminateSystemThread�ĵ�һ��call
	for (cPtr = (PUCHAR)urlPsTerminateSystemThreadAddr; cPtr < (PUCHAR)urlPsTerminateSystemThreadAddr + PAGE_SIZE;) 
	{
		//��ñ��������ֽ���
		ulLength = SizeOfCode(cPtr, &pOpcode);

		//��������ʧ�ܵĻ�
		if (ulLength==0) 
		{
			return NULL;
		}

		//E8��ʾcall
		if (*pOpcode == 0xE8)
		{
			//CALL�������̵��ã���ʵ����push �������ص�ַ������push���ָ��һ���ֽڣ��������ص�ַ�ĸ��ֽڣ����Ծ���5���ֽ�
			//��E8 ��������ľ������ƫ�Ƶ�ַ
			//(*(PULONG)(pOpcode + 1)�õ����ƫ�Ƶ�ַ + ��ǰ��ַ(ULONG)cPtr �ڼ��Ϻ������ص�ַ�Rջ���õ�5���ֽ� = call ����ĺ�����ַ
			urlPspTerminateThreadByPointerAddr = (*(PULONG)(pOpcode + 1) + (ULONG)cPtr + 5);
			break;
		}

		//����
		cPtr = cPtr + ulLength;
	}

	//����
	DbgPrint("process_getPspTerminateThreadByPointerAddress urlPspTerminateThreadByPointerAddr :%08X",urlPspTerminateThreadByPointerAddr);

	//���ص�ַ
	return urlPspTerminateThreadByPointerAddr;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("process_getPspTerminateThreadByPointerAddress EXCEPTION_EXECUTE_HANDLER error!");
		return NULL;
	}
}

//�õ�KeSuspendThread�ĵ�ַ
ULONG process_getKeSuspendThreadAddress()
{
	UCHAR *cPtr, *pOpcode;
	ULONG ulLength;
	ULONG ulCallCount=1;

	ULONG ulNtSuspendThread,ulPsSuspendThread,ulKeSuspendThread;;

	__try
	{
		if (ulOSVersion == OS_WINDOWS_XP)
		{
			//���ulNtSuspendThread�ĵ�ַ
			ulNtSuspendThread = *((PULONG)(KeServiceDescriptorTable->ServiceTableBase) + NtSuspendThread_XP);
		}
		else if(ulOSVersion == OS_WINDOWS_7_2008R2)
		{
			//���ulNtSuspendThread�ĵ�ַ
			ulNtSuspendThread = *((PULONG)(KeServiceDescriptorTable->ServiceTableBase) + NtSuspendThread_WIN7);
		}

		//���PsSuspendThread��ַ
		for (cPtr = (PUCHAR)ulNtSuspendThread; cPtr < (PUCHAR)ulNtSuspendThread + PAGE_SIZE;) 
		{
			//��ñ��������ֽ���
			ulLength = SizeOfCode(cPtr, &pOpcode);

			//��������ʧ�ܵĻ�
			if (ulLength==0) 
			{
				return NULL;
			}

			//E8��ʾcall
			if (*pOpcode == 0xE8)
			{
				if (ulCallCount==3)
				{
					//��E8 ��������ľ������ƫ�Ƶ�ַ
					//(*(PULONG)(pOpcode + 1)�õ����ƫ�Ƶ�ַ + ��ǰ��ַ(ULONG)cPtr �ڼ��Ϻ������ص�ַ�Rջ���õ�5���ֽ� = call ����ĺ�����ַ
					ulPsSuspendThread = (*(PULONG)(pOpcode + 1) + (ULONG)cPtr + 5);

					DbgPrint("process_getKeSuspendThreadAddress ulPsSuspendThread :%X",ulPsSuspendThread);
					break;
				}

				//
				ulCallCount++;
			}

			//����
			cPtr = cPtr + ulLength;
		}

		ulCallCount = 1;

		//���KeSuspendThread��ַ
		for (cPtr = (PUCHAR)ulPsSuspendThread; cPtr < (PUCHAR)ulPsSuspendThread + PAGE_SIZE;) 
		{
			//��ñ��������ֽ���
			ulLength = SizeOfCode(cPtr, &pOpcode);

			//��������ʧ�ܵĻ�
			if (ulLength==0) 
			{
				return NULL;
			}

			//E8��ʾcall
			if (*pOpcode == 0xE8)
			{
				if (ulCallCount==4)
				{
					//��E8 ��������ľ������ƫ�Ƶ�ַ
					//(*(PULONG)(pOpcode + 1)�õ����ƫ�Ƶ�ַ + ��ǰ��ַ(ULONG)cPtr �ڼ��Ϻ������ص�ַ�Rջ���õ�5���ֽ� = call ����ĺ�����ַ
					ulKeSuspendThread = (*(PULONG)(pOpcode + 1) + (ULONG)cPtr + 5);

					DbgPrint("process_getKeSuspendThreadAddress ulKeSuspendThread :%X",ulKeSuspendThread);
					break;
				}

				//
				ulCallCount++;
			}

			//����
			cPtr = cPtr + ulLength;
		}

		//���ص�ַ
		return ulKeSuspendThread;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("process_getKeSuspendThreadAddress EXCEPTION_EXECUTE_HANDLER error!");
		return NULL;
	}
}

//�õ�KeResumeThread�ĵ�ַ
ULONG process_getKeResumeThreadAddress()
{
	UCHAR *cPtr, *pOpcode;
	ULONG ulLength;
	ULONG ulCallCount=1;

	ULONG ulNtResumeThread,ulPsResumeThread,ulKeResumeThread;;

	__try
	{
		if (ulOSVersion == OS_WINDOWS_XP)
		{
			//���ulNtSuspendThread�ĵ�ַ
			ulNtResumeThread = *((PULONG)(KeServiceDescriptorTable->ServiceTableBase) + NtResumeThread_XP);
		}
		else if(ulOSVersion == OS_WINDOWS_7_2008R2)
		{
			//���ulNtResumeThread�ĵ�ַ
		    ulNtResumeThread = *((PULONG)(KeServiceDescriptorTable->ServiceTableBase) + NtResumeThread_WIN7);
		}

		//���PsSuspendThread��ַ
		for (cPtr = (PUCHAR)ulNtResumeThread; cPtr < (PUCHAR)ulNtResumeThread + PAGE_SIZE;) 
		{
			//��ñ��������ֽ���
			ulLength = SizeOfCode(cPtr, &pOpcode);

			//��������ʧ�ܵĻ�
			if (ulLength==0) 
			{
				return NULL;
			}

			//E8��ʾcall
			if (*pOpcode == 0xE8)
			{
				if (ulCallCount==3)
				{
					//��E8 ��������ľ������ƫ�Ƶ�ַ
					//(*(PULONG)(pOpcode + 1)�õ����ƫ�Ƶ�ַ + ��ǰ��ַ(ULONG)cPtr �ڼ��Ϻ������ص�ַ�Rջ���õ�5���ֽ� = call ����ĺ�����ַ
					ulPsResumeThread = (*(PULONG)(pOpcode + 1) + (ULONG)cPtr + 5);

					DbgPrint("process_getKeResumeThreadAddress ulPsResumeThread :%X",ulPsResumeThread);
					break;
				}

				//
				ulCallCount++;
			}

			//����
			cPtr = cPtr + ulLength;
		}

		//���KeSuspendThread��ַ
		for (cPtr = (PUCHAR)ulPsResumeThread; cPtr < (PUCHAR)ulPsResumeThread + PAGE_SIZE;) 
		{
			//��ñ��������ֽ���
			ulLength = SizeOfCode(cPtr, &pOpcode);

			//��������ʧ�ܵĻ�
			if (ulLength==0) 
			{
				return NULL;
			}

			//E8��ʾcall
			if (*pOpcode == 0xE8)
			{
				//��E8 ��������ľ������ƫ�Ƶ�ַ
				//(*(PULONG)(pOpcode + 1)�õ����ƫ�Ƶ�ַ + ��ǰ��ַ(ULONG)cPtr �ڼ��Ϻ������ص�ַ�Rջ���õ�5���ֽ� = call ����ĺ�����ַ
				ulKeResumeThread = (*(PULONG)(pOpcode + 1) + (ULONG)cPtr + 5);

				DbgPrint("process_getKeResumeThreadAddress ulKeResumeThread :%X",ulKeResumeThread);
				break;				
			}

			//����
			cPtr = cPtr + ulLength;
		}

		//���ص�ַ
		return ulKeResumeThread;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("process_getKeResumeThreadAddress EXCEPTION_EXECUTE_HANDLER error!");
		return NULL;
	}
}

//
BOOLEAN process_suspendThread(ULONG ulTID)
{
	NTSTATUS status;
	PETHREAD pEThread;
	ULONG ulKeSuspendThread;

	__try
	{
		//��õ�ַ
		ulKeSuspendThread = process_getKeSuspendThreadAddress();

		//�ж���ַ��Ч��
		if (ulKeSuspendThread==0 || ulKeSuspendThread <= 0x80000000)
		{
			DbgPrint("process_suspendThread ulKeSuspendThread error!");
			return FALSE;
		}

		//
		KeSuspendThread = (PKeSuspendThread)ulKeSuspendThread;

		//���ETHREAD
		status = PsLookupThreadByThreadId(ulTID,&pEThread);

		//���ʧ��
		if (!NT_SUCCESS(status))
		{
			DbgPrint("process_suspendThread PsLookupThreadByThreadId error!");
			return FALSE;
		}

		if (KeSuspendThread((PKTHREAD)pEThread))
		{
			DbgPrint("false");
			return FALSE;
		}
		else
		{
			DbgPrint("true");
			return TRUE;
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("process_suspendThread EXCEPTION_EXECUTE_HANDLER error!");
		return FALSE;
	}
}

//
BOOLEAN process_resumeThread(ULONG ulTID)
{
	NTSTATUS status;
	PETHREAD pEThread;
	ULONG ulKeResumeThread;

	__try
	{
		//��õ�ַ
		ulKeResumeThread = process_getKeResumeThreadAddress();

		//�ж���ַ��Ч��
		if (ulKeResumeThread==0 || ulKeResumeThread <= 0x80000000)
		{
			DbgPrint("process_resumeThread ulKeResumeThread error!");
			return FALSE;
		}

		//
		KeResumeThread = (PKeResumeThread)ulKeResumeThread;

		//���ETHREAD
		status = PsLookupThreadByThreadId(ulTID,&pEThread);

		//���ʧ��
		if (!NT_SUCCESS(status))
		{
			DbgPrint("process_resumeThread PsLookupThreadByThreadId error!");
			return FALSE;
		}

		if (KeResumeThread((PKTHREAD)pEThread))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("process_resumeThread EXCEPTION_EXECUTE_HANDLER error!");
		return FALSE;
	}
}

//
BOOLEAN process_killThreadByPspTerminateThreadByPointer(ULONG ulPID,ULONG ulTID)
{
	NTSTATUS status;
	PEPROCESS pEProcess;
    ULONG ulPEprocess;
    ULONG ulTListHead,ulTListNext;

	//PKTHREAD
	ULONG ulCurThread;

	__try
	{
		PspTerminateThreadByPointer = (PPspTerminateThreadByPointer)process_getPspTerminateThreadByPointerAddress();

		if (PspTerminateThreadByPointer==NULL)
		{
			DbgPrint("process_killThread PspTerminateThreadByPointer error!");
			return FALSE;
		}

		//���PEPROCESS
		status = PsLookupProcessByProcessId(ulPID,&pEProcess);

		//������ʧ��
		if (!NT_SUCCESS(status))
		{
			return FALSE;
		}

        /*
		+0x03c UserTime         : Uint4B
		+0x040 ReadyListHead    : _LIST_ENTRY
		+0x048 SwapListEntry    : _SINGLE_LIST_ENTRY
		+0x04c VdmTrapcHandler  : Ptr32 Void
		+0x050 ThreadListHead   : _LIST_ENTRY  //*
		+0x058 ProcessLock      : Uint4B
		*/

		//��ȡ�߳��б�ͷ
		ulPEprocess=(ULONG)pEProcess;

		ulTListHead = ulPEprocess + 0x50;

	    /*
		kd> dt _LIST_ENTRY
		ntdll!_LIST_ENTRY
		+0x000 Flink            : Ptr32 _LIST_ENTRY
		+0x004 Blink            : Ptr32 _LIST_ENTRY
		*/

		//_LIST_ENTRY��ǰ�ĸ��ֽڴ�����һ���ڵ�ĵ�ַ
		//PULONGת�ɵ�ַָ�룬Ȼ��*ȡ��ַ�����ֵ���ɺ����¸��ڵ�ĵ�ַ
		ulTListNext = *(PULONG)ulTListHead;

		while(ulTListNext != ulTListHead)
		{   
			//
			ulCurThread =ulTListNext-0x1b0;

			//0x1ec Cid��ַ���ڼ�4ΪUniqueThread�����߳�ID��0x1ec+4=0x1f0
			//�߳�ID
			if (*(PULONG)(ulCurThread + 0x1f0) == ulTID)
			{
				status = PspTerminateThreadByPointer((PETHREAD)ulCurThread, 0);

				if (NT_SUCCESS(status))
				{
					return TRUE;
				}
			}

			//��һ���߳̿�
			ulTListNext = *(PULONG)ulTListNext; //check next thread
		}

		return FALSE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("process_killThread EXCEPTION_EXECUTE_HANDLER error!");
		return FALSE;
	}
}



//��������
BOOLEAN process_killProcessByPspTerminateThreadByPointer(ULONG ulPID)
{
	NTSTATUS status;
	PEPROCESS pEProcess;
    ULONG ulPEprocess;
    ULONG ulTListHead,ulTListNext;

	//PKTHREAD
	ULONG ulCurThread;

	__try
	{
		PspTerminateThreadByPointer = (PPspTerminateThreadByPointer)process_getPspTerminateThreadByPointerAddress();

		if (PspTerminateThreadByPointer==NULL)
		{
			DbgPrint("process_killThread PspTerminateThreadByPointer error!");
			return FALSE;
		}

		//���PEPROCESS
		status = PsLookupProcessByProcessId(ulPID,&pEProcess);

		//������ʧ��
		if (!NT_SUCCESS(status))
		{
			return FALSE;
		}

        /*
		+0x03c UserTime         : Uint4B
		+0x040 ReadyListHead    : _LIST_ENTRY
		+0x048 SwapListEntry    : _SINGLE_LIST_ENTRY
		+0x04c VdmTrapcHandler  : Ptr32 Void
		+0x050 ThreadListHead   : _LIST_ENTRY  //*
		+0x058 ProcessLock      : Uint4B
		*/

		//��ȡ�߳��б�ͷ
		ulPEprocess=(ULONG)pEProcess;

		ulTListHead = ulPEprocess + 0x50;

	    /*
		kd> dt _LIST_ENTRY
		ntdll!_LIST_ENTRY
		+0x000 Flink            : Ptr32 _LIST_ENTRY
		+0x004 Blink            : Ptr32 _LIST_ENTRY
		*/

		//_LIST_ENTRY��ǰ�ĸ��ֽڴ�����һ���ڵ�ĵ�ַ
		//PULONGת�ɵ�ַָ�룬Ȼ��*ȡ��ַ�����ֵ���ɺ����¸��ڵ�ĵ�ַ
		ulTListNext = *(PULONG)ulTListHead;

		while(ulTListNext != ulTListHead)
		{   
			//
			ulCurThread =ulTListNext-0x1b0;

			status = PspTerminateThreadByPointer((PETHREAD)ulCurThread, 0);

			if (!NT_SUCCESS(status))
			{
				return FALSE;
			}

			//��һ���߳̿�
			ulTListNext = *(PULONG)ulTListNext; //check next thread
		}

		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("process_killProcess EXCEPTION_EXECUTE_HANDLER error!");
		return FALSE;
	}
}

//��ý����б�
BOOLEAN process_getProcessInfo()
{
	NTSTATUS status;
	//ƫ��
	ULONG ulOffset;
	PLIST_ENTRY pList_entry;
	PEPROCESS pEproOld,pEproCurrent;

	__try
	{
		//��õ�ǰ���̵�EPROCESS
		pEproOld  = pEproCurrent = PsGetCurrentProcess();

		if (ulOSVersion == OS_WINDOWS_XP)
		{
			//xp sp3
			//kd> dt _eprocess
			//	nt!_EPROCESS
			//	+0x000 Pcb              : _KPROCESS
			//	+0x06c ProcessLock      : _EX_PUSH_LOCK
			//	+0x070 CreateTime       : _LARGE_INTEGER
			//	+0x078 ExitTime         : _LARGE_INTEGER
			//	+0x080 RundownProtect   : _EX_RUNDOWN_REF
			//	+0x084 UniqueProcessId  : Ptr32 Void
			//	+0x088 ActiveProcessLinks : _LIST_ENTRY

			ulOffset = 0x088;
		}
		else if (ulOSVersion == OS_WINDOWS_7_2008R2)
		{
			//win7 sp1
			ulOffset = 0x0b8;
		}

		//�����б�
		do 
		{
			//win7 sp1ϵͳ��EPROCESS�е�imageNameƫ��Ϊ0x16c
			//PUCHAR pName = (PUCHAR)((ULONG)pEproCurrent + 0x16c);
			//uniqueprocessID��ƫ��Ϊ0x0b4
			//DbgPrint("process name:%s,process id:%d",pName,*(PULONG)((ULONG)pEproCurrent + 0x0b4));

			//��������봦�����

			//�¸�PLIST_ENTRY
			pList_entry = (PLIST_ENTRY)((ULONG)pEproCurrent + ulOffset);

			//������֮����Ҫ���¼�ȥƫ�ƣ�����Ϊ�¸�Plist_entry���������¸�EPROCESS��ƫ��0x0b8��������Ҫ��ȥƫ�ƺ�����¸�EPROCESS
			pEproCurrent = (PEPROCESS)((ULONG)pList_entry->Flink - ulOffset);

		} while (pEproOld!=pEproCurrent);

		return FALSE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("process_hideProcess  EXCEPTION_EXECUTE_HANDLER error!");
		return FALSE;
	}
}

//���Ŀ����̵��߳���Ϣ
PPROCESS_THREAD_INFO process_getProcessThreadInfo(ULONG ulPID)
{

	//��ǰ���̵�PEPROCESS
	ULONG ulCurProcess;

	//PKTHREAD
	ULONG ulCurThread;

	ULONG ulPListHead,ulPListNext;

	ULONG ulTListHead,ulTListNext;

	PPROCESS_THREAD_INFO pProcessThreadInfo;
    PPROCESS_THREAD_INFO pProcessThreadInfoTmp;

	/*
	xp sp3

	+0x000 Pcb              : _KPROCESS
	+0x06c ProcessLock      : _EX_PUSH_LOCK
	+0x070 CreateTime       : _LARGE_INTEGER
	+0x078 ExitTime         : _LARGE_INTEGER
	+0x080 RundownProtect   : _EX_RUNDOWN_REF
	+0x084 UniqueProcessId  : Ptr32 Void      //*
	+0x088 ActiveProcessLinks : _LIST_ENTRY   //*
	...
	+0x160 PhysicalVadList  : _LIST_ENTRY
	+0x168 PageDirectoryPte : _HARDWARE_PTE_X86
	+0x168 Filler           : Uint8B
	+0x170 Session          : Ptr32 Void
	+0x174 ImageFileName    : [16] UChar      //*
	+0x184 JobLinks 
	*/

	//IDƫ��
	ULONG ulPIDOffset;
	//nameƫ��
	ULONG ulPNameOffset;

	__try
	{
		//�����ǰϵͳ��xp
	if (ulOSVersion == OS_WINDOWS_XP)
	{
		ulPIDOffset = 0x084;
        ulPNameOffset = 0x174;
	}
	else if (ulOSVersion == OS_WINDOWS_7_2008R2)
	{
		//win7 sp1ϵͳ��EPROCESS�е�imageNameƫ��Ϊ0x16c
		//PUCHAR pName = (PUCHAR)((ULONG)pEproCurrent + 0x16c);
		//uniqueprocessID��ƫ��Ϊ0x0b4
		//DbgPrint("process name:%s,process id:%d",pName,*(PULONG)((ULONG)pEproCurrent + 0x0b4));
		ulPIDOffset = 0x0b4;
		ulPNameOffset = 0x16c;
	}

	//�����б�ͷ��
	pProcessThreadInfo = pProcessThreadInfoTmp = (PPROCESS_THREAD_INFO)ExAllocatePool(NonPagedPool,sizeof(PROCESS_THREAD_INFO));

	//��õ�ǰ����
	ulCurProcess =(ULONG)PsGetCurrentProcess();

	/*
		+0x088 ActiveProcessLinks : _LIST_ENTRY   //*
		����ϵͳ������ͬ��ƫ�ƣ�������ﲻҪ�ж�
	*/

	ulPListHead = ulCurProcess + 0x088;

	//�õ���һ��EPROCESS�ṹ��ActiveProcessLinksƫ�Ƶ�ַ
	ulPListNext =*(PULONG)ulPListHead;

	while(ulPListNext != ulPListHead)
		{    
			//ActiveProcessLinks ��ַ - ���ƫ���� = PEPROCESS��ַ
			ulCurProcess = ulPListNext - 0x088;

			//win7 sp1ϵͳ��EPROCESS�е�imageNameƫ��Ϊ0x16c
			//PUCHAR pName = (PUCHAR)((ULONG)pEproCurrent + 0x16c);
			//uniqueprocessID��ƫ��Ϊ0x0b4
			//DbgPrint("process name:%s,process id:%d",pName,*(PULONG)((ULONG)pEproCurrent + 0x0b4));

			//�����ǰ�б����ID����Ҫ��ѯ�Ľ���ID�Ļ�
			if (*(PULONG)(ulCurProcess + ulPIDOffset) == ulPID)
			{    
				/*
				+0x03c UserTime         : Uint4B
				+0x040 ReadyListHead    : _LIST_ENTRY
				+0x048 SwapListEntry    : _SINGLE_LIST_ENTRY
				+0x04c VdmTrapcHandler  : Ptr32 Void
				+0x050 ThreadListHead   : _LIST_ENTRY  //*
				+0x058 ProcessLock      : Uint4B
				*/

				//��ȡ�߳��б�ͷ
				ulTListHead = ulCurProcess + 0x050;

                /*
				kd> dt _LIST_ENTRY
				ntdll!_LIST_ENTRY
				+0x000 Flink            : Ptr32 _LIST_ENTRY
				+0x004 Blink            : Ptr32 _LIST_ENTRY
				*/

				//_LIST_ENTRY��ǰ�ĸ��ֽڴ�����һ���ڵ�ĵ�ַ
				//PULONGת�ɵ�ַָ�룬Ȼ��*ȡ��ַ�����ֵ���ɺ����¸��ڵ�ĵ�ַ
				ulTListNext = *(PULONG)ulTListHead;

				//�����б�
				while(ulTListNext != ulTListHead)
				{   
					/*
					+0x168 StackBase        : Ptr32 Void
					+0x16c SuspendApc       : _KAPC
					+0x19c SuspendSemaphore : _KSEMAPHORE
					+0x1b0 ThreadListEntry  : _LIST_ENTRY  //*
					+0x1b8 FreezeCount      : Char
					+0x1b9 SuspendCount     : Char
					*/

					////ThreadListEntry ��ַ - ���ƫ���� = ETHREAD��ַ
					ulCurThread = ulTListNext - 0x1b0;

					/*
					+0x1e4 ActiveTimerListHead : _LIST_ENTRY
					+0x1ec Cid              : _CLIENT_ID   //*
					+0x1f4 LpcReplySemaphore : _KSEMAPHORE

					kd> dt _CLIENT_ID
					ntdll!_CLIENT_ID
					+0x000 UniqueProcess    : Ptr32 Void
					+0x004 UniqueThread     : Ptr32 Void
					*/

					//0x1ec Cid��ַ���ڼ�4ΪUniqueThread�����߳�ID��0x1ec+4=0x1f0
					//�߳�ID
					pProcessThreadInfoTmp->ulTID  = *(PULONG)(ulCurThread + 0x1f0);

					//PETHREAD��ַ
					pProcessThreadInfoTmp->ulPEThread = ulCurThread;

					/*
					+0x010 MutantListHead   : _LIST_ENTRY
					+0x018 InitialStack     : Ptr32 Void
					+0x01c StackLimit       : Ptr32 Void
					+0x020 Teb              : Ptr32 Void  //*
					*/
                    pProcessThreadInfoTmp->ulTEB = *(PULONG)(ulCurThread + 0x020);
  
					/*
					+0x224 StartAddress     : Ptr32 Void
					+0x228 Win32StartAddress : Ptr32 Void
					*/

					//�̵߳�ַ
					pProcessThreadInfoTmp->ulBaseAddress =*(PULONG)(ulCurThread + 0x228);

					//ָ������
					pProcessThreadInfo->next = pProcessThreadInfoTmp;
					pProcessThreadInfo = pProcessThreadInfoTmp;

					//��һ���߳̿�
					ulTListNext = *(PULONG)ulTListNext;
				}
				break;    
			}

			//��һ�����̿�
			ulPListNext = *(PULONG)ulPListNext;
		}

	    return pProcessThreadInfo;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("process_getProcessThreadInfo EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//����̵߳Ļ�ַ 
ULONG process_getThreadBaseAddr(ULONG ulPID,ULONG ulTID) {     
	__try    
	{        
		//�߳̾��         
		HANDLE hThread;         
		//�̻߳�ַ        
		ULONG ulBaseAddr;        
		NTSTATUS status;       
		OBJECT_ATTRIBUTES ObjectAttributes;      

		//�Ѵ�RING3�´��������ݽ�������ת��     
		CLIENT_ID clientID;         
		clientID.UniqueProcess=ulPID;     
		clientID.UniqueThread=ulTID;       

		InitializeObjectAttributes(&ObjectAttributes, NULL, 0, NULL, NULL );          

        //�����߳�
		status=ZwOpenThread(&hThread,THREAD_ALL_ACCESS,&ObjectAttributes,&clientID);       

		//����򿪳ɹ�
		if (NT_SUCCESS(status))        
		{            
			status = ZwQueryInformationThread(hThread,ThreadQuerySetWin32StartAddress,&ulBaseAddr,sizeof(ulBaseAddr),NULL);  

			if (!NT_SUCCESS(status))
			{
				DbgPrint("process_getThreadBaseAddr ZwQueryInformationThread error!");
			}

			ZwClose(hThread);        
		}                  

		return ulBaseAddr;
	}
	_except( EXCEPTION_EXECUTE_HANDLER )   
	{          
		DbgPrint("process_getThreadBaseAddr EXCEPTION_EXECUTE_HANDLER error��");   
	} 
} 


//��ý���ģ����Ϣ
BOOLEAN process_getProcessModuleInfo(ULONG ulPID)
{
	NTSTATUS status; 
	PLDR_DATA_TABLE_ENTRY pLdr = NULL;

	PLIST_ENTRY pListHead,plistCurrent;
	PEPROCESS pEProcess;
	ULONG ulLDR;
	ULONG ulPEB;

	__try
	{
		//���EPROCESS
		status = PsLookupProcessByProcessId((HANDLE)ulPID,&pEProcess);

		//������ɹ�
		if (!NT_SUCCESS(status))
		{
			DbgPrint("process_hideModule PsLookupProcessByProcessId error!");
			return FALSE;
		}

		//xp sp3�µ�ƫ����
		//+0x1ac LastThreadExitStatus : Int4B
		//+0x1b0 Peb              : Ptr32 _PEB
		//+0x1b4 PrefetchTrace    : _EX_FAST_REF

		//_PEB_LDR_DATA��PEB�е�ƫ��
		//+0x008 ImageBaseAddress : Ptr32 Void
		//+0x00c Ldr              : Ptr32 _PEB_LDR_DATA
		//+0x010 ProcessParameters : Ptr32 _RTL_USER_PROCESS_PARAMETERS

		//WIN7 SP1�е�ƫ����
		//+0x1a4 LastThreadExitStatus : Int4B
		//+0x1a8 Peb              : Ptr32 _PEB
		//+0x1ac PrefetchTrace    : _EX_FAST_REF

		//_PEB_LDR_DATA��PEB�е�ƫ��
		//+0x008 ImageBaseAddress : Ptr32 Void
		//+0x00c Ldr              : Ptr32 _PEB_LDR_DATA
		//+0x010 ProcessParameters : Ptr32 _RTL_USER_PROCESS_PARAMETERS

		if (ulOSVersion == OS_WINDOWS_XP)
		{
			ulPEB = *(PULONG)((ULONG)pEProcess + 0x1b0);
		}
		else if (ulOSVersion == OS_WINDOWS_7_2008R2)
		{
			ulPEB = *(PULONG)((ULONG)pEProcess + 0x1a8);		
		}

		//PPEB_LDR_DATA
		ulLDR = *(PULONG)(ulPEB + 0x00c);

		//��Ϊ����ͷ��
		pListHead = (PLIST_ENTRY)(ulLDR + 0x00c);

		for (plistCurrent=pListHead->Flink;plistCurrent!=pListHead;plistCurrent=plistCurrent->Flink)
		{
			pLdr = CONTAINING_RECORD(plistCurrent,
				LDR_DATA_TABLE_ENTRY,
				InLoadOrderLinks);

			
			if (pLdr->EntryPoint && pLdr->FullDllName.Buffer) 
			{
				DbgPrint("DriveName : %ws\n",pLdr->FullDllName.Buffer);
				DbgPrint("ImageBase : 0x%08X.\n",pLdr->DllBase);
				DbgPrint("ImageSize : 0x%08X.\n",pLdr->SizeOfImage);
				DbgPrint("EntryPoint : 0x%08X.\n",pLdr->EntryPoint);
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("process_hideModule  EXCEPTION_EXECUTE_HANDLER error!");
		return FALSE;
	}
}

//��ý��̾����Ϣ
NTSTATUS process_getProcessHandleInfo(ULONG ulPID){
	NTSTATUS status;
	int i;
    ULONG ulSize = 0x1000;
	PSYSTEM_HANDLE_INFORMATION pSystem_handle_information;
    CLIENT_ID clientID;
    OBJECT_ATTRIBUTES object_attributes;
    HANDLE hProcess;
    HANDLE hDupObj;
    OBJECT_BASIC_INFORMATION object_basic_information;
    PKOBJECT_NAME_INFORMATION pKobject_name_information;  
    WCHAR wstrHandleTypeName[25];
    PPROCESS_HANDLE_INFO p;
    PFILE_NAME_INFORMATION pFile_name_information;
	PUNICODE_STRING ustrName;
    IO_STATUS_BLOCK ioStatus;

	__try
	{
		pProcessHandleInfo = pProcessHandleInfoHead = (PPROCESS_HANDLE_INFO)ExAllocatePool(PagedPool,sizeof(PROCESS_HANDLE_INFO));
        pProcessHandleInfo->next = NULL;

		//
		pSystem_handle_information = (PSYSTEM_HANDLE_INFORMATION)ExAllocatePool(PagedPool,ulSize); 

		while ((status = ZwQuerySystemInformation(SystemHandleInformation,pSystem_handle_information, ulSize, NULL))
			== STATUS_INFO_LENGTH_MISMATCH)            
		{                 
			ExFreePool(pSystem_handle_information);                 
			pSystem_handle_information = (PSYSTEM_HANDLE_INFORMATION)ExAllocatePool(PagedPool,ulSize = ulSize*2);   
		} 

		if (!NT_SUCCESS(status))
		{
			DbgPrint("process_getProcessHandleInfo ZwQuerySystemInformation error!");
			ExFreePool(pSystem_handle_information);
			return status;
		}

		//
		//DbgPrint("pSystem_handle_information->NumberOfHandles:%d",pSystem_handle_information->NumberOfHandles);

		for (i=0;i<pSystem_handle_information->NumberOfHandles;i++)
		{
			if (pSystem_handle_information->Information[i].UniqueProcessId == ulPID)
			{			
				//
                p = (PPROCESS_HANDLE_INFO)ExAllocatePool(PagedPool,sizeof(PROCESS_HANDLE_INFO));
                RtlZeroMemory(p,sizeof(PROCESS_HANDLE_INFO));
		
				if (pSystem_handle_information->Information[i].UniqueProcessId != ulProcessID)
				{
					RtlZeroMemory(&clientID,sizeof(CLIENT_ID));
					clientID.UniqueProcess = (HANDLE)ulPID;
					clientID.UniqueThread = NULL;

					InitializeObjectAttributes(&object_attributes,NULL,0,NULL,NULL);

					status = ZwOpenProcess(&hProcess ,PROCESS_DUP_HANDLE ,&object_attributes ,&clientID);
					if (!NT_SUCCESS(status))
					{
						DbgPrint("process_getProcessHandleInfo ZwOpenProcess error!");
						ExFreePool(pSystem_handle_information);
						return status;
					}

					status = ZwDuplicateObject(hProcess,(HANDLE)pSystem_handle_information->Information[i].HandleValue,ZwCurrentProcess(),\
						&hDupObj,PROCESS_ALL_ACCESS,0,DUPLICATE_SAME_ACCESS);
					if (!NT_SUCCESS(status))
					{
						DbgPrint("process_getProcessHandleInfo ZwDuplicateObject error!");
						ExFreePool(pSystem_handle_information);
						return status;
					}
				}
				else
				{
					hDupObj = (HANDLE)pSystem_handle_information->Information[i].HandleValue;
				}

				//basic information
				RtlZeroMemory(&object_basic_information,sizeof(OBJECT_BASIC_INFORMATION));
				ZwQueryObject(hDupObj,ObjectBasicInformation,&object_basic_information,sizeof(OBJECT_BASIC_INFORMATION),NULL);

				RtlZeroMemory(&wstrHandleTypeName,sizeof(wstrHandleTypeName));
				common_getHandleTypeName(pSystem_handle_information->Information[i].ObjectTypeIndex,&wstrHandleTypeName);

				//DbgPrint("typeName:%ws,ObjectName:%ws,handle:0x%08X,handleObject:0x%08X,handleType:%d,ReferenceCount:%d",wstrHandleName,pKobject_name_information->Name.Buffer,pSystem_handle_information->Information[i].HandleValue,pSystem_handle_information->Information[i].Object,pSystem_handle_information->Information[i].ObjectTypeIndex,object_basic_information.ReferenceCount);	
		       
				wcscpy(p->wstrHandleTypeName,wstrHandleTypeName);

				//�ļ����������ر���
				if(pSystem_handle_information->Information[i].ObjectTypeIndex == OB_TYPE_FILE)
				{
	                pFile_name_information = (PFILE_NAME_INFORMATION)ExAllocatePool(PagedPool,sizeof(WCHAR)*256);
					RtlZeroMemory(pFile_name_information,sizeof(WCHAR)*256);

					status = ZwQueryInformationFile(hDupObj,&ioStatus,pFile_name_information,sizeof(WCHAR)*256,FileNameInformation);
					if (!NT_SUCCESS(status))
					{
						//DbgPrint("status %08X",status);
						wcscpy(p->wstrHandleName,L"");
					}
					else
					{
						DbgPrint("name:%ws",pFile_name_information->FileName);
						wcscpy(p->wstrHandleName,pFile_name_information->FileName);
					}		

					ExFreePool(pFile_name_information);
				}
				//ע��������ͬ����Ҫ�ر���
				else if(pSystem_handle_information->Information[i].ObjectTypeIndex == OB_TYPE_KEY)
				{
					wcscpy(p->wstrHandleName,L"");
				}
				else
				{
					pKobject_name_information = (PKOBJECT_NAME_INFORMATION)ExAllocatePool(PagedPool,object_basic_information.NameInformationLength);
					RtlZeroMemory(pKobject_name_information ,object_basic_information.NameInformationLength);

					status = ZwQueryObject(hDupObj ,ObjectNameInformation ,pKobject_name_information ,object_basic_information.NameInformationLength ,NULL);
					if (!NT_SUCCESS(status))
					{
						wcscpy(p->wstrHandleName,L"");
					}
					else
					{
						wcscpy(p->wstrHandleName,pKobject_name_information->Name.Buffer);
					}		

					ExFreePool(pKobject_name_information);
				}
                
                p->ulHandle = pSystem_handle_information->Information[i].HandleValue;
                p->ulObject = (ULONG)pSystem_handle_information->Information[i].Object;
                p->ulTypeIndex = pSystem_handle_information->Information[i].ObjectTypeIndex;
                p->ulReferenceCount = object_basic_information.ReferenceCount;
                p->next = NULL;

				pProcessHandleInfo->next = p;
				pProcessHandleInfo = p;
			}
		}

        pProcessHandleInfo = pProcessHandleInfoHead;

		ExFreePool(pSystem_handle_information);
		return STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("process_getProcessHandleInfo EXCEPTION_EXECUTE_HANDLER error!");
		ExFreePool(pSystem_handle_information);
		return STATUS_UNSUCCESSFUL;
	}
}

//�ͷ��ڴ�
VOID process_freeProcessHandleInfo()
{
	PPROCESS_HANDLE_INFO p = NULL;

	__try
	{
		while(pProcessHandleInfo != NULL)
		{
			p = pProcessHandleInfo->next;
			ExFreePool(pProcessHandleInfo);
			pProcessHandleInfo   = p;
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("process_freeProcessHandleInfo EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//��ô�С
ULONG process_getProcessHandleInfoCount()
{
	__try
	{
		PPROCESS_HANDLE_INFO p=NULL;
		ULONG ulCount=0;

		//��ô�С
		for (p=pProcessHandleInfo->next;p;p=p->next)
		{
			ulCount++;
		}

		DbgPrint("process_getProcessHandleInfoCount ulCount:%d",ulCount);
		return ulCount;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("process_getProcessHandleInfoCount EXCEPTION_EXECUTE_HANDLER error!");
		return 0;
	}	
}

//�������ݵ�ring3
NTSTATUS process_getProcessHandleInfo_send(PPROCESS_HANDLE_INFO pBuffer)
{
	PPROCESS_HANDLE_INFO p=NULL;
	ULONG ulIndex = 0;

	__try
	{
		//����
		for (p=pProcessHandleInfo->next;p;p=p->next)
		{
			RtlCopyMemory(pBuffer + ulIndex,p,sizeof(PROCESS_HANDLE_INFO));

			ulIndex++;
		}

		//�ͷ��ڴ�
		process_freeProcessHandleInfo();

		return STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("process_getProcessHandleInfo_send EXCEPTION_EXECUTE_HANDLER error!");
		return STATUS_UNSUCCESSFUL;
	}
}