#include <stddef.h>
#include "dll.h"
#include "dllext.h"
#include "HyColorText.h"
#include "HyDbgHelp.h"

int strcmpi(const char*,const char*);
int getch(void);

static SvrHnds g_shs = {0};
//NT�����ļ���ȫ·��
static char g_KernelFilePath[MAX_PATH] = {0};
//NT�����ļ�������,������׺.exe
static char g_KernelFileName[32] = {0};
//DbgHelp Module��������NT�����ļ��ļ��ػ���ַ
static DWORD64 g_KernelBaseLD = 0x0LLU;
//NT����ʵ����ϵͳ����ʱ�Ļ���ַ
static PVOID g_KernelBaseRT = NULL;
//����ʱSSDT�ĵ�ַ
static PVOID g_pSSDT = NULL;
//����ʱSSDT Shadow�ĵ�ַ
static PVOID g_pSSDTSdw = NULL;
//SSDT.Base��SSDTSdw.Base
static PVOID g_SSDTBases[2] = {NULL,NULL};
//Win32k.sys�ļ�ȫ·��
static char g_Win32kFilePath[MAX_PATH] = {0};
//Win32k.sys�Ĳ�����׺����
static char *g_pWin32kFileName = "win32k";
//Win32k DgbHelp����ʱ��ַ
static DWORD64 g_Win32kBaseLD = 0x0LLU;
//Win32kϵͳ����ʱ��ַ
static PVOID g_Win32kBaseRT = NULL;
//�����Ƿ��������׼����̨
bool g_bIsStdOut = false;

//�ȴ��û�������������������esc������true�����򷵻�false��
bool WaitAnyKey(size_t i)
{
	//������Ǳ�׼����̨������򲻵ȴ��û�����س�����
	if(!g_bIsStdOut) return false;
	if(!(i%24) && i)
	{
		HyPrintColorText("press any key to continue...[ESC to QUIT]",\
			CLR_TEXT_BRIGHT|CLR_TEXT_GREEN);
		fflush(stdin);
		//getchar���ܲ���ESC���������getch��������������F1,F2֮��ļ�ʱ
		//�Ჶ��2�Σ����顣
		char c = getch();
		//printf("[%s]%02x\n",__func__,c);
		if(c == 0x0D) puts("");
		if(c == 0x1B) return true;
	}
	return false;
}

//������ʱ�ں��еĵ�ַת��Ϊ�ں��ļ��еĶ�Ӧƫ�Ƶ�ַ��
inline PVOID GetKernelOffsetLD(PVOID AddrRT)
{
	return (PVOID)((ULONG)AddrRT - (ULONG)g_KernelBaseRT + (ULONG)g_KernelBaseLD);
}

//���ں��ļ��м��صĵ�ַת��Ϊ����ʱ���ں�ƫ�Ƶ�ַ��
inline PVOID GetKernelOffsetRT(PVOID AddrLD)
{
	return (PVOID)((ULONG)AddrLD - (ULONG)g_KernelBaseLD + (ULONG)g_KernelBaseRT);
}

static PVOID GetSysModListAddr(void)
{
	PVOID SysModListAddr = HyGetSymAddrRaw(g_KernelBaseLD,\
		"PsLoadedModuleList");
	if(!SysModListAddr)
	{
		PRINT("[%s]err : Get PsLoadedModuleList Addr Failed!\n",\
			__func__);
		goto QUIT;
	}
	//����SSDTSdwʵ������ʱ��ƫ��
	SysModListAddr = GetKernelOffsetRT(SysModListAddr);
QUIT:
	return SysModListAddr;
}

//���ں˱���ϵͳģ���б�,�鿴���������Ҫ����DebugView
static void EnumSysMod(void)
{
	PVOID SysModListAddr = GetSysModListAddr();
	if(!SysModListAddr) goto QUIT;
	PRINT("[%s]SysModListAddr is 0X%08X\n",__func__,SysModListAddr);
	HyEnumSysMod(&g_shs,SysModListAddr);
QUIT:
	return;
}

