// Plugin.h

#pragma once

#include "..\common\common.h"
#include "..\common\common_net.h"
#include "..\common\process.h"
#include "..\common\com_.h"
#include "..\common\sigverif.h"
#include "ListViewItemComparer.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace Deianeira;

namespace Plugin {

	public ref class plugin abstract
	{
		// TODO: �ڴ˴���Ӵ���ķ�����

	public:

		static TabControl^ tc_;

		//�������
		static HANDLE hDriver;

		//��ʼ������Ҫ�Ǹ���������Ҫ��ɶ����
		virtual Plugin::InitData^ Init() abstract;

		//����Ƕ����ĳ��������
		virtual System::Void Run() abstract;

		//����Ƕ����ĳ��������
		//virtual System::Void InitializeComponent(TabControl^ tc_) abstract;
		virtual System::Void InitializeComponent(TabControl^ tc_){
			this->tc_ = tc_;
		}

		//
		virtual System::Void Load() abstract;

		//
		static TabControl^ GetTabControl(){
			//
			return tc_;
		}

		//�������ģ����
		static System::Void GetHDriver(HANDLE h){
			hDriver = h;
		}
		
		//
#pragma region sigverif
		//����ļ��Ƿ�ǩ��
		//lpFileName:Ŀ���ļ�·��
		//����:true:��ǩ����false:û��ǩ��
		static bool plugin_sigverif_check(LPCWSTR lpFileName){
			//
			return sigverif_check(lpFileName);
		}


		//�������ǩ������
		//lpFileName:Ŀ���ļ�·��
		//����:����ǩ���������ƣ�����ΪNULL
		static LPWSTR plugin_sigverif_getCompanyName(LPCWSTR lpFileName){
			//
			return sigverif_getCompanyName(lpFileName);
		}
#pragma endregion sigverif

		//
#pragma region common
		//�ػ�����
		//bReboot:��־�ǹػ���������
		//����:true:�ɹ���false:ʧ��
		static bool plugin_common_shutdown(bool bReboot){
			//
			return common_shutdown(bReboot);
		}


		//��������Ȩ��
		//hToken:��������
		//lpszPrivilege:Ȩ�ޱ���
		//bEnablePrivilege:ʹ��
		//����:true:�ɹ���false:ʧ��
		static bool plugin_common_setPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege){
			//
			return common_setPrivilege(hToken,lpszPrivilege,bEnablePrivilege);
		}

		//��������Ȩ��
		//ulPID:����ID
		//bEnablePrivilege:ʹ��
		//����:true:�ɹ���false:ʧ��
		static bool plugin_common_setPrivilege(ULONG ulPID, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege){
			//
			return common_setPrivilege(ulPID,lpszPrivilege,bEnablePrivilege);
		}

		//��������
		//lpFileName:�����ļ�·��
		//lpLinkName:��������
		//����:�ɹ��򷵻��������,ʧ���򷵻�NULL
		static HANDLE plugin_common_loadDriver(LPCTSTR lpFileName,LPTSTR lpLinkName){
			//
			return common_loadDriver(lpFileName,lpLinkName);
		}

		//ж������
		//hDriver:����ģ����
		//lpLinkName:��������
		//����:true:�ɹ���false:ʧ��
		static bool plugin_common_unloadDriver(HANDLE hDriver,LPTSTR lpLinkName){
			//
			return common_unloadDriver(hDriver,lpLinkName);
		}


		//���ļ����ԶԻ���
		//lpFilePath:Ŀ���ļ�·��
		//����:��
		static void plugin_common_showFileProperty(LPTSTR lpFilePath){
			//
			return common_showFileProperty(lpFilePath);
		}

		//��λ�ļ�
		//lpFilePath:Ŀ���ļ�·��
		//����:��
		static void plugin_common_explorerFile(LPSTR lpFilePath){
			//
			return common_explorerFile(lpFilePath);
		}

		//��λע���
		//lpRegPath:ע���·��
		//����:true:�ɹ���false:ʧ��
		static bool plugin_common_explorerReg(LPTSTR lpRegPath){
			//
			return common_explorerReg(lpRegPath);
		}

		//������ԴID�������ļ� 
		//resId:��ԴID 
		//lpResType:��Դ����
		//lpFileName:Ŀ���ļ�·�� 
		//����:true:�ɹ���false:ʧ��
		static bool plugin_common_createFileFromResource(int resId,LPTSTR lpResType,LPTSTR lpFileName){
			//
			return common_createFileFromResource(resId,lpResType,lpFileName);
		}

