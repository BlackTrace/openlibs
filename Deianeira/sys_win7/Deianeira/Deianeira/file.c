
#include "file.h"


//����ԭ�ȿ�ͷ��5���Լ�
BYTE file_IoCreateFileOldAddress[5]={0};
//jmp�µĺ�����ַ
BYTE file_IoCreateFileNewAddress[5]={0xe9,0,0,0,0};

//����ԭ�ȿ�ͷ��5���Լ�
BYTE file_NtDeleteFileOldAddress[5]={0};
//jmp�µĺ�����ַ
BYTE file_NtDeleteFileNewAddress[5]={0xe9,0,0,0,0};

//����ԭ�ȿ�ͷ��5���Լ�
BYTE file_NtSetInformationFileOldAddress[5]={0};
//�º�����ַ
BYTE file_NtSetInformationFileNewAddress[5]={0xe9,0,0,0,0};
//jmp������ַ
BYTE file_NtSetInformationFileJmpAddress[5]={0xe9,0,0,0,0};

PNtDeleteFile NtDeleteFile = NULL;

//�߲�����ʹ��IoAllocateIrp��IoBuildAsynchronousFsdRequest ����/���� IRP��
//�ڵ���IoCallDriverʱ�������� ����/���� ��IRP����ǰ���봫������InvokeOnXXX����ΪTRUE�����ô�������̡�
//���������ͨ�����IRP������ʱ�������ͷ�����������IRP���κ�������ص���Դ�����磺IoBuildPartialMdl�����MDL��
//���������������IoFreeIrp��ֹI/0 manager����������������IRP���᷵��STATUS_MORE_PROCESSING_REQUIRED��
//��Such a driver should return STATUS_MORE_PROCESSING_REQUIRED when it calls IoFreeIrp to forestall the I/0 manager's completion processing for the driver-allocated IRP.��
//���������ǵ��������ִ��ǰ���ܱ�unload�ķǼ��弴��������Ҫʹ��IoSetCompletionRoutineEx��
//IoSetCompletionRoutine�ĵ����߱���������IRQL<=DISPATCH_LEVEL.

//������http://blog.csdn.net/skyair624/archive/2009/10/09/4644154.aspx

//���ļ�
HANDLE file_openFile(
				IN LPTSTR lpFileName,
				IN ACCESS_MASK DesiredAccess,
				IN ULONG ShareAccess
				)
{
	NTSTATUS              status;
	UNICODE_STRING        ustrFileName;
	OBJECT_ATTRIBUTES     objectAttributes;
	HANDLE                hFile;
	IO_STATUS_BLOCK       ioStatus;

    //IoCreateFile����������PASSIVE_LEVEL
	if (KeGetCurrentIrql() > PASSIVE_LEVEL)
	{
		DbgPrint("file_openFile KeGetCurrentIrql() error!");
		return NULL;
	}

	RtlInitUnicodeString(&ustrFileName, lpFileName);

	//��ʼ���������Խṹ��
	InitializeObjectAttributes(&objectAttributes, &ustrFileName,
		OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, NULL);

	//���ļ�
	status = IoCreateFile(&hFile,
		DesiredAccess,
		&objectAttributes,
		&ioStatus,
		0,
		FILE_ATTRIBUTE_NORMAL,
		ShareAccess,
		FILE_OPEN,
		0,
		NULL,
		0,
		0,
		NULL,
		IO_NO_PARAMETER_CHECKING);

	//�����ʧ��
	if (!NT_SUCCESS(status))
	{
		DbgPrint("file_openFile IoCreateFile() error!");
		return NULL;
	}

	//���ؾ��
	return hFile;
}

//������̺���
NTSTATUS file_completionRoutine(
							   IN PDEVICE_OBJECT DeviceObject,
							   IN PIRP pIrp,
							   IN PVOID Context
							   )
{
	pIrp->UserIosb->Status = pIrp->IoStatus.Status;
	pIrp->UserIosb->Information = pIrp->IoStatus.Information;

	//�����¼�
	KeSetEvent(pIrp->UserEvent, IO_NO_INCREMENT, FALSE);

	//�ͷ�IRP
	IoFreeIrp(pIrp);

	//������IoFreeIrp�ͷţ����Ա��뷵��STATUS_MORE_PROCESSING_REQUIRED
	return STATUS_MORE_PROCESSING_REQUIRED;
}

