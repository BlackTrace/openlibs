#include "common.h"


//���win32k��ַ
ULONG GetWin32kBase();

//���ݷ���Ż��ԭʼ������ַ
ULONG GetOldShadowSSDTAddress(ULONG ulIndex);

//��ʽ2
ULONG GetSSDTShadowAddress2();