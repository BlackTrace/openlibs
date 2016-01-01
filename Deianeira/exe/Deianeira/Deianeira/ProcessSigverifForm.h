#pragma once
#include "..\common\common.h"
#include "..\common\common_net.h"
#include "..\common\sigverif.h"

namespace Deianeira {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Diagnostics;
	using namespace System::Threading;

	/// <summary>
	/// ProcessSigverifForm ժҪ
	/// </summary>
	public ref class ProcessSigverifForm : public System::Windows::Forms::Form
	{
	public:
		ProcessSigverifForm(void)
		{
			InitializeComponent();
			//
			//TODO: �ڴ˴���ӹ��캯������
			//
		}

	protected:
		/// <summary>
		/// ������������ʹ�õ���Դ��
		/// </summary>
		~ProcessSigverifForm()
		{
			if (components)
			{
				delete components;
			}
		}

	protected: 

	protected: 



	private: System::Windows::Forms::ContextMenuStrip^  cms_;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_refresh;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_unload;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_unloadAll;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator3;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_delete;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator4;



	private: System::Windows::Forms::ToolStripMenuItem^  cms_copyAddr;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator6;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_explorer;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_showProperty;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator7;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_export;
	private: System::Windows::Forms::ColumnHeader^  columnHeader1;
	private: System::Windows::Forms::ColumnHeader^  columnHeader2;
	private: System::Windows::Forms::ColumnHeader^  columnHeader3;
	private: System::Windows::Forms::ListView^  lv_;
	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// ����������������
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// �����֧������ķ��� - ��Ҫ
		/// ʹ�ô���༭���޸Ĵ˷��������ݡ�
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->cms_ = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->cms_refresh = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->cms_unload = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cms_unloadAll = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator3 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->cms_delete = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator4 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->cms_copyAddr = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator6 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->cms_explorer = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cms_showProperty = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator7 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->cms_export = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader2 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader3 = (gcnew System::Windows::Forms::ColumnHeader());
			this->lv_ = (gcnew System::Windows::Forms::ListView());
			this->cms_->SuspendLayout();
			this->SuspendLayout();
			// 
			// cms_
			// 
			this->cms_->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(13) {this->cms_refresh, this->toolStripSeparator1, 
				this->cms_unload, this->cms_unloadAll, this->toolStripSeparator3, this->cms_delete, this->toolStripSeparator4, this->cms_copyAddr, 
				this->toolStripSeparator6, this->cms_explorer, this->cms_showProperty, this->toolStripSeparator7, this->cms_export});
			this->cms_->Name = L"cms_";
			this->cms_->Size = System::Drawing::Size(179, 232);
			// 
			// cms_refresh
			// 
			this->cms_refresh->Name = L"cms_refresh";
			this->cms_refresh->Size = System::Drawing::Size(178, 22);
			this->cms_refresh->Text = L"ˢ��";
			this->cms_refresh->Click += gcnew System::EventHandler(this, &ProcessSigverifForm::cms_refresh_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(175, 6);
			// 
			// cms_unload
			// 
			this->cms_unload->Name = L"cms_unload";
			this->cms_unload->Size = System::Drawing::Size(178, 22);
			this->cms_unload->Text = L"ж��ģ��";
			this->cms_unload->Click += gcnew System::EventHandler(this, &ProcessSigverifForm::cms_unload_Click);
			// 
			// cms_unloadAll
			// 
			this->cms_unloadAll->Name = L"cms_unloadAll";
			this->cms_unloadAll->Size = System::Drawing::Size(178, 22);
			this->cms_unloadAll->Text = L"ȫ��ж��ģ��(����)";
			this->cms_unloadAll->Click += gcnew System::EventHandler(this, &ProcessSigverifForm::cms_unloadAll_Click);
			// 
			// toolStripSeparator3
			// 
			this->toolStripSeparator3->Name = L"toolStripSeparator3";
			this->toolStripSeparator3->Size = System::Drawing::Size(175, 6);
			// 
			// cms_delete
			// 
			this->cms_delete->Name = L"cms_delete";
			this->cms_delete->Size = System::Drawing::Size(178, 22);
			this->cms_delete->Text = L"ɾ��ģ���ļ�";
			this->cms_delete->Click += gcnew System::EventHandler(this, &ProcessSigverifForm::cms_delete_Click);
			// 
			// toolStripSeparator4
			// 
			this->toolStripSeparator4->Name = L"toolStripSeparator4";
			this->toolStripSeparator4->Size = System::Drawing::Size(175, 6);
			// 
			// cms_copyAddr
			// 
			this->cms_copyAddr->Name = L"cms_copyAddr";
			this->cms_copyAddr->Size = System::Drawing::Size(178, 22);
			this->cms_copyAddr->Text = L"����ģ��·��";
			this->cms_copyAddr->Click += gcnew System::EventHandler(this, &ProcessSigverifForm::cms_copyAddr_Click);
			// 
			// toolStripSeparator6
			// 
			this->toolStripSeparator6->Name = L"toolStripSeparator6";
			this->toolStripSeparator6->Size = System::Drawing::Size(175, 6);
			// 
			// cms_explorer
			// 
			this->cms_explorer->Name = L"cms_explorer";
			this->cms_explorer->Size = System::Drawing::Size(178, 22);
			this->cms_explorer->Text = L"��λ��ģ���ļ�";
			this->cms_explorer->Click += gcnew System::EventHandler(this, &ProcessSigverifForm::cms_explorer_Click);
			// 
			// cms_showProperty
			// 
			this->cms_showProperty->Name = L"cms_showProperty";
			this->cms_showProperty->Size = System::Drawing::Size(178, 22);
			this->cms_showProperty->Text = L"�鿴ģ���ļ�����";
			this->cms_showProperty->Click += gcnew System::EventHandler(this, &ProcessSigverifForm::cms_showProperty_Click);
			// 
			// toolStripSeparator7
			// 
			this->toolStripSeparator7->Name = L"toolStripSeparator7";
			this->toolStripSeparator7->Size = System::Drawing::Size(175, 6);
			// 
			// cms_export
			// 
			this->cms_export->Name = L"cms_export";
			this->cms_export->Size = System::Drawing::Size(178, 22);
			this->cms_export->Text = L"����";
			this->cms_export->Click += gcnew System::EventHandler(this, &ProcessSigverifForm::cms_export_Click);
			// 
			// columnHeader1
			// 
			this->columnHeader1->Text = L"��������";
			this->columnHeader1->Width = 123;
			// 
			// columnHeader2
			// 
			this->columnHeader2->Text = L"����ID";
			this->columnHeader2->Width = 71;
			// 
			// columnHeader3
			// 
			this->columnHeader3->Text = L"ӳ��·��";
			this->columnHeader3->Width = 348;
			// 
			// lv_
			// 
			this->lv_->AllowColumnReorder = true;
			this->lv_->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(3) {this->columnHeader1, this->columnHeader2, 
				this->columnHeader3});
			this->lv_->ContextMenuStrip = this->cms_;
			this->lv_->Dock = System::Windows::Forms::DockStyle::Fill;
			this->lv_->FullRowSelect = true;
			this->lv_->Location = System::Drawing::Point(0, 0);
			this->lv_->MultiSelect = false;
			this->lv_->Name = L"lv_";
			this->lv_->ShowItemToolTips = true;
			this->lv_->Size = System::Drawing::Size(547, 268);
			this->lv_->TabIndex = 0;
			this->lv_->UseCompatibleStateImageBehavior = false;
			this->lv_->View = System::Windows::Forms::View::Details;
			this->lv_->ColumnClick += gcnew System::Windows::Forms::ColumnClickEventHandler(this, &ProcessSigverifForm::lv__ColumnClick);
			this->lv_->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &ProcessSigverifForm::lv__MouseUp);
			// 
			// ProcessSigverifForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(547, 268);
			this->Controls->Add(this->lv_);
			this->MaximizeBox = false;
			this->Name = L"ProcessSigverifForm";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"����û������ǩ��ģ��";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &ProcessSigverifForm::ProcessSigverifForm_FormClosing);
			this->Load += gcnew System::EventHandler(this, &ProcessSigverifForm::ProcessSigverifForm_Load);
			this->cms_->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion

		 HANDLE hDriver;

		 //ListViewItem�������裬��¼��ǰ��
		 int lv_item_curColumn; 
		 //��ʾ����������
		 bool lv_item_bAsc; 

		 Thread^ thread_sigverif;