//��ӡ��MM�йص��ں˱���
static void HyPrintSysMmVal(void)
{
	HySymPrintStructElements(g_KernelBaseLD,"_MMSUPPORT");
	//PrintErr();
}

//��ӡϵͳ�ں˱�����ֵ
static void HyPrintSysVal(void)
{
	HyPrintSysMmVal();
}

//��ӡϵͳģ��
static void HyPrintSysMod(void)
{
	HyGetRTModInfo("Print*",(PVOID)100);
}

//����ϵͳ������
static const char *GetSysSegType(PGATE pGate)
{
	if(!pGate->P)
		return "---";
	
	//DT==0��ʾϵͳ��,DT==1��ʾ�洢��
	if(pGate->DT)
		return "NOT SYSSEG!";
	
	switch(pGate->Type)
	{
	case 0:case 8:case 0xA:case 0xD:
	default:return "Unknow SysSeg";
	case 1:	return "Avail 286TSS";
	case 2:	return "LDT";
	case 3:	return "Busy 286TSS";
	case 4:	return "286 Call Gate";
	case 5:	return "Task Gate";
	case 6:	return "286 Interrupt Gate";
	case 7:	return "286 Trap Gate";
	case 9:	return "Avail 386TSS";
	case 0xB:return "Busy 386TSS";
	case 0xC:return "386 Call Gate";
	case 0xE:return "386 Interrupt Gate";
	case 0xF:return "386 Trap Gate";
	}
}

//��ӡϵͳIDT
static void HyPrintIDT(void)
{
	//��RING3��ȡIDT(sidt)��VMware�л����!!!
	GATE Gates[256] = {{0}};
	HyGetIDT(&g_shs,Gates,sizeof(Gates));
	
	PSYMBOL_INFO pSI = malloc(sizeof(SYMBOL_INFO)+MAX_SYM_NAME);
	if(!pSI) goto QUIT;
	pSI->SizeOfStruct = sizeof(SYMBOL_INFO);
	pSI->MaxNameLen = MAX_SYM_NAME;
	
	puts("  NUM      OFFSET   DPL  P       TYPE             SYMBOL");
	for(size_t i = 0;i < 256;++i)
	{
		if(!HySymFromAddrRaw((DWORD64)(DWORD)GetKernelOffsetLD(\
			(PVOID)FULLOFFSET(Gates[i])),\
			pSI))
		{
			strcpy(pSI->Name,"<Unknow Symbol>");
		}
		
		printf("%04d[%02X] 0X%08X  %d   %-2s  %-18s  %-15s\n",i,\
			i,FULLOFFSET(Gates[i]),\
			Gates[i].DPL,Gates[i].P?"P":"NP",\
			GetSysSegType(&Gates[i]),pSI->Name);
		//����û�����esc�������˳���ӡ��
		if(WaitAnyKey(i)) break;
	}
QUIT:
	free(pSI);
}

//ȡ�ô洢�εĶν���
static DWORD GetSegLimit(PSEGMENT pSeg)
{
	DWORD dwRet = pSeg->G?0x1000:1;
	return dwRet * FULLLIMIT(*pSeg) + dwRet - 1;
}

//ȡ�ö�����
static const char *GetSegAttr(PSEGMENT pSeg)
{
	static char szRet[32];
	if(!pSeg->P)
		return "---";
	else if(!pSeg->DT)	//ϵͳ��
		return GetSysSegType((PGATE)pSeg);
	else
	{
		//�洢��
		bool bIsCode = false;
		strcpy(szRet,"");
		if(pSeg->Type & 1)
			strcat(szRet,"A");		//�ѷ���
		
		if(pSeg->Type & 1<<3)	
		{
			strcat(szRet," CS");	//�����
			bIsCode = true;
		}
		else
			strcat(szRet," DS");	//���ݶ�
		
		if(pSeg->Type & 1<<1 && bIsCode)	//�Ǵ������1λ��λ��ʾ�ɶ������
			strcat(szRet," R");
		else if(pSeg->Type & 1<<1 && !bIsCode)
			strcat(szRet," W");				//�����ݶ���1λ��λ��ʾ��д���ݶ�
		
		if(pSeg->Type & 1<<2 && bIsCode)
			strcat(szRet," C");		//һ�´����
		else if(pSeg->Type & 1<<2 && !bIsCode)
			strcat(szRet," H->L");	//���ݶδӸ������չ
		return szRet;
	}
}