//�����ļ�����
BOOLEAN file_setFileAttributes(
							IN HANDLE hFile,
							IN ULONG FileAttributes
							)
{
	NTSTATUS          status;
	PFILE_OBJECT      pFileObject;
	PDEVICE_OBJECT    pDeviceObject;
	PIRP              pIrp;
	KEVENT            kevent;
	FILE_BASIC_INFORMATION    fileBasicInformation;
	IO_STATUS_BLOCK ioStatus;
	PIO_STACK_LOCATION irpSp;

	status = ObReferenceObjectByHandle(hFile,
		DELETE,
		*IoFileObjectType,
		KernelMode,
		&pFileObject,
		NULL);

	//���ʧ��
	if (!NT_SUCCESS(status))
	{
		return FALSE;
	}

	//��IoGetRelatedDeviceObject������������һ��FileObject��Ӧ����ײ���Ǹ�������������(FDO)��
	pDeviceObject = IoGetRelatedDeviceObject(pFileObject);

	//����IRP
	pIrp = IoAllocateIrp(pDeviceObject->StackSize, TRUE);

	//����ʧ��
	if (pIrp == NULL)
	{
		//���ü�����һ
		ObDereferenceObject(pFileObject);
		return FALSE;
	}

	//��ʼ���¼�����
	KeInitializeEvent(&kevent, SynchronizationEvent, FALSE);

	//�ڴ�����
	RtlZeroMemory(&fileBasicInformation,0,sizeof(FILE_BASIC_INFORMATION));

	//�����ļ�����
	//fileBasicInformation.FileAttributes = FILE_ATTRIBUTE_NORMAL;
    fileBasicInformation.FileAttributes = FileAttributes;

	//���IRP�ṹ
	pIrp->AssociatedIrp.SystemBuffer = &fileBasicInformation;
	pIrp->UserEvent = &kevent;
	pIrp->UserIosb = &ioStatus;
	pIrp->Tail.Overlay.OriginalFileObject = pFileObject;
	pIrp->Tail.Overlay.Thread = (PETHREAD)KeGetCurrentThread();
	pIrp->RequestorMode = KernelMode;

	//Irp��ջ
	irpSp = IoGetNextIrpStackLocation(pIrp);

	//�����ļ������ԣ�ֱ��ͨ��IRP��ʵ�֣�����������ļ�����api
	irpSp->MajorFunction = IRP_MJ_SET_INFORMATION;
	irpSp->DeviceObject = pDeviceObject;
	irpSp->FileObject = pFileObject;
	irpSp->Parameters.SetFile.Length = sizeof(FILE_BASIC_INFORMATION);
	irpSp->Parameters.SetFile.FileInformationClass = FileBasicInformation;
	irpSp->Parameters.SetFile.FileObject = pFileObject;

	//ע��һ��IO������̣����������̽����ڵ��ô˺�������������һ���������IRPָ���Ĳ�������ʱ������
	IoSetCompletionRoutine(
		pIrp,
		file_completionRoutine,
		&kevent,
		TRUE,
		TRUE,
		TRUE);

	//��Ŀ�����ö��󣬷ַ�IRP
	IoCallDriver(pDeviceObject, pIrp);

	//�ȴ��¼������
	KeWaitForSingleObject(&kevent, Executive, KernelMode, TRUE, NULL);

	//���ü�����һ
	ObDereferenceObject(pFileObject);

	//�ɹ�����TRUE
	return TRUE;
}

