#pragma once
#include "windows.h"
#include "MyNativeHead.h"
#include "SSDT.h"


#pragma warning( disable: 4312 )
#pragma warning( disable: 4311 )
#pragma warning( disable: 4244 )
#pragma warning( disable: 4267 )
#pragma warning( disable: 4996 )


//#include <Ntsecapi.h>
#include <Aclapi.h>
#include <tlhelp32.h>
#include "winioctl.h"
#define KSBINSWORD_IOCTL_BASE	0x800
#define FILE_DEVICE_KSBINSWORD	0x8000
#define CTL_CODE_KSBINSWORD(i) CTL_CODE(FILE_DEVICE_KSBINSWORD, KSBINSWORD_IOCTL_BASE+i, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_KSBINSWORD_HELLO	CTL_CODE_KSBINSWORD(0)
#define IOCTL_KSBINSWORD_TEST	CTL_CODE_KSBINSWORD(1)
#define IOCTL_KSBINSWORD_GETPRONUM	CTL_CODE_KSBINSWORD(2)//�õ����̵��������������ڴ�
#define IOCTL_KSBINSWORD_GETPRO	CTL_CODE_KSBINSWORD(3)//�õ����̵ĸ�������
#define IOCTL_KSBINSWORD_GETTHRNUM	CTL_CODE_KSBINSWORD(4)//�õ��߳�
#define IOCTL_KSBINSWORD_GETTHR	CTL_CODE_KSBINSWORD(5)//�õ��߳�
#define IOCTL_KSBINSWORD_PMON CTL_CODE_KSBINSWORD(6)//���̼��ӿ�
#define IOCTL_KSBINSWORD_PMOFF CTL_CODE_KSBINSWORD(7)//���̼��ӹ�
#define IOCTL_KSBINSWORD_RMON CTL_CODE_KSBINSWORD(8)//ע�����ӿ�
#define IOCTL_KSBINSWORD_RMOFF CTL_CODE_KSBINSWORD(9)//ע�����ӹ�
#define IOCTL_KSBINSWORD_MMON CTL_CODE_KSBINSWORD(10)//ģ����ӿ�
#define IOCTL_KSBINSWORD_MMOFF CTL_CODE_KSBINSWORD(11)//ģ����ӹ�
#define IOCTL_KSBINSWORD_GETSSDT CTL_CODE_KSBINSWORD(12)//�õ�SSDT
#define IOCTL_KSBINSWORD_SETSSDT CTL_CODE_KSBINSWORD(13)//����SSDT
#define IOCTL_KSBINSWORD_GETHOOK CTL_CODE_KSBINSWORD(14)//��ѯSSDT HOOK������ַ
#define IOCTL_KSBINSWORD_SETHOOK CTL_CODE_KSBINSWORD(15)//����SSDT HOOK������ַ
#define IOCTL_KSBINSWORD_KILLTHREAD CTL_CODE_KSBINSWORD(16)//ǿ��kill �߳�

#define IOCTL_KSBINSWORD_KILLPROCESS CTL_CODE_KSBINSWORD(17)//ǿ��kill ����
#define IOCTL_KSBINSWORD_CLEARPROCESS CTL_CODE_KSBINSWORD(18)//�����ѯ���̺ۼ�
#define IOCTL_KSBINSWORD_CLEARTHREAD CTL_CODE_KSBINSWORD(19)//�����ѯ�̺߳ۼ�

#define IOCTL_KSBINSWORD_SCSI_READ  CTL_CODE_KSBINSWORD(20)//SRB��ȡ����
#define IOCTL_KSBINSWORD_SCSI_WRITE CTL_CODE_KSBINSWORD(21)//SRBд������

#define IOCTL_KSBINSWORD_GETMONINFO CTL_CODE_KSBINSWORD(22)//�õ�xx��������Ϣ

#define MAX_MODULE_NAME_LEN 16

typedef struct tagThreadParam 
{
	DWORD dwFreeLibrary;                    // FreeLibrary ��ַ
	DWORD dwGetModuleHandle;                // GetModuleHandle ��ַ
	TCHAR szModuleName[MAX_MODULE_NAME_LEN];// ��Ҫж�ص�ģ�������
}ThreadParam,*PThreadParam;

typedef struct tagKillThreadParam 
{
	DWORD dwOpenThread;                    // OpenThread ��ַ
	DWORD dwTerminateThread;                // TerminateThread ��ַ
	DWORD dwCid;// ��Ҫж�ص��߳�Cid
}KillThreadParam,*PKillThreadParam;

typedef struct tagKillThreadStruct
{
   ULONG dwProcessId;
   ULONG dwCid;
}KillThreadStruct,*PKillThreadStruct;

typedef struct tagReadSectorStruct
{
	__int64 Sect;
	BYTE OutBuf[0x200];
}ReadSectorStruct,*PReadSectorStruct;



