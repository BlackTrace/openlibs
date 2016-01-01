//sigverif.h
#ifndef _SIGVERIF_
#define _SIGVERIF_

#include "common.h"


#define ENCODING (X509_ASN_ENCODING | PKCS_7_ASN_ENCODING)

//��ؽṹ��
typedef struct _SPROG_PUBLISHERINFO{
	LPCWSTR lpszProgramName;
	LPCWSTR lpszPublisherLink;
	LPCWSTR lpszMoreInfoLink;
} SPROG_PUBLISHERINFO, *PSPROG_PUBLISHERINFO;


//����ļ��Ƿ�ǩ��
//lpFileName:Ŀ���ļ�·��
//����:true:��ǩ����false:û��ǩ��
bool sigverif_check(LPCWSTR lpFileName);


//�������ǩ������
//lpFileName:Ŀ���ļ�·��
//����:����ǩ���������ƣ�����ΪNULL
LPWSTR sigverif_getCompanyName(LPCWSTR lpFileName);

#endif //_SIGVERIF_