//��ӡϵͳGDT
static void HyPrintGDT(void)
{
	//��RING3��ȡGDT(sgdt)��VMware�л����!!!
	SEGMENT Segs[128] = {{0}};
	HyGetGDT(&g_shs,Segs,sizeof(Segs));
	
	puts("  NUM       BASE       LIMIT   DPL P  BEAD TYPE    ATTRIBUTE");
	for(size_t i = 0;i < 128;++i)
	{
		printf("%04d[%02X] 0X%08X 0X%08X  %d  %s  %s  TYPE:%X  %s\n",i,\
			i,FULLBASE(Segs[i]),\
			GetSegLimit(&Segs[i]),Segs[i].DPL,Segs[i].P?"P":"NP",\
			Segs[i].G?"4KB":"1B",Segs[i].Type,\
			GetSegAttr(&Segs[i]));
		//����û�����esc�������˳���ӡ��
		if(WaitAnyKey(i)) break;
	}
}

//����ɫ��ӡSSDT��ÿһ����Ŀ��
static void PrintSSDTItem(size_t id,PVOID OrgAddr,PVOID RealAddr,\
	const char *SvrName)
{
	char buf[256];
	
	if(!SvrName) return;
	printf("[%04d] 0x%08X ",id,OrgAddr);
	if(OrgAddr != RealAddr)
	{
		//��ԭʼ�����ַ��ʵ�ʷ����ַ��һ�£���˵����HOOKED������
		//������ɫ��Ŀ��ʾ�÷�����Ŀ��
		snprintf(buf,sizeof(buf),"*0x%08X  %s\n",(UINT)RealAddr,SvrName);
		//������Ǳ�׼����� HyPrintColorText ����ʵ��������ļ����컻��
		//printf���!
		if(g_bIsStdOut)
			HyPrintColorText(buf,CLR_TEXT_RED|CLR_TEXT_BRIGHT);
		else
			printf(buf);
	}
	else
		printf("0x%08X  %s\n",RealAddr,SvrName);
}

//����PE�ļ���ָ���ε��ļ�ƫ�Ƶ�ַ��
//��ΪWin32k��֧�ֵ�SSDTSdw����Win32k.sys�ļ��е�ƫ�Ƶ�ַ
//���������ݶε��ļ�ƫ��0����(2k,xp,2k3,win7)
//ע����ĳЩ�汾��xp(sp2)�������⣬���ų�������������ŵ�ԭ��
static PVOID GetPESectionOffset(FILE *pf,const char *SectionName)
{	
	PVOID RetDO = NULL;
	size_t SectionCount = 0;
	PVOID pNTHeader;
	IMAGE_NT_HEADERS ImgNTHeaders;
	
	if(!SectionName) return RetDO;
	//��λIMAGE_NT_HEADERS�ṹ��λ����Ϣ
	fseek(pf,offsetof(IMAGE_DOS_HEADER,e_lfanew),SEEK_SET);
	//��ȡIMAGE_NT_HEADERS��λ��
	fread(&pNTHeader,sizeof(long),1,pf);
	//���ļ�ָ�붨λ��IMAGE_NT_HEADERS��λ��
	fseek(pf,(long)pNTHeader,SEEK_SET);
	//��ȡIMAGE_NT_HEADERS�ṹ
	fread(&ImgNTHeaders,sizeof(IMAGE_NT_HEADERS),1,pf);
	//ȡ��SECTION��Ŀ
	SectionCount = ImgNTHeaders.FileHeader.NumberOfSections;
	PRINT("[%s]msg : SectionCount is %d\n",__func__,SectionCount);
	
	//����Ҫ��ʽfseek��SECTION TABLE��ǰ��fread�Ѿ��ƶ�����ȷ��ƫ���ˡ�
	//fseek(pf,(long)sizeof(IMAGE_NT_HEADERS),SEEK_CUR);
	IMAGE_SECTION_HEADER ImgSecHeaders[SectionCount];
	
	//��ȡSECTION����
	fread(ImgSecHeaders,sizeof(ImgSecHeaders),1,pf);
	for(size_t i = 0;i<SectionCount;++i)
	{
		if(strstr(ImgSecHeaders[i].Name,SectionName))
		{
			RetDO = (PVOID)ImgSecHeaders[i].PointerToRawData;
			break;
		}
	}
	return RetDO;
}

