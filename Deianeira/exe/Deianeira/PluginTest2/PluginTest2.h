// PluginTest2.h

#pragma once
#include <Windows.h>
#using <..\..\..\release\Plugin.dll>

using namespace System;
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Diagnostics;

namespace MyPlugin {

	public ref class myPlugin: Plugin::plugin
	{
		// TODO: �ڴ˴���Ӵ���ķ�����
	private: System::Windows::Forms::TabPage^	tp_;
	private: System::Windows::Forms::ListView^  lv_;
	private: System::Windows::Forms::ColumnHeader^  columnHeader1;
	private: System::Windows::Forms::ColumnHeader^  columnHeader2;
	private: System::Windows::Forms::Button^  btnSuspend;
	private: System::Windows::Forms::Button^  btnResume;
	private: System::Windows::Forms::Button^  btnKill;
	private: System::Windows::Forms::Button^  btnKill2;
	private: System::Windows::Forms::Button^  btnKill3;
	private: System::Windows::Forms::Button^  btnRefresh;
	private: System::Windows::Forms::ColumnHeader^  columnHeader3;
	private: System::Windows::Forms::ColumnHeader^  columnHeader4;
	private: System::Windows::Forms::ColumnHeader^  columnHeader5;
	private: System::Windows::Forms::ContextMenuStrip^  cms_;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_refresh;

	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_sigverif;

	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_explorer;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_showFileProperty;


	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator3;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_export;

	public:

		//��ʼ��
		virtual Plugin::InitData^ Init() override {

			Plugin::InitData^ initData = gcnew Plugin::InitData;
			initData->Name = "Plugin Test2";
			//���IsWindowApp=true����ôtc_���Բ�Ҫ��ֵ
			initData->IsWindowApp = false;
			//��ʾ�ڡ���������������������ؼ�
			initData->tc_ = Plugin::TabControls::tc_other;
			//�����������ʾ֮ǰ��Ҫ��listview�г��ֳ���ʱ�Ľ����б�
			initData->IsDoAnythingBeforeLoad = true;
			initData->GetHDriver = true;

			return initData;
		}

		//��ʼ��
		virtual System::Void Run() override {
			//�����ö�
			//(gcnew PluginTest::Form1())->ShowDialog();
			//Ҳ���Բ��ö�
			//(gcnew PluginTest::Form1())->Show();
		}

		virtual System::Void InitializeComponent(TabControl^ tc_) override{
			//ִ����дǰ�ķ�����
			__super::InitializeComponent(tc_);

			this->tp_ = (gcnew System::Windows::Forms::TabPage());
			this->tp_->SuspendLayout();
			this->lv_ = (gcnew System::Windows::Forms::ListView());
			this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader2 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader3 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader4 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader5 = (gcnew System::Windows::Forms::ColumnHeader());
			this->btnSuspend = (gcnew System::Windows::Forms::Button());
			this->btnResume = (gcnew System::Windows::Forms::Button());
			this->btnKill = (gcnew System::Windows::Forms::Button());
			this->btnKill2 = (gcnew System::Windows::Forms::Button());
			this->btnKill3 = (gcnew System::Windows::Forms::Button());
			this->btnRefresh = (gcnew System::Windows::Forms::Button());
			this->cms_ = (gcnew System::Windows::Forms::ContextMenuStrip());
			this->cms_refresh = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->cms_sigverif = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->cms_explorer = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cms_showFileProperty = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator3 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->cms_export = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cms_->SuspendLayout();

			// 
			// tp_
			// 
			this->tp_->Controls->Add(this->btnRefresh);
			this->tp_->Controls->Add(this->btnKill3);
			this->tp_->Controls->Add(this->btnKill2);
			this->tp_->Controls->Add(this->btnKill);
			this->tp_->Controls->Add(this->btnResume);
			this->tp_->Controls->Add(this->btnSuspend);
			this->tp_->Controls->Add(this->lv_);
			this->tp_->Location = System::Drawing::Point(4, 21);
			this->tp_->Name = L"tp_";
			this->tp_->Padding = System::Windows::Forms::Padding(3);
			this->tp_->Size = System::Drawing::Size(860, 415);
			this->tp_->TabIndex = 7;
			this->tp_->Text = L"�������";
			this->tp_->UseVisualStyleBackColor = true;

			// 
			// lv_
			// 
			this->lv_->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(5) {this->columnHeader1, this->columnHeader2, 
				this->columnHeader3, this->columnHeader4, this->columnHeader5});
			this->lv_->ContextMenuStrip = this->cms_;
			this->lv_->FullRowSelect = true;
			this->lv_->Location = System::Drawing::Point(12, 21);
			this->lv_->MultiSelect = false;
			this->lv_->Name = L"lv_";
			this->lv_->Size = System::Drawing::Size(760, 373);
			this->lv_->TabIndex = 0;
			this->lv_->UseCompatibleStateImageBehavior = false;
			this->lv_->View = System::Windows::Forms::View::Details;
			this->lv_->ColumnClick += gcnew System::Windows::Forms::ColumnClickEventHandler(this, &myPlugin::lv__ColumnClick);
			this->lv_->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &myPlugin::lv__MouseUp);

