/*********************************************************************
* QQwry.h
*
* ˵��:����IP���ݿ� QQWry.dat�Ĳ�����
* 
* ���ִ�����Դ������,���������ʹ��,�������޸�.���ǲ���������ҵ��;
*********************************************************************/
#include "QQwry.h"

//first 4 bytes is IP(host byte order),last 3 bytes is offset of IP infor 
const int nIpRecordCound = 7;

LPCSTR lpszBoradCast = "�㲥��ַ";
LPCSTR lpszUnknow = "δ֪����";

//
QQwry::QQwry()
{
	m_pDataBuffer = NULL;
	m_dwRecordCount = 0;
	m_bInit = FALSE;
}

//����QQwry.dat
QQwry::QQwry(LPCTSTR lpFileName)
{
	//
	OpenQQwry(lpFileName);
}

//��������
QQwry::~QQwry()
{
	//
	CloseQQwry();
}

//
BOOL QQwry::OpenQQwry(LPCTSTR lpFileName)
{
	BOOL bReturn = FALSE;

	//
	HANDLE hSrcFile = CreateFile(lpFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	//
	if(hSrcFile)
	{
		//Get file size
		DWORD dwNumberOfBytesToRead = SetFilePointer(hSrcFile,0,NULL,FILE_END);
		//reset to begin for data reading
		SetFilePointer(hSrcFile,0,NULL,FILE_BEGIN);
		//would not be failed in win32?
		m_pDataBuffer = new BYTE[dwNumberOfBytesToRead + 1];

		if(m_pDataBuffer)
		{
			//
			DWORD dwReaded;
			//
			if(ReadFile(hSrcFile,m_pDataBuffer,dwNumberOfBytesToRead,&dwReaded,NULL))
			{
				//calculate the ip information record count
				DWORD *pIntData = (DWORD *)m_pDataBuffer;
				//Start position of record infor in file
				DWORD nBuffer_0 = *pIntData;
				//End position of record infor in file
				DWORD nBuffer_4	= *(pIntData + 1);
				nBuffer_4 -= nBuffer_0;
				//IP record count
				m_dwRecordCount = nBuffer_4 / 7;
				int nsuiv = nBuffer_4 % 7;
				//must be integral info struct,7 bytes
				if(nsuiv == 0)
				{
					m_bInit = TRUE;
					bReturn = m_bInit;
				}			
			}
		}
		
		//close IP data file
		CloseHandle(hSrcFile);
	}
	
	if(!m_bInit)
	{		
		//Verify the null buffer if we got some error
		CloseQQwry();
	}

	return bReturn;
}

//
BOOL QQwry::GetIpInfo(LPSTR lpszDotIp,LPSTR lpszInfoBuf,int nMaxBufSize)
{
	BOOL bReturn = FALSE;
	lpszInfoBuf[0] = '\0';

	if(m_bInit)
	{
		//��ַת��
		ULONG lHostByteOrderAddr = inet_addr(lpszDotIp);

		if(lHostByteOrderAddr != INADDR_NONE)
		{
			ULONG lNetByteOrderAddr = htonl(lHostByteOrderAddr);
			bReturn = GetIpInfo(lNetByteOrderAddr,lpszInfoBuf,nMaxBufSize);
		}
	}

	return bReturn;
}	

//
BOOL QQwry::GetIpInfo(ULONG uIP ,LPSTR lpszInfoBuf,int nMaxBufSize)
{
	lpszInfoBuf[0] = '\0';

	//BoradCast IP
	if(uIP == (ULONG)-1)
	{
		lstrcpyA(lpszInfoBuf,lpszBoradCast);
		return TRUE;
	}

	LPSTR lpszPosition = lpszInfoBuf;
	DWORD *pIntData = (DWORD *)m_pDataBuffer;
	DWORD nBuffer_0 = *pIntData;
	BYTE *pesi = m_pDataBuffer + nBuffer_0;
	BYTE *pData = pesi + m_dwRecordCount * 7 + 4;
	pIntData = (DWORD *)pData;
	DWORD nedx = *pIntData;
	nedx &= 0xFFFFFF;
	
	pIntData = (DWORD *)(m_pDataBuffer + nedx);
	if(uIP <= *pIntData)
	{
		DWORD nLowIdx = 0;
		DWORD nUpIdx;
		DWORD nTotal = m_dwRecordCount;
		//ʹ�ö��ַ�����
		while(TRUE)
		{
			nUpIdx = nTotal - 1;
			if(nLowIdx >= nUpIdx)
				break;

			nUpIdx = nTotal + nLowIdx;
			nUpIdx >>= 1;
			pIntData = (DWORD *)(pesi + nUpIdx * 7);
			if(uIP >= *pIntData)
				nLowIdx = nUpIdx;
			else
				nTotal = nUpIdx;
		}

		DWORD dwEdx = nLowIdx * 7;
		pData = pesi + dwEdx + 4;
		pIntData = (DWORD *)pData;
		//pedx---esi
		nedx = *pIntData;
		nedx &= 0xFFFFFF;
		pIntData = (DWORD *)(m_pDataBuffer + nedx);
		char sign;
		int cccc;

		if(uIP <= *pIntData)
		{
			pData = m_pDataBuffer + nedx + 4;
			sign = *pData;

			if(sign != 0x01)
			{
				//
				nedx += 4;
				//eax
				DWORD dwdddd;
				if(sign == 0x02)
				{
					pData = m_pDataBuffer + nedx + 1;
					pIntData = (DWORD *)pData;
					dwdddd = *pIntData;
					dwdddd &= 0xFFFFFF;
					cccc = 3;
					dwEdx = cccc;
				}
				else
				{
					dwdddd = nedx;
					cccc = 0;
					if(sign	== 0x00)
					{
						dwEdx = cccc;
					}
					else
					{
						BYTE *pEdi;
						BYTE *psss;
						do
						{
							cccc++;
							dwEdx = cccc;
							DWORD off = cccc;
							off &= 0xFF;
							pEdi = m_pDataBuffer + off;
							psss = pEdi + nedx;
						}
						while(*psss != 0);
					}
				}

				pData = m_pDataBuffer + dwdddd;
				while(*pData != 0)
				{
					*lpszPosition++ = *pData++;
				}
				dwEdx &= 0xFF;
				dwdddd = nedx + dwEdx + 1;
				pData = m_pDataBuffer + dwdddd;

				if(*pData == 0x02)
				{
					pData = m_pDataBuffer + dwdddd + 1;
					pIntData = (DWORD *)pData;
					dwdddd = *pIntData;
					dwdddd &= 0xFFFFFF;
				}

				pData = m_pDataBuffer + dwdddd;
				while(*pData != 0)
				{
					*lpszPosition++ = *pData++;
				}
				*lpszPosition = 0;
				
				return TRUE;
			}
			else//sign==1
			{
				//nedx---esi
				pData = m_pDataBuffer + nedx + 5;
				pIntData = (DWORD *)pData;
				DWORD dwData = *pIntData;
				dwData &= 0xFFFFFF;
				pData = m_pDataBuffer + dwData;
				char dl = *pData;
				pData = m_pDataBuffer + dwData + 1;
				pIntData = (DWORD *)pData;
				DWORD dwEdi = *pIntData;
				dwEdi &= 0xFFFFFF;
				cccc = 3;
				DWORD dwEsi = cccc;

				if(dl != 2)
				{
					cccc = 0;
					dwEdi = dwData;
					if(dl == 0)
						dwEsi = cccc;
					else
					{
						BYTE *pEdi;
						BYTE *psss;
						do
						{
							cccc++;
							dwEsi = cccc;
							DWORD off = cccc;
							off &= 0xFF;
							pEdi = m_pDataBuffer + off;
							psss = pEdi + nedx;
						}while(*psss != 0);
					}
				}

				pData = m_pDataBuffer + dwEdi;
				//����IP��ַ��Ϣ��dword_10009B0C
				while(*pData != 0)
				{
					*lpszPosition++ = *pData++;
				}

				//��������λ��
				dwData += cccc + 1;
				pData = m_pDataBuffer + dwData;

				//�����2,�����ƫ��һ���ֽھ��Ǹ�����Ϣ���ļ�ƫ��λ��
				if(*pData == 0x02)
				{
					pData = m_pDataBuffer + dwData + 1;
					pIntData = (DWORD *)pData;
					dwData = *pIntData;
					dwData &= 0xFFFFFF;//edi
					pData = m_pDataBuffer + dwData;
				}

				//����IP��ַ������Ϣ��dword_10009B10
				while(*pData != 0)
				{
					*lpszPosition++ = *pData++;
				}
				*lpszPosition = 0;

				return TRUE;
			}
		}
	}
	
	//unknow IP
	lstrcpyA(lpszInfoBuf,lpszUnknow);
	return FALSE;
}

//
void QQwry::CloseQQwry()
{
	//
	if(m_pDataBuffer)
	{
		delete []m_pDataBuffer;
	}

	m_pDataBuffer = NULL;
	m_dwRecordCount = 0;
	m_bInit = FALSE;
}