class CMySystem
{
public:
	CMySystem(void);
public:
	~CMySystem(void);
public:
	static int HexToInt(CString Hex);
	static __int64 HexToBigInt(CString Hex);
public:
	//BOOL static EnablePrivilege(HANDLE hToken,LPCTSTR szPrivName,BOOL fEnable);
	BOOL static EnablePrivilege(LPCTSTR szPrivName);
public:
	DWORD static GetPidByName(LPCWSTR szName) ;
public:
	static int  KillProcess(ULONG dwProcessId);
	static BOOL ForceKillProcess(HANDLE hMyHandle,ULONG dwProcessId);

	static BOOL ForceKillThread(HANDLE hMyHandle,ULONG dwProcessId,ULONG dwCid);

	static bool ClearPro(HANDLE hMyHandle);
	static bool ClearThr(HANDLE hMyHandle);
public:
	void static RemoteThreadFun(PThreadParam PParam);
	bool static AdjustPurview(void);
	BOOL static FreeModuleByPid(TCHAR * szModuleName,DWORD dwPid);
	HANDLE static
		My_OpenProcess (
		DWORD dwDesiredAccess,
		BOOL   bInhert,
		DWORD ProcessId,
		BOOL   bOpenIt,
		LPDWORD aryPids
		);
	HANDLE static OpenDevice(LPCTSTR lpszDevicePath);
	int static GetProNum(HANDLE hMyHandle);
	DWORD static GetPro(HANDLE hMyHandle,void*ProcessNum,int size);
	int static GetThrNum(HANDLE hMyHandle,int Tid);
	int static GetThr(HANDLE hMyHandle,void*ThreadNum,int size);
	void static CMySystem::BeginMon();
	void static CMySystem::ThreadMon();


	static void   LoadDriver( IN WCHAR * lpszDriverName );
	static void UnloadDriver(    WCHAR * szSvrName ) ;
	static BOOL GetSSDT( IN HANDLE hDriver, OUT PSSDT ssdt );
	static BOOL SetSSDT( IN HANDLE hDriver, IN PSSDT ssdt );
	static BOOL GetHook( IN HANDLE hDriver, IN ULONG ulIndex, OUT PULONG ulAddr );
	static BOOL SetHook( IN HANDLE hDriver, IN ULONG ulIndex, IN OUT PULONG ulAddr );
	static PSYSMODULELIST CreateModList( OUT PULONG ulNtBase );
	static void DestroyModList( IN PSYSMODULELIST pList );
	static BOOL GetModuleNameByAddr( IN ULONG ulAddr, IN PSYSMODULELIST pList, OUT LPSTR buf, IN DWORD dwSize );
	static BOOL SSDTSTOrderByServiceNum( pSSDTSaveTable pSsdtST );
	static BOOL EnumSSDT( IN HANDLE hDriver );
	static BOOL ReSSDT( IN HANDLE hDriver );
	static BOOL ReSSDTAndThrowSpilth( IN HANDLE hDriver );

    static void CharToWCHAR(WCHAR* wctar,char*cSource);
	static void WCHARToChar(char*cSource,WCHAR* wctar);
	static DWORD GetHeaders(PCHAR ibase,
		PIMAGE_FILE_HEADER *pfh,
		PIMAGE_OPTIONAL_HEADER *poh,
		PIMAGE_SECTION_HEADER *psh);
	static DWORD FindKiServiceTable(HMODULE hModule,DWORD dwKSDT);
	static	void GetOldSSDTAddress();

	static __int64 MyFileSeek (HANDLE hf, __int64 distance, DWORD MoveMethod);
	static BOOL WriteSector(__int64 Sect,BYTE* InBuf);
	static BOOL ReadSector(__int64 Sect,BYTE* OutBuf);
	static int WriteSectorByDrive(HANDLE hMyHandle,ULONG Sect,BYTE* InBuf);//дһ������
	static int ReadSectorByDrive(HANDLE hMyHandle,ULONG Sect,BYTE* InBuf);//��һ������
	
	static void TrimPathW(WCHAR szModPath[]);

	static void TrimPath(char szModPath[]);
	static void KillThreadByTerminateThread(DWORD dwProcessId,DWORD dwCid);
	static void KillThreadFun(PKillThreadParam PParam);
	HANDLE static  hMyHandle;

	BOOL static bdrv;
	DWORD static controlbuff[64];
	char static outputbuff[256]; 
	DWORD static dw;
	DWORD static stringcount;
	static char * strings[256]; 


	static ULONG NTBase ;			//NT ��ַ
	static ULONG TotalSSDTCount ;		//SSDT�������
	static pSSDTSaveTable pSSDTST ;	//�����SSDT�����
	static PSYSMODULELIST pList ;	//ģ����Ϣ����
	static ULONG RealCount ;		//ö�����ҵ��ķ������
    static pNtQuerySystemInformationProto pNtQuerySystemInformation ;
};