//��Win32k.sys�л��ԭʼSSDTSdw���е���Ŀ��
static bool GetSSDTSdwOrg(PVOID buf,size_t size)
{
	bool bRet = false;
	if(!buf || !size) goto QUIT;
	FILE *pf = fopen(g_Win32kFilePath,"rb");
	if(!pf) goto QUIT;
	
	//��ȡWin32k.sys��data�ε��ļ�ƫ��,SSDTSdw�ͷ��ڸ�ƫ�ơ�
	PVOID SSDTSdwFileOffset = GetPESectionOffset(pf,".data");
	if(!SSDTSdwFileOffset) goto QUIT;
	
	fseek(pf,(long)SSDTSdwFileOffset,SEEK_SET);
	fread(buf,size,1,pf);
	bRet = true;
QUIT:
	if(pf) fclose(pf);
	return bRet;
}

//���ں��ļ��л��ԭʼSSDT���е���Ŀ��
static bool GetSSDTOrg(PVOID buf,size_t size)
{
	bool bRet = false;
	if(!buf || !size) goto QUIT;
	HANDLE hKernel = LoadLibrary(g_KernelFilePath);
	if(!hKernel) goto QUIT;
	
	//�ں�����ʱSSDT��ԭʼ���ݷ����ں��ļ��У�����ȡ�������ļ�ƫ�ơ�
	PVOID SSDTFileOffset = (PVOID)((ULONG)hKernel + (ULONG)g_SSDTBases[0] - \
		(ULONG)g_KernelBaseRT);
	
	memcpy(buf,SSDTFileOffset,size);
	bRet = true;
QUIT:
	if(hKernel) FreeLibrary(hKernel);
	return bRet;
}

//�ڿ���̨��ӡSSDT
static void HyPrintSSDT(bool bIsFixedOnly)
{
	static PVOID SSDT[512] = {0};
	static PVOID SSDTOrg[512] = {0};
	
	if(!HyGetSSDT(&g_shs,SSDT,sizeof(SSDT)))
	{
		PRINT("[%s]err : HyGetSSDT Failed!\n",__func__);
		goto QUIT;
	}
	
	if(!GetSSDTOrg(SSDTOrg,sizeof(SSDTOrg)))
		PRINT("[%s]err : GetSSDTOrg Failed!\n",__func__);
	
	PSYMBOL_INFO pSI = malloc(sizeof(SYMBOL_INFO)+MAX_SYM_NAME);
	if(!pSI) goto QUIT;
	pSI->SizeOfStruct = sizeof(SYMBOL_INFO);
	pSI->MaxNameLen = MAX_SYM_NAME;
	
	printf("  ID    ORG_ADDR  REAL_ADDR  SYMOL_NAME\n");   
	for(size_t i = 0;SSDT[i];++i)
	{
		if(!HySymFromAddrRaw((DWORD64)(DWORD)GetKernelOffsetLD(SSDT[i]),\
			pSI))
		{
			//���SSDT�ѱ�HOOKED,����SSDTOrg����ԭʼ����
			if(!HySymFromAddrRaw((DWORD64)(DWORD)SSDTOrg[i],pSI))
				strcpy(pSI->Name,"<Unknow Symbol>");
		}
		
		DWORD OrgAddr = (ULONG)SSDTOrg[i] - (ULONG)g_KernelBaseLD + \
			(ULONG)g_KernelBaseRT;
		if(!bIsFixedOnly || (PVOID)OrgAddr != SSDT[i])
		{
			PrintSSDTItem(i,(PVOID)OrgAddr,SSDT[i],pSI->Name);
			//����û�����esc�������˳���ӡ��
			if(WaitAnyKey(i)) break;
		}
	}	
QUIT:
	free(pSI);
	return;
}

