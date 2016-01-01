/*********************************************************************
* QQwry.h
*
* ˵��:����IP���ݿ� QQWry.dat�Ĳ�����
* 
* ���ִ�����Դ������,���������ʹ��,�������޸�.���ǲ���������ҵ��;
*********************************************************************/
#ifndef _QQWRY_
#define _QQWRY_

#include "..\..\..\exe\Deianeira\common\common.h"

class QQwry
{
	//
public:
	QQwry();
	QQwry(LPCTSTR lpszFilePathName);
	~QQwry();

	//
public:
	//
	BOOL OpenQQwry(LPCTSTR lpszFilePathName);
	BOOL GetIpInfo(LPSTR lpszDotIp,LPSTR lpszInfoBuf,int nMaxBufSize); //�ɵ��IP��ַ�õ���Ϣ
	BOOL GetIpInfo(ULONG uIP ,LPSTR lpszInfoBuf,int nMaxBufSize);        //�������ֽ�˳��IP��ַ�õ���Ϣ
	DWORD GetRecordCount() const { return m_dwRecordCount; };
	void CloseQQwry();

	//
protected:
	LPBYTE m_pDataBuffer;
	DWORD m_dwRecordCount;
	BOOL m_bInit;
};


#endif //_QQWRY_