//ɾ���ļ�
BOOLEAN file_deleteFile(
						IN HANDLE hFile
						)
{
	NTSTATUS          status;
	PFILE_OBJECT      pFileObject;
	PDEVICE_OBJECT    pDeviceObject;
	PIRP              pIrp;
	KEVENT            kevent;
	FILE_DISPOSITION_INFORMATION    fileDispositionInformation;
	IO_STATUS_BLOCK ioStatus;
	PIO_STACK_LOCATION irpSp;
	PSECTION_OBJECT_POINTERS pSectionObjectPointer;     

	//�����ļ�����Ϊһ������
	file_setFileAttributes(hFile,FILE_ATTRIBUTE_NORMAL);          

	//������ת��
	status = ObReferenceObjectByHandle(hFile,
		DELETE,
		*IoFileObjectType,
		KernelMode,
		&pFileObject,
		NULL);

	//���ʧ��
	if (!NT_SUCCESS(status))
	{
		return FALSE;
	}

	//��ö�Ӧ����������
	//��IoGetRelatedDeviceObject������������һ��FileObject��Ӧ����ײ���Ǹ�������������(FDO)��
	//����������Ȼ�ƹ�����Щ����������ȴͬʱҲ�ƹ���������FSD��Ntfs/Fastfat����Ϊ������FSDҲ����Ϊһ�������������ڵġ�
	//�����ļ�����Ķ�Ӧ����ײ��FDO��Ftdisk.sys�����Ѿ���Ϊ���ڵײ�����ܴ�������Ͷ�ݵ�IRP����
	//��ʵ������FSD��Ϣ�洢��һ��Vpb�ṹ�У����ǿ���ʹ��IoGetBaseFileSystemDeviceObject���δ�������ں˺������õ��������������Ƿ���IRP��Ŀ���ˡ�

	pDeviceObject = IoGetRelatedDeviceObject(pFileObject);

	//ʹ��IoAllocateIrp����һ��IRP
	pIrp = IoAllocateIrp(pDeviceObject->StackSize, TRUE);

	if (pIrp == NULL)
	{
		ObDereferenceObject(pFileObject);
		return FALSE;
	}

	//����Ҫ�������IRP���ܼ�ʱ��ɣ����첽�ķ��ص���������ǰ�װһ��CompletionRoutine
	//��CompletionRoutine��������һ���¼�Ϊ�Ѽ��֪ͨ���ǵ����̶߳�ȡ����д������Ѿ����
	// ���ڿ��Է���IRP�ˡ��������ȡ����Ĵ�ʩ�Ļ���IRP����Ŀ����FileObject��Ӧ��DeviceObject
	//���ͺ󣬵ȴ�IRP����ɲ����ͷ���Դ������

	KeInitializeEvent(&kevent, SynchronizationEvent, FALSE);

	//�ڴ�����
	RtlZeroMemory(&fileDispositionInformation,0,sizeof(FILE_DISPOSITION_INFORMATION));

	//������ز���
	fileDispositionInformation.DeleteFile = TRUE;

	//���IRP�ĸ�������
	pIrp->AssociatedIrp.SystemBuffer = &fileDispositionInformation;
	pIrp->UserEvent = &kevent;
	pIrp->UserIosb = &ioStatus;
	pIrp->Tail.Overlay.OriginalFileObject = pFileObject;
	pIrp->Tail.Overlay.Thread = (PETHREAD)KeGetCurrentThread();
	pIrp->RequestorMode = KernelMode;

	irpSp = IoGetNextIrpStackLocation(pIrp);
	//���ù��ܺ�ΪIRP_MJ_SET_INFORMATION
	irpSp->MajorFunction = IRP_MJ_SET_INFORMATION;
	irpSp->DeviceObject = pDeviceObject;
	irpSp->FileObject = pFileObject;
	irpSp->Parameters.SetFile.Length = sizeof(FILE_DISPOSITION_INFORMATION);
	irpSp->Parameters.SetFile.FileInformationClass = FileDispositionInformation;
	irpSp->Parameters.SetFile.FileObject = pFileObject;

    //ע���������
	IoSetCompletionRoutine(
		pIrp,
		file_completionRoutine,
		&kevent,
		TRUE,
		TRUE,
		TRUE);

	//�ټ������������д��� ��MmFlushImageSection    ����ͨ������ṹ������Ƿ����ɾ���ļ���
	pSectionObjectPointer = pFileObject->SectionObjectPointer;
	pSectionObjectPointer->ImageSectionObject = 0;
	pSectionObjectPointer->DataSectionObject = 0;

    //�ַ����²�����
	IoCallDriver(pDeviceObject, pIrp);

	//�ȴ�
	KeWaitForSingleObject(&kevent, Executive, KernelMode, TRUE, NULL);

	//���ü�����һ
	ObDereferenceObject(pFileObject);

	//���سɹ�
	return TRUE;
}

