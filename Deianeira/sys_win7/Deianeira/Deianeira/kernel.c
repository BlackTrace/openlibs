#include "kernel.h"

//����������������������ַ
ULONG GetRoutineArraryAddress(UNICODE_STRING MainFuncName,ULONG Control)
{
	ULONG ArraryAddr;
	ULONG MainAddress=(ULONG)MmGetSystemRoutineAddress(&MainFuncName);

	if (MainAddress!=0)
	{

		//6a00            push    0
		__asm{
			mov eax,Control
				cmp eax,1//Ϊ1����CmRegisterCALLBACK
				jz Second
				mov esi,MainAddress
				xor ebx,ebx
star:
			mov dl,byte ptr [esi+ebx]
			cmp dl,0x6a
				jz ass
				inc ebx
				jmp star

ass:
			mov dl,byte ptr [esi+ebx+1]
			cmp dl,0x00
				jz righ
				inc ebx
				jmp star

righ:
			mov edx,[esi+ebx-4]
			mov ArraryAddr,edx
				mov dl,byte ptr [esi+ebx-5]
			cmp dl,0xbf
				jz end
				cmp dl,0xbe
				jz end
				inc ebx
				jmp star
				//������ȣ��������ǰ�
				//����һ��
				//805c6e94 bfe0b25580      mov     edi,offset nt!PspCreateProcessNotifyRoutine (8055b2e0)
				//805c6e99 6a00            push    0
				mov ArraryAddr,0

Second:
			mov esi,MainAddress
				xor ebx,ebx
Second1:
			inc ebx
				mov dl,byte ptr [esi+ebx]
			cmp dl,0x89
				jnz Second1
				mov dl,byte ptr [esi+ebx+1]
			cmp dl,0x7d
				jnz Second1
				mov eax,[esi+ebx-4]
			mov ArraryAddr,eax
end:
		}

		return ArraryAddr;
	}

	return 0;
}

//��ȡ������ַ
VOID kernel_systemCallbackGetFuncAddress(ULONG ulFuncAddress,ULONG *LoadImage,BOOLEAN ArryOrList)
{
	ULONG i;
	if (ArryOrList)
	{
		PEX_FAST_REF FastRef=(PEX_FAST_REF)ulFuncAddress;
		for (i=0;i<8;i++)
		{
			PEX_CALLBACK_ROUTINE_BLOCK Point=(PEX_CALLBACK_ROUTINE_BLOCK)((FastRef->Value>>3)<<3);
			if (MmIsAddressValid(Point))
		    {
		   	    LoadImage[i]=(ULONG)Point->Function;
		    }
			FastRef++;
		}
	}
}

