// PluginTest.h

#pragma once
#include <Windows.h>
#using <..\..\..\release\Plugin.dll>

#include "Form1.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Collections;
using namespace System::Windows::Forms;

namespace MyPlugin {

	public ref class myPlugin: Plugin::plugin
	{
		// TODO: �ڴ˴���Ӵ���ķ�����
	public:

		//��ʼ��
		virtual Plugin::InitData^ Init() override {

			Plugin::InitData^ initData = gcnew Plugin::InitData;
			initData->Name = "Plugin Test";
			initData->IsWindowApp = true;
			initData->GetHDriver = true;

		    return initData;
		}

		//��ʼ��
		virtual System::Void Run() override {
			//�����ö�
			(gcnew PluginTest::Form1())->ShowDialog();
			//Ҳ���Բ��ö�
			//(gcnew PluginTest::Form1())->Show();
		}

		virtual System::Void Load() override {

		}

		//��ʼ��
		virtual System::Void InitializeComponent(TabControl^ tc_) override {
			//
		}
	};
}
