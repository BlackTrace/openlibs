
#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "ntifs.h"

typedef enum WIN_VER_DETAIL {
	WINDOWS_VERSION_NONE,       //  0
	WINDOWS_VERSION_2K,
	WINDOWS_VERSION_XP,
	WINDOWS_VERSION_2K3,
	WINDOWS_VERSION_2K3_SP1_SP2,
	WINDOWS_VERSION_VISTA_2008,
	WINDOWS_VERSION_7
} WIN_VER_DETAIL;

WIN_VER_DETAIL WinVersion;

typedef NTSTATUS (NTAPI * PFN_RtlGetVersion)(OUT PRTL_OSVERSIONINFOW lpVersionInformation);

 WIN_VER_DETAIL GetWindowsVersion();
#endif