//
VOID kernel_systemCallbackFind(ULONG ulFuncAddress,PKERNEL_SYSTEM_CALLBACK_INFO pKernelSystemCallBackInfo)
{
	NTSTATUS status;
    PSYSTEM_MODULE_INFO p;

	__try
	{
		if (!global_pDriverObj)
		{
			DbgPrint("kernel_systemCallbackFind global_pDriverObj error!");
			return;
		}

		//����б���������ģ����Ϣ�Ļ������������ͷ��ڴ�
		if (pSystemModuleInfo)
		{
			//�ͷ��ڴ�
			sysmodule_freeDriverModuleInfo();
		}
		
		//����б�
		status = sysmodule_enumDrivers(global_pDriverObj);

		//���ʧ��
		if (!NT_SUCCESS(status))
		{
			DbgPrint("kernel_systemCallbackFind sysmodule_enumDrivers error!");
			return;
		}

		//����б�����������Ϣ�Ļ�����
		//ѭ�������б�
		for (p = pSystemModuleInfo;p;p = p->next)
		{
			if (ulFuncAddress > p->ulBaseAddress && ulFuncAddress < (p->ulBaseAddress + p->ulSize))
			{
				wcscpy(pKernelSystemCallBackInfo->PathName,p->FullName);
				return;
			}			
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("kernel_systemCallbackFind EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//��õ�ַ
PLIST_ENTRY kernel_systemCallbackGetBugCheckCallbackListHeadOffset()
{
	UCHAR *cPtr, *pOpcode;
	ULONG ulLength = 0;
	ULONG i;

	__try
	{
		/*
		nt!KeRegisterBugCheckCallback+0x2b:
		804f8ebf 8b4d0c          mov     ecx,dword ptr [ebp+0Ch]
		804f8ec2 8b5510          mov     edx,dword ptr [ebp+10h]
		804f8ec5 894808          mov     dword ptr [eax+8],ecx
		804f8ec8 56              push    esi
		804f8ec9 8b7514          mov     esi,dword ptr [ebp+14h]
		804f8ecc 03ca            add     ecx,edx
		804f8ece 57              push    edi
		804f8ecf 8b7d18          mov     edi,dword ptr [ebp+18h]
		804f8ed2 03ce            add     ecx,esi
		804f8ed4 03cf            add     ecx,edi
		804f8ed6 897010          mov     dword ptr [eax+10h],esi
		804f8ed9 897814          mov     dword ptr [eax+14h],edi
		804f8edc 89500c          mov     dword ptr [eax+0Ch],edx
		804f8edf 894818          mov     dword ptr [eax+18h],ecx
		804f8ee2 c6401c01        mov     byte ptr [eax+1Ch],1
		804f8ee6 8b0d78405580    mov     ecx,dword ptr [nt!KeBugCheckCallbackListHead (80554078)]  //*
		804f8eec 8908            mov     dword ptr [eax],ecx
		804f8eee c7400478405580  mov     dword ptr [eax+4],offset nt!KeBugCheckCallbackListHead (80554078)
		804f8ef5 5f              pop     edi
		804f8ef6 894104          mov     dword ptr [ecx+4],eax
		804f8ef9 a378405580      mov     dword ptr [nt!KeBugCheckCallbackListHead (80554078)],eax
		804f8efe c645ff01        mov     byte ptr [ebp-1],1
		804f8f02 5e              pop     esi
		*/

		//
		for (cPtr = (PUCHAR)KeRegisterBugCheckCallback; cPtr < (PUCHAR)KeRegisterBugCheckCallback + PAGE_SIZE;cPtr+=ulLength) 
		{
			//��ñ��������ֽ���
			ulLength = SizeOfCode(cPtr, &pOpcode);

			//��������ʧ�ܵĻ�
			if (ulLength==0) 
			{
				DbgPrint("kernel_systemCallbackGetBugCheckCallbackListHeadOffset ulLength error!");
				return NULL;
			}

			//��һ��b9��
			if((*pOpcode == 0x8B) && (*(pOpcode+1) == 0xD))
			{			
				DbgPrint("kernel_systemCallbackGetBugCheckCallbackListHeadOffset address:%08X",(*((PULONG)(cPtr + 2))));
				//���ص�ַ
				return(PLIST_ENTRY)(*((PULONG)(cPtr + 2)));	
			}
		}

		return NULL;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("kernel_systemCallbackGetBugCheckCallbackListHeadOffset EXCEPTION_EXECUTE_HANDLER error!");
		return NULL;
	}
}

//ö��BugCheckReasonCallback
NTSTATUS kernel_systemCallbackEnumBugCheckReasonCallback()
{
	ULONG ulDispatch;
	PLIST_ENTRY CurrEntry = NULL;
	PLIST_ENTRY g_KeBugCheckReasonCallbackListHead = NULL;
	PKERNEL_SYSTEM_CALLBACK_INFO pKernelSystemCallbackInfo_tmp;

	__try
	{
		//��ö��е�ַ
		g_KeBugCheckReasonCallbackListHead = kernel_systemCallbackGetBugCheckCallbackListHeadOffset();

		//������ʧ��
		if (g_KeBugCheckReasonCallbackListHead == NULL)
		{
			DbgPrint("EnumBugCheckReasonCallback g_KeBugCheckReasonCallbackListHead error!");
			//����ʧ��
			return STATUS_UNSUCCESSFUL;
		}

		//�����б�
		for(CurrEntry=g_KeBugCheckReasonCallbackListHead->Flink; CurrEntry != g_KeBugCheckReasonCallbackListHead; CurrEntry=CurrEntry->Flink)
		{
			ulDispatch = *(PULONG)((ULONG)CurrEntry+0x8);

			pKernelSystemCallbackInfo_tmp = (PKERNEL_SYSTEM_CALLBACK_INFO)ExAllocatePool(NonPagedPool,sizeof(KERNEL_SYSTEM_CALLBACK_INFO));

			RtlZeroMemory(pKernelSystemCallbackInfo_tmp,sizeof(KERNEL_SYSTEM_CALLBACK_INFO));

			//��õ�ַ���ڵ�ģ��
			kernel_systemCallbackFind(ulDispatch,pKernelSystemCallbackInfo_tmp);

			pKernelSystemCallbackInfo_tmp->ulCallBackAddress = ulDispatch;
			pKernelSystemCallbackInfo_tmp->ulType = 6;

			pKernelSystemCallbackInfo_tmp->next = NULL;

			pKernelSystemCallbackInfo->next = pKernelSystemCallbackInfo_tmp;
			pKernelSystemCallbackInfo = pKernelSystemCallbackInfo_tmp;

			DbgPrint("[BugCheckReason]:0x%X,path:%ws",pKernelSystemCallbackInfo_tmp->ulCallBackAddress,pKernelSystemCallbackInfo_tmp->PathName);
		}

		//���سɹ�
		return STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("kernel_systemCallbackEnumBugCheckReasonCallback EXCEPTION_EXECUTE_HANDLER error!");
		return STATUS_UNSUCCESSFUL;
	}
}

//
PLIST_ENTRY kernel_systemCallbackGetReasonCallbackListHeadOffset()
{
	UCHAR *cPtr, *pOpcode;
	ULONG ulLength = 0;
	ULONG i;

	__try
	{
		/*
		nt!KeRegisterBugCheckReasonCallback+0x29:
		804f9047 8b4d0c          mov     ecx,dword ptr [ebp+0Ch]
		804f904a 8b5510          mov     edx,dword ptr [ebp+10h]
		804f904d 894808          mov     dword ptr [eax+8],ecx
		804f9050 56              push    esi
		804f9051 8b7514          mov     esi,dword ptr [ebp+14h]
		804f9054 03ca            add     ecx,edx
		804f9056 03ce            add     ecx,esi
		804f9058 89700c          mov     dword ptr [eax+0Ch],esi
		804f905b 895014          mov     dword ptr [eax+14h],edx
		804f905e 894810          mov     dword ptr [eax+10h],ecx
		804f9061 c6401801        mov     byte ptr [eax+18h],1
		804f9065 8b0d70405580    mov     ecx,dword ptr [nt!KeBugCheckReasonCallbackListHead (80554070)]  //*
		804f906b 8908            mov     dword ptr [eax],ecx
		804f906d c7400470405580  mov     dword ptr [eax+4],offset nt!KeBugCheckReasonCallbackListHead (80554070)
		804f9074 894104          mov     dword ptr [ecx+4],eax
		804f9077 a370405580      mov     dword ptr [nt!KeBugCheckReasonCallbackListHead (80554070)],eax
		804f907c fec3            inc     bl
		804f907e 5e              pop     esi
		*/

		//
		for (cPtr = (PUCHAR)KeRegisterBugCheckReasonCallback; cPtr < (PUCHAR)KeRegisterBugCheckReasonCallback + PAGE_SIZE;cPtr+=ulLength) 
		{
			//��ñ��������ֽ���
			ulLength = SizeOfCode(cPtr, &pOpcode);

			//��������ʧ�ܵĻ�
			if (ulLength==0) 
			{
				DbgPrint("kernel_systemCallbackGetReasonCallbackListHeadOffset ulLength error!");
				return NULL;
			}

			//��һ��b9��
			if((*pOpcode == 0x8B) && (*(pOpcode+1) == 0xD))
			{			
				DbgPrint("kernel_systemCallbackGetReasonCallbackListHeadOffset address:%08X",(*((PULONG)(cPtr + 2))));
				//���ص�ַ
				return(PLIST_ENTRY)(*((PULONG)(cPtr + 2)));	
			}
		}

		return NULL;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("kernel_systemCallbackGetReasonCallbackListHeadOffset EXCEPTION_EXECUTE_HANDLER error!");
		return NULL;
	}
}

//
NTSTATUS kernel_systemCallbackEnumBugCheckCallback()
{
	ULONG ulDispatch;
	PLIST_ENTRY CurrEntry = NULL;
	PLIST_ENTRY g_KeBugCheckCallbackListHead = NULL;
	PKERNEL_SYSTEM_CALLBACK_INFO pKernelSystemCallbackInfo_tmp;

	__try
	{
		//��ö��е�ַ
		g_KeBugCheckCallbackListHead = kernel_systemCallbackGetReasonCallbackListHeadOffset();

		//������ʧ��
		if (g_KeBugCheckCallbackListHead == NULL)
		{
			DbgPrint("kernel_systemCallbackEnumBugCheckCallback g_KeBugCheckCallbackListHead error!");
			//
			return STATUS_UNSUCCESSFUL;
		}

		//�����б�
		for(CurrEntry=g_KeBugCheckCallbackListHead->Flink; CurrEntry != g_KeBugCheckCallbackListHead; CurrEntry=CurrEntry->Flink)
		{
			ulDispatch = *(PULONG)((ULONG)CurrEntry+0x8);

			pKernelSystemCallbackInfo_tmp = (PKERNEL_SYSTEM_CALLBACK_INFO)ExAllocatePool(NonPagedPool,sizeof(KERNEL_SYSTEM_CALLBACK_INFO));

			RtlZeroMemory(pKernelSystemCallbackInfo_tmp,sizeof(KERNEL_SYSTEM_CALLBACK_INFO));

			//��õ�ַ���ڵ�ģ��
			kernel_systemCallbackFind(ulDispatch,pKernelSystemCallbackInfo_tmp);

			pKernelSystemCallbackInfo_tmp->ulCallBackAddress = ulDispatch;
			pKernelSystemCallbackInfo_tmp->ulType = 5;
			pKernelSystemCallbackInfo_tmp->next = NULL;

			pKernelSystemCallbackInfo->next = pKernelSystemCallbackInfo_tmp;
			pKernelSystemCallbackInfo = pKernelSystemCallbackInfo_tmp;

			//DbgPrint("[BugCheckCallback]:0x%X,path:%ws",pKernelSystemCallbackInfo_tmp->ulCallBackAddress,pKernelSystemCallbackInfo_tmp->PathName);
		}

		//
		return STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("kernel_systemCallbackEnumBugCheckCallback EXCEPTION_EXECUTE_HANDLER error!");
		return STATUS_UNSUCCESSFUL;
	}
}

PLIST_ENTRY kernel_systemCallbackGetShutdownQueueHeadOffset()
{
	UCHAR *cPtr, *pOpcode;
	ULONG ulLength = 0;
	ULONG i;

	__try
	{
		/*
	    nt!IoRegisterShutdownNotification+0x21:
	    8056abf7 56              push    esi
	    8056abf8 8b7508          mov     esi,dword ptr [ebp+8]
	    8056abfb 8bce            mov     ecx,esi
	    8056abfd 897708          mov     dword ptr [edi+8],esi
	    8056ac00 e87b8dfbff      call    nt!ObfReferenceObject (80523980)
	    8056ac05 8bd7            mov     edx,edi
	    8056ac07 b960295580      mov     ecx,offset nt!IopNotifyShutdownQueueHead (80552960)  //*
	    8056ac0c e8739bf8ff      call    nt!IopInterlockedInsertHeadList (804f4784)
	    8056ac11 804e1d08        or      byte ptr [esi+1Dh],8
	    8056ac15 33c0            xor     eax,eax
	    8056ac17 5e              pop     esi
	    */
		//
		for (cPtr = (PUCHAR)IoRegisterShutdownNotification; cPtr < (PUCHAR)IoRegisterShutdownNotification + PAGE_SIZE;cPtr+=ulLength) 
		{
			//��ñ��������ֽ���
			ulLength = SizeOfCode(cPtr, &pOpcode);

			//��������ʧ�ܵĻ�
			if (ulLength==0) 
			{
				DbgPrint("kernel_systemCallbackGetShutdownQueueHeadOffset ulLength error!");
				return NULL;
			}

			//��һ��b9��
			if(*pOpcode == 0xB9)
			{			
				DbgPrint("kernel_systemCallbackGetShutdownQueueHeadOffset address:%08X",(*((PULONG)(cPtr + 1))));
				//���ص�ַ
				return(PLIST_ENTRY)(*((PULONG)(cPtr + 1)));	
			}
		}

		return NULL;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("kernel_systemCallbackGetShutdownQueueHeadOffset EXCEPTION_EXECUTE_HANDLER error!");
		return NULL;
	}
}

//
NTSTATUS kernel_systemCallbackEnumShutdownCallback()
{
	PDEVICE_OBJECT DevObj = NULL;
	PDRIVER_OBJECT DrvObj = NULL;
	ULONG ulDispatch = 0;
	PLIST_ENTRY CurrEntry = NULL;
    PLIST_ENTRY g_IopNotifyShutdownQueueHead = NULL;
	PKERNEL_SYSTEM_CALLBACK_INFO pKernelSystemCallbackInfo_tmp;

	__try
	{
		//��ö��е�ַ
		g_IopNotifyShutdownQueueHead = kernel_systemCallbackGetShutdownQueueHeadOffset();

		//������ʧ��
		if (g_IopNotifyShutdownQueueHead ==NULL)
		{
			DbgPrint("kernel_systemCallbackEnumShutdownCallback g_IopNotifyShutdownQueueHead error!");
			return STATUS_UNSUCCESSFUL;
		}

		//�����б�
		for(CurrEntry=g_IopNotifyShutdownQueueHead->Flink; CurrEntry != g_IopNotifyShutdownQueueHead; CurrEntry=CurrEntry->Flink)
		{
			DevObj = (PDEVICE_OBJECT)(*(PULONG)((ULONG)CurrEntry+8));
			DrvObj = DevObj->DriverObject;
			ulDispatch = (ULONG)(DrvObj->MajorFunction[IRP_MJ_SHUTDOWN]);

			pKernelSystemCallbackInfo_tmp = (PKERNEL_SYSTEM_CALLBACK_INFO)ExAllocatePool(NonPagedPool,sizeof(KERNEL_SYSTEM_CALLBACK_INFO));

			RtlZeroMemory(pKernelSystemCallbackInfo_tmp,sizeof(KERNEL_SYSTEM_CALLBACK_INFO));

			//��õ�ַ���ڵ�ģ��
			kernel_systemCallbackFind(ulDispatch,pKernelSystemCallbackInfo_tmp);

			pKernelSystemCallbackInfo_tmp->ulCallBackAddress = ulDispatch;
			pKernelSystemCallbackInfo_tmp->ulType = 7;
			pKernelSystemCallbackInfo_tmp->ulAttatch = (ULONG)DevObj;
			pKernelSystemCallbackInfo_tmp->next = NULL;

			pKernelSystemCallbackInfo->next = pKernelSystemCallbackInfo_tmp;
			pKernelSystemCallbackInfo = pKernelSystemCallbackInfo_tmp;

			//DbgPrint("[shutdown]:0x%X,path:%ws",pKernelSystemCallbackInfo_tmp->ulCallBackAddress,pKernelSystemCallbackInfo_tmp->PathName);
		}

		//
		return STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("kernel_systemCallbackEnumShutdownCallback EXCEPTION_EXECUTE_HANDLER error!");
		return STATUS_UNSUCCESSFUL;
	}
}

//���ϵͳ�Ļص�����
NTSTATUS kernel_systemCallbackEnum()
{
	UNICODE_STRING ustrFuncName;
	ULONG ulFuncAddress;

	ULONG LoadImage[8]={0};
	ULONG CreateProcess[8]={0};
	ULONG CreateThread[8]={0};
	ULONG CmRCallback[8]={0};

	ULONG i;
    PKERNEL_SYSTEM_CALLBACK_INFO pKernelSystemCallbackInfo_tmp;
    NTSTATUS status;

	__try
	{
		//pKernelSystemCallbackInfo��¼ͷ��
		pKernelSystemCallbackInfo = pKernelSystemCallbackInfoHead = (PKERNEL_SYSTEM_CALLBACK_INFO)ExAllocatePool(NonPagedPool,sizeof(KERNEL_SYSTEM_CALLBACK_INFO));
		pKernelSystemCallbackInfo->next = NULL;

		//PsSetCreateProcessNotifyRoutine
		RtlInitUnicodeString(&ustrFuncName,L"PsSetCreateProcessNotifyRoutine");
		ulFuncAddress=GetRoutineArraryAddress(ustrFuncName,0);

		if (ulFuncAddress)
		{
			kernel_systemCallbackGetFuncAddress(ulFuncAddress,CreateProcess,TRUE);	
		}	

		for (i=0;i<8;i++)
		{
			if (CreateProcess[i]!=0)
			{
				pKernelSystemCallbackInfo_tmp = (PKERNEL_SYSTEM_CALLBACK_INFO)ExAllocatePool(NonPagedPool,sizeof(KERNEL_SYSTEM_CALLBACK_INFO));

				RtlZeroMemory(pKernelSystemCallbackInfo_tmp,sizeof(KERNEL_SYSTEM_CALLBACK_INFO));

				kernel_systemCallbackFind(CreateProcess[i],pKernelSystemCallbackInfo_tmp);

				pKernelSystemCallbackInfo_tmp->ulEntryAddress = ulFuncAddress;
				pKernelSystemCallbackInfo_tmp->ulCallBackAddress = CreateProcess[i];
				pKernelSystemCallbackInfo_tmp->ulType=1;
				pKernelSystemCallbackInfo_tmp->next = NULL;

				//DbgPrint("kernel_systemCallbackEnum CreateProcess fullName:%ws",pKernelSystemCallbackInfo_tmp->PathName);
				pKernelSystemCallbackInfo->next = pKernelSystemCallbackInfo_tmp;

				pKernelSystemCallbackInfo = pKernelSystemCallbackInfo_tmp;
			}
		}

		//PsSetCreateThreadNotifyRoutine
		RtlInitUnicodeString(&ustrFuncName,L"PsSetCreateThreadNotifyRoutine");
		ulFuncAddress=GetRoutineArraryAddress(ustrFuncName,0);

		if (ulFuncAddress)
		{
			kernel_systemCallbackGetFuncAddress(ulFuncAddress,CreateThread,TRUE);	
		}

		for (i=0;i<8;i++)
		{
			if (CreateThread[i]!=0)
			{
				pKernelSystemCallbackInfo_tmp = (PKERNEL_SYSTEM_CALLBACK_INFO)ExAllocatePool(NonPagedPool,sizeof(KERNEL_SYSTEM_CALLBACK_INFO));

				RtlZeroMemory(pKernelSystemCallbackInfo_tmp,sizeof(KERNEL_SYSTEM_CALLBACK_INFO));

				kernel_systemCallbackFind(CreateThread[i],pKernelSystemCallbackInfo_tmp);

				pKernelSystemCallbackInfo_tmp->ulEntryAddress = ulFuncAddress;
				pKernelSystemCallbackInfo_tmp->ulCallBackAddress = CreateThread[i];
				pKernelSystemCallbackInfo_tmp->ulType=2;
				pKernelSystemCallbackInfo_tmp->next = NULL;

				//DbgPrint("kernel_systemCallbackEnum CreateThread fullName:%ws",pKernelSystemCallbackInfo_tmp->PathName);
				pKernelSystemCallbackInfo->next = pKernelSystemCallbackInfo_tmp;
				pKernelSystemCallbackInfo = pKernelSystemCallbackInfo_tmp;
			}
		}

		//PsSetLoadImageNotifyRoutine
		RtlInitUnicodeString(&ustrFuncName,L"PsSetLoadImageNotifyRoutine");
		ulFuncAddress=GetRoutineArraryAddress(ustrFuncName,0);

		//����ҵ���ַ
		if (ulFuncAddress)
		{
			kernel_systemCallbackGetFuncAddress(ulFuncAddress,LoadImage,TRUE);
		}

		for (i=0;i<8;i++)
		{
			if (LoadImage[i]!=0)
			{
				pKernelSystemCallbackInfo_tmp = (PKERNEL_SYSTEM_CALLBACK_INFO)ExAllocatePool(NonPagedPool,sizeof(KERNEL_SYSTEM_CALLBACK_INFO));

				RtlZeroMemory(pKernelSystemCallbackInfo_tmp,sizeof(KERNEL_SYSTEM_CALLBACK_INFO));

				kernel_systemCallbackFind(LoadImage[i],pKernelSystemCallbackInfo_tmp);

				pKernelSystemCallbackInfo_tmp->ulEntryAddress = ulFuncAddress;
				pKernelSystemCallbackInfo_tmp->ulCallBackAddress = LoadImage[i];
				pKernelSystemCallbackInfo_tmp->ulType=3;
				pKernelSystemCallbackInfo_tmp->next = NULL;

				//DbgPrint("kernel_systemCallbackEnum LoadImage fullName:%ws",pKernelSystemCallbackInfo_tmp->PathName);
				pKernelSystemCallbackInfo->next = pKernelSystemCallbackInfo_tmp;
				pKernelSystemCallbackInfo = pKernelSystemCallbackInfo_tmp;
			}
		}
			
		//CmRegisterCallback
		RtlInitUnicodeString(&ustrFuncName,L"CmRegisterCallback");
		ulFuncAddress=GetRoutineArraryAddress(ustrFuncName,1);

		if (ulFuncAddress)
		{
			kernel_systemCallbackGetFuncAddress(ulFuncAddress,CmRCallback,TRUE);	
		}	

		for (i=0;i<8;i++)
		{
			if (CmRCallback[i]!=0)
			{
				pKernelSystemCallbackInfo_tmp = (PKERNEL_SYSTEM_CALLBACK_INFO)ExAllocatePool(NonPagedPool,sizeof(KERNEL_SYSTEM_CALLBACK_INFO));

				RtlZeroMemory(pKernelSystemCallbackInfo_tmp,sizeof(KERNEL_SYSTEM_CALLBACK_INFO));

				kernel_systemCallbackFind(CmRCallback[i],pKernelSystemCallbackInfo_tmp);

				pKernelSystemCallbackInfo_tmp->ulEntryAddress = ulFuncAddress;
				pKernelSystemCallbackInfo_tmp->ulCallBackAddress = CmRCallback[i];
				pKernelSystemCallbackInfo_tmp->ulType=4;
				pKernelSystemCallbackInfo_tmp->next = NULL;

				//DbgPrint("kernel_systemCallbackEnum CmRCallback fullName:%ws",pKernelSystemCallbackInfo_tmp->PathName);
				pKernelSystemCallbackInfo->next = pKernelSystemCallbackInfo_tmp;
				pKernelSystemCallbackInfo = pKernelSystemCallbackInfo_tmp;
			}
		}	

		status = kernel_systemCallbackEnumBugCheckCallback();

		//���ʧ��
		if (!NT_SUCCESS(status))
		{
			DbgPrint("kernel_systemCallbackGetInfo kernel_systemCallbackEnumBugCheckCallback error!");
		}

		status = kernel_systemCallbackEnumBugCheckReasonCallback();

		//���ʧ��
		if (!NT_SUCCESS(status))
		{
			DbgPrint("kernel_systemCallbackGetInfo kernel_systemCallbackEnumBugCheckReasonCallback error!");
		}

		status = kernel_systemCallbackEnumShutdownCallback();

		//���ʧ��
		if (!NT_SUCCESS(status))
		{
			DbgPrint("kernel_systemCallbackGetInfo kernel_systemCallbackEnumShutdownCallback error!");
		}

		pKernelSystemCallbackInfo = pKernelSystemCallbackInfoHead->next;
		//���سɹ�
		return STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("kernel_systemCallbackGetInfo EXCEPTION_EXECUTE_HANDLER error!");
		return STATUS_UNSUCCESSFUL;
	}
}

//�ͷ��ڴ�
VOID kernel_freeSystemCallback()
{
	PKERNEL_SYSTEM_CALLBACK_INFO p = NULL;

	__try
	{
		while(pKernelSystemCallbackInfo != NULL)
		{
			p = pKernelSystemCallbackInfo->next;
			ExFreePool(pKernelSystemCallbackInfo);
			pKernelSystemCallbackInfo   = p;
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("kernel_freeSystemCallback EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//��ô�С
ULONG kernel_systemCallbackGetCount()
{
	__try
	{
		PKERNEL_SYSTEM_CALLBACK_INFO p=NULL;
		ULONG ulCount=0;

		//��ô�С
		for (p=pKernelSystemCallbackInfo;p;p=p->next)
		{
			ulCount++;
		}

		DbgPrint("kernel_systemCallbackGetCount ulCount:%d",ulCount);
		return ulCount;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("kernel_systemCallbackGetCount EXCEPTION_EXECUTE_HANDLER error!");
		return 0;
	}	
}

//�������ݵ�ring3
NTSTATUS kernel_systemCallbackGetInfo(PKERNEL_SYSTEM_CALLBACK_INFO pBuffer)
{
	PKERNEL_SYSTEM_CALLBACK_INFO p=NULL;
	ULONG ulIndex = 0;

	__try
	{
		//����
		for (p=pKernelSystemCallbackInfo;p;p=p->next)
		{
			RtlCopyMemory(pBuffer + ulIndex,p,sizeof(KERNEL_SYSTEM_CALLBACK_INFO));

			ulIndex++;
		}

		//�ͷ��ڴ�
		kernel_freeSystemCallback();

		return STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("kernel_systemCallbackGetInfo EXCEPTION_EXECUTE_HANDLER error!");
		return STATUS_UNSUCCESSFUL;
	}
}


//ж��
NTSTATUS kernel_systemCallbackKill(ULONG ulEntryAddress,ULONG ulFuncAddr)
{
	KIRQL kIRQL;
    KSPIN_LOCK  SpinLock;
	PEX_FAST_REF FastRef;
	ULONG i;

	__try
	{
		//�ж���ǰ�жϼ���
		if (KeGetCurrentIrql()>DISPATCH_LEVEL)
		{
			return STATUS_UNSUCCESSFUL;
		}

		FastRef=(PEX_FAST_REF)ulEntryAddress;

		for (i=0;i<8;i++)
		{

			PEX_CALLBACK_ROUTINE_BLOCK Point=(PEX_CALLBACK_ROUTINE_BLOCK)((FastRef->Value>>3)<<3);

			//�����ַ����
			if (MmIsAddressValid(Point))
			{
				if ((ULONG)Point->Function==ulFuncAddr)
				{
					//��Ҫ���������������ͬ��
					KeAcquireSpinLock(&SpinLock,&kIRQL);

					//ȥ���ڴ汣��
					UnProtected();

					__asm{
						pushad
							mov esi,ulEntryAddress
							mov eax,i
							mov dword ptr [esi + eax*4],0
							popad
					}

					//�ָ��ڴ汣��
					Protected();

					//�ͷ�������
					KeReleaseSpinLock(&SpinLock,kIRQL);

					//���سɹ�
					return STATUS_SUCCESS;
				}
			}
			FastRef++;
		}

		return STATUS_UNSUCCESSFUL;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("kernel_systemCallbackKill EXCEPTION_EXECUTE_HANDLER error!");
		return STATUS_UNSUCCESSFUL;
	}
}

//
NTSTATUS kernel_filterDriverEnumFunc(UNICODE_STRING ustrDirPath,ULONG ulDirType)
{
	HANDLE hDriver;
	NTSTATUS status;
	ULONG ulContext,ulRet=0;
	ULONG ulSize = 0x1000;
    OBJECT_ATTRIBUTES objectAttribute;
	PDEVICE_OBJECT pDeviceObject;
	PDRIVER_OBJECT pDriverObject;
	PDEVICE_OBJECT pAttachObject;
	UNICODE_STRING ustrDriverName;
	PDIRECTORY_BASIC_INFORMATION pDirectoryBasicInformation,pDirectoryBasicInformation_tmp;
	PKERNEL_FILTERDRIVER_INFO pKernelFilterDriverInfo_tmp;
	PLDR_DATA_TABLE_ENTRY pLdr;
	WCHAR DriverName[64]={L'\0'};

	__try
	{
		//��ʼ���ṹ��
		InitializeObjectAttributes(&objectAttribute,&ustrDirPath,OBJ_CASE_INSENSITIVE,NULL,NULL);

		//������Ŀ¼����
		status=ZwOpenDirectoryObject(&hDriver,DIRECTORY_QUERY,&objectAttribute);

		//����ɹ�
		if (!NT_SUCCESS(status))
		{
			DbgPrint("kernel_filterDriverEnumFunc ZwOpenDirectoryObject error!");
			return STATUS_UNSUCCESSFUL;
		}

		do 
		{
			//������С����
			ulSize = 2*ulSize;
			//����
			pDirectoryBasicInformation = (PDIRECTORY_BASIC_INFORMATION)ExAllocatePool(NonPagedPool,ulSize);
			//��������ڴ�ʧ��
			if (pDirectoryBasicInformation==NULL)
			{
				DbgPrint("kernel_filterDriverEnumFunc pDirectoryBasicInformation error!");
				return STATUS_UNSUCCESSFUL;
			}
			//
			status = ZwQueryDirectoryObject(hDriver,pDirectoryBasicInformation,ulSize,FALSE,TRUE,&ulContext,&ulRet);
		} while (status == STATUS_BUFFER_TOO_SMALL || status == STATUS_MORE_ENTRIES);

		//���û�гɹ�
		if (!NT_SUCCESS(status))
		{
			DbgPrint("kernel_filterDriverEnumFunc ZwQueryDirectoryObject error!");
			return STATUS_UNSUCCESSFUL;
		}

		pDirectoryBasicInformation_tmp = pDirectoryBasicInformation;

		while (pDirectoryBasicInformation_tmp->ObjectName.Length!=0 && pDirectoryBasicInformation_tmp->ObjectTypeName.Length!=0)
		{
			__try
			{

				if (ulDirType==0)
				{
					wcscpy(DriverName,L"\\Driver\\");
					wcscat(DriverName,pDirectoryBasicInformation_tmp->ObjectName.Buffer);
					RtlInitUnicodeString(&ustrDriverName,DriverName);
				}
				else
				{
					wcscpy(DriverName,L"\\FileSystem\\");
					wcscat(DriverName,pDirectoryBasicInformation_tmp->ObjectName.Buffer);
					RtlInitUnicodeString(&ustrDriverName,DriverName);
				}

				//���ü�����һ
				status=ObReferenceObjectByName(&ustrDriverName,OBJ_CASE_INSENSITIVE,NULL,FILE_ALL_ACCESS,IoDriverObjectType,KernelMode,NULL,(PVOID *)&pDriverObject);

				//�������ʧ��
				if (!NT_SUCCESS(status))
				{
					//DbgPrint("kernel_filterDriverEnumFunc ObReferenceObjectByName error!");
					pDirectoryBasicInformation_tmp++;
					continue;
				}

				if (pDriverObject==NULL)
				{
					pDirectoryBasicInformation_tmp++;
					continue;
				}

				pDeviceObject = pDriverObject->DeviceObject;

				while (pDeviceObject)
				{
					// �жϵ�ǰ�豸���Ƿ��й�������
					if (pDeviceObject->AttachedDevice)
					{
						//�����豸����
						pAttachObject = pDeviceObject->AttachedDevice;
						//�����豸ջ
						while (pAttachObject)
						{
							switch (pAttachObject->DeviceType)
							{
							case FILE_DEVICE_DISK:
							case FILE_DEVICE_DISK_FILE_SYSTEM:
							case FILE_DEVICE_FILE_SYSTEM:
							case FILE_DEVICE_KEYBOARD:
							case FILE_DEVICE_MOUSE:
							case FILE_DEVICE_NETWORK:
							case FILE_DEVICE_8042_PORT:
							case FILE_DEVICE_DFS_VOLUME:
							case FILE_DEVICE_DFS_FILE_SYSTEM:
							case FILE_DEVICE_UNKNOWN:
								{
									//�����ڴ�
									pKernelFilterDriverInfo_tmp = (PKERNEL_FILTERDRIVER_INFO)ExAllocatePool(NonPagedPool,sizeof(KERNEL_FILTERDRIVER_INFO));
									
									RtlZeroMemory(pKernelFilterDriverInfo_tmp,sizeof(KERNEL_FILTERDRIVER_INFO));

									//�ڴ�����
									RtlZeroMemory(pKernelFilterDriverInfo_tmp,sizeof(KERNEL_FILTERDRIVER_INFO));

									//�豸����
									pKernelFilterDriverInfo_tmp->ulObjType = pAttachObject->DeviceType;
									//�����豸
									pKernelFilterDriverInfo_tmp->ulAttachDevice = (ULONG)pAttachObject;

									//��������
									wcscpy(pKernelFilterDriverInfo_tmp->FileName,pAttachObject->DriverObject->DriverName.Buffer);
									//������������
									wcscpy(pKernelFilterDriverInfo_tmp->HostFileName,pDriverObject->DriverName.Buffer);

									//
									pLdr=(PLDR_DATA_TABLE_ENTRY)(pAttachObject->DriverObject->DriverSection);
									//
									if (pLdr!=NULL && pLdr->FullDllName.Buffer!=NULL)
									{
										//·��
										wcscpy(pKernelFilterDriverInfo_tmp->FilePath,pLdr->FullDllName.Buffer);
									}
									else
									{
										wcscpy(pKernelFilterDriverInfo_tmp->FilePath,pAttachObject->DriverObject->DriverName.Buffer);
									}

									pKernelFilterDriverInfo_tmp->next = NULL;

									pKernelFilterDriverInfo->next = pKernelFilterDriverInfo_tmp;
                                    pKernelFilterDriverInfo = pKernelFilterDriverInfo_tmp;

									//���������Ϣ
									//DbgPrint("fileName: %ws, \t\t hostFileName: %ws",pKernelFilterDriverInfo_tmp->FileName,pKernelFilterDriverInfo_tmp->HostFileName);
								}
								
								break;
							}

							//ָ����һ�������豸
							pAttachObject = pAttachObject->AttachedDevice;
						}
					}

					// �õ������ڴ������ϵ���һ���豸 DEVICE_OBJECT 
					pDeviceObject = pDeviceObject->NextDevice;
				}
				
				//���ü�����һ
				ObDereferenceObject(pDriverObject);

				//ָ���ƶ�����һ���ṹ��
				pDirectoryBasicInformation_tmp++;
				
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				//ָ���ƶ�����һ���ṹ��
				pDirectoryBasicInformation_tmp++;
			}
		}
		

		return STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("kernel_filterDriverEnumFunc EXCEPTION_EXECUTE_HANDLER error!");
		return STATUS_UNSUCCESSFUL;
	}
}

//ö�ٹ�������
NTSTATUS kernel_filterDriverEnum()
{
	NTSTATUS status;
	UNICODE_STRING ustrDirPath;

	__try
	{
		//���嵥�������ͷ��
		pKernelFilterDriverInfo = pKernelFilterDriverInfoHead = (PKERNEL_FILTERDRIVER_INFO)ExAllocatePool(NonPagedPool,sizeof(KERNEL_FILTERDRIVER_INFO));
        pKernelFilterDriverInfo->next = NULL;

		RtlInitUnicodeString(&ustrDirPath,L"\\FileSystem");
		status = kernel_filterDriverEnumFunc(ustrDirPath,1);	

		if (NT_SUCCESS(status))
		{
			DbgPrint("kernel_filterDriverEnumFunc FileSystem success!");
		}

		RtlInitUnicodeString(&ustrDirPath,L"\\Driver");
		status = kernel_filterDriverEnumFunc(ustrDirPath,0);	

		if (NT_SUCCESS(status))
		{
			DbgPrint("kernel_filterDriverEnumFunc Driver success!");
		}	

		pKernelFilterDriverInfo = pKernelFilterDriverInfoHead->next;

		return STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("kernel_filterDriverEnum EXCEPTION_EXECUTE_HANDLER error!");
		return STATUS_UNSUCCESSFUL;
	}
}

//�ͷ��ڴ�
VOID kernel_freeFilterDriver()
{
	PKERNEL_FILTERDRIVER_INFO p = NULL;

	__try
	{
		while(pKernelFilterDriverInfo != NULL)
		{
			p = pKernelFilterDriverInfo->next;
			ExFreePool(pKernelFilterDriverInfo);
			pKernelFilterDriverInfo   = p;
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("kernel_freeFilterDriver EXCEPTION_EXECUTE_HANDLER error!");
	}
}


//��ô�С
ULONG kernel_filterDriverGetCount()
{
	__try
	{
		PKERNEL_FILTERDRIVER_INFO p=NULL;
		ULONG ulCount=0;

		//��ô�С
		for (p=pKernelFilterDriverInfo;p;p=p->next)
		{
			ulCount++;
		}

		DbgPrint("kernel_filterDriverGetCount ulCount:%d",ulCount);
		return ulCount;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("kernel_filterDriverGetCount EXCEPTION_EXECUTE_HANDLER error!");
		return 0;
	}	
}

//�������ݵ�ring3
NTSTATUS kernel_filterDriverGetInfo(PKERNEL_FILTERDRIVER_INFO pBuffer)
{
	PKERNEL_FILTERDRIVER_INFO p=NULL;
	ULONG ulIndex = 0;

	__try
	{
		//����
		for (p=pKernelFilterDriverInfo;p;p=p->next)
		{
			RtlCopyMemory(pBuffer + ulIndex,p,sizeof(KERNEL_FILTERDRIVER_INFO));

			ulIndex++;
		}

		kernel_freeFilterDriver();

		return STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("kernel_filterDriverGetInfo EXCEPTION_EXECUTE_HANDLER error!");
		return STATUS_UNSUCCESSFUL;
	}
}

//ժ����������
NTSTATUS kernel_filterDriverKill(PDEVICE_OBJECT pAttachObj)
{
	PDEVICE_OBJECT pAttachedTo,pAttachedObj;

	ULONG ulDeviceObjectExtension;

	__try
	{
		//��ȡAttachObject��AttachedTo�豸

		//pAttachObj����ҽӵ��豸
		pAttachedObj=pAttachObj->AttachedDevice;

		ulDeviceObjectExtension=(ULONG)pAttachObj->DeviceObjectExtension;

		//pAttachObj�ҽӵ����豸
		pAttachedTo=(PDEVICE_OBJECT)(*(PULONG)(ulDeviceObjectExtension + 0x18));


		//��ʼժ��
		//pAttachObj����û�йҽ��豸
		if (pAttachedObj==NULL)
		{
			//����豸�������豸�������豸Ϊ����������棬û�о������棬�൱�ڶ�����
			pAttachedTo->AttachedDevice = pAttachedObj;
			//����豸�������Ǹ��豸ΪNULL
			*(PULONG)(ulDeviceObjectExtension + 0x18)=0;
		}
		else
		{
			pAttachedTo->AttachedDevice = pAttachedObj;
			*(PULONG)((ULONG)ulDeviceObjectExtension + 0x18)=0;
			*(PULONG)((ULONG)pAttachedObj->DeviceObjectExtension + 0x18)=(ULONG)pAttachedTo;
		}

		return STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("kernel_filterDriverKill EXCEPTION_EXECUTE_HANDLER error!");
		return STATUS_UNSUCCESSFUL;
	}
}


////��hash�����ҵ���Ӧ��base��size
//VOID FSDHashBaseToEnd(PFSD_INFO PFsd)
//{
//	for (int i=0;i<MAXDRIVER_NUM;i++)
//	{
//		if (phd->HashDriverObject[i].DriverObject!=0 && phd->HashDriverObject[i].ShortName!=NULL && phd->HashDriverObject[i].Size)
//		{
//
//			if (PFsd->CurrentAddr>phd->HashDriverObject[i].Base && PFsd->CurrentAddr<(phd->HashDriverObject[i].Base+phd->HashDriverObject[i].Size))
//			{
//				//�ҵ���
//
//				RtlCopyMemory(PFsd->ModulePath,phd->HashDriverObject[i].PathName,sizeof(phd->HashDriverObject[i].PathName));
//
//				return;
//			}
//		}
//
//	}
//
//	//û���ҵ���
//	wcscpy(PFsd->ModulePath,L"δ֪ģ��");
//	return;
//}
//
////���ntfs.sys
//NTSTATUS NtfsFSD(PFSD_INFO PFsd)
//{
//	UNICODE_STRING DriverName2;
//	NTSTATUS Status;
//
//	PDRIVER_OBJECT DriverObject=NULL;
//	RtlInitUnicodeString(&DriverName2,L"\\FileSystem\\Ntfs");
//
//	Status=ObReferenceObjectByName(&DriverName2,OBJ_CASE_INSENSITIVE,NULL,FILE_ALL_ACCESS,IoDriverObjectType,KernelMode,NULL,(PVOID *)&DriverObject);
//	if (NT_SUCCESS(Status))
//	{
//		//�õ�DRIVEROBJECT��
//		if (DriverObject)
//		{
//			for (ULONG i=0;i<28;i++)
//			{
//
//				////����ַ���������ص���Ϣ
//				PFsd[i].CurrentAddr=(ULONG)DriverObject->MajorFunction[i];
//				FSDHashBaseToEnd(&PFsd[i]);
//			}
//		}
//		ObDereferenceObject(DriverObject);
//		return STATUS_SUCCESS;	
//
//
//	}
//	else
//	{
//		return STATUS_UNSUCCESSFUL;
//	}
//}
//
////���fastfat.sys
//NTSTATUS FastFatFSD(PFSD_INFO PFsd)
//{
//	UNICODE_STRING DriverName2;
//	NTSTATUS Status;
//	LONG Major[28];
//	PDRIVER_OBJECT DriverObject=NULL;
//	RtlInitUnicodeString(&DriverName2,L"\\FileSystem\\Fastfat");
//
//	Status=ObReferenceObjectByName(&DriverName2,OBJ_CASE_INSENSITIVE,NULL,FILE_ALL_ACCESS,IoDriverObjectType,KernelMode,NULL,(PVOID *)&DriverObject);
//	if (NT_SUCCESS(Status))
//	{
//		//�õ�DRIVEROBJECT��
//		if (DriverObject)
//		{
//			for (ULONG i=0;i<28;i++)
//			{
//
//				//����ַ���������ص���Ϣ
//				PFsd[i].CurrentAddr=(ULONG)DriverObject->MajorFunction[i];
//				FSDHashBaseToEnd(&PFsd[i]);
//
//			}
//		}
//		ObDereferenceObject(DriverObject);
//		return STATUS_SUCCESS;	
//
//
//	}
//	else
//	{
//		return STATUS_UNSUCCESSFUL;
//	}
//
//}
//
//NTSTATUS FSD(PFSD_INFO PFsd)
//{
//	NTSTATUS Status1,Status2;
//	Status1=NtfsFSD(&PFsd[0]);	
//	Status2=FastFatFSD(&PFsd[28]);
//	if (NT_SUCCESS(Status1))//��ֹֻ��ntfs��ϵͳ���ߵ�һ��fastfat
//	{
//		return Status1;
//	}
//	else
//	{
//		return Status2;
//	}
//
//}
//
//NTSTATUS GetFatNtfsBase(PULONG Base)
//{
//	;
//	for (int i=0;i<MAXDRIVER_NUM;i++)
//	{
//		if (phd->HashDriverObject[i].DriverObject!=0 && phd->HashDriverObject[i].ShortName!=NULL && phd->HashDriverObject[i].Size)
//		{
//
//			if (_wcsicmp(phd->HashDriverObject[i].ShortName,L"Fastfat.sys")==0)
//			{
//				//�ҵ���
//
//
//				*Base=phd->HashDriverObject[i].Base;
//
//
//			}
//			else if (_wcsicmp(phd->HashDriverObject[i].ShortName,L"Ntfs.sys")==0)
//			{
//
//				*(Base+1)=phd->HashDriverObject[i].Base;
//			}
//
//		}
//
//	}
//	if (*Base!=0 || *(Base+1)!=0)//��һ��ȫ���е�
//	{
//		return STATUS_SUCCESS;
//	}
//	return STATUS_UNSUCCESSFUL;
//}
//
//
//NTSTATUS UnHookFsd(ULONG index,ULONG CurAddr,ULONG OldAddr)
//{
//	//0-27��ntfs��
//	//28-55��fat��
//	UNICODE_STRING DriverName2;
//	NTSTATUS Status;
//	KIRQL KirqlLoca;	
//	PDRIVER_OBJECT DriverObject=NULL;
//	if (index<28)//NTFS
//	{
//		RtlInitUnicodeString(&DriverName2,L"\\FileSystem\\Ntfs");
//	}
//	else if (index<56)
//	{
//
//		index-=28;
//		RtlInitUnicodeString(&DriverName2,L"\\FileSystem\\Ntfs");
//
//	}
//	else
//	{
//
//		return STATUS_UNSUCCESSFUL;
//	}
//	DbgPrint("%S\n",DriverName2.Buffer);
//	Status=ObReferenceObjectByName(&DriverName2,OBJ_CASE_INSENSITIVE,NULL,FILE_ALL_ACCESS,IoDriverObjectType,KernelMode,NULL,(PVOID *)&DriverObject);
//	if (NT_SUCCESS(Status))
//	{
//		//�õ�DRIVEROBJECT��
//		if (DriverObject)
//		{
//
//			if ((ULONG)(DriverObject->MajorFunction[index])==CurAddr)
//			{
//				KeAcquireSpinLock(&SpinLock,&KirqlLoca);
//				Unprotect();
//				__asm{
//
//					//ԭ�Ӳ���ָ��
//
//				}
//				DriverObject->MajorFunction[index]=(PDRIVER_DISPATCH)OldAddr;
//				Protect();
//				KeReleaseSpinLock(&SpinLock,KirqlLoca);
//			}
//			else
//			{
//				DbgPrint("[UnHookFsd]:failed\n");
//				Status=STATUS_UNSUCCESSFUL;
//			}
//
//		}
//		ObDereferenceObject(DriverObject);
//		return Status;	
//
//
//	}
//	else
//	{
//		return STATUS_UNSUCCESSFUL;
//	}
//
//}