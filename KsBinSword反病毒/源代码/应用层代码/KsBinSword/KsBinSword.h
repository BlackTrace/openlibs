// KsBinSword.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CKsBinSwordApp:
// �йش����ʵ�֣������ KsBinSword.cpp
//
typedef  BOOL (__stdcall *PFNUSkinInit)( PCHAR lpszUserName,PCHAR lpszRegCode,PCHAR lpszFileName );

typedef BOOL  (__stdcall * PFNUSkinExit)();
typedef BOOL  (__stdcall * PFNUSkinLoadSkin)(PCHAR lpszFileName);
class CKsBinSwordApp : public CWinApp
{
public:
	CKsBinSwordApp();

// ��д
public:	HMODULE hModuleUSkin ;

		PFNUSkinInit USkinInit;
        PFNUSkinExit USkinExit;
		PFNUSkinLoadSkin USkinLoadSkin;
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

};





extern CKsBinSwordApp theApp;