		//��ӷ���
		//lpName:��������
		//lpDisplayName:�Ѻ���ʾ����
		//ulStartMode:��������
		//lpPath:Ӧ�ó���·��
		//����:true:�ɹ���false:ʧ��
		static bool plugin_common_addService(LPTSTR lpName,LPTSTR lpDisplayName,ULONG ulStartMode,LPTSTR lpPath){
			//
			return common_addService(lpName,lpDisplayName,ulStartMode,lpPath);
		}

		//ɾ������
		//lpName:��������
		//����:true:�ɹ���false:ʧ��
		static bool plugin_common_delService(LPTSTR lpName){
			//
			return common_delService(lpName);
		}

		//���ô���
		//lpIP:IP��ַ
		//lpPort:�˿�
		//����:true:�ɹ���false:ʧ��
		static bool plugin_common_setProxy(LPTSTR lpIP,LPTSTR lpPort){
			//
			return common_setProxy(lpIP,lpPort);
		}

		//ȡ������
		//����:true:�ɹ���false:ʧ��
		static bool plugin_common_delProxy(){
			//
			return common_delProxy();
		}
#pragma endregion common


		//Net����������
#pragma region common_net
		//ListView����
		//column:��
		//bAsc:�������ǽ���
		//type:0�����֣�1���ַ�����2��ʱ��
		//type2:ֻ��typeΪ0ʱ��Ч������10����ʾʮ���ƱȽϣ�Ϊ16��ʾ16���ƱȽ�
		static ListViewItemComparer^ plugin_common_net_sortLV(int column,bool bAsc,int type,int type2){
			//
			return gcnew ListViewItemComparer(column, bAsc, type, type2);
		}

		//���ϵͳ�汾
		static ULONG plugin_common_net_getOSVersion(){
			//
			return Deianeira::common_net::common_getOSVersion();
		}

		//�й�ת��Ϊ���й�
		static LPSTR plugin_common_net_stringToHGlobalAnsi(String^ str_){
			//
			return Deianeira::common_net::common_stringToHGlobalAnsi(str_);
		}

		//�й�ת��Ϊ���й�
		static LPTSTR plugin_common_net_stringToHGlobalUni(String^ str_){
			//
			return Deianeira::common_net::common_stringToHGlobalUni(str_);
		}

		//��������Ϣ
		//str_secName:������
		//str_keyName:Ҫ��ȡ���ֶ�����
		//str_filePath:�����ļ�·��
		//����:Ŀ���ֶ�ֵ
		static System::String^ plugin_common_net_getPrivateProfileString(String^ str_secName,String^ str_keyName,String^ str_filePath){
			//
			return Deianeira::common_net::config_getPrivateProfileString(str_secName,str_keyName,str_filePath);
		}

		//д������Ϣ
		//str_secName:������
		//str_keyName:Ҫ���õ��ֶ�����
		//str_keyValue:�ֶ�ֵ
		//str_filePath:�����ļ�·��
		//����:��
		static System::Void plugin_common_net_writePrivateProfileString(String^ str_secName,String^ str_keyName,String^ str_keyValue,String^ str_filePath){
			//
			return Deianeira::common_net::config_writePrivateProfileString(str_secName,str_keyName,str_keyValue,str_filePath);
		}

		//���ݽ���ID��ý�������
		static System::String^ plugin_common_net_getProcessName(int PID) {
			//
			return Deianeira::common_net::common_getProcessName(PID);
		}

		//���ݽ���ID��ý���ӳ���ļ�·��
		static System::String^ plugin_common_net_getProcessPath(int PID) {
			//
			return Deianeira::common_net::common_getProcessPath(PID);
		}

		//���ݽ���ID��ý���ӳ���ļ���������
		static System::String^ plugin_common_net_getProcessCompanyName(int PID){
			//
			return Deianeira::common_net::common_getProcessCompanyName(PID);
		}

		//����ļ���������
		static System::String^ plugin_common_net_getFileCompanyName(String^ str_) {
			//
			return Deianeira::common_net::common_getFileCompanyName(str_);
		}

		//����ļ��ĳ����������ƣ��Ƿ���΢�������
		//str_:Ŀ���ļ��ĳ�����������
		//����:true:Ϊ΢�������ƣ�false:����
		static bool plugin_common_net_checkFileCompanyName(String^ str_){
			//
			return Deianeira::common_net::common_checkFileCompanyName(str_);
		}