//
VOID file_inlineHookDeleteFileFunc()
{
	//��ֵǰ�涨�������
	KIRQL Irql;

	//���NtDeleteFile�ĵ�ַ
	NtDeleteFile = (PNtDeleteFile)GetSSDTFuncAddress(ZwDeleteFile);

	//���溯��ǰ����ֽ�����
	RtlCopyMemory(file_NtDeleteFileOldAddress,(PBYTE)NtDeleteFile,5);
	RtlCopyMemory(file_NtSetInformationFileOldAddress,(PBYTE)NtSetInformationFile,5);

	//�����º�������ֽ�֮��ƫ��
	*(PULONG)(file_NtDeleteFileNewAddress+1)=(ULONG)NewZwDeleteFile-((ULONG)NtDeleteFile+5);
    *(PULONG)(file_NtSetInformationFileNewAddress+1)=(ULONG)NewNtSetInformationFile-((ULONG)NtSetInformationFile+5);

	//��ʼinline hook
	//�ر��ڴ�д����
	UnProtected();

	//����IRQL�жϼ�
	Irql=KeRaiseIrqlToDpcLevel();
	//������ͷ����ֽ�дJMP 
	RtlCopyMemory((PBYTE)NtDeleteFile,file_NtDeleteFileNewAddress,5);						

	RtlCopyMemory((PBYTE)NtSetInformationFile,file_NtSetInformationFileNewAddress,5);
	*(PULONG)(file_NtSetInformationFileJmpAddress+1) =(ULONG)NtSetInformationFile-((ULONG)JmpNtSetInformationFile+5);
	RtlCopyMemory((PBYTE)JmpNtSetInformationFile, file_NtSetInformationFileOldAddress, 5);
	RtlCopyMemory((PBYTE)JmpNtSetInformationFile + 5, file_NtSetInformationFileJmpAddress, 5);

	//�ָ�Irql
	KeLowerIrql(Irql);

	//�����ڴ�д����
	Protected();
}

//�ָ�
VOID file_unInlineHookDeleteFileFunc()
{

	//������ֽ���д�ص�ԭ����
	KIRQL Irql;

	//�ر�д����
	UnProtected();

	//����IRQL��Dpc
	Irql=KeRaiseIrqlToDpcLevel();
	//��ԭ�ȱ���ĵ�ַ����ǰ5���ֽ�
	RtlCopyMemory((PBYTE)NtDeleteFile,file_NtDeleteFileOldAddress,5);
    RtlCopyMemory((PBYTE)NtSetInformationFile,file_NtSetInformationFileOldAddress,5);

	//����IRQL��Dpc
	KeLowerIrql(Irql);

	//����д����
	Protected();
}


//������InlineHook
VOID file_inlineHookIoCreateFile()
{

	//��ֵǰ�涨�������
	KIRQL Irql;

	//���NtDeleteFile�ĵ�ַ
	NtDeleteFile = (PNtDeleteFile)GetSSDTFuncAddress(ZwDeleteFile);

	//���溯��ǰ����ֽ�����
	RtlCopyMemory(file_IoCreateFileOldAddress,(PBYTE)IoCreateFile,5);

	//�����º�������ֽ�֮��ƫ��
	*(PULONG)(file_IoCreateFileNewAddress+1)=(ULONG)NewIoCreateFile-((ULONG)IoCreateFile+5);

	//��ʼinline hook
	//�ر��ڴ�д����
	UnProtected();

	//����IRQL�жϼ�
	Irql=KeRaiseIrqlToDpcLevel();
	//������ͷ����ֽ�дJMP 
	RtlCopyMemory((PBYTE)IoCreateFile,file_IoCreateFileNewAddress,5);					

	//�ָ�Irql
	KeLowerIrql(Irql);

	//�����ڴ�д����
	Protected();
}