			// 
			// columnHeader1
			// 
			this->columnHeader1->Text = L"����ID";
			this->columnHeader1->Width = 81;

			// 
			// columnHeader2
			// 
			this->columnHeader2->Text = L"��������";
			this->columnHeader2->Width = 162;

			// 
			// columnHeader3
			// 
			this->columnHeader3->Text = L"������ID";
			this->columnHeader3->Width = 80;

			// 
			// columnHeader4
			// 
			this->columnHeader4->Text = L"ӳ��·��";
			this->columnHeader4->Width = 221;

			// 
			// columnHeader5
			// 
			this->columnHeader5->Text = L"�ļ�����";
			this->columnHeader5->Width = 152;

			// 
			// btnSuspend
			// 
			this->btnSuspend->Location = System::Drawing::Point(778, 66);
			this->btnSuspend->Name = L"btnSuspend";
			this->btnSuspend->Size = System::Drawing::Size(75, 23);
			this->btnSuspend->TabIndex = 1;
			this->btnSuspend->Text = L"����";
			this->btnSuspend->UseVisualStyleBackColor = true;
			this->btnSuspend->Click += gcnew System::EventHandler(this, &myPlugin::btnSuspend_Click);

			// 
			// btnResume
			// 
			this->btnResume->Location = System::Drawing::Point(778, 95);
			this->btnResume->Name = L"btnResume";
			this->btnResume->Size = System::Drawing::Size(75, 23);
			this->btnResume->TabIndex = 2;
			this->btnResume->Text = L"�ָ�";
			this->btnResume->UseVisualStyleBackColor = true;
			this->btnResume->Click += gcnew System::EventHandler(this, &myPlugin::btnResume_Click);

			// 
			// btnKill
			// 
			this->btnKill->Location = System::Drawing::Point(778, 165);
			this->btnKill->Name = L"btnKill";
			this->btnKill->Size = System::Drawing::Size(75, 23);
			this->btnKill->TabIndex = 3;
			this->btnKill->Text = L"����";
			this->btnKill->UseVisualStyleBackColor = true;
			this->btnKill->Click += gcnew System::EventHandler(this, &myPlugin::btnKill_Click);

			// 
			// btnKill2
			// 
			this->btnKill2->Location = System::Drawing::Point(778, 194);
			this->btnKill2->Name = L"btnKill2";
			this->btnKill2->Size = System::Drawing::Size(75, 23);
			this->btnKill2->TabIndex = 3;
			this->btnKill2->Text = L"ǿ�ƽ���";
			this->btnKill2->UseVisualStyleBackColor = true;
			this->btnKill2->Click += gcnew System::EventHandler(this, &myPlugin::btnKill2_Click);