//������ʱWin32k�еĵ�ַת��ΪWin32k.sys�ļ��еĶ�Ӧƫ�Ƶ�ַ��
inline PVOID GetWin32kOffsetLD(PVOID AddrRT)
{
	return (PVOID)((ULONG)AddrRT - (ULONG)g_Win32kBaseRT + (ULONG)g_Win32kBaseLD);
}

//�ڿ���̨��ӡSSDTSdw
static void HyPrintSSDTSdw(bool bIsFixedOnly)
{
	static PVOID SSDTSdw[1024] = {0};
	static PVOID SSDTSdwOrg[1024] = {0};
	if(!g_pSSDTSdw) goto QUIT;
	if(!HyGetSSDTSdw(&g_shs,g_pSSDTSdw,SSDTSdw,\
		sizeof(SSDTSdw)))
	{
		PRINT("[%s]err : HyGetSSDTSdw Failed!\n",__func__);
		goto QUIT;
	}
		
	if(!GetSSDTSdwOrg(SSDTSdwOrg,sizeof(SSDTSdwOrg)))
		PRINT("[%s]err : GetSSDTSdwOrg Failed!\n",__func__);
		
	PSYMBOL_INFO pSI = malloc(sizeof(SYMBOL_INFO)+MAX_SYM_NAME);
	if(!pSI) goto QUIT;
	pSI->SizeOfStruct = sizeof(SYMBOL_INFO);
	pSI->MaxNameLen = MAX_SYM_NAME;
	
	printf("  ID    ORG_ADDR  REAL_ADDR  SYMOL_NAME\n");
	for(size_t i = 0;SSDTSdw[i];++i)
	{
		if(!HySymFromAddrRaw((DWORD64)(DWORD)GetWin32kOffsetLD(SSDTSdw[i]),\
			pSI))
		{
			//���SSDTSdw�ѱ�HOOKED,����SSDTSdwOrg����ԭʼ����
			if(!HySymFromAddrRaw((DWORD64)(DWORD)SSDTSdwOrg[i],pSI))
				strcpy(pSI->Name,"<Unknow Symbol>");
		}
		
		DWORD OrgAddr = (ULONG)SSDTSdwOrg[i] - (ULONG)g_Win32kBaseLD + \
			(ULONG)g_Win32kBaseRT;
		if(!bIsFixedOnly || (PVOID)OrgAddr != SSDTSdw[i])
		{
			PrintSSDTItem(i,(PVOID)OrgAddr,SSDTSdw[i],pSI->Name);
			//����û�����esc�������˳���ӡ��
			if(WaitAnyKey(i)) break;
		}
	}
QUIT:
	free(pSI);
	return;
}

//ȡ���ں��ļ��Լ�Win32k�ļ�·����Ϣ
static void GetSysFilePath(void)
{
	char SysPath[MAX_PATH];
	GetSystemDirectory(SysPath,sizeof(SysPath));
	//����ں˿���PAE���ں��ļ���Ϊntkrnlpa,����Ϊntoskrnl
	if(HyIsOpenPAE())
		strcpy(g_KernelFileName,"ntkrnlpa");
	else
		strcpy(g_KernelFileName,"ntoskrnl");
	
	//�����ں��ļ�·��
	strcpy(g_KernelFilePath,SysPath);
	strcat(g_KernelFilePath,"\\");
	strcat(g_KernelFilePath,g_KernelFileName);
	strcat(g_KernelFilePath,".exe");
	PRINT("%s\n",g_KernelFilePath);
	
	//����Win32k�ļ�·��
	strcpy(g_Win32kFilePath,SysPath);
	strcat(g_Win32kFilePath,"\\");
	strcat(g_Win32kFilePath,g_pWin32kFileName);
	strcat(g_Win32kFilePath,".sys");
	PRINT("%s\n",g_Win32kFilePath);
}

