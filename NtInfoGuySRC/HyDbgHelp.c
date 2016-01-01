#include "HyDbgHelp.h"

bool WaitAnyKey(size_t i);

const static char *g_pDefaultMsSymUrl = "http://msdl.microsoft.com/download/symbols";
//ȫ��ϵͳģ����Ϣ��ָ��
static PVOID gp_pModulesInfo = NULL;	//gp_ : ȫ��˽�б���

//ˢ��ϵͳģ����Ϣ������ģ����Ϣ������������0��ʾ����ʧ�ܡ�
size_t HyFlushSysModsInfo(void)
{
	size_t dwRet = 0;
	NTSTATUS status;
	static size_t InitSize = 0x4000;

	if(gp_pModulesInfo)
	{
		free(gp_pModulesInfo);
		gp_pModulesInfo = NULL;
	}

	while(true)
	{
		gp_pModulesInfo = malloc(InitSize);
		if(!gp_pModulesInfo) goto QUIT;
		if(STATUS_INFO_LENGTH_MISMATCH == \
			(status = NtQuerySystemInformation(SystemModuleInformation,\
			gp_pModulesInfo,InitSize,NULL)))
		{
			free(gp_pModulesInfo);
			InitSize <<= 1;
			continue;
		}
		if(!NT_SUCCESS(status))
		{
			PRINT("[%s]err : Query Modules Info Failed!\n",__func__);
		}
		else
		{
			dwRet = *(size_t*)gp_pModulesInfo;
			PRINT("[%s]InitSize : %08x , Actual Size : %08x\n",\
				__func__,InitSize,sizeof(SYSTEM_MODULE_INFORMATION) * dwRet);
		}
		break;
	}
QUIT:
	return dwRet;
}

//��ȡ�ļ��Ķ�·����
static const char *GetShortPath(const char *Path)
{
	const char *pSplit = strrchr(Path,'\\');
	if(pSplit) return pSplit + 1;
	return Path;
}

//ȡ������ʱģ����Ϣ���������pModNameΪ"Print*",���ʾ�ڿ���̨��ӡ����ϵͳ
//ģ�����Ϣ��
bool HyGetRTModInfo(const char *pModName,OUT PSYSTEM_MODULE_INFORMATION pModInfo)
{
	bool bRet = false;
	size_t count;
	PSYSTEM_MODULE_INFORMATION l_pModInfo;	//l_ local�ֲ�����

	if(!pModName || !gp_pModulesInfo) goto QUIT;

	count = *(size_t*)gp_pModulesInfo;
	l_pModInfo = (PSYSTEM_MODULE_INFORMATION)((PULONG)gp_pModulesInfo + 1);

	if(!strcmp(pModName,"Print*"))
	{
		ULONG Limit;
		const char *pstr = NULL;
		
		//ö������ϵͳģ��,���Ǻ�������false��
		for(size_t i = 0;i < count;++i)
		{
			switch(HyCheckWinTrustFile(l_pModInfo[i].ImageName))
			{
			case 0:
				pstr = "Not Trust Module";break;
			case 1:
				pstr = "Trust Module";break;
			case -1:
				pstr = "File Not Found!";break;
			}
			
			Limit = (ULONG)l_pModInfo[i].Base + l_pModInfo[i].Size;
			printf("[%03d]%-20s @%08X-%08X flags:%08x %s\n",i+1,\
				GetShortPath(l_pModInfo[i].ImageName),\
				l_pModInfo[i].Base,Limit,l_pModInfo[i].Flags,pstr);
			if(WaitAnyKey(i)) break;
		}
	}
	else
	{
		//ȡ��ָ��ϵͳģ����Ϣ
		if(!pModInfo) goto QUIT;
		for(size_t i = 0;i < count;++i)
		{
			if(strstr(l_pModInfo[i].ImageName,pModName))
			{
				*pModInfo = l_pModInfo[i];
				bRet = true;
				break;
			}
		}
	}
QUIT:
	return bRet;
}

