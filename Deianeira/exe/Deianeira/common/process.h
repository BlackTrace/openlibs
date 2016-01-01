//process.h:����һЩ���̲�����
//
//
#ifndef _PROCESS_
#define _PROCESS_
#include "common.h"

//
extern PPROCESS_MODULE_INFO pProcessModuleInfo,pProcessModuleInfoHead;

//��ȡ�̵߳�״̬
//ulPID:Ŀ�����ID
//ulTID:Ŀ���߳�ID
//����:0:��ʾ�����쳣,1:��ʾ���̴��ڹ���״̬,2:��ʾ����û�б�����
ULONG process_getThreadState(ULONG ulPID,ULONG ulTID);

//�ں˹����߳�
//hDriver:�������
//ulPID:Ŀ�����ID
//����:true:��ʾ����ɹ�,false:��ʾ����ʧ��
bool process_suspendThread(HANDLE hDriver,ULONG ulTID);

//�ں˻ָ��߳�
//hDriver:�������
//ulPID:Ŀ�����ID
//����:true:��ʾ�ָ��ɹ�,false:��ʾ�ָ�ʧ��
bool process_resumeThread(HANDLE hDriver,ULONG ulTID);

//�ں˽����߳�
//hDriver:�������
//ulTID:Ŀ���߳�ID
//����:true:��ʾɾ���ɹ�,false:��ʾɾ��ʧ��
bool process_killThread(HANDLE hDriver,ULONG ulTID);

//��ȡ���̵ĸ�����ID
//ulPID:Ŀ�����ID
//����:Ŀ����̵ĸ�����ID
ULONG process_getProcessParentPID(ULONG ulPID);

//��ȡ���̵�״̬
//ulPID:Ŀ�����ID
//����:0:��ʾ�����쳣,1:��ʾ���̴��ڹ���״̬,2:��ʾ����û�б�����
ULONG process_getProcessState(ULONG ulPID);

//��������
//ulPID:Ŀ�����ID
//����:true:��ʾ����ɹ�,false:��ʾ����ʧ��
bool process_suspendProcess(ULONG ulPID);

//�ں���������
//hDriver:�������
//ulPID:Ŀ�����ID
//����:true:��ʾ����ɹ�,false:��ʾ����ʧ��
bool process_suspendProcess(HANDLE hDriver,ULONG ulPID);

//�ָ�����
//ulPID:Ŀ�����ID
//����:true:��ʾ�ָ��ɹ�,false:��ʾ�ָ�ʧ��
bool process_resumeProcess(ULONG ulPID);

//�ں˻ָ�����
//hDriver:�������
//ulPID:Ŀ�����ID
//����:true:��ʾ�ָ��ɹ�,false:��ʾ�ָ�ʧ��
bool process_resumeProcess(HANDLE hDriver,ULONG ulPID);

//�ں˽�������
//hDriver:�������
//ulPID:Ŀ�����ID
//����:true:��ʾ�ָ��ɹ�,false:��ʾ�ָ�ʧ��
bool process_killProcess(HANDLE hDriver,ULONG ulPID);

//�ں˽�������,�ڴ�����
//hDriver:�������
//ulPID:Ŀ�����ID
//����:true:��ʾ�ָ��ɹ�,false:��ʾ�ָ�ʧ��
bool process_killProcess_memclr(HANDLE hDriver,ULONG ulPID);

//�ں˽���ģ��ж��
//hDriver:�������
//ulPID:Ŀ�����ID
//ulDllBase:DLLģ���ַ
//����:true:��ʾж�سɹ�,false:��ʾж��ʧ��
bool process_unloadModule(HANDLE hDriver,ULONG ulPID,ULONG ulDllBase);

//ע��DLL
//lpFilePath:DLL�ļ�·��
//ulPID:����ID
//����:true:ע��ɹ�,false:ע��ʧ��
bool process_injectDll(LPTSTR lpFilePath,ULONG ulPID);

//��ý��̵�ģ����Ϣ�б�
//����:true:�ɹ�,false:ʧ��
bool process_getModuleInfoTable();

//��ý����߳��б�
PSYSTEM_PROCESSES GetThreadList();

#endif //_PROCESS_