//��ʼ���ں���Ϣ,��Ҫ�������ں��ļ���PDB��Ϣ
static bool InitKernelInfo(void)
{
	bool bRet = false;
	//�����ں��ļ���PDB
	if(!HyLoadModulePdb(g_KernelFilePath))
	{
		PRINT("[%s]err : Load NT Kernel Pdb Failed!\n",__func__);
		//goto QUIT;
	}
	//�����ں��ļ�
	g_KernelBaseLD = HyLoadModule(g_KernelFilePath);
	if(!g_KernelBaseLD)
	{
		PRINT("[%s]err : Load NT Kernel File Failed!\n",__func__);
		goto QUIT;
	}
	PRINT("[%s]msg : NT Kernel LD Base : %I64x\n",__func__,g_KernelBaseLD);
	//��ӡ�ں��ļ���Ϣ
	HyPrintModuleInfo(g_KernelBaseLD);
	PrintErr();
	//ȡ��ʵ���ں��ļ�
	if(!HyStoreModFile(g_KernelFilePath))
		PRINT("[%s]err : HyStoreModFile Failed!(%d)\n",__func__,\
			GetLastError());
	//ȡ������ʱ�ں˻�ַ
	g_KernelBaseRT = HyGetRTModBase(g_KernelFileName);
	if(!g_KernelBaseRT)
	{
		PRINT("[%s]err : Get NT Kernel RT Base Failed!\n",__func__);
		goto QUIT;
	}
	PRINT("[%s]msg : NT Kernel RT Base : %p\n",__func__,g_KernelBaseRT);
	bRet = true;
QUIT:
	return bRet;
}

//��ʼ��Win32k��Ϣ,��Ҫ������Win32k.sys�ļ���PDB��Ϣ
static bool InitWin32kInfo(void)
{
	bool bRet = false;
	//����Win32k.sys��PDB
	if(!HyLoadModulePdb(g_Win32kFilePath))
	{
		PRINT("[%s]err : Load NT Win32k Pdb Failed!\n",__func__);
		//goto QUIT;
	}
	//����Win32k.sys
	g_Win32kBaseLD = HyLoadModule(g_Win32kFilePath);
	if(!g_Win32kBaseLD)
	{
		PRINT("[%s]err : Load NT Win32k	File Failed!\n",__func__);
		goto QUIT;
	}
	PRINT("[%s]msg : NT Win32k LD Base : %I64x\n",__func__,g_Win32kBaseLD);
	//��ӡWin32k.sys�ļ���Ϣ
	HyPrintModuleInfo(g_Win32kBaseLD);
	PrintErr();
	//ȡ��ʵ��Win32k.sys�ļ�
	if(!HyStoreModFile(g_Win32kFilePath))
		PRINT("[%s]err : HyStoreModFile Failed!(%d)\n",__func__,\
			GetLastError());
	//ȡ������ʱWin32k.sys��ַ
	g_Win32kBaseRT = HyGetRTModBase(g_pWin32kFileName);
	if(!g_Win32kBaseRT)
	{
		PRINT("[%s]err : Get NT Win32k RT Base Failed!\n",__func__);
		goto QUIT;
	}
	PRINT("[%s]msg : NT Win32k RT Base : %p\n",__func__,g_Win32kBaseRT);
	bRet = true;
QUIT:
	return bRet;
}