//ȡ������ʱģ��Ļ�ַ
PVOID HyGetRTModBase(const char *pModName)
{
	SYSTEM_MODULE_INFORMATION ModInfo;
	if(!HyGetRTModInfo(pModName,&ModInfo)) return NULL;
	//printf("[%s]%s base is %08x\n",__func__,pModName,ModInfo.Base);
	return ModInfo.Base;
}

//���ϵͳ������PAE�򷵻�true�����򷵻�false
bool HyIsOpenPAE(void)
{
	return (bool)HyGetRTModBase("ntkrnlpa");
}

//���ϵͳģ����Ϣռ�õ���Դ
void HyClearSysModsInfo(void)
{
	free(gp_pModulesInfo);
	gp_pModulesInfo = NULL;
}

//��ָ��·���½���symsrv.yse�ļ�
static void CreateSymYesFile(const char *Path)
{
	FILE *pf = NULL;
	char FullName[MAX_PATH];

	if(!Path) return;
	strcpy(FullName,Path);
	strcat(FullName,"\\symsrv.yes");

	pf = fopen(FullName,"r");
	if(!pf) pf = fopen(FullName,"w+");
	fclose(pf);
}

//��ʼ��DbgHelp�����������Դ.����ΪPdb�����ļ���ŵ�·��,�������NULL,��
//��ʾ��Ӧ�ó���ĵ�ǰ·����syms��Ŀ¼�д��Pdb�����ļ���
bool HyInitDbgHelp(const char *pSymPath)
{
	char pCurPath[MAX_PATH];
	char l_pSymPath[MAX_PATH*2];
	DWORD Options = SymGetOptions();
	Options |= SYMOPT_DEBUG;
	SymSetOptions(Options);

	if(!pSymPath)
	{
		//���pSymPathΪNULL,��ʹ��Ӧ�ó���ĵ�ǰĿ¼
		GetCurrentDirectory(sizeof(pCurPath),(LPTSTR)pCurPath);
		//����symsrv.yes�ļ������ⵯ��΢���symbol��ɶԻ���
		CreateSymYesFile(pCurPath);
		strcpy(l_pSymPath,"SRV*");
		strcat(l_pSymPath,pCurPath);
		strcat(l_pSymPath,"\\syms*");
		strcat(l_pSymPath,g_pDefaultMsSymUrl);
		pSymPath = l_pSymPath;
	}
	PRINT("%s\n",pSymPath);
	return SymInitialize(GetCurrentProcess(),(LPSTR)pSymPath,false);
}

//���DbgHelp�������Դ
bool HyClearDbgHelp(void)
{
	return SymCleanup(GetCurrentProcess());
}

//����ָ��ģ���Pdb�ļ�.���б�Ҫ���΢����ŷ���������,��ʱsymsrv.dll�ļ�����
//�ܱ�����̬����.
bool HyLoadModulePdb(const char *pModName)
{
	char PdbPath[MAX_PATH];
	if(!pModName) return false;

	return SymGetSymbolFile(GetCurrentProcess(),NULL,pModName,sfPdb,PdbPath,\
		sizeof(PdbPath),PdbPath,sizeof(PdbPath));
}

//�ڿ���̨��ӡָ��DllBase��dll��Ϣ��
void HyPrintModuleInfo(DWORD64 DllBase)
{
	char *pSymType;
	IMAGEHLP_MODULE64 ModInfo64 = {0};
	ModInfo64.SizeOfStruct = sizeof(ModInfo64); 
	
	//��ĳЩXP SP3ϵͳ�£�SymGetModuleInfo64��Ȼ����false,ͬʱ�������
	//Ϊ ERROR_INVALID_PARAMETER ʱ������Ȼ����ȷ���ModInfo64�ṹ��
	if(SymGetModuleInfo64(GetCurrentProcess(),DllBase,&ModInfo64) || \
		GetLastError()==ERROR_INVALID_PARAMETER)
	{
		switch(ModInfo64.SymType)
		{
		case SymCoff:
			pSymType = "COFF Syms";break;
		case SymCv:
			pSymType = "CodeView Syms";break;
		case SymDeferred:
			pSymType = "Deferred Syms";break;
		case SymDia:
			pSymType = "DIA Syms";break;
		case SymExport:
			pSymType = "Export Table Syms";break;
		case SymNone:
			pSymType = "No Syms";break;
		case SymPdb:
			pSymType = "PDB Syms";break;
		case SymSym:
			pSymType = ".sym File Syms";break;
		case SymVirtual:
			pSymType = "Virtual Syms";break;
		default:
			pSymType = "Unknow Syms";break;
		}

		PRINT("[%s]msg : \n%s@0x%I64X : ImgSize:%d , ImgStamp:%08x , \n" \
			"CheckSum:%08x , NumSyms:%d , SymType:%s\n",\
			__func__,ModInfo64.ModuleName,ModInfo64.BaseOfImage,ModInfo64.ImageSize,\
			ModInfo64.TimeDateStamp,ModInfo64.CheckSum,ModInfo64.NumSyms,pSymType);
	}
	else
		PRINT("[%s]err : SymGetModuleInfo64 Failed!\n",__func__);
}

