#ifndef _REG_
#define _REG_

#include "common.h"


//������InlineHook,��ֹ������ͼ�ֵ
VOID reg_inlineHook_disableCreate();

//ֹͣ��ֹ������ͼ�ֵ
VOID reg_inlineHook_disableCreate_stop();

//������InlineHook,��ֹɾ����ͼ�ֵ
VOID reg_inlineHook_disableDelete();

//ֹͣ��ֹɾ����ͼ�ֵ
VOID reg_inlineHook_disableDelete_stop();

//������InlineHook,��ֹ��ѯ
VOID reg_inlineHook_disableQuery();

//ֹͣ��ֹ��ѯ
VOID reg_inlineHook_disableQuery_stop();

//��DisableCreateRegInfo�в���Ŀ�����ID
//����
//0��ʾû���ҵ���1��ʾ�ҵ���
ULONG reg_findPIDInDisableCreateRegInfo(ULONG ulPID);

//��DisableDeleteRegInfo�в���Ŀ�����ID
//����
//0��ʾû���ҵ���1��ʾ�ҵ���
ULONG reg_findPIDInDisableDeleteRegInfo(ULONG ulPID);

//��DisableOpenRegInfo�в���Ŀ�����ID
//����
//0��ʾû���ҵ���1��ʾ�ҵ���
ULONG reg_findPIDInDisableOpenRegInfo(ULONG ulPID);

//��DisableCreateRegInfo��ɾ��Ŀ�����ID
VOID reg_deletePIDInDisableCreateRegInfo(ULONG ulPID);

//��DisableDeleteRegInfo��ɾ��Ŀ�����ID
VOID reg_deletePIDInDisableDeleteRegInfo(ULONG ulPID);

//��DisableOpenRegInfo��ɾ��Ŀ�����ID
VOID reg_deletePIDInDisableOpenRegInfo(ULONG ulPID);

#endif //_REG_