private: System::Void thread_lv__sigverifAll_func()
		 {
			 try
			 {
				 ULONG ulCount=0;

				 array<Process^>^ ps = Process::GetProcesses(Environment::MachineName);

				 for each(Process^ p in ps)
				 {
					 try
					 {
						 ProcessModuleCollection^ pmc_ = p->Modules; 

						 for each(ProcessModule^ pm_ in pmc_)
						 {
							 try
							 {
								 //
								 this->Text = "["+p->MainModule->ModuleName+"]"+"--->>>[" + pm_->FileName + "]";

								 if (!sigverif_check((common_net::common_stringToHGlobalUni(pm_->FileName))))
								 {
									 ListViewItem^ item_ = gcnew ListViewItem(p->MainModule->ModuleName);

									 if (File::Exists(pm_->FileName))
									 {
										 item_->SubItems->AddRange(gcnew array<String^>{p->Id.ToString(),pm_->FileName});

										 //�������ж��ļ�����
										 if (!pm_->FileVersionInfo->CompanyName->Contains("Microsoft Cor"))
										 {
											 item_->ForeColor=Color::Blue;
										 }
									 }
									 else
									 {
										 item_->SubItems->AddRange(gcnew array<String^>{p->Id.ToString(), "�ļ������ڣ�"});
										 item_->ForeColor=Color::Blue;
									 }

									 //����tag
									 item_->Tag = pm_;

									 lv_->Items->Add(item_);

									 ulCount++;
								 }
							 }
							 catch(Exception^ ex){ }
						 }	
					 }
					 catch(Exception^ ex){ }
				 }

				 this->Text = "����û������ǩ��ģ��"+"("+ulCount.ToString()+")";
			 }
			 catch(Exception^ ex){  }
		 }

		 //�������
private: System::Void ProcessSigverifForm_Load(System::Object^  sender, System::EventArgs^  e) {
			 //�����߳���֤
			 thread_sigverif= gcnew Thread(gcnew ThreadStart(this,&ProcessSigverifForm::thread_lv__sigverifAll_func));
			 thread_sigverif->IsBackground = true;
			 thread_sigverif->Start();
		 }

		 //�ر�
private: System::Void ProcessSigverifForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
			 if (thread_sigverif!=nullptr && thread_sigverif->IsAlive)
			 {
				 thread_sigverif->Abort();
			 }
		 }

		 //�Ҽ��˵�
private: System::Void lv__MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if (lv_->Items->Count ==0)
			 {
				 cms_unload->Enabled = false;
				 cms_unloadAll->Enabled = false;
				 cms_delete->Enabled = false;
				 cms_copyAddr->Enabled = false;
				 cms_explorer->Enabled = false;
				 cms_showProperty->Enabled = false;
			 }
			 else if (lv_->SelectedItems->Count ==0)
			 {
				 cms_unload->Enabled = false;
				 cms_unloadAll->Enabled = false;
				 cms_delete->Enabled = false;
				 cms_copyAddr->Enabled = false;
				 cms_explorer->Enabled = false;
				 cms_showProperty->Enabled = false;
			 }
			 else
			 {
				 cms_unload->Enabled = true;
				 cms_unloadAll->Enabled = true;
				 cms_delete->Enabled = true;
				 cms_copyAddr->Enabled = true;
				 cms_explorer->Enabled = true;
				 cms_showProperty->Enabled = true;
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

				 if(e->Column == 1)
				 {
					 //��������
					 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,0,10);
				 }
				 else
				 {
					 //�ַ�������
					 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,1,0);
				 }
			 }
			 else
			 {		
				 lv_item_bAsc = false;

				 if(e->Column == 1)
				 {
					 //��������
					 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,0,10);
				 }
				 else
				 {
					 //�ַ�������
					 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,1,0);
				 }

				 //��¼��ǰ
				 lv_item_curColumn = e->Column;
			 }
		 }

		 //ˢ��
private: System::Void cms_refresh_Click(System::Object^  sender, System::EventArgs^  e) {
			 lv_->Items->Clear();
			 ProcessSigverifForm_Load(sender,e);
		 }

		 //ж��
private: System::Void cms_unload_Click(System::Object^  sender, System::EventArgs^  e) {
			 UNLOADDLL_CLIENT_ID clientID;
			 BOOL bRet=FALSE;
			 DWORD dwRet=0;

			 ListViewItem^ item_ = lv_->SelectedItems[0];

			 clientID.ulPID = int::Parse(item_->SubItems[1]->Text);
			 clientID.ulBaseAddress = (int)((ProcessModule^)item_->Tag)->BaseAddress;

			 bRet = DeviceIoControl(hDriver, IOCTL_PROCESS_MODULE_UNLOAD, &clientID, sizeof(UNLOADDLL_CLIENT_ID), NULL, 0, &dwRet, NULL);
			 if (!bRet)
			 {
				 MessageBox::Show("ж��ʧ�ܣ�");
				 return;
			 }

			 //ˢ���б�
			 item_->Remove();
		 }

		 //ж��ȫ��
private: System::Void cms_unloadAll_Click(System::Object^  sender, System::EventArgs^  e) {
		 }

		 //ɾ���ļ�
private: System::Void cms_delete_Click(System::Object^  sender, System::EventArgs^  e) {
			 BOOL bRet=FALSE;
			 DWORD dwRet=0;

			 ListViewItem^ item_ = lv_->SelectedItems[0];
			 LPTSTR lpFilePath = common_net::common_stringToHGlobalUni("\\??\\" + item_->SubItems[2]->Text);

			 bRet = DeviceIoControl(hDriver, IOCTL_FILE_DELETE, lpFilePath, wcslen(lpFilePath)*sizeof(WCHAR), NULL, 0, &dwRet, NULL);
			 if (!bRet)
			 {
				 MessageBox::Show("ɾ��ʧ�ܣ�");
				 return;
			 }
		 }

		 //���Ƶ�ַ
private: System::Void cms_copyAddr_Click(System::Object^  sender, System::EventArgs^  e) {
			 ListViewItem^ item_ = lv_->SelectedItems[0];
			 //���Ƶ����а�
			 Clipboard::SetDataObject(item_->SubItems[2]->Text, true);
		 }

		 //��λ
private: System::Void cms_explorer_Click(System::Object^  sender, System::EventArgs^  e) {
			 ListViewItem^ item_ = lv_->SelectedItems[0];
			 common_explorerFile(common_net::common_stringToHGlobalAnsi(item_->SubItems[2]->Text));
		 }

		 //����
private: System::Void cms_showProperty_Click(System::Object^  sender, System::EventArgs^  e) {
			 ListViewItem^ item_ = lv_->SelectedItems[0];
			 common_showFileProperty(common_net::common_stringToHGlobalUni(item_->SubItems[2]->Text));
		 }

		 //����
private: System::Void cms_export_Click(System::Object^  sender, System::EventArgs^  e) {
			  common_net::common_getListViewData(lv_);
		 }

};
}