//����ָ����Module���ڴ棬���ҷ��ؼ��صĻ�ַ.
DWORD64 HyLoadModule(const char *pModName)
{
	return SymLoadModule64(GetCurrentProcess(),NULL,pModName,NULL,0,0);
}

//ж��ָ��DllBase��dll��
bool HyUnloadModule(DWORD64 DllBase)
{
	return SymUnloadModule64(GetCurrentProcess(),DllBase);
}

//��ָ����ģ���ļ���ŵ�Ӧ�ó���ǰĿ¼��
bool HyStoreModFile(const char *pModName)
{
	PCSTR pPath;
	char ModPath[MAX_PATH * 2];
	char CurPath[MAX_PATH];
	GetCurrentDirectory(sizeof(CurPath),(LPTSTR)CurPath);
	strcpy(ModPath,"SRV*");
	strcat(ModPath,CurPath);
	strcat(ModPath,"*");

	pPath = SymSrvStoreFile(GetCurrentProcess(),ModPath,\
		pModName,SYMSTOREOPT_OVERWRITE);
	if(pPath) 
	{
		PRINT("[%s]msg : %s\n",__func__,pPath);
		return true;
	}
	return false;
}	

//ģ���ö�ٻص���������HyGetSymAddrRaw�ڲ��ص���
static bool CALLBACK EnumSymProc(PSYMBOL_INFO pSI,ULONG SymSize,PVOID Context)
{
	static int i = 0;
	char *pSymName = (char *)((PVOID*)Context)[0];
	if(!strcmp(pSymName,"Print*"))
	{
		printf("[%04d] %s@%I64X\n",++i,pSI->Name,pSI->Address);
	}
	else if(!strcmp(pSymName,pSI->Name))
	{
		((PVOID*)Context)[1] = (PVOID)(DWORD)pSI->Address;
		return false;
	}
	return true;
}

//����ָ�����ŵ�ƫ�Ƶ�ַ���õ�ַ�������ModuleĬ��Base��ƫ�ƣ����ܲ���
//�����ڴ��е�ʵ��ƫ�ƣ���Ҫ�ֶ�����ʵ��ƫ�ơ�
//���pSymNameΪ"Print*",���ʾ�ڿ���̨��ӡ���и�ģ����ŵ���Ϣ��
PVOID HyGetSymAddrRaw(DWORD64 DllBase,const char *pSymName)
{
	PVOID Context[2] = {(PVOID)pSymName,NULL};
	SymEnumSymbols(GetCurrentProcess(),DllBase,0,EnumSymProc,(PVOID)Context);
	return Context[1];
}

//ȡ��ָ����ַ���ķ�����Ϣ.
//��Ҫ�ֶ����ڴ��е�offsetת��Ϊʵ�ʼ����е�offset Addr��
bool HySymFromAddrRaw(DWORD64 Addr,OUT PSYMBOL_INFO pSI)
{
	return SymFromAddr(GetCurrentProcess(),Addr,NULL,pSI);
}