//�ָ�
VOID file_unInlineHookIoCreateFile()
{

	//������ֽ���д�ص�ԭ����
	KIRQL Irql;

	//�ر�д����
	UnProtected();

	//����IRQL��Dpc
	Irql=KeRaiseIrqlToDpcLevel();
	//��ԭ�ȱ���ĵ�ַ����ǰ5���ֽ�
	RtlCopyMemory((PBYTE)IoCreateFile,file_IoCreateFileOldAddress,5);

	//����IRQL��Dpc
	KeLowerIrql(Irql);

	//����д����
	Protected();
}


//�Լ������ԭʼ����
_declspec(naked) NTSTATUS OldIoCreateFile(
	__out PHANDLE FileHandle,
	__in  ACCESS_MASK DesiredAccess,
	__in  POBJECT_ATTRIBUTES ObjectAttributes,
	__out PIO_STATUS_BLOCK IoStatusBlock,
	__in_opt PLARGE_INTEGER AllocationSize,
	__in  ULONG FileAttributes,
	__in  ULONG ShareAccess,
	__in  ULONG Disposition,
	__in  ULONG CreateOptions,
	__in_opt PVOID EaBuffer,
	__in  ULONG EaLength,
	__in  CREATE_FILE_TYPE CreateFileType,
	__in_opt PVOID InternalParameters,
	__in  ULONG Options
	)
{
	_asm
	{   
		mov edi,edi
			push ebp
			mov ebp,esp
			mov eax,IoCreateFile
			add eax,5
			jmp eax                
	}
}

_declspec(naked) NTSTATUS OldZwDeleteFile(
	IN POBJECT_ATTRIBUTES ObjectAttributes
	)
{
	_asm
	{   
		mov edi,edi
			push ebp
			mov ebp,esp
			mov eax,NtDeleteFile
			add eax,5
			jmp eax                
	}
}

_declspec(naked) NTSTATUS JmpNtSetInformationFile (
	__in HANDLE FileHandle,
	__out PIO_STATUS_BLOCK IoStatusBlock,
	__in_bcount(Length) PVOID FileInformation,
	__in ULONG Length,
	__in FILE_INFORMATION_CLASS FileInformationClass
	)
{
	_asm
	{   
		_emit 0x90
			_emit 0x90
			_emit 0x90
			_emit 0x90
			_emit 0x90
			_emit 0x90
			_emit 0x90
			_emit 0x90
			_emit 0x90
			_emit 0x90              
	}
}