			// 
			// btnKill3
			// 
			this->btnKill3->Location = System::Drawing::Point(778, 223);
			this->btnKill3->Name = L"btnKill3";
			this->btnKill3->Size = System::Drawing::Size(75, 23);
			this->btnKill3->TabIndex = 4;
			this->btnKill3->Text = L"��������";
			this->btnKill3->UseVisualStyleBackColor = true;
			this->btnKill3->Click += gcnew System::EventHandler(this, &myPlugin::btnKill3_Click);

			// 
			// btnRefresh
			// 
			this->btnRefresh->Location = System::Drawing::Point(778, 21);
			this->btnRefresh->Name = L"btnRefresh";
			this->btnRefresh->Size = System::Drawing::Size(75, 23);
			this->btnRefresh->TabIndex = 5;
			this->btnRefresh->Text = L"ˢ��";
			this->btnRefresh->UseVisualStyleBackColor = true;
			this->btnRefresh->Click += gcnew System::EventHandler(this, &myPlugin::btnRefresh_Click);

			// 
			// cms_
			// 
			this->cms_->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(8) {this->cms_refresh, this->toolStripSeparator1, 
				this->cms_sigverif, this->toolStripSeparator2, this->cms_explorer, this->cms_showFileProperty, this->toolStripSeparator3, this->cms_export});
			this->cms_->Name = L"cms_";
			this->cms_->Size = System::Drawing::Size(167, 154);

			// 
			// cms_refresh
			// 
			this->cms_refresh->Name = L"cms_refresh";
			this->cms_refresh->Size = System::Drawing::Size(166, 22);
			this->cms_refresh->Text = L"ˢ��";
			this->cms_refresh->Click += gcnew System::EventHandler(this, &myPlugin::cms_refresh_Click);

			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(163, 6);

			// 
			// cms_sigverif
			// 
			this->cms_sigverif->Name = L"cms_sigverif";
			this->cms_sigverif->Size = System::Drawing::Size(166, 22);
			this->cms_sigverif->Text = L"У������ǩ��";
			this->cms_sigverif->Click += gcnew System::EventHandler(this, &myPlugin::cms_sigverif_Click);

			// 
			// toolStripSeparator2
			// 
			this->toolStripSeparator2->Name = L"toolStripSeparator2";
			this->toolStripSeparator2->Size = System::Drawing::Size(163, 6);

			// 
			// cms_explorer
			// 
			this->cms_explorer->Name = L"cms_explorer";
			this->cms_explorer->Size = System::Drawing::Size(166, 22);
			this->cms_explorer->Text = L"��λӳ���ļ�";
			this->cms_explorer->Click += gcnew System::EventHandler(this, &myPlugin::cms_explorer_Click);

			// 
			// cms_showFileProperty
			// 
			this->cms_showFileProperty->Name = L"cms_showFileProperty";
			this->cms_showFileProperty->Size = System::Drawing::Size(166, 22);
			this->cms_showFileProperty->Text = L"�鿴ӳ���ļ�·��";
			this->cms_showFileProperty->Click += gcnew System::EventHandler(this, &myPlugin::cms_showFileProperty_Click);

			// 
			// toolStripSeparator3
			// 
			this->toolStripSeparator3->Name = L"toolStripSeparator3";
			this->toolStripSeparator3->Size = System::Drawing::Size(163, 6);

			// 
			// cms_export
			// 
			this->cms_export->Name = L"cms_export";
			this->cms_export->Size = System::Drawing::Size(166, 22);
			this->cms_export->Text = L"����";
			this->cms_export->Click += gcnew System::EventHandler(this, &myPlugin::cms_export_Click);


			tc_->Controls->Add(this->tp_);
		}

		//ListViewItem�������裬��¼��ǰ��
		static int lv_item_curColumn = -1; 
		//��ʾ����������
		static bool lv_item_bAsc = false; 

		//
		virtual System::Void Load() override{
			 try
			 {
				 //
				 lv_->Items->Clear();

				 array<Process^>^ ps = Process::GetProcesses(Environment::MachineName);
				 for each (Process^ p in ps)
				 {				 
					 try
					 {
						 ListViewItem^ item = gcnew ListViewItem(p->Id.ToString());
						 item->SubItems->AddRange(gcnew array<String^>{
							 Plugin::plugin::plugin_common_net_getProcessName(p->Id),
								 Plugin::plugin::plugin_process_getProcessParentPID(p->Id).ToString(),
								 Plugin::plugin::plugin_common_net_formatString(Plugin::plugin::plugin_common_net_getProcessPath(p->Id)),
								 Plugin::plugin::plugin_common_net_getProcessCompanyName(p->Id)
						 });
						 lv_->Items->Add(item);			

						 if (!Plugin::plugin::plugin_common_net_checkFileCompanyName(Plugin::plugin::plugin_common_net_getProcessCompanyName(p->Id)))
						 {
							 item->ForeColor = Color::Blue;
						 }
					 }
					 catch(Exception^ ex){ }
				 }
			 }
			 catch(Exception^ ex){ }
		 }

		 //ˢ��
private: System::Void btnRefresh_Click(System::Object^  sender, System::EventArgs^  e) {
			 lv_->Items->Clear();
			 Load();
		 }

		 //����
private: System::Void btnSuspend_Click(System::Object^  sender, System::EventArgs^  e) {
			 ListViewItem^ item_ = lv_->SelectedItems[0];
			 int ulPID = int::Parse(item_->SubItems[0]->Text);

			 if (!Plugin::plugin::plugin_process_suspendProcess(ulPID))
			 {
				 MessageBox::Show("����ʧ�ܣ�");
				 return;
			 }
		 }

		 //�ָ�
private: System::Void btnResume_Click(System::Object^  sender, System::EventArgs^  e) {
			 ListViewItem^ item_ = lv_->SelectedItems[0];
			 int ulPID = int::Parse(item_->SubItems[0]->Text);

			 if (!Plugin::plugin::plugin_process_resumeProcess(ulPID))
			 {
				 MessageBox::Show("�ָ�ʧ�ܣ�");
				 return;
			 }
		 }

		 //��ͨɾ��
private: System::Void btnKill_Click(System::Object^  sender, System::EventArgs^  e) {
			 try
			 {
				 ListViewItem^ item_ = lv_->SelectedItems[0];
				 Process^ p = Process::GetProcessById(int::Parse(item_->SubItems[0]->Text));

				 p->Kill();

				 item_->Remove();
			 }
			 catch(Exception^ ex){ MessageBox::Show(ex->Message); }
		 }

		 //ǿ��ɾ��
private: System::Void btnKill2_Click(System::Object^  sender, System::EventArgs^  e) {

			 ListViewItem^ item_ = lv_->SelectedItems[0];
			 int ulPID = int::Parse(item_->SubItems[0]->Text);

			 if (!Plugin::plugin::plugin_process_killPrcess(ulPID))
			 {
				 MessageBox::Show("ǿ��ɾ��ʧ�ܣ�");
				 return;
			 }

			 item_->Remove();
		 }

		 //����ɾ��
private: System::Void btnKill3_Click(System::Object^  sender, System::EventArgs^  e) {
			 ListViewItem^ item_ = lv_->SelectedItems[0];
			 int ulPID = int::Parse(item_->SubItems[0]->Text);

			 if (!Plugin::plugin::plugin_process_forceKillProcess(ulPID))
			 {
				 MessageBox::Show("ǿ��ɾ��ʧ�ܣ�");
				 return;
			 }

			 item_->Remove();
		 }

		 //�����Ҽ��˵���ʾ