		//����ļ�MD5
		static System::String^ plugin_common_net_getFileMD5(String^ str_){
			return Deianeira::common_net::common_getFileMD5(str_);
		}

		//�����ַ���
		//str_:Ŀ���ַ���
		//����:������Ľ��
		static System::String^ plugin_common_net_formatString(String^ str_){
			//
			return Deianeira::common_net::common_formatString(str_);
		}

		//����ListView���е����ݵ��ļ�
		static System::Void plugin_common_net_getListViewData(ListView^ lv_){
			return Deianeira::common_net::common_getListViewData(lv_);
		}

		//���IP�����ַ
		static System::String^ plugin_common_net_getIPAddr(String^ str_){
			return Deianeira::common_net::common_getIPAddr(str_);
		}

		//�ж�ָ��rk_���Ƿ����str_�Ӽ�
		static bool plugin_common_net_checkRegkey(RegistryKey^ rk_ ,String^ str_){
			return Deianeira::common_net::common_checkRegkey(rk_,str_);
		}

		//�ж�ָ��rk_���Ƿ����str_ֵ
		static bool plugin_common_net_checkRegkeyValue(RegistryKey^ rk_ ,String^ str_){
			return Deianeira::common_net::common_checkRegkeyValue(rk_,str_);
		}
#pragma endregion common_net


		//���̷���
#pragma region process
		//��ȡ���̵�״̬
		//����0����ʾ�����쳣
		//����1����ʾ���̴��ڹ���״̬
		//����2����ʾ����û�б�����
		static ULONG plugin_process_getProcessState(ULONG ulPID){
			//
			return process_getProcessState(ulPID);
		}

		//��ȡ���̵ĸ�����ID
		//ulPID:Ŀ�����ID
		//����:-1:ʧ�ܣ�����ɹ�
		static ULONG plugin_process_getProcessParentPID(ULONG ulPID){
			//
			return process_getProcessParentPID(ulPID);
		}

		//��ȡ�̵߳�״̬
		//����0����ʾ�����쳣
		//����1����ʾ�̴߳��ڹ���״̬
		//����2����ʾ�߳�û�б�����
		static ULONG plugin_process_getThreadState(ULONG ulPID,ULONG ulTID){
			//
			return process_getThreadState(ulPID,ulTID);
		}

		//ע��DLL
		//lpFilePath:DLL�ļ�·��
		//ulPID:����ID
		static bool plugin_process_injectDll(LPTSTR lpFilePath,ULONG ulPID){
			//
			return process_injectDll(lpFilePath,ulPID);
		}

		//ж�ؽ���ģ��
		static bool plugin_process_unloadModule(ULONG ulPID,ULONG ulDllBase){
			//���ж��ģ��ɹ�
			if (process_unloadModule(hDriver,ulPID,ulDllBase))
			{
				return true;
			}

			//
			return false;
		}

		//�����߳�
		static bool plugin_process_suspendThread(ULONG ulTID){
			//��������̳߳ɹ�
			if (process_suspendThread(hDriver,ulTID))
			{
				return true;
			}

			//
			return false;
		}

		//�ָ��߳�
		static bool plugin_process_resumeThread(ULONG ulTID){
			//����ָ��̳߳ɹ�
			if (process_resumeThread(hDriver,ulTID))
			{
				return true;
			}

			//
			return false;
		}

		//�����߳�
		static bool plugin_process_killThread(ULONG ulTID){
			//��������̳߳ɹ�
			if (process_killThread(hDriver,ulTID))
			{
				return true;
			}

			//
			return false;
		}

		//�������
		static bool plugin_process_suspendProcess(ULONG ulPID){
			//���������̳ɹ�
			if (process_suspendProcess(hDriver,ulPID))
			{
				return true;
			}

			//
			return false;
		}

		//�ָ���������
		static bool plugin_process_resumeProcess(ULONG ulPID){
			//����ָ��������гɹ�
			if (process_resumeProcess(hDriver,ulPID))
			{
				return true;
			}

			//
			return false;
		}

		//��������
		static bool plugin_process_killPrcess(ULONG ulPID){
			//����������̳ɹ�
			if (process_killProcess(hDriver,ulPID))
			{
				return true;
			}

			//
			return false;
		}

		//�ڴ�����
		static bool plugin_process_forceKillProcess(ULONG ulPID){
			//����ڴ�����ɹ�
			if (process_killProcess_memclr(hDriver,ulPID))
			{
				return true;
			}

			//
			return false;
		}
#pragma endregion process


	};
}
