//////////////////////////////////////////////////////////////////////////
//
//

#include "common.h"
#include "monitor.h"
#include "protection.h"
#include "process.h"
#include "file.h"
#include "shadow.h"
#include "monitor2.h"
#include "reg.h"
#include "sysmodule.h"
#include "kernel.h"


//����IRP_MJ_CREATE
NTSTATUS DispatchCreateClose(PDEVICE_OBJECT pDevObj,PIRP pIrp)
{
	DbgPrint("DispatchCreateClose��");
	pIrp->IoStatus.Status=STATUS_SUCCESS;
	IoCompleteRequest(pIrp,IO_NO_INCREMENT);
	return STATUS_SUCCESS; 
}


//����ж��
VOID DriverUnload(IN PDRIVER_OBJECT   pDriverObj)
{
	PDEVICE_OBJECT    pDevObj = pDriverObj->DeviceObject;
	UNICODE_STRING    ustrLinkName;

	DbgPrint("DriverUnload��");

	RtlInitUnicodeString(&ustrLinkName, LINK_NAME);
    //ɾ����������
	IoDeleteSymbolicLink(&ustrLinkName);
    //ɾ���豸
	IoDeleteDevice(pDevObj);
}

// I/O������ǲ����
NTSTATUS DispatchIoctl(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;
	PVOID InputBuffer;
	PVOID OutputBuffer;
    ULONG uInSize;
    ULONG uOutSize;

	InputBuffer = OutputBuffer = pIrp->AssociatedIrp.SystemBuffer;

	//�õ����뻺������С
	uInSize = IoGetCurrentIrpStackLocation(pIrp)->Parameters.DeviceIoControl.InputBufferLength;
   
	//�õ������������С
	uOutSize = IoGetCurrentIrpStackLocation(pIrp)->Parameters.DeviceIoControl.OutputBufferLength;

    //���ܺ���
	switch(IoGetCurrentIrpStackLocation(pIrp)->MajorFunction)
	{
	case IRP_MJ_CREATE:
		break;
	case IRP_MJ_CLOSE :
		break;
	case IRP_MJ_DEVICE_CONTROL:
		switch(IoGetCurrentIrpStackLocation(pIrp)->Parameters.DeviceIoControl.IoControlCode)
		{
			//��ʼ��
		case IOCTL_APPLOAD:
			{

			}
			break;
			//
		case IOCTL_APPUNLOAD:
			{

			}
			break;
			//���ϵͳ�汾��
		case IOCTL_GETOSVERSION:	
			{
				__try
				{
					ulOSVersion = *(PULONG)InputBuffer;

					DbgPrint("ulOSVersion:%d",ulOSVersion);
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_GETOSVERSION error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//�õ�ring3��Ӧ�ó���ID
		case IOCTL_GETPROCESSID:	
			{
				__try
				{
					ulProcessID = *(PULONG)InputBuffer;

					PsLookupProcessByProcessId((HANDLE)ulProcessID,&pEProcess);

					DbgPrint("ulProcessID:%d",ulProcessID);
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_GETPROCESSID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//����ssdt���ұ���
		case IOCTL_SELFPROTECTION_START_SSDT:	
			{
				__try
				{
					//�������ұ���
					protection_startInlineHook();
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_SELFPROTECTION_START_SSDT error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//ֹͣssdt���ұ���
		case IOCTL_SELFPROTECTION_STOP_SSDT:	
			{
				__try
				{
					//ֹͣ���ұ���
					protection_stopInlineHook();
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_SELFPROTECTION_STOP_SSDT error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//����shadow���ұ���
		case IOCTL_SELFPROTECTION_START_SHADOW:	
			{
				__try
				{
					//�������ұ���
					protection_startShadowSSDTHook();
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_SELFPROTECTION_START_SHADOW error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//ֹͣshadow���ұ���
		case IOCTL_SELFPROTECTION_STOP_SHADOW:	
			{
				__try
				{
					//ֹͣ���ұ���
					protection_stopShadowSSDTHook();
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_SELFPROTECTION_STOP_SHADOW error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//�����������ұ���
		case IOCTL_SELFPROTECTION_START_ALL:	
			{
				__try
				{
					//�������ұ���
                    protection_startInlineHook();
					protection_startShadowSSDTHook();

					//��������
					b_startSelfProtectionByObReferenceObjectByHandle = TRUE;

					//�ж��Ƿ�Ҫ����
					if (!bGlobal_disableCreateProcess && !b_disableCreateProcess && !bGlobal_disableTerminateProcess && !b_disableTerminateProcess && !bGlobal_disableCreateThread && !bGlobal_disableTerminateThread)
					{
						monitor2_inlineHookObReferenceObjectByHandle();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_SELFPROTECTION_START_ALL error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//ֹͣ�������ұ���
		case IOCTL_SELFPROTECTION_STOP_ALL:	
			{
				__try
				{
					//ֹͣ���ұ���
					protection_stopInlineHook();
					protection_stopShadowSSDTHook();

					//���ñ���
					b_startSelfProtectionByObReferenceObjectByHandle = FALSE;
					//�ж��Ƿ�Ҫ����
					if (!bGlobal_disableCreateProcess && !b_disableCreateProcess && !bGlobal_disableTerminateProcess && !b_disableTerminateProcess && !bGlobal_disableCreateThread && !bGlobal_disableTerminateThread)
					{
						monitor2_unInlineHookObReferenceObjectByHandle();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_SELFPROTECTION_STOP_ALL error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//�õ�SSDT
		case IOCTL_SSDT_GET:	
			{
				__try
				{
					*((PULONG)OutputBuffer) = KeServiceDescriptorTable->NumberOfServices;
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_SSDT_GET error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//��ѯSSDTָ����ַ
		case IOCTL_SSDT_GETFUNCADDR:	
			{
				__try
				{
					//����������û����λ��
					*((PULONG)OutputBuffer) = *((PULONG)(KeServiceDescriptorTable->ServiceTableBase) + *(PULONG)InputBuffer);
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_SSDT_GETFUNCADDR error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//����SSDTָ����ַ
		case IOCTL_SSDT_SETFUNCADDR:
			{
				__try
				{
					PSSDT_CLIENT_ID pSSDTClientID = NULL;
					//ȥ���ڴ汣��
					UnProtected();

                    pSSDTClientID = (PSSDT_CLIENT_ID)InputBuffer;
					DbgPrint("ulIndex:%d,ulAddress:%08X",pSSDTClientID->ulIndex,pSSDTClientID->ulAddress);
					//�������
					
					//���ǵ�ַ
					*((PULONG)(KeServiceDescriptorTable->ServiceTableBase) + pSSDTClientID->ulIndex) = pSSDTClientID->ulAddress;

					//�����ڴ汣��
					Protected();
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_SSDT_SETFUNCADDR error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;	
			//��ѯShadow SSDTָ����ַ
		case IOCTL_SHADOW_GETFUNCADDR:	
			{
				__try
				{
					//����������û����λ��
					*((PULONG)OutputBuffer) = KeServiceDescriptorTableShadow[1].ServiceTableBase[*(PULONG)InputBuffer];
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_SSDT_GETFUNCADDR error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//��ѯShadow SSDTԭʼ��ַ
		case IOCTL_SHADOW_GETFUNCADDR_ORI:	
			{
				__try
				{
					DbgPrint("Shadow SSDT Func ulIndex:%d",*(PULONG)InputBuffer);
					//����������û����λ��
					*((PULONG)OutputBuffer) = GetOldShadowSSDTAddress(*(PULONG)InputBuffer);
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_SHADOW_GETFUNCADDR_ORI error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//����Shadow SSDTָ����ַ
		case IOCTL_SHADOW_SETFUNCADDR:
			{
				__try
				{
					PSHADOW_SSDT_CLIENT_ID pShadowSSDTClientID = NULL;
					//ȥ���ڴ汣��
					UnProtected();

					pShadowSSDTClientID = (PSHADOW_SSDT_CLIENT_ID)InputBuffer;
					DbgPrint("ulIndex:%d,ulAddress:%08X",pShadowSSDTClientID->ulIndex,pShadowSSDTClientID->ulAddress);
					//�������

					//���ǵ�ַ
					KeServiceDescriptorTableShadow[1].ServiceTableBase[pShadowSSDTClientID->ulIndex] = pShadowSSDTClientID->ulAddress;

					//�����ڴ汣��
					Protected();				
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_SHADOW_SETFUNCADDR error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//�������
		case IOCTL_PROCESS_SUSPEND:
			{
				__try
				{
					if(!process_suspendProcess(*(PULONG)InputBuffer))
					{
						break;
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_PROCESS_SUSPEND error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//�ָ�����
		case IOCTL_PROCESS_RESUME:
			{
				__try
				{
					if(!process_resumeProcess(*(PULONG)InputBuffer))
					{
						break;
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_PROCESS_RESUME error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//���̽���,��������
		case IOCTL_PROCESS_KILL:
			{
				__try
				{
					process_killProcess(*(PULONG)InputBuffer);
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_PROCESS_KILL error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//���̽���2
		case IOCTL_PROCESS_KILL2:
			{
				__try
				{
					//��ô�ring3�������Ľ���ID
					BOOLEAN bRet = FALSE;
					bRet = process_killProcessByPspTerminateThreadByPointer(*(PULONG)InputBuffer);

					if (!bRet)
					{
						break;
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_PROCESS_KILL2 error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//��������
		case IOCTL_PROCESS_HIDE:
			{
				__try
				{
					DbgPrint("IOCTL_PROCESS_HIDE ulPID:%d",*(PULONG)InputBuffer);
					process_hideProcess(*(PULONG)InputBuffer);
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_PROCESS_HIDE error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//�����߳�
		case IOCTL_PROCESS_KILLTHREAD:
			{
				__try
				{
					CLIENT_ID clientID;
					BOOLEAN bRet = FALSE;
					//��ô�ring3�������Ľ���clientID
					
					//��������
					RtlCopyMemory(&clientID,InputBuffer,sizeof(CLIENT_ID));

					bRet = process_killThreadByPspTerminateThreadByPointer(clientID.UniqueProcess,clientID.UniqueThread);

					if (!bRet)
					{
						break;
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_PROCESS_KILLTHREAD error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//ж�ؽ���ģ��
		case IOCTL_PROCESS_MODULE_UNLOAD:
			{
				__try
				{
					UNLOADDLL_CLIENT_ID clientID;
					BOOLEAN bRet = FALSE;
					//��ô�ring3�������Ľ���clientID

					//��������
					RtlCopyMemory(&clientID,InputBuffer,sizeof(UNLOADDLL_CLIENT_ID));

					//ж��
					bRet = process_unloadModule(clientID.ulPID,clientID.ulBaseAddress);

					if (!bRet)
					{
						break;
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_PROCESS_MODULE_UNLOAD error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//����̻߳�ַ
		case IOCTL_PROCESS_THREAD_GETBASEADDR:
			{
				__try
				{
					CLIENT_ID clientID;
					ULONG ulBaseAddress;

					//��ô�ring3�������Ľ���clientID

					//��������
					RtlCopyMemory(&clientID,InputBuffer,sizeof(CLIENT_ID));

					ulBaseAddress = process_getThreadBaseAddr(clientID.UniqueProcess,clientID.UniqueThread);

					//��������
					RtlCopyMemory(OutputBuffer,&ulBaseAddress,sizeof(ULONG));
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_PROCESS_THREAD_GETBASEADDR error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//�����߳�
		case IOCTL_PROCESS_THREAD_SUSPEND:
			{
				__try
				{
					ULONG ulTID;
					ulTID = *(PULONG)InputBuffer;

					DbgPrint("IOCTL_PROCESS_THREAD_RESUME ulTID:%d",ulTID);

					//�������ʧ�ܣ�ֱ�ӷ���
					if (!process_suspendThread(ulTID))
					{
						break;
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_PROCESS_THREAD_SUSPEND error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;			
			//�ָ��߳�����
		case IOCTL_PROCESS_THREAD_RESUME:
			{
				__try
				{
					ULONG ulTID;
					ulTID = *(PULONG)InputBuffer;

					DbgPrint("IOCTL_PROCESS_THREAD_RESUME ulTID:%d",ulTID);

					//�������ʧ�ܣ�ֱ�ӷ���
					if (!process_resumeThread(ulTID))
					{
						break;
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_PROCESS_THREAD_RESUME error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//ǿ��ɾ���ļ�
		case IOCTL_FILE_DELETE:
			{
				__try
				{
                    //����ļ�·��
					HANDLE  hFile;
					LPTSTR	lpFilePath = (LPTSTR)InputBuffer;

					DbgPrint("IOCTL_FILE_DELETE lpFilePath :%s",lpFilePath);

					//���ļ�
					hFile = file_openFile(lpFilePath, FILE_READ_ATTRIBUTES, FILE_SHARE_DELETE);

			    	//������ļ��ɹ�
					if (hFile!=NULL)
					{
						if(!file_deleteFile(hFile))
						{
							//���ɾ���ļ�ʧ��
							ZwClose(hFile);
							break;
						}
						ZwClose(hFile);
					}				
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_FILE_DELETE error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//��ֹ���̵Ĵ���
		case IOCTL_MONITOR2_PROCESS_DISABLECREATE:
			{
				__try
				{
					//ȫ��
					bGlobal_disableCreateProcess = TRUE;

					//�ж��Ƿ�Ҫ����
					if (!b_startSelfProtectionByObReferenceObjectByHandle && !b_disableCreateProcess && !bGlobal_disableTerminateProcess && !b_disableTerminateProcess && !bGlobal_disableCreateThread && !bGlobal_disableTerminateThread)
					{
						monitor2_inlineHookObReferenceObjectByHandle();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_PROCESS_DISABLECREATE error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//��ֹ���̵Ĵ���2
		case IOCTL_MONITOR2_PROCESS_DISABLECREATE2:
			{
				__try
				{
					PDISABLE_CREATE_PROCESS p;

					//
					b_disableCreateProcess = TRUE;

					p = (PDISABLE_CREATE_PROCESS)ExAllocatePool(NonPagedPool,sizeof(DISABLE_CREATE_PROCESS));
					p->ulPID = *(PULONG)InputBuffer;
					p->next = NULL;

					//��������һ���ڵ����Ϣ����ָ���ƶ������һ���ڵ�λ��
					if (pDisableCreateProcessTmp)
					{
						pDisableCreateProcessInfo = pDisableCreateProcessTmp;
					}

					pDisableCreateProcessInfo->next = p;

					//��¼�����ڵ��λ��
					pDisableCreateProcessTmp = pDisableCreateProcessInfo = p;

					//�ײ�
					pDisableCreateProcessInfo = pDisableCreateProcessHead->next;

					//�ж��Ƿ�Ҫ����
					if (!b_startSelfProtectionByObReferenceObjectByHandle && !bGlobal_disableCreateProcess && !bGlobal_disableTerminateProcess && !b_disableTerminateProcess && !bGlobal_disableCreateThread && !bGlobal_disableTerminateThread)
					{
						monitor2_inlineHookObReferenceObjectByHandle();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_PROCESS_DISABLECREATE2 error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//�ӵ��������������PID
		case IOCTL_MONITOR2_PROCESS_DISABLECREATE2_FINDPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					ulRet = monitor2_findPIDInDisableCreateProcessInfo(ulPID);

					//�ѷ���ֵ����ring3
					*(PULONG)OutputBuffer = ulRet;
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_PROCESS_DISABLECREATE2_FINDPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//�ӵ�����������ɾ��PID
		case IOCTL_MONITOR2_PROCESS_DISABLECREATE2_DELETEPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					monitor2_deletePIDInDisableCreateProcessInfo(ulPID);
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_PROCESS_DISABLECREATE2_DELETEPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//ֹͣ��ֹ���̵Ĵ���
		case IOCTL_MONITOR2_PROCESS_DISABLECREATE_STOP:
			{
				__try
				{
					//ȫ�� ������
					bGlobal_disableCreateProcess = FALSE;

					//�ж�
					if (!b_startSelfProtectionByObReferenceObjectByHandle && !b_disableCreateProcess && !bGlobal_disableTerminateProcess && !b_disableTerminateProcess && !bGlobal_disableCreateThread && !bGlobal_disableTerminateThread)
					{
						monitor2_unInlineHookObReferenceObjectByHandle();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_PROCESS_DISABLECREATE_STOP error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//��ֹ���̵Ľ���
		case IOCTL_MONITOR2_PROCESS_DISABLETERMINATE:
			{
				__try
				{
					//ȫ��
					bGlobal_disableTerminateProcess = TRUE;

					//�ж�
					if (!b_startSelfProtectionByObReferenceObjectByHandle && !bGlobal_disableCreateProcess && !b_disableCreateProcess && !b_disableTerminateProcess && !bGlobal_disableCreateThread && !bGlobal_disableTerminateThread)
					{
						monitor2_inlineHookObReferenceObjectByHandle();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_PROCESS_DISABLETERMINATE error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//��ֹ���̵Ľ���2
		case IOCTL_MONITOR2_PROCESS_DISABLETERMINATE2:
			{
				__try
				{
					PDISABLE_TERMINATE_PROCESS p;

					//
					b_disableTerminateProcess = TRUE;

					p = (PDISABLE_TERMINATE_PROCESS)ExAllocatePool(NonPagedPool,sizeof(DISABLE_TERMINATE_PROCESS));
					p->ulPID = *(PULONG)InputBuffer;
					p->next = NULL;

					//��������һ���ڵ����Ϣ����ָ���ƶ������һ���ڵ�λ��
					if (pDisableTerminateProcessTmp)
					{
						pDisableTerminateProcessInfo = pDisableTerminateProcessTmp;
					}

					pDisableTerminateProcessInfo->next = p;
					
					//��¼�����ڵ��λ��
					pDisableTerminateProcessTmp = pDisableTerminateProcessInfo = p;

					//�ײ�
					pDisableTerminateProcessInfo = pDisableTerminateProcessHead->next;

					//�ж�
					if (!b_startSelfProtectionByObReferenceObjectByHandle && !bGlobal_disableCreateProcess && !b_disableCreateProcess && !bGlobal_disableTerminateProcess && !bGlobal_disableCreateThread && !bGlobal_disableTerminateThread)
					{
						monitor2_inlineHookObReferenceObjectByHandle();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_PROCESS_DISABLETERMINATE2 error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//�ӵ��������������PID
		case IOCTL_MONITOR2_PROCESS_DISABLETERMINATE2_FINDPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					ulRet = monitor2_findPIDInDisableTerminateProcessInfo(ulPID);

					//�ѷ���ֵ����ring3
					*(PULONG)OutputBuffer = ulRet;
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_PROCESS_DISABLETERMINATE2_FINDPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//�ӵ�����������ɾ��PID
		case IOCTL_MONITOR2_PROCESS_DISABLETERMINATE2_DELETEPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					monitor2_deletePIDInDisableTerminateProcessInfo(ulPID);
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_PROCESS_DISABLETERMINATE2_DELETEPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//ֹͣ��ֹ���̵Ľ���
		case IOCTL_MONITOR2_PROCESS_DISABLETERMINATE_STOP:
			{
				__try
				{
					//ȫ�� ������
					bGlobal_disableTerminateProcess = FALSE;

					//�ж�
					if (!b_startSelfProtectionByObReferenceObjectByHandle && !bGlobal_disableCreateProcess && !b_disableCreateProcess && !b_disableTerminateProcess && !bGlobal_disableCreateThread && !bGlobal_disableTerminateThread)
					{
						monitor2_unInlineHookObReferenceObjectByHandle();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_PROCESS_DISABLETERMINATE_STOP error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//��ֹ�̵߳Ĵ���
		case IOCTL_MONITOR2_THREAD_DISABLECREATE:
			{
				__try
				{
					//ȫ��
					bGlobal_disableCreateThread = TRUE;

					//�ж�
					if (!b_startSelfProtectionByObReferenceObjectByHandle && !bGlobal_disableCreateProcess && !b_disableCreateProcess && !bGlobal_disableTerminateProcess && !b_disableTerminateProcess && !bGlobal_disableTerminateThread)
					{
						monitor2_inlineHookObReferenceObjectByHandle();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_THREAD_DISABLECREATE error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//ֹͣ��ֹ�̵߳Ĵ���
		case IOCTL_MONITOR2_THREAD_DISABLECREATE_STOP:
			{
				__try
				{
					//ȫ��
					bGlobal_disableCreateThread = FALSE;

					//�ж�
					if (!b_startSelfProtectionByObReferenceObjectByHandle && !bGlobal_disableCreateProcess && !b_disableCreateProcess && !bGlobal_disableTerminateProcess && !b_disableTerminateProcess && !bGlobal_disableTerminateThread)
					{
						monitor2_unInlineHookObReferenceObjectByHandle();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_THREAD_DISABLECREATE_STOP error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//��ֹ�̵߳Ľ���
		case IOCTL_MONITOR2_THREAD_DISABLETERMINATE:
			{
				__try
				{
					//ȫ��
					bGlobal_disableTerminateThread = TRUE;

					//�ж�
					if (!b_startSelfProtectionByObReferenceObjectByHandle && !bGlobal_disableCreateProcess && !b_disableCreateProcess && !bGlobal_disableTerminateProcess && !b_disableTerminateProcess && !bGlobal_disableCreateThread)
					{
						monitor2_inlineHookObReferenceObjectByHandle();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_THREAD_DISABLETERMINATE error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//ֹͣ��ֹ�̵߳Ľ���
		case IOCTL_MONITOR2_THREAD_DISABLETERMINATE_STOP:
			{
				__try
				{
					//ȫ��
					bGlobal_disableTerminateThread = FALSE;

					//�ж�
					if (!b_startSelfProtectionByObReferenceObjectByHandle && !bGlobal_disableCreateProcess && !b_disableCreateProcess && !bGlobal_disableTerminateProcess && !b_disableTerminateProcess && !bGlobal_disableCreateThread)
					{
						monitor2_unInlineHookObReferenceObjectByHandle();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_THREAD_DISABLETERMINATE_STOP error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//��ֹ�ļ��Ĵ���
		case IOCTL_MONITOR2_FILE_DISABLECREATE:
			{
				__try
				{
					//ȫ��
					bGlobal_disableCreateFile = TRUE;

					if (!b_disableCreateFile && !bGlobal_disableOpenFile && !b_disableOpenFile)
					{
						file_inlineHookIoCreateFile();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLECREATE error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//��ֹ�ļ��Ĵ���2
		case IOCTL_MONITOR2_FILE_DISABLECREATE2:
			{
				__try
				{
					PDISABLE_CREATE_FILE p;

					//
					b_disableCreateFile = TRUE;

					p = (PDISABLE_CREATE_FILE)ExAllocatePool(NonPagedPool,sizeof(DISABLE_CREATE_FILE));
					p->ulPID = *(PULONG)InputBuffer;
					p->next = NULL;

					//��������һ���ڵ����Ϣ����ָ���ƶ������һ���ڵ�λ��
					if (pDisableCreateFileTmp)
					{
						pDisableCreateFileInfo = pDisableCreateFileTmp;
					}

					pDisableCreateFileInfo->next = p;

					//��¼�����ڵ��λ��
					pDisableCreateFileTmp = pDisableCreateFileInfo = p;

					//�ײ�
					pDisableCreateFileInfo = pDisableCreateFileHead->next;

					if (!bGlobal_disableCreateFile && !bGlobal_disableOpenFile && !b_disableOpenFile)
					{
						file_inlineHookIoCreateFile();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLECREATE2 error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//�ӵ��������������PID
		case IOCTL_MONITOR2_FILE_DISABLECREATE2_FINDPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					ulRet = file_findPIDInDisableCreateFileInfo(ulPID);

					//�ѷ���ֵ����ring3
					*(PULONG)OutputBuffer = ulRet;
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLECREATE2_FINDPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//�ӵ�����������ɾ��PID
		case IOCTL_MONITOR2_FILE_DISABLECREATE2_DELETEPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					file_deletePIDInDisableCreateFileInfo(ulPID);
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLECREATE2_DELETEPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//ֹͣ��ֹ�ļ��Ĵ���
		case IOCTL_MONITOR2_FILE_DISABLECREATE_STOP:
			{
				__try
				{
					//ȫ��
					bGlobal_disableCreateFile = FALSE;

					if (!b_disableCreateFile && !bGlobal_disableOpenFile && !b_disableOpenFile)
					{
						file_unInlineHookIoCreateFile();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLECREATE_STOP error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//��ֹ�ļ�ɾ��
		case IOCTL_MONITOR2_FILE_DISABLEDELETE:
			{
				__try
				{
					//ȫ��
					bGlobal_disableDeleteFile = TRUE;		

					if (!b_disableDeleteFile)
					{
						file_inlineHookDeleteFileFunc();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLEDELETE error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//��ֹ�ļ�ɾ��2
		case IOCTL_MONITOR2_FILE_DISABLEDELETE2:
			{
				__try
				{
					PDISABLE_DELETE_FILE p;

					//��һ
					b_disableDeleteFile = TRUE;

					p = (PDISABLE_DELETE_FILE)ExAllocatePool(NonPagedPool,sizeof(DISABLE_DELETE_FILE));
					p->ulPID = *(PULONG)InputBuffer;
					p->next = NULL;

					//��������һ���ڵ����Ϣ����ָ���ƶ������һ���ڵ�λ��
					if (pDisableDeleteFileTmp)
					{
						pDisableDeleteFileInfo = pDisableDeleteFileTmp;
					}

					pDisableDeleteFileInfo->next = p;

					//��¼�����ڵ��λ��
					pDisableDeleteFileTmp = pDisableDeleteFileInfo = p;

					//�ײ�
					pDisableDeleteFileInfo = pDisableDeleteFileHead->next;

					if (!bGlobal_disableDeleteFile)
					{
						file_inlineHookDeleteFileFunc();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLEDELETE2 error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//�ӵ��������������PID
		case IOCTL_MONITOR2_FILE_DISABLEDELETE2_FINDPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					ulRet = file_findPIDInDisableDeleteFileInfo(ulPID);

					//�ѷ���ֵ����ring3
					*(PULONG)OutputBuffer = ulRet;
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLEDELETE2_FINDPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//�ӵ�����������ɾ��PID
		case IOCTL_MONITOR2_FILE_DISABLEDELETE2_DELETEPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					file_deletePIDInDisableDeleteFileInfo(ulPID);
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLEDELETE2_DELETEPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//ֹͣ��ֹ�ļ�ɾ��
		case IOCTL_MONITOR2_FILE_DISABLEDELETE_STOP:
			{
				__try
				{
					bGlobal_disableDeleteFile = FALSE;

					if (!b_disableDeleteFile)
					{
						file_unInlineHookDeleteFileFunc();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLEDELETE_STOP error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//��ֹ�ļ���
		case IOCTL_MONITOR2_FILE_DISABLEOPEN:
			{
				__try
				{
					//ȫ��
					bGlobal_disableOpenFile = TRUE;

					if (!bGlobal_disableCreateFile && !b_disableCreateFile && !b_disableOpenFile)
					{
						file_inlineHookIoCreateFile();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLEOPEN error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//��ֹ�ļ���2
		case IOCTL_MONITOR2_FILE_DISABLEOPEN2:
			{
				__try
				{
					PDISABLE_OPEN_FILE p;

					//ȫ��
					b_disableOpenFile = TRUE;

					p = (PDISABLE_OPEN_FILE)ExAllocatePool(NonPagedPool,sizeof(DISABLE_OPEN_FILE));
					p->ulPID = *(PULONG)InputBuffer;
					p->next = NULL;

					//��������һ���ڵ����Ϣ����ָ���ƶ������һ���ڵ�λ��
					if (pDisableOpenFileTmp)
					{
						pDisableOpenFileInfo = pDisableOpenFileTmp;
					}

					pDisableOpenFileInfo->next = p;

					//��¼�����ڵ��λ��
					pDisableOpenFileTmp = pDisableOpenFileInfo = p;

					//�ײ�
					pDisableOpenFileInfo = pDisableOpenFileHead->next;

					if (!bGlobal_disableCreateFile && !b_disableCreateFile && !bGlobal_disableOpenFile)
					{
						file_inlineHookIoCreateFile();
					}

				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLEDELETE2 error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//�ӵ��������������PID
		case IOCTL_MONITOR2_FILE_DISABLEOPEN2_FINDPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					ulRet = file_findPIDInDisableOpenFileInfo(ulPID);

					//�ѷ���ֵ����ring3
					*(PULONG)OutputBuffer = ulRet;
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLEOPEN2_FINDPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//�ӵ�����������ɾ��PID
		case IOCTL_MONITOR2_FILE_DISABLEOPEN2_DELETEPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					file_deletePIDInDisableOpenFileInfo(ulPID);
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLEOPEN2_DELETEPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//ֹͣ��ֹ�ļ��Ĵ�
		case IOCTL_MONITOR2_FILE_DISABLEOPEN_STOP:
			{
				__try
				{
					bGlobal_disableOpenFile = FALSE;

					if (!bGlobal_disableCreateFile && !b_disableCreateFile && !b_disableOpenFile)
					{
						file_unInlineHookIoCreateFile();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLEOPEN_STOP error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//��ֹע���Ĵ���
		case IOCTL_MONITOR2_REG_DISABLECREATE:
			{
				__try
				{
					//ȫ�ֽ�ֹ
					bGlobal_disableCreateReg = TRUE;

					if (!b_disableCreateReg)
					{
						reg_inlineHook_disableCreate();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLECREATE error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//��ֹע���Ĵ���2
		case IOCTL_MONITOR2_REG_DISABLECREATE2:
			{
				__try
				{
					PDISABLE_CREATE_REG p;

					//ȫ��
					b_disableCreateReg = TRUE;

					p = (PDISABLE_CREATE_REG)ExAllocatePool(NonPagedPool,sizeof(DISABLE_CREATE_REG));
					p->ulPID = *(PULONG)InputBuffer;
					p->next = NULL;

					//��������һ���ڵ����Ϣ����ָ���ƶ������һ���ڵ�λ��
					if (pDisableCreateRegTmp)
					{
						pDisableCreateRegInfo = pDisableCreateRegTmp;
					}

					pDisableCreateRegInfo->next = p;

					//��¼�����ڵ��λ��
					pDisableCreateRegTmp = pDisableCreateRegInfo = p;

					//�ײ�
					pDisableCreateRegInfo = pDisableCreateRegHead->next;	

					if (!bGlobal_disableCreateReg)
					{
						reg_inlineHook_disableCreate();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLECREATE2 error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//�ӵ��������������PID
		case IOCTL_MONITOR2_REG_DISABLECREATE2_FINDPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					ulRet = reg_findPIDInDisableCreateRegInfo(ulPID);

					//�ѷ���ֵ����ring3
					*(PULONG)OutputBuffer = ulRet;
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLECREATE2_FINDPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//�ӵ�����������ɾ��PID
		case IOCTL_MONITOR2_REG_DISABLECREATE2_DELETEPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					reg_deletePIDInDisableCreateRegInfo(ulPID);
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLECREATE2_DELETEPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//ֹͣ��ֹע���Ĵ���
		case IOCTL_MONITOR2_REG_DISABLECREATE_STOP:
			{
				__try
				{
					bGlobal_disableCreateReg = FALSE;

					if (!b_disableCreateReg)
					{
						reg_inlineHook_disableCreate_stop();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLECREATE_STOP error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//��ֹע����ɾ��
		case IOCTL_MONITOR2_REG_DISABLEDELETE:
			{
				__try
				{
					bGlobal_disableDeleteReg = TRUE;

					if (!b_disableDeleteReg)
					{
						reg_inlineHook_disableDelete();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLEDELETE error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//��ֹע����ɾ��2
		case IOCTL_MONITOR2_REG_DISABLEDELETE2:
			{
				__try
				{
					PDISABLE_DELETE_REG p;

					//ȫ��
					b_disableDeleteReg = TRUE;

					p = (PDISABLE_DELETE_REG)ExAllocatePool(NonPagedPool,sizeof(DISABLE_DELETE_REG));
					p->ulPID = *(PULONG)InputBuffer;
					p->next = NULL;

					//��������һ���ڵ����Ϣ����ָ���ƶ������һ���ڵ�λ��
					if (pDisableDeleteRegTmp)
					{
						pDisableDeleteRegInfo = pDisableDeleteRegTmp;
					}

					pDisableDeleteRegInfo->next = p;

					//��¼�����ڵ��λ��
					pDisableDeleteRegTmp = pDisableDeleteRegInfo = p;

					//�ײ�
					pDisableDeleteRegInfo = pDisableDeleteRegHead->next;

					if (!bGlobal_disableDeleteReg)
					{
						reg_inlineHook_disableDelete();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLEDELETE2 error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//�ӵ��������������PID
		case IOCTL_MONITOR2_REG_DISABLEDELETE2_FINDPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					ulRet = reg_findPIDInDisableDeleteRegInfo(ulPID);

					//�ѷ���ֵ����ring3
					*(PULONG)OutputBuffer = ulRet;
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLEDELETE2_FINDPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//�ӵ�����������ɾ��PID
		case IOCTL_MONITOR2_REG_DISABLEDELETE2_DELETEPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					reg_deletePIDInDisableDeleteRegInfo(ulPID);
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLEDELETE2_DELETEPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//ֹͣ��ֹע����ɾ��
		case IOCTL_MONITOR2_REG_DISABLEDELETE_STOP:
			{
				__try
				{
					bGlobal_disableDeleteReg = FALSE;

					if (!b_disableDeleteReg)
					{
						reg_inlineHook_disableDelete_stop();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLEDELETE_STOP error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//��ֹע���Ĳ�ѯ
		case IOCTL_MONITOR2_REG_DISABLEQUERY:
			{
				__try
				{
					bGlobal_disableOpenReg = TRUE;

					if (!b_disableOpenReg)
					{
						reg_inlineHook_disableQuery();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLEQUERY error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//��ֹע����ɾ��2
		case IOCTL_MONITOR2_REG_DISABLEQUERY2:
			{
				__try
				{
					PDISABLE_OPEN_REG p;

					//ȫ��
					b_disableOpenReg = TRUE;

					p = (PDISABLE_OPEN_REG)ExAllocatePool(NonPagedPool,sizeof(DISABLE_OPEN_REG));
					p->ulPID = *(PULONG)InputBuffer;
					p->next = NULL;

					//��������һ���ڵ����Ϣ����ָ���ƶ������һ���ڵ�λ��
					if (pDisableOpenRegTmp)
					{
						pDisableOpenRegInfo = pDisableOpenRegTmp;
					}

					pDisableOpenRegInfo->next = p;

					//��¼�����ڵ��λ��
					pDisableOpenRegTmp = pDisableOpenRegInfo = p;

					//�ײ�
					pDisableOpenRegInfo = pDisableOpenRegHead->next;

					if (!bGlobal_disableOpenReg)
					{
						reg_inlineHook_disableQuery();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLEQUERY2 error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//�ӵ��������������PID
		case IOCTL_MONITOR2_REG_DISABLEQUERY2_FINDPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					ulRet = reg_findPIDInDisableOpenRegInfo(ulPID);

					//�ѷ���ֵ����ring3
					*(PULONG)OutputBuffer = ulRet;
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLEQUERY2_FINDPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//�ӵ�����������ɾ��PID
		case IOCTL_MONITOR2_REG_DISABLEQUERY2_DELETEPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					reg_deletePIDInDisableOpenRegInfo(ulPID);
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLEQUERY2_DELETEPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//ֹͣ��ֹע���Ĳ�ѯ
		case IOCTL_MONITOR2_REG_DISABLEQUERY_STOP:
			{
				__try
				{
					bGlobal_disableOpenReg = FALSE;

					if (!b_disableOpenReg)
					{
						reg_inlineHook_disableQuery_stop();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLEQUERY_STOP error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//��ֹ��������
		case IOCTL_SYSMODULE_DISABLELOADDRIVER:
			{
				__try
				{
					//���inline hook ʧ��
					if (!sysmodule_inlineHook_disableLoadDriver())
					{
						break;
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_SYSMODULE_DISABLELOADDRIVER error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//ֹͣ��ֹ��������
		case IOCTL_SYSMODULE_DISABLELOADDRIVER_STOP:
			{
				__try
				{
					sysmodule_inlineHook_disableLoadDriver_stop();
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_SYSMODULE_DISABLELOADDRIVER_STOP error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//�������ģ���б��С
		case IOCTL_SYSMODULE_GETSIZE:
			{
				__try
				{
					ULONG ulCount=0;

					//���ȫ����������Ϊ��
					if(!global_pDriverObj)
					{
						break;
					}

					//��õ�������ͷ��
					status = sysmodule_enumDrivers(global_pDriverObj);

					//�����ȡʧ��
					if (!NT_SUCCESS(status))
					{
						break;
					}

					//����б��С
					ulCount = sysmodule_getDriverModuleCount();

					*(PULONG)OutputBuffer  = ulCount;
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_SYSMODULE_GETSIZE error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//����б�
		case IOCTL_SYSMODULE_GETINFO:
			{
				__try
				{
					//�������ݵ�ring3
					status = sysmodule_getDriverModuleInfo((PSYSTEM_MODULE_INFO)OutputBuffer);
					if (!NT_SUCCESS(status))
					{
						break;
					}
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_SYSMODULE_GETINFO error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//���ϵͳ�ص���С
		case IOCTL_KERNEL_SYSTEMCALLBACK_GETSIZE:
			{
				__try
				{
					ULONG ulCount=0;

					//��õ�������ͷ��
					status = kernel_systemCallbackEnum();

					//�����ȡʧ��
					if (!NT_SUCCESS(status))
					{
						break;
					}

					//����б��С
					ulCount = kernel_systemCallbackGetCount();

					*(PULONG)OutputBuffer  = ulCount;
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_KERNEL_SYSTEMCALLBACK_GETSIZE error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//����б�
		case IOCTL_KERNEL_SYSTEMCALLBACK_GETINFO:
			{
				__try
				{
					//�������ݵ�ring3
					status = kernel_systemCallbackGetInfo((PKERNEL_SYSTEM_CALLBACK_INFO)OutputBuffer);
					if (!NT_SUCCESS(status))
					{
						break;
					}
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_KERNEL_SYSTEMCALLBACK_GETINFO error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//ϵͳ�ص�����
		case IOCTL_KERNEL_SYSTEMCALLBACK_KILL:
			{
				__try
				{
					SYSTEM_CALLBACK_CLIENT_ID clientID;
					BOOLEAN bRet = FALSE;
					//��ô�ring3�������Ľ���clientID

					//��������
					RtlCopyMemory(&clientID,InputBuffer,sizeof(SYSTEM_CALLBACK_CLIENT_ID));

					bRet = kernel_systemCallbackKill(clientID.ulEntryAddress,clientID.ulFuncAddress);

					if (!bRet)
					{
						break;
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_PROCESS_KILLTHREAD_APC error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//��ù���������С
		case IOCTL_KERNEL_FILTERDRIVER_GETSIZE:
			{
				__try
				{
					ULONG ulCount=0;

					//��õ�������ͷ��
					status = kernel_filterDriverEnum();

					//�����ȡʧ��
					if (!NT_SUCCESS(status))
					{
						break;
					}

					//����б��С
					ulCount = kernel_filterDriverGetCount();

					*(PULONG)OutputBuffer  = ulCount;
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_KERNEL_FILTERDRIVER_GETSIZE error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//����б�
		case IOCTL_KERNEL_FILTERDRIVER_GETINFO:
			{
				__try
				{
					//�������ݵ�ring3
					status = kernel_filterDriverGetInfo((PKERNEL_FILTERDRIVER_INFO)OutputBuffer);
					if (!NT_SUCCESS(status))
					{
						break;
					}
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_KERNEL_FILTERDRIVER_GETINFO error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//ժ����������
		case IOCTL_KERNEL_FILTERDRIVER_KILL:
			{
				__try
				{
					ULONG ulAttachObj=0;

					ulAttachObj = *(PULONG)InputBuffer;

					status = kernel_filterDriverKill((PDEVICE_OBJECT)ulAttachObj);
					if (!NT_SUCCESS(status))
					{
						break;
					}
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_KERNEL_FILTERDRIVER_GETINFO error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//�����
		case IOCTL_DISASM:
			{
				__try
				{
					DISASM_CLIENT_ID clientID;

					//��������
					RtlCopyMemory(&clientID,InputBuffer,sizeof(DISASM_CLIENT_ID));

					//�����ڴ����ݵ�ring3
					RtlCopyMemory((PBYTE)OutputBuffer,(PBYTE)clientID.ulStartAddress,clientID.ulSize);
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_DISASM error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//�������
		case IOCTL_MONITOR_START:
			{
				__try
				{
					monitor_start();
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_MONITOR_START error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//ֹͣ���
		case IOCTL_MONITOR_STOP:
			{
				__try
				{
					monitor_stop();
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_MONITOR_STOP error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//
		case IOCTL_MONITOR_GETSIZE:
			{
				__try
				{
					ULONG ulCount = 0;

					//����б��С
					ulCount = monitor_getCount();

					*(PULONG)OutputBuffer  = ulCount;
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_MONITOR_GETSIZE error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//��ü����Ϣ
		case IOCTL_MONITOR_GETINFO:
			{
				__try
				{
					//�������ݵ�ring3
					status = monitor_getInfo((PMONITOR_INFO)OutputBuffer);
					if (!NT_SUCCESS(status))
					{
						break;
					}
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_MONITOR_GETINFO error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//��ü�ع�����Ϣ
		case IOCTL_MONITOR_GETFILTERINFO_PROCESS:
			{
				__try
				{
					PMONITOR_PROCESS_FILTER_INFO p = (PMONITOR_PROCESS_FILTER_INFO)ExAllocatePool(NonPagedPool,sizeof(MONITOR_PROCESS_FILTER_INFO));

					if (!p)
					{
						break;
					}

					DbgPrint("process name:%ws,bInclude:%d",p->ProcessName,p->bProcessInclude);

					RtlCopyMemory(p,InputBuffer,sizeof(MONITOR_PROCESS_FILTER_INFO));

					//
					status = monitor_updateProcessMonitorFilerInfo(p);

					if (!NT_SUCCESS(status))
					{
						break;
					}
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_MONITOR_GETFILTERINFO_PROCESS error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//��ü�ع�����Ϣ
		case IOCTL_MONITOR_GETFILTERINFO_PATH:
			{
				__try
				{
					PMONITOR_PATH_FILTER_INFO p = ExAllocatePool(NonPagedPool,sizeof(MONITOR_PATH_FILTER_INFO));

					if (!p)
					{
						break;
					}

					RtlCopyMemory(p,(PMONITOR_PATH_FILTER_INFO)InputBuffer,sizeof(MONITOR_PATH_FILTER_INFO));

					//
					status = monitor_updatePathMonitorFilerInfo(p);

					if (!NT_SUCCESS(status))
					{
						break;
					}
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_MONITOR_GETFILTERINFO_PATH error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//��ü�ع�����Ϣ
		case IOCTL_MONITOR_FREEFILTERINFO:
			{
				__try
				{
					monitor_freeMonitorFilerInfo();
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_MONITOR_FREEFILTERINFO error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//
		case IOCTL_PROCESS_GETHANDLEINFOSIZE:
			{
				__try
				{
					ULONG ulCount = 0;

					//��ý��̾����Ϣ
					status =  process_getProcessHandleInfo(*(PULONG)InputBuffer);

					if (!NT_SUCCESS(status))
					{
						break;
					}

					//����б��С
					ulCount = process_getProcessHandleInfoCount();

					*(PULONG)OutputBuffer  = ulCount;
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_PROCESS_GETHANDLEINFOSIZE error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//��ý��̾����Ϣ
		case IOCTL_PROCESS_GETHANDLEINFO:
			{
				__try
				{
					//�������ݵ�ring3
					status = process_getProcessHandleInfo_send((PPROCESS_HANDLE_INFO)OutputBuffer);
					if (!NT_SUCCESS(status))
					{
						break;
					}
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_PROCESS_GETHANDLEINFO error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//
		case IOCTL_FILE_GETHANDLEINFOSIZE:
			{
				__try
				{
					ULONG ulCount = 0;

					LPTSTR lpFilePath = (LPTSTR)InputBuffer;

					DbgPrint("IOCTL_FILE_GETHANDLEINFOSIZE lpFilePath:%ws",lpFilePath);

					//��ý��̾����Ϣ
					status =  file_getFileHandleInfo(lpFilePath);

					if (!NT_SUCCESS(status))
					{
						break;
					}

					//����б��С
					ulCount = file_getFileHandleInfoCount();

					DbgPrint("IOCTL_FILE_GETHANDLEINFOSIZE ulCount:%d",ulCount);

					*(PULONG)OutputBuffer  = ulCount;
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_FILE_GETHANDLEINFOSIZE error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//
		case IOCTL_FILE_GETHANDLEINFO:
			{
				__try
				{
					//�������ݵ�ring3
					status = file_getFileHandleInfo_send((PFILE_HANDLE_INFO)OutputBuffer);
					if (!NT_SUCCESS(status))
					{
						break;
					}
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_FILE_GETHANDLEINFO error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
		}
		break;
	}
	if(status == STATUS_SUCCESS)
	{
		pIrp->IoStatus.Information = uOutSize;
	}
	else
	{
		pIrp->IoStatus.Information = 0;
	}
	// �������
	pIrp->IoStatus.Status = status;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return status;
}


//�������
NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObj,IN PUNICODE_STRING RegistryPath)
{
	//�豸����
	UNICODE_STRING        ustrDeviceName;
	//������������
	UNICODE_STRING        ustrLinkName;

	NTSTATUS              status;
	PDEVICE_OBJECT        pDevObj;


	DbgPrint("��������������ڣ�");

	RtlInitUnicodeString(&ustrDeviceName, DEVICE_NAME);
	RtlInitUnicodeString(&ustrLinkName,LINK_NAME);

	//��ʼ������ǲ����
	pDriverObj->MajorFunction[IRP_MJ_CREATE]=DispatchCreateClose;
	pDriverObj->MajorFunction[IRP_MJ_CLOSE]=DispatchCreateClose;
	pDriverObj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchIoctl;
	pDriverObj->DriverUnload = DriverUnload;

	//�����豸����
	status = IoCreateDevice(pDriverObj,0x100u,&ustrDeviceName,FILE_DEVICE_UNKNOWN,
		FILE_DEVICE_SECURE_OPEN,
		TRUE,
		&pDevObj);

	//�ж��豸�����Ƿ񴴽��ɹ�
	if (!NT_SUCCESS(status))
	{
		return status;
	}

	//������������
	status = IoCreateSymbolicLink(&ustrLinkName, &ustrDeviceName);

    //�жϷ��������Ƿ񴴽��ɹ�
	if (!NT_SUCCESS(status))
	{
		//ɾ���豸����
		IoDeleteDevice(pDevObj);
		return status;
	}

	//���Shadow SSDT��ַ
	KeServiceDescriptorTableShadow = (PServiceDescriptorTable)getAddressOfShadowTable();

    //DbgPrint("KeServiceDescriptorTableShadow base:%X",KeServiceDescriptorTableShadow->ServiceTableBase);
    //DbgPrint("KeServiceDescriptorTableShadow[1] base:%X",KeServiceDescriptorTableShadow[1].ServiceTableBase);
	//DbgPrint("KeServiceDescriptorTableShadow NumberOfServices:%d",KeServiceDescriptorTableShadow[1].NumberOfServices);

	global_pDriverObj = pDriverObj;

	pDisableCreateProcessInfo = pDisableCreateProcessHead = (PDISABLE_CREATE_PROCESS)ExAllocatePool(NonPagedPool,sizeof(DISABLE_CREATE_PROCESS));
	pDisableTerminateProcessInfo = pDisableTerminateProcessHead = (PDISABLE_TERMINATE_PROCESS)ExAllocatePool(NonPagedPool,sizeof(DISABLE_TERMINATE_PROCESS));
    //
	pDisableCreateFileInfo = pDisableCreateFileHead = (PDISABLE_CREATE_FILE)ExAllocatePool(NonPagedPool,sizeof(DISABLE_CREATE_FILE));
	pDisableDeleteFileInfo = pDisableDeleteFileHead = (PDISABLE_DELETE_FILE)ExAllocatePool(NonPagedPool,sizeof(DISABLE_DELETE_FILE));
	pDisableOpenFileInfo = pDisableOpenFileHead = (PDISABLE_OPEN_FILE)ExAllocatePool(NonPagedPool,sizeof(DISABLE_OPEN_FILE));
    //
	pDisableCreateRegInfo = pDisableCreateRegHead = (PDISABLE_CREATE_REG)ExAllocatePool(NonPagedPool,sizeof(DISABLE_CREATE_REG));
    pDisableDeleteRegInfo = pDisableDeleteRegHead = (PDISABLE_DELETE_REG)ExAllocatePool(NonPagedPool,sizeof(DISABLE_DELETE_REG));
    pDisableOpenRegInfo = pDisableOpenRegHead = (PDISABLE_OPEN_REG)ExAllocatePool(NonPagedPool,sizeof(DISABLE_OPEN_REG));

	pMonitorProcessFilterInfo = pMonitorProcessFilterInfoHead = (PMONITOR_PROCESS_FILTER_INFO)ExAllocatePool(NonPagedPool,sizeof(MONITOR_PROCESS_FILTER_INFO));
    pMonitorPathFilterInfo = pMonitorPathFilterInfoHead = (PMONITOR_PATH_FILTER_INFO)ExAllocatePool(NonPagedPool,sizeof(MONITOR_PATH_FILTER_INFO));

	//
	pDisableCreateProcessInfo->next = NULL;
	pDisableTerminateProcessInfo->next = NULL;
    //
	pDisableCreateFileInfo->next = NULL;
	pDisableDeleteFileInfo->next = NULL;
	pDisableOpenFileInfo->next = NULL;
    //
	pDisableCreateRegInfo->next = NULL;
	pDisableDeleteRegInfo->next = NULL;
	pDisableOpenRegInfo->next = NULL;

	pMonitorProcessFilterInfo->next = NULL;
	pMonitorPathFilterInfo->next = NULL;

	return status;
}