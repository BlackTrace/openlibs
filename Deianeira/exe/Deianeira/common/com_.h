//com_.h
#ifndef _com__
#define _com__

#pragma once

namespace Plugin {

	using namespace System;

	//ö������
    public enum class TabControls{
		//�����
		tc_main,
		//�ں�������
		tc_kernel,
		//Ӧ�ò�������
		tc_app,
		//��������������
		tc_net,
		//��������������
		tc_other
	};

	//
	public ref class InitData 
	{
	public:
		InitData(void)
		{
			//
			//TODO: �ڴ˴���ӹ��캯������
			//
		}

		//�������,��ʾ�ڡ�����������˵���
		String^ Name;

		//��־�Ƿ��Ƕ����������,���Ǹ��ӵ�����������
		bool IsWindowApp;

		//tabControl
		TabControls tc_;

		//�Ƿ���Load�¼����������֮ǰ�Ƿ�Ҫִ����غ�������Form_Load���ڴ�������ǰҪִ�еĴ��룩
		bool IsDoAnythingBeforeLoad;

		//Ҫ������������ر�����Ϣ
		//�Ƿ�������������
		bool GetHDriver;

	};
}

#endif //com_