//��Ring3��ȡ��SSDTSdw�ĵ�ַ,����ʧ�ܷ���NULL��
static PVOID GetSSDTSdwAddr(void)
{
	//ȡ��SSDTSdw��Rawƫ��
	PVOID pSSDTSdw = HyGetSymAddrRaw(g_KernelBaseLD,\
		"KeServiceDescriptorTableShadow");
	if(!pSSDTSdw)
	{
		PRINT("[%s]err : Get KeServiceDescriptorTableShadow Addr Failed!\n",\
			__func__);
		goto QUIT;
	}
	//����SSDTSdwʵ������ʱ��ƫ��
	pSSDTSdw = (PVOID)((ULONG)pSSDTSdw - (ULONG)g_KernelBaseLD + \
		(ULONG)g_KernelBaseRT);
QUIT:
	return pSSDTSdw;
}

//��Ring3��ȡ��SSDT�ĵ�ַ��ʧ�ܷ���NULL��
static PVOID GetSSDTAddrAtRing3(void)
{
	PVOID RetAddr = NULL;
	HANDLE hKernel = LoadLibrary(g_KernelFilePath);
	if(!hKernel)
	{
		PRINT("[%s]LoadLibrary Failed!\n",__func__);
		goto QUIT;
	}
	//ȡ��SSDT���ں��ļ��е�ƫ��
	RetAddr = GetProcAddress(hKernel,"KeServiceDescriptorTable");
	if(!RetAddr)
	{
		PRINT("[%s]Get KeServiceDescriptorTable Addr Failed!\n",__func__);
		goto QUIT;
	}
	//����SSDT������ʱ��ƫ��
	RetAddr = (PVOID)((ULONG)RetAddr - (ULONG)hKernel + (ULONG)g_KernelBaseRT);
QUIT:
	if(hKernel) FreeLibrary(hKernel);
	return RetAddr;
}

//ȡ������ʱSSDT.Base��SSDTSdw.Base
static bool GetSSDTBases(void)
{
	if(!HyGetSSDTBases(&g_shs,g_SSDTBases,sizeof(g_SSDTBases)))
	{
		PRINT("[%s]err : HyGetSSDTBases Failed!\n",__func__);
		return false;
	}
	PRINT("[%s]msg SSDT.Base : %p , SSDTSdw.Base : %p\n",\
		__func__,g_SSDTBases[0],g_SSDTBases[1]);
	return true;
}

//EXE�ĳ�ʼ����
static bool init(void)
{
	bool bRet = false;
	//��������
	if(!NewSvr(&g_shs))
	{
		PRINT("[%s]err : NewSvr Failed!\n",__func__);
		goto QUIT;
	}
	//��������
	if(!StartSvr(&g_shs))
	{
		PRINT("[%s]err : StartSvr Failed!\n",__func__);
		goto QUIT;
	}
	//ˢ���ں�ģ����Ϣ
	HyFlushSysModsInfo();
	//ȡ���ں˺�Win32k�ļ�·��
	GetSysFilePath();
	//��ʼ��DbgHelp������Դ
	if(!HyInitDbgHelp(NULL))
	{
		PRINT("[%s]err : InitDbgHelp Failed!\n",__func__);
		goto QUIT;
	}
	//��ʼ���ں���Ϣ
	if(!InitKernelInfo()) goto QUIT;
	//��ʼ��Win32k��Ϣ
	if(!InitWin32kInfo()) goto QUIT;
	
	//��Ring3���SSDT��RT��ַ
	g_pSSDT = GetSSDTAddrAtRing3();
	if(!g_pSSDT) goto QUIT;
	PRINT("[%s]msg : SSDT Get At Ring3 is : %p\n",__func__,g_pSSDT);
	
	//��Ring3���SSDT Shadow���ַ
	g_pSSDTSdw = GetSSDTSdwAddr();
	if(!g_pSSDTSdw) goto QUIT;
	PRINT("[%s]msg : SSDTSdw Get At Ring3 is : %p\n",__func__,g_pSSDTSdw);
	BYTE Dummy[1331];
	//ֻ����sysext��SSDTSdw�ĵ�ַ��
	HyGetSSDTSdw(&g_shs,g_pSSDTSdw,&Dummy,sizeof(Dummy));
	PrintErr();
	
	if(!GetSSDTBases())
	{
		PRINT("[%s]GetSSDTBases Failed!\n",__func__);goto QUIT;
		goto QUIT;
	}
	bRet = true;
QUIT:
	return bRet;
}