private: System::Void lv__MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if (lv_->Items->Count == 0)
			 {
				 cms_sigverif->Enabled = false;
				 cms_explorer->Enabled = false;
				 cms_showFileProperty->Enabled = false;
			 }
			 else if(lv_->SelectedItems->Count == 0)
			 {
				 cms_sigverif->Enabled = false;
				 cms_explorer->Enabled = false;
				 cms_showFileProperty->Enabled = false;
			 }
			 else
			 {
				 ListViewItem^ item_ = lv_->SelectedItems[0];
				 int ulPID = int::Parse(item_->SubItems[0]->Text);

				 //�жϽ���״̬
				 switch (Plugin::plugin::plugin_process_getProcessState(ulPID))
				 {
				 case 0:
					 break;
					 //���ڹ���״̬
				 case 1:
					 {
						 btnSuspend->Enabled=false;
						 btnResume->Enabled=true;
					 }
					 break;
					 //û�й���
				 case 2:
					 {
						 btnSuspend->Enabled=true;
						 btnResume->Enabled=false;
					 }
					 break;
				 }
			 }
		 }

		 //����
private: System::Void lv__ColumnClick(System::Object^  sender, System::Windows::Forms::ColumnClickEventArgs^  e) {
			 if(e->Column == lv_item_curColumn)
			 {		 
				 if (lv_item_bAsc)
				 {
					 lv_item_bAsc = false;
				 }
				 else
				 {
					 lv_item_bAsc = true;
				 }

				 if (e->Column == 0 || e->Column == 2)
				 {
					 //��������
					 lv_->ListViewItemSorter = Plugin::plugin::plugin_common_net_sortLV(e->Column,lv_item_bAsc,0,10);
				 }
				 else
				 {
					 //�ַ�������
					 lv_->ListViewItemSorter = Plugin::plugin::plugin_common_net_sortLV(e->Column,lv_item_bAsc,1,0);
				 }		 
			 }
			 else
			 {		
				 lv_item_bAsc = false;

				 if (e->Column == 0 || e->Column == 2)
				 {
					 //��������
					 lv_->ListViewItemSorter = Plugin::plugin::plugin_common_net_sortLV(e->Column,lv_item_bAsc,0,10);
				 }
				 else
				 {
					 //�ַ�������
					 lv_->ListViewItemSorter = Plugin::plugin::plugin_common_net_sortLV(e->Column,lv_item_bAsc,1,0);
				 }	

				 //��¼��ǰ
				 lv_item_curColumn = e->Column;
			 }
		 }

		 //ˢ��
private: System::Void cms_refresh_Click(System::Object^  sender, System::EventArgs^  e) {
			 btnRefresh_Click(sender,e);
		 }

		 //У������ǩ��
private: System::Void cms_sigverif_Click(System::Object^  sender, System::EventArgs^  e) {
			 ListViewItem^ item_ = lv_->SelectedItems[0];

			 LPTSTR lpFilePath = Plugin::plugin::plugin_common_net_stringToHGlobalUni(item_->SubItems[3]->Text);

			 if (Plugin::plugin::plugin_sigverif_check(lpFilePath))
			 {
				 MessageBox::Show("���ļ���ǩ����");
				 return;
			 }

			 MessageBox::Show("���ļ�û��ǩ����");;
		 }

		 //��λ�ļ�
private: System::Void cms_explorer_Click(System::Object^  sender, System::EventArgs^  e) {
			 ListViewItem^ item_ = lv_->SelectedItems[0];

			 LPSTR lpFilePath = Plugin::plugin::plugin_common_net_stringToHGlobalAnsi(item_->SubItems[3]->Text);

			 Plugin::plugin::plugin_common_explorerFile(lpFilePath);
		 }

		 //�鿴�ļ�����
private: System::Void cms_showFileProperty_Click(System::Object^  sender, System::EventArgs^  e) {
			 ListViewItem^ item_ = lv_->SelectedItems[0];

			 LPTSTR lpFilePath = Plugin::plugin::plugin_common_net_stringToHGlobalUni(item_->SubItems[3]->Text);

			 Plugin::plugin::plugin_common_showFileProperty(lpFilePath);
		 }

		 //����
private: System::Void cms_export_Click(System::Object^  sender, System::EventArgs^  e) {
			 Plugin::plugin::plugin_common_net_getListViewData(lv_);
		 }

	};
}