NTSTATUS NewIoCreateFile(
						 __out PHANDLE FileHandle,
						 __in  ACCESS_MASK DesiredAccess,
						 __in  POBJECT_ATTRIBUTES ObjectAttributes,
						 __out PIO_STATUS_BLOCK IoStatusBlock,
						 __in_opt PLARGE_INTEGER AllocationSize,
						 __in  ULONG FileAttributes,
						 __in  ULONG ShareAccess,
						 __in  ULONG Disposition,
						 __in  ULONG CreateOptions,
						 __in_opt PVOID EaBuffer,
						 __in  ULONG EaLength,
						 __in  CREATE_FILE_TYPE CreateFileType,
						 __in_opt PVOID InternalParameters,
						 __in  ULONG Options
						 )
{
	__try
	{
		//��ֹ�ļ�����
		if (Disposition == FILE_CREATE)
		{
			//ȫ��
			if (bGlobal_disableCreateFile)
			{
				return STATUS_ACCESS_DENIED;
			}	

			//������ָ��Ŀ����̲���
			if (b_disableCreateFile)
			{
				NTSTATUS status;
				PDISABLE_CREATE_FILE p = NULL;
				PEPROCESS pEpr = NULL;

				//���������б�
				for (p=pDisableCreateFileInfo;p;p=p->next)
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
		}

		//��ֹ�Ĵ��ļ�
		if (Disposition == FILE_OPEN)
		{
			//ȫ��
			if (bGlobal_disableOpenFile)
			{
				//���ؾ����Ч
				return STATUS_ACCESS_DENIED;
			}	

			//������ָ��Ŀ����̲���
			if (b_disableOpenFile)
			{
				NTSTATUS status;
				PDISABLE_OPEN_FILE p = NULL;
				PEPROCESS pEpr = NULL;

				//���������б�
				for (p=pDisableOpenFileInfo;p;p=p->next)
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
		}
		
		//����
		return OldIoCreateFile(
			FileHandle,
			DesiredAccess,
			ObjectAttributes,
			IoStatusBlock,
			AllocationSize,
			FileAttributes,
			ShareAccess,
			Disposition,
			CreateOptions,
			EaBuffer,
			EaBuffer,
			CreateFileType,
			InternalParameters,
			Options
			);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("file.c NewIoCreateFile error!");
		return STATUS_UNSUCCESSFUL;
	}
}


NTSTATUS NewNtSetInformationFile (
								  __in HANDLE FileHandle,
								  __out PIO_STATUS_BLOCK IoStatusBlock,
								  __in_bcount(Length) PVOID FileInformation,
								  __in ULONG Length,
								  __in FILE_INFORMATION_CLASS FileInformationClass
								  )
{
	__try
	{
		if (FileInformationClass == FileDispositionInformation)
		{
			//ȫ��
			if (bGlobal_disableDeleteFile)
			{
				//���ؾ����Ч
				return STATUS_ACCESS_DENIED;	
			}				

			//������ָ��Ŀ����̲���
			if (b_disableDeleteFile)
			{
				NTSTATUS status;
				PDISABLE_DELETE_FILE p = NULL;
				PEPROCESS pEpr = NULL;

				//���������б�
				for (p=pDisableDeleteFileInfo;p;p=p->next)
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
		}		

		return JmpNtSetInformationFile(FileHandle,IoStatusBlock,FileInformation,Length,FileInformationClass);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("file.c NewNtSetInformationFile error!");
		return STATUS_UNSUCCESSFUL;
	}
}


NTSTATUS NewZwDeleteFile(
						 IN POBJECT_ATTRIBUTES ObjectAttributes
						 )
{
	__try
	{
		//ȫ��
		if (bGlobal_disableDeleteFile)
		{
			return STATUS_ACCESS_DENIED;
		}

		//������ָ��Ŀ����̲���
		if (b_disableDeleteFile)
		{
			NTSTATUS status;
			PDISABLE_DELETE_FILE p = NULL;
			PEPROCESS pEpr = NULL;

			//���������б�
			for (p=pDisableDeleteFileInfo;p;p=p->next)
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

		return OldZwDeleteFile(ObjectAttributes);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("file.c NewZwDeleteFile error!");
		return STATUS_UNSUCCESSFUL;
	}
}


//��DisableCreateFileInfo�в���Ŀ�����ID
//����
//0��ʾû���ҵ���1��ʾ�ҵ���
ULONG file_findPIDInDisableCreateFileInfo(ULONG ulPID)
{
	PDISABLE_CREATE_FILE p = NULL;

	__try
	{
		//���������б�
		for (p=pDisableCreateFileInfo;p;p=p->next)
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
		DbgPrint("file_findPIDInDisableCreateFileInfo EXCEPTION_EXECUTE_HANDLER error!");
		return 0;
	}
}

//��DisableCreateFileInfo��ɾ��Ŀ�����ID
VOID file_deletePIDInDisableCreateFileInfo(ULONG ulPID)
{
	PDISABLE_CREATE_FILE p = NULL;
	PDISABLE_CREATE_FILE p2 = NULL;

	__try
	{
		for (p2 = p =pDisableCreateFileInfo;p;p2=p,p=p->next)
		{
			if (p->ulPID == ulPID)
			{
				if (pDisableCreateFileInfo==p)
				{
					pDisableCreateFileInfo = pDisableCreateFileInfo->next;
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
		DbgPrint("file_deletePIDInDisableCreateFileInfo EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//��DisableDeleteFileInfo�в���Ŀ�����ID
//����
//0��ʾû���ҵ���1��ʾ�ҵ���
ULONG file_findPIDInDisableDeleteFileInfo(ULONG ulPID)
{
	PDISABLE_DELETE_FILE p = NULL;

	__try
	{
		//���������б�
		for (p=pDisableDeleteFileInfo;p;p=p->next)
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
		DbgPrint("file_findPIDInDisableDeleteFileInfo EXCEPTION_EXECUTE_HANDLER error!");
		return 0;
	}
}

//��DisableDeleteFileInfo��ɾ��Ŀ�����ID
VOID file_deletePIDInDisableDeleteFileInfo(ULONG ulPID)
{
	PDISABLE_DELETE_FILE p = NULL;
	PDISABLE_DELETE_FILE p2 = NULL;

	__try
	{
		for (p2 = p =pDisableDeleteFileInfo;p;p2=p,p=p->next)
		{
			if (p->ulPID == ulPID)
			{
				if (pDisableDeleteFileInfo==p)
				{
					pDisableDeleteFileInfo = pDisableDeleteFileInfo->next;
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
		DbgPrint("file_deletePIDInDisableDeleteFileInfo EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//��DisableOpenFileInfo�в���Ŀ�����ID
//����
//0��ʾû���ҵ���1��ʾ�ҵ���
ULONG file_findPIDInDisableOpenFileInfo(ULONG ulPID)
{
	PDISABLE_OPEN_FILE p = NULL;

	__try
	{
		//���������б�
		for (p=pDisableOpenFileInfo;p;p=p->next)
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
		DbgPrint("file_findPIDInDisableOpenFileInfo EXCEPTION_EXECUTE_HANDLER error!");
		return 0;
	}
}

//��DisableOpenFileInfo��ɾ��Ŀ�����ID
VOID file_deletePIDInDisableOpenFileInfo(ULONG ulPID)
{
	PDISABLE_OPEN_FILE p = NULL;
	PDISABLE_OPEN_FILE p2 = NULL;

	__try
	{
		for (p2 = p =pDisableOpenFileInfo;p;p2=p,p=p->next)
		{
			if (p->ulPID == ulPID)
			{
				if (pDisableOpenFileInfo==p)
				{
					pDisableOpenFileInfo = pDisableOpenFileInfo->next;
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
		DbgPrint("file_deletePIDInDisableOpenFileInfo EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//����ļ������Ϣ
NTSTATUS file_getFileHandleInfo(LPTSTR lpFilePath){
	NTSTATUS status;
	int i;
	ULONG ulSize = 0x1000;
	PSYSTEM_HANDLE_INFORMATION pSystem_handle_information;
	CLIENT_ID clientID;
	OBJECT_ATTRIBUTES object_attributes;
	HANDLE hProcess;
	HANDLE hDupObj;
	OBJECT_BASIC_INFORMATION object_basic_information;
	PFILE_HANDLE_INFO p;
	PFILE_NAME_INFORMATION pFile_name_information;
	PUNICODE_STRING ustrName;
	IO_STATUS_BLOCK ioStatus;

	__try
	{
		pFileHandleInfo = pFileHandleInfoHead = (PFILE_HANDLE_INFO)ExAllocatePool(PagedPool,sizeof(FILE_HANDLE_INFO));
		pFileHandleInfo->next = NULL;

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
			DbgPrint("file_getFileHandleInfo ZwQuerySystemInformation error!");
			ExFreePool(pSystem_handle_information);
			return status;
		}

		//
		//DbgPrint("pSystem_handle_information->NumberOfHandles:%d",pSystem_handle_information->NumberOfHandles);

		for (i=0;i<pSystem_handle_information->NumberOfHandles;i++)
		{
			if (pSystem_handle_information->Information[i].UniqueProcessId == ulProcessID)
			{
				continue;
			}

			if (pSystem_handle_information->Information[i].UniqueProcessId != ulProcessID)
			{
				RtlZeroMemory(&clientID,sizeof(CLIENT_ID));
				clientID.UniqueProcess = (HANDLE)pSystem_handle_information->Information[i].UniqueProcessId;
				clientID.UniqueThread = NULL;

				InitializeObjectAttributes(&object_attributes,NULL,0,NULL,NULL);

				status = ZwOpenProcess(&hProcess ,PROCESS_DUP_HANDLE ,&object_attributes ,&clientID);
				if (!NT_SUCCESS(status))
				{
					DbgPrint("file_getFileHandleInfo ZwOpenProcess error!");
					ExFreePool(pSystem_handle_information);
					return status;
				}

				status = ZwDuplicateObject(hProcess,(HANDLE)pSystem_handle_information->Information[i].HandleValue,ZwCurrentProcess(),\
					&hDupObj,PROCESS_ALL_ACCESS,0,DUPLICATE_SAME_ACCESS);
				if (!NT_SUCCESS(status))
				{
					DbgPrint("file_getFileHandleInfo ZwDuplicateObject error!");
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

			//�ļ����������ر���
			if(pSystem_handle_information->Information[i].ObjectTypeIndex == OB_TYPE_FILE)
			{
				pFile_name_information = (PFILE_NAME_INFORMATION)ExAllocatePool(PagedPool,sizeof(WCHAR)*256);
				RtlZeroMemory(pFile_name_information,sizeof(WCHAR)*256);

				status = ZwQueryInformationFile(hDupObj,&ioStatus,pFile_name_information,sizeof(WCHAR)*256,FileNameInformation);

				if (NT_SUCCESS(status))
				{
					DbgPrint("file_getFileHandleInfo fileName:%ws",pFile_name_information->FileName);

					if (_wcsnicmp(pFile_name_information->FileName,lpFilePath,wcslen(lpFilePath)) >= 0)
					{
						p = (PFILE_HANDLE_INFO)ExAllocatePool(PagedPool,sizeof(FILE_HANDLE_INFO));
						RtlZeroMemory(p,sizeof(FILE_HANDLE_INFO));

						DbgPrint("name:%ws",pFile_name_information->FileName);

						p->ulPID = pSystem_handle_information->Information[i].UniqueProcessId;
						p->ulHandle = pSystem_handle_information->Information[i].HandleValue;
						wcscpy(p->wstrHandleName,pFile_name_information->FileName);           
						p->next = NULL;

						pFileHandleInfo->next = p;
						pFileHandleInfo = p;
					}		
				}		

				ExFreePool(pFile_name_information);
			}
		}

		pFileHandleInfo = pFileHandleInfoHead;

		ExFreePool(pSystem_handle_information);
		return STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("file_getFileHandleInfo EXCEPTION_EXECUTE_HANDLER error!");
		ExFreePool(pSystem_handle_information);
		return STATUS_UNSUCCESSFUL;
	}
}

//�ͷ��ڴ�
VOID file_freeFileHandleInfo()
{
	PFILE_HANDLE_INFO p = NULL;

	__try
	{
		while(pFileHandleInfo != NULL)
		{
			p = pFileHandleInfo->next;
			ExFreePool(pFileHandleInfo);
			pFileHandleInfo   = p;
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("file_freeFileHandleInfo EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//��ô�С
ULONG file_getFileHandleInfoCount()
{
	__try
	{
		PFILE_HANDLE_INFO p=NULL;
		ULONG ulCount=0;

		DbgPrint("file_getFileHandleInfoCount in...");

		//��ô�С
		for (p=pFileHandleInfo->next;p;p=p->next)
		{
			DbgPrint("ulCount... %d",ulCount);
			ulCount++;
		}

		DbgPrint("file_getFileHandleInfoCount ulCount:%d",ulCount);
		return ulCount;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("file_getFileHandleInfoCount EXCEPTION_EXECUTE_HANDLER error!");
		return 0;
	}	
}

//�������ݵ�ring3
NTSTATUS file_getFileHandleInfo_send(PFILE_HANDLE_INFO pBuffer)
{
	PFILE_HANDLE_INFO p=NULL;
	ULONG ulIndex = 0;

	__try
	{
		//����
		for (p=pFileHandleInfo->next;p;p=p->next)
		{
			RtlCopyMemory(pBuffer + ulIndex,p,sizeof(FILE_HANDLE_INFO));

			ulIndex++;
		}

		//�ͷ��ڴ�
		file_freeFileHandleInfo();

		return STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("file_getFileHandleInfo_send EXCEPTION_EXECUTE_HANDLER error!");
		return STATUS_UNSUCCESSFUL;
	}
}