static bool CALLBACK EnumTypeProc(PSYMBOL_INFO pSI,ULONG SymSize,PVOID Context)
{
	//static int i = 0;
	const char *TypeName = (const char *)((PVOID*)Context)[0];
	/*
	PRINT("[%d]TypeName : %s , TypeIdx : %d\n",i++,pSI->Name,\
		pSI->TypeIndex);
	*/
	if(!strcmp(pSI->Name,TypeName))
	{
		PSYMBOL_INFO pSI_Type = (PSYMBOL_INFO)((PVOID*)Context)[1];
		//����ҵ���Context[0]����ΪNULL
		((PVOID*)Context)[0] = NULL;
		//ע��:����һ�д�����ܲ��ܽ�Name��ֵ��pSI_Type
		*pSI_Type = *pSI;
		return false;
	}
	return true;
}

//ȡ��ָ��Type����Ϣ
bool HySymGetTypeInfo(DWORD64 ModBase,const char *StructName,\
	OUT PSYMBOL_INFO pSI)
{
	bool bRet = false;
	if(!StructName || !pSI) goto QUIT;
	PVOID Context[2] = {(PVOID)StructName,(PVOID)pSI};
	
	if(!SymEnumTypes(GetCurrentProcess(),ModBase,EnumTypeProc,\
		(PVOID)Context))
	{
		PRINT("[%s]SymEnumTypes Failed!\n",__func__);
		goto QUIT;
	}
	//���SymEnumTypes�ɹ�����Context[0] == NULL,���ʾ�ҵ��˶�Ӧ
	//��Type��
	if(!Context[0])
	{
		PRINT("[%s]msg : Find Type %s !Index is %d\n",__func__,\
			StructName,pSI->TypeIndex);
	}
	else
	{
		PRINT("[%s]err : Can't Find Type %s!\n",__func__,\
			StructName);
		goto QUIT;
	}
	bRet = true;
QUIT:
	return bRet;
}

//��ӡ��ָ���ṹ��Ԫ�����Ƽ�ƫ��
bool HySymPrintStructElements(DWORD64 ModBase,const char *StructName)
{
	bool bRet = false;
	PSYMBOL_INFO pSI = malloc(sizeof(SYMBOL_INFO)+MAX_SYM_NAME);
	if(!pSI) goto QUIT;
	pSI->SizeOfStruct = sizeof(SYMBOL_INFO);
	pSI->MaxNameLen = MAX_SYM_NAME;
	pSI->ModBase = ModBase;
	
	if(!HySymGetTypeInfo(ModBase,StructName,pSI))
	{
		PRINT("[%s]err : Get Struct Info Failed!\n",__func__);
		goto QUIT;
	}
	else
	{
		PRINT("[%s]msg Struct TypeIndex is %d\n",__func__,\
			pSI->TypeIndex);
	}
	
	UINT ElementCount;
	PVOID pData = &ElementCount;
	if(!SymGetTypeInfo(GetCurrentProcess(),\
		ModBase,pSI->TypeIndex,TI_GET_CHILDRENCOUNT,pData))
	{
		PRINT("[%s]err : Get Type Element Count Failed!\n",\
			__func__);
		goto QUIT;
	}
	PRINT("[%s]msg : Element Count is %d\n",__func__,\
		ElementCount);
	TI_FINDCHILDREN_PARAMS *pCP = malloc(sizeof(ULONG)*(2+ElementCount));
	if(!pCP) goto QUIT;
	
	memset(pCP,'\0',sizeof(ULONG)*(2+ElementCount));
	pCP->Count = ElementCount;
	if(!SymGetTypeInfo(GetCurrentProcess(),ModBase,\
		pSI->TypeIndex,TI_FINDCHILDREN,pCP))
	{
		PRINT("[%s]err : Get Type Element Failed!\n",__func__);
		goto QUIT;
	}
	
	WCHAR *pNameW = NULL;
	for(int i = 0;i < ElementCount;++i)
	{
		PRINT("[%02d] TypeIndex is %d\n",i,pCP->ChildId[i]);
		if(SymGetTypeInfo(GetCurrentProcess(),ModBase,\
			pCP->ChildId[i],TI_GET_SYMNAME,&pNameW))
		{
			wprintf(L"Name is %s\n",pNameW);
			LocalFree(pNameW);
		}
		else
			PrintErr();
	}
	
	bRet = true;
QUIT:
	free(pCP);
	free(pSI);
	return bRet;
}




