//EXE���������
static void clear(void)
{
	//���ϵͳģ����Ϣ
	HyClearSysModsInfo();
	//����б�Ҫ���ֱ�ж���ں��ļ���Win32k.sys�ļ�
	if(g_KernelBaseLD)
	{
		if(!HyUnloadModule(g_KernelBaseLD))
			PRINT("[%s]err : Unload Kernel Failed!\n",__func__);
	}
	else g_KernelBaseLD = 0x0LLU;
	
	if(g_Win32kBaseLD)
	{
		if(!HyUnloadModule(g_Win32kBaseLD))
			PRINT("[%s]err : Unload Win32k Failed!\n",__func__);
	}
	else g_Win32kBaseLD = 0x0LLU;
	//���DbgHelp������Դ
	HyClearDbgHelp();
	//ж�ط���
	if(!DelSvr(&g_shs))
	{
		PRINT("[%s]err : DelSvr Failed!\n",__func__);
		PRINT("[%s]Try To Force Del The Drv!\n",__func__);
		DelSvrForce();
	}
}

//��ӡʹ�ð���
static void usage(void)
{
	HyPrintColorText("NtInfoGuy! Code By ����è����|panda hopy 2010 v1.0\n",\
		CLR_TEXT_YELLOW|CLR_TEXT_BRIGHT);
	HyPrintColorText("��è��blog : http://blog.csdn.net/mydo\n",\
		CLR_TEXT_RED|CLR_TEXT_BRIGHT);
	puts("\nNtInfoGuy SSDT|SSDTSDW|GDT|IDT|SYSMOD[K]|SYSVAL [fixed]");
	puts("SSDT \t: ��ӡϵͳ�����");
	puts("SSDTSDW : ��ӡϵͳ����Shadow��");
	puts("GDT \t: ��ӡȫ����������");
	puts("IDT \t: ��ӡ�ж���������");
	puts("SYSMOD \t: ��ӡϵͳ����ģ����Ϣ");
	puts("SYSMODK : ��ӡϵͳ����ģ����Ϣ(��RING0ȡ��,��DbgView����ʾ)");
	puts("SYSVAL \t: ��ӡϵͳ�ں˱�����ֵ");
	puts("fixed \t: ֻ��ӡ���Ĺ�������");
}

int main(int argc,char *argv[])
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	//�������Ƿ����������׼����̨���.
	//��ֹ�û���ʹ������ NtInfoGuy ssdt>a.txt �ض�λ�������
	//WaitAnyKey��ȴ��û����롣
	g_bIsStdOut = (int)hOut == CONSOLE_STDOUT ;	//7:��׼����̨���
	
	if(!init())
	{
		printf("[%s]err : init failed!\n",__func__);
		goto QUIT;
	}
	
	if(argc > 3 || argc == 1)
	{
		usage();goto QUIT;
	}
	
	if(!strcmpi(argv[1],"SSDT"))
		HyPrintSSDT(argc==3&&!strcmpi(argv[2],"FIXED"));
	else if(!strcmpi(argv[1],"SSDTSDW"))
		HyPrintSSDTSdw(argc==3&&!strcmpi(argv[2],"FIXED"));
	else if(!strcmpi(argv[1],"GDT"))
		HyPrintGDT();
	else if(!strcmpi(argv[1],"IDT"))
		HyPrintIDT();
	else if(!strcmpi(argv[1],"SYSMOD"))
		HyPrintSysMod();
	else if(!strcmpi(argv[1],"SYSMODK"))
		EnumSysMod();
	else if(!strcmpi(argv[1],"SYSVAL"))
		HyPrintSysVal();
	else
		usage();
QUIT:
	clear();
	//getchar();
	return 0;
}
