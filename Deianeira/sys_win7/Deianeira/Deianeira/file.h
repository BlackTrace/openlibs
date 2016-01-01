#ifndef _FILE_
#define _FILE_

#include "common.h"
#include <string.h>

//�߳���Ϣ�����б�
typedef struct _FileHandleInfo
{   
	//
	ULONG ulPID;
	//
	ULONG ulHandle;
	//
	WCHAR wstrHandleName[256];
	//�����б�ָ��
	struct FILE_HANDLE_INFO* next;
}FILE_HANDLE_INFO, *PFILE_HANDLE_INFO; 

//
PFILE_HANDLE_INFO pFileHandleInfo,pFileHandleInfoHead;


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
						 );


NTSTATUS JmpNtSetInformationFile (
								  __in HANDLE FileHandle,
								  __out PIO_STATUS_BLOCK IoStatusBlock,
								  __in_bcount(Length) PVOID FileInformation,
								  __in ULONG Length,
								  __in FILE_INFORMATION_CLASS FileInformationClass
								  );

NTSTATUS NewNtSetInformationFile (
								  __in HANDLE FileHandle,
								  __out PIO_STATUS_BLOCK IoStatusBlock,
								  __in_bcount(Length) PVOID FileInformation,
								  __in ULONG Length,
								  __in FILE_INFORMATION_CLASS FileInformationClass
								  );

typedef NTSTATUS (*PNtDeleteFile)(
								  IN POBJECT_ATTRIBUTES ObjectAttributes
								  );

NTSTATUS NewZwDeleteFile(
						 IN POBJECT_ATTRIBUTES ObjectAttributes
						 );

//���ļ�
HANDLE file_openFile(
					 IN LPTSTR lpFileName,
					 IN ACCESS_MASK DesiredAccess,
					 IN ULONG ShareAccess
					 );

//�����ļ�����
BOOLEAN file_setFileAttributes(
							   IN HANDLE hFile,
							   IN ULONG FileAttributes
							   );

//ɾ���ļ�
BOOLEAN file_deleteFile(
						IN HANDLE hFile
						);

//
VOID file_inlineHookDeleteFileFunc();

//
VOID file_unInlineHookDeleteFileFunc();

//�ҹ�IoCreateFile
VOID file_inlineHookIoCreateFile();

//�ָ�IoCreateFile
VOID file_unInlineHookIoCreateFile();


//��DisableCreateFileInfo�в���Ŀ�����ID
//����
//0��ʾû���ҵ���1��ʾ�ҵ���
ULONG file_findPIDInDisableCreateFileInfo(ULONG ulPID);

//��DisableCreateFileInfo��ɾ��Ŀ�����ID
VOID file_deletePIDInDisableCreateFileInfo(ULONG ulPID);


//��DisableDeleteFileInfo�в���Ŀ�����ID
//����
//0��ʾû���ҵ���1��ʾ�ҵ���
ULONG file_findPIDInDisableDeleteFileInfo(ULONG ulPID);

//��DisableDeleteFileInfo��ɾ��Ŀ�����ID
VOID file_deletePIDInDisableDeleteFileInfo(ULONG ulPID);


//��DisableOpenFileInfo�в���Ŀ�����ID
//����
//0��ʾû���ҵ���1��ʾ�ҵ���
ULONG file_findPIDInDisableOpenFileInfo(ULONG ulPID);

//��DisableOpenFileInfo��ɾ��Ŀ�����ID
VOID file_deletePIDInDisableOpenFileInfo(ULONG ulPID);

//����ļ������Ϣ
NTSTATUS file_getFileHandleInfo(LPTSTR lpFilePath);

//��ô�С
ULONG file_getFileHandleInfoCount();

//�������ݵ�ring3
NTSTATUS file_getFileHandleInfo_send(PFILE_HANDLE_INFO pBuffer);

#endif //_FILE_