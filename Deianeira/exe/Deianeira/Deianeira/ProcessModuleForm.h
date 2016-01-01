#pragma once
#include "..\common\common.h"
#include "..\common\common_net.h"
#include "..\common\sigverif.h"

namespace Deianeira {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
    using namespace System::Diagnostics;
	using namespace System::Threading;

	/// <summary>
	/// ProcessModuleForm ժҪ
	/// </summary>
	public ref class ProcessModuleForm : public System::Windows::Forms::Form
	{
	public:
		ProcessModuleForm(ULONG ulPID,String^ str_,HANDLE hDriver,ListView^ lv_)
		{
			InitializeComponent();
			//
			//TODO: �ڴ˴���ӹ��캯������
			//
			//�����ﴦ�����
			this->ulPID = ulPID;
			this->Text = this->str_ =  "[" + str_ +"]����ģ��";
			this->hDriver = hDriver;

			this->lv_process = lv_;
		}

	protected:
		/// <summary>
		/// ������������ʹ�õ���Դ��
		/// </summary>
		~ProcessModuleForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ListView^  lv_;
	protected: 

	protected: 
	private: System::Windows::Forms::ColumnHeader^  columnHeader1;
	private: System::Windows::Forms::ColumnHeader^  columnHeader2;
	private: System::Windows::Forms::ColumnHeader^  columnHeader3;
	private: System::Windows::Forms::ColumnHeader^  columnHeader4;
	private: System::Windows::Forms::ContextMenuStrip^  cms_;
	private: System::Windows::Forms::ToolStripMenuItem^  lv_refresh;

	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripMenuItem^  lv_copyMem;


	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
	private: System::Windows::Forms::ToolStripMenuItem^  lv_unload;
	private: System::Windows::Forms::ToolStripMenuItem^  lv_unloadAll;


	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator3;
	private: System::Windows::Forms::ToolStripMenuItem^  lv_delete;

	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator4;
	private: System::Windows::Forms::ToolStripMenuItem^  lv_sigverif;
	private: System::Windows::Forms::ToolStripMenuItem^  lv_sigverifAll;


	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator5;
	private: System::Windows::Forms::ToolStripMenuItem^  lv_copyAddr;

	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator6;
	private: System::Windows::Forms::ToolStripMenuItem^  lv_explorer;
	private: System::Windows::Forms::ToolStripMenuItem^  lv_showProperty;


	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator7;
	private: System::Windows::Forms::ToolStripMenuItem^  lv_export;

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
			this->lv_ = (gcnew System::Windows::Forms::ListView());
			this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader2 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader3 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader4 = (gcnew System::Windows::Forms::ColumnHeader());
			this->cms_ = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->lv_refresh = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->lv_copyMem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->lv_unload = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->lv_unloadAll = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator3 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->lv_delete = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator4 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->lv_sigverif = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->lv_sigverifAll = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator5 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->lv_copyAddr = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator6 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->lv_explorer = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->lv_showProperty = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator7 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->lv_export = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cms_->SuspendLayout();
			this->SuspendLayout();
			// 
			// lv_
			// 
			this->lv_->AllowColumnReorder = true;
			this->lv_->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(4) {this->columnHeader1, this->columnHeader2, 
				this->columnHeader3, this->columnHeader4});
			this->lv_->ContextMenuStrip = this->cms_;
			this->lv_->Dock = System::Windows::Forms::DockStyle::Fill;
			this->lv_->FullRowSelect = true;
			this->lv_->Location = System::Drawing::Point(0, 0);
			this->lv_->MultiSelect = false;
			this->lv_->Name = L"lv_";
			this->lv_->ShowItemToolTips = true;
			this->lv_->Size = System::Drawing::Size(565, 302);
			this->lv_->TabIndex = 0;
			this->lv_->UseCompatibleStateImageBehavior = false;
			this->lv_->View = System::Windows::Forms::View::Details;
			this->lv_->ColumnClick += gcnew System::Windows::Forms::ColumnClickEventHandler(this, &ProcessModuleForm::lv__ColumnClick);
			this->lv_->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &ProcessModuleForm::lv__MouseUp);
			// 
			// columnHeader1
			// 
			this->columnHeader1->Text = L"ģ��·��";
			this->columnHeader1->Width = 198;
			// 
			// columnHeader2
			// 
			this->columnHeader2->Text = L"����ַ";
			this->columnHeader2->Width = 100;
			// 
			// columnHeader3
			// 
			this->columnHeader3->Text = L"ģ���С";
			this->columnHeader3->Width = 89;
			// 
			// columnHeader4
			// 
			this->columnHeader4->Text = L"�ļ�����";
			this->columnHeader4->Width = 173;
			// 
			// cms_
			// 
			this->cms_->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(18) {this->lv_refresh, this->toolStripSeparator1, 
				this->lv_copyMem, this->toolStripSeparator2, this->lv_unload, this->lv_unloadAll, this->toolStripSeparator3, this->lv_delete, 
				this->toolStripSeparator4, this->lv_sigverif, this->lv_sigverifAll, this->toolStripSeparator5, this->lv_copyAddr, this->toolStripSeparator6, 
				this->lv_explorer, this->lv_showProperty, this->toolStripSeparator7, this->lv_export});
			this->cms_->Name = L"cms_";
			this->cms_->Size = System::Drawing::Size(179, 310);
			// 
			// lv_refresh
			// 
			this->lv_refresh->Name = L"lv_refresh";
			this->lv_refresh->Size = System::Drawing::Size(178, 22);
			this->lv_refresh->Text = L"ˢ��";
			this->lv_refresh->Click += gcnew System::EventHandler(this, &ProcessModuleForm::lv_refresh_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(175, 6);
			// 
			// lv_copyMem
			// 
			this->lv_copyMem->Name = L"lv_copyMem";
			this->lv_copyMem->Size = System::Drawing::Size(178, 22);
			this->lv_copyMem->Text = L"����ģ���ڴ�";
			this->lv_copyMem->Click += gcnew System::EventHandler(this, &ProcessModuleForm::lv_copyMem_Click);
			// 
			// toolStripSeparator2
			// 
			this->toolStripSeparator2->Name = L"toolStripSeparator2";
			this->toolStripSeparator2->Size = System::Drawing::Size(175, 6);
			// 
			// lv_unload
			// 
			this->lv_unload->Name = L"lv_unload";
			this->lv_unload->Size = System::Drawing::Size(178, 22);
			this->lv_unload->Text = L"ж��ģ��";
			this->lv_unload->Click += gcnew System::EventHandler(this, &ProcessModuleForm::lv_unload_Click);
			// 
			// lv_unloadAll
			// 
			this->lv_unloadAll->Name = L"lv_unloadAll";
			this->lv_unloadAll->Size = System::Drawing::Size(178, 22);
			this->lv_unloadAll->Text = L"ȫ��ж��ģ��(����)";
			this->lv_unloadAll->Click += gcnew System::EventHandler(this, &ProcessModuleForm::lv_unloadAll_Click);
			// 
			// toolStripSeparator3
			// 
			this->toolStripSeparator3->Name = L"toolStripSeparator3";
			this->toolStripSeparator3->Size = System::Drawing::Size(175, 6);
			// 
			// lv_delete
			// 
			this->lv_delete->Name = L"lv_delete";
			this->lv_delete->Size = System::Drawing::Size(178, 22);
			this->lv_delete->Text = L"ɾ��ģ���ļ�";
			this->lv_delete->Click += gcnew System::EventHandler(this, &ProcessModuleForm::lv_delete_Click);
			// 
			// toolStripSeparator4
			// 
			this->toolStripSeparator4->Name = L"toolStripSeparator4";
			this->toolStripSeparator4->Size = System::Drawing::Size(175, 6);
			// 
			// lv_sigverif
			// 
			this->lv_sigverif->Name = L"lv_sigverif";
			this->lv_sigverif->Size = System::Drawing::Size(178, 22);
			this->lv_sigverif->Text = L"У������ǩ��";
			this->lv_sigverif->Click += gcnew System::EventHandler(this, &ProcessModuleForm::lv_sigverif_Click);
			// 
			// lv_sigverifAll
			// 
			this->lv_sigverifAll->Name = L"lv_sigverifAll";
			this->lv_sigverifAll->Size = System::Drawing::Size(178, 22);
			this->lv_sigverifAll->Text = L"У����������ǩ��";
			this->lv_sigverifAll->Click += gcnew System::EventHandler(this, &ProcessModuleForm::lv_sigverifAll_Click);
			// 
			// toolStripSeparator5
			// 
			this->toolStripSeparator5->Name = L"toolStripSeparator5";
			this->toolStripSeparator5->Size = System::Drawing::Size(175, 6);
			// 
			// lv_copyAddr
			// 
			this->lv_copyAddr->Name = L"lv_copyAddr";
			this->lv_copyAddr->Size = System::Drawing::Size(178, 22);
			this->lv_copyAddr->Text = L"����ģ��·��";
			this->lv_copyAddr->Click += gcnew System::EventHandler(this, &ProcessModuleForm::lv_copyAddr_Click);
			// 
			// toolStripSeparator6
			// 
			this->toolStripSeparator6->Name = L"toolStripSeparator6";
			this->toolStripSeparator6->Size = System::Drawing::Size(175, 6);
			// 
			// lv_explorer
			// 
			this->lv_explorer->Name = L"lv_explorer";
			this->lv_explorer->Size = System::Drawing::Size(178, 22);
			this->lv_explorer->Text = L"��λ��ģ���ļ�";
			this->lv_explorer->Click += gcnew System::EventHandler(this, &ProcessModuleForm::lv_explorer_Click);
			// 
			// lv_showProperty
			// 
			this->lv_showProperty->Name = L"lv_showProperty";
			this->lv_showProperty->Size = System::Drawing::Size(178, 22);
			this->lv_showProperty->Text = L"�鿴ģ���ļ�����";
			this->lv_showProperty->Click += gcnew System::EventHandler(this, &ProcessModuleForm::lv_showProperty_Click);
			// 
			// toolStripSeparator7
			// 
			this->toolStripSeparator7->Name = L"toolStripSeparator7";
			this->toolStripSeparator7->Size = System::Drawing::Size(175, 6);
			// 
			// lv_export
			// 
			this->lv_export->Name = L"lv_export";
			this->lv_export->Size = System::Drawing::Size(178, 22);
			this->lv_export->Text = L"����";
			this->lv_export->Click += gcnew System::EventHandler(this, &ProcessModuleForm::lv_export_Click);
			// 
			// ProcessModuleForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(565, 302);
			this->Controls->Add(this->lv_);
			this->MaximizeBox = false;
			this->Name = L"ProcessModuleForm";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"ProcessModuleForm";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &ProcessModuleForm::ProcessModuleForm_FormClosing);
			this->Load += gcnew System::EventHandler(this, &ProcessModuleForm::ProcessModuleForm_Load);
			this->cms_->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion

		 //�����ߴ��ݹ����Ĳ���
		 ULONG ulPID;
		 ULONG ulCount;
		 String^ str_;

		 HANDLE hDriver;

		 //ListViewItem�������裬��¼��ǰ��
		 int lv_item_curColumn; 
		 //��ʾ����������
		 bool lv_item_bAsc; 

		 Thread^ thread_sigverif;

		 ListView^ lv_process;

		 //����ļ����¼�
private: System::Void ProcessModuleForm_Load(System::Object^  sender, System::EventArgs^  e) {
			 try
			 {
				 Process^ p = Process::GetProcessById(ulPID);
				 ProcessModuleCollection^ pmc_ = p->Modules;

				 ulCount =0;
				 for each(ProcessModule^ pm_ in pmc_)
				 {
					 try
					 {
						 ListViewItem^ item_ = gcnew ListViewItem(pm_->FileName);
						 
						 if (File::Exists(pm_->FileName))
						 {
							 item_->SubItems->AddRange(gcnew array<String^>{"0x" + pm_->BaseAddress.ToString("X8"),"0x" + pm_->ModuleMemorySize.ToString("X8"),
								 pm_->FileVersionInfo->CompanyName});

							 //�������ж��ļ�����
							 if (!pm_->FileVersionInfo->CompanyName->Contains("Microsoft Cor"))
							 {
								 item_->ForeColor=Color::Blue;
							 }
						 }
						 else
						 {
							 item_->SubItems->AddRange(gcnew array<String^>{"0x" + pm_->BaseAddress.ToString("X8"),"0x" + pm_->ModuleMemorySize.ToString("X8"),
								 "�ļ������ڣ�"});
							 item_->ForeColor=Color::Blue;
						 }

						 lv_->Items->Add(item_);
						 ulCount++;
					 }
					 catch(Exception^ ex){ }
				 }	 

				 this->Text = "[" + str_ +"]����ģ��" + "[" + ulCount.ToString()+ "]";
			 }
			 catch(Exception^ ex){  }

			 //ListViewItem�������裬��¼��ǰ��
			 lv_item_curColumn = -1; 
			 //��ʾ����������
			 lv_item_bAsc = false; 
		 }

		 //�ڹر�ʱ�����߳�
private: System::Void ProcessModuleForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
			 if (thread_sigverif!=nullptr && thread_sigverif->IsAlive)
			 {
				 thread_sigverif->Abort();
			 }
		 }

		 //�����Ҽ��˵�����ʾ
private: System::Void lv__MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if (lv_->Items->Count ==0)
			 {
				 lv_copyMem->Enabled = false;
				 lv_unload->Enabled = false;
				 lv_unloadAll->Enabled = false;
				 lv_delete->Enabled = false;
				 lv_sigverif->Enabled = false;
				 lv_sigverifAll->Enabled = false;
				 lv_copyAddr->Enabled = false;
				 lv_explorer->Enabled = false;
				 lv_showProperty->Enabled = false;
			 }
			 else if (lv_->SelectedItems->Count ==0)
			 {
				 lv_copyMem->Enabled = false;
				 lv_unload->Enabled = false;
				 lv_unloadAll->Enabled = false;
				 lv_delete->Enabled = false;
				 lv_sigverif->Enabled = false;
				 lv_sigverifAll->Enabled = true;
				 lv_copyAddr->Enabled = false;
				 lv_explorer->Enabled = false;
				 lv_showProperty->Enabled = false;
			 }
			 else
			 {
				 lv_copyMem->Enabled = true;
				 lv_unload->Enabled = true;
				 lv_unloadAll->Enabled = true;
				 lv_delete->Enabled = true;
				 lv_sigverif->Enabled = true;
				 lv_sigverifAll->Enabled = true;
				 lv_copyAddr->Enabled = true;
				 lv_explorer->Enabled = true;
				 lv_showProperty->Enabled = true;
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

				 if(e->Column == 2 || e->Column == 3)
				 {
					 //��������
					 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,0,16);
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

				 if(e->Column == 2 || e->Column == 3)
				 {
					 //��������
					 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,0,16);
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
private: System::Void lv_refresh_Click(System::Object^  sender, System::EventArgs^  e) {
			 lv_->Items->Clear();
			 ProcessModuleForm_Load(sender,e);
		 }

		 //����ģ���ڴ�
private: System::Void lv_copyMem_Click(System::Object^  sender, System::EventArgs^  e) {
		 }

		 //ж��
private: System::Void lv_unload_Click(System::Object^  sender, System::EventArgs^  e) {
			 UNLOADDLL_CLIENT_ID clientID;
			 BOOL bRet=FALSE;
			 DWORD dwRet=0;

			 ListViewItem^ item_ = lv_->SelectedItems[0];

			 clientID.ulPID = ulPID;
			 clientID.ulBaseAddress = Convert::ToUInt32(item_->SubItems[1]->Text->Replace("0x",""),16);

			 bRet = DeviceIoControl(hDriver, IOCTL_PROCESS_MODULE_UNLOAD, &clientID, sizeof(UNLOADDLL_CLIENT_ID), NULL, 0, &dwRet, NULL);
			 if (!bRet)
			 {
				 MessageBox::Show("ж��ʧ�ܣ�");
				 return;
			 }

			 //ˢ���б�
			 item_->Remove();
		 }

		 //ȫ��ж��
private: System::Void lv_unloadAll_Click(System::Object^  sender, System::EventArgs^  e) {
			 DWORD dwRet=0;
			 try
			 {
				 ListViewItem^ item_ = lv_->SelectedItems[0];

				 for each(ListViewItem^ item_1 in lv_process->Items)
				 {
					 try
					 {
						 int pid = int::Parse(item_1->SubItems[1]->Text);

						 //�ж��Ƿ����Լ��Ľ���ID
						 if (pid == Process::GetCurrentProcess()->Id)
						 {
							 //����ǣ���������
							 continue;
						 }

						 Process^ p = Process::GetProcessById(pid);
						 ProcessModuleCollection^ pmc_ = p->Modules;

						 for each(ProcessModule^ pm_ in pmc_)
						 {
							 try
							 {
								 if (item_->SubItems[1]->Text == "0x" + pm_->BaseAddress.ToString("X8"))
								 {
									 UNLOADDLL_CLIENT_ID clientID;

									 clientID.ulPID = pid;
									 clientID.ulBaseAddress = (ULONG)(int)pm_->BaseAddress;

									 DeviceIoControl(hDriver, IOCTL_PROCESS_MODULE_UNLOAD, &clientID, sizeof(UNLOADDLL_CLIENT_ID), NULL, 0, &dwRet, NULL);

									 break;
								 }
							 }
							 catch(Exception^ ex){ }
						 }
					 }
					 catch(Exception^ ex){ }		 
				 }	 
			 }
			 catch(Exception^ ex){ MessageBox::Show(ex->Message); }
		 }

		 //ɾ��
private: System::Void lv_delete_Click(System::Object^  sender, System::EventArgs^  e) {
			 BOOL bRet=FALSE;
			 DWORD dwRet=0;

			 ListViewItem^ item_ = lv_->SelectedItems[0];
			 LPTSTR lpFilePath = common_net::common_stringToHGlobalUni("\\??\\" + item_->SubItems[0]->Text);

			 bRet = DeviceIoControl(hDriver, IOCTL_FILE_DELETE, lpFilePath, wcslen(lpFilePath)*sizeof(WCHAR), NULL, 0, &dwRet, NULL);
			 if (!bRet)
			 {
				 MessageBox::Show("ɾ��ʧ�ܣ�");
				 return;
			 }
		 }

		 //У������ǩ��
private: System::Void lv_sigverif_Click(System::Object^  sender, System::EventArgs^  e) {
			 ListViewItem^ item_ = lv_->SelectedItems[0];
			 if (sigverif_check(common_net::common_stringToHGlobalUni(item_->SubItems[0]->Text)))
			 {
				 MessageBox::Show("���ļ���ǩ����");
			 }
			 else
			 {
				 MessageBox::Show("���ļ�û��ǩ����");
			 }
		 }

private: System::Void thread_lv__sigverifAll_func()
		 {
			 List<ListViewItem^>^ lst_item_red = gcnew List<ListViewItem^>();
			 List<ListViewItem^>^ lst_item_blue = gcnew List<ListViewItem^>();
			 List<ListViewItem^>^ lst_item_default = gcnew List<ListViewItem^>();

			 for each(ListViewItem^ item in lv_->Items)
			 {
				 //������Ϣ
				 this->Text = "--->>>[" + item->SubItems[0]->Text + "]";
				 //��֤����ǩ��
				 if (!sigverif_check(common_net::common_stringToHGlobalUni(item->SubItems[0]->Text)))
				 {
					 lst_item_red->Add(item);
				 }
				 else
					 if(item->ForeColor==Color::Blue)
					 {
						 lst_item_blue->Add(item);
					 }
					 else
					 {
						 lst_item_default->Add(item);			
					 }
			 }

			 this->Text = "[" + str_ +"]����ģ��" + "[" + ulCount.ToString()+ "]";

			 lv_->BeginUpdate();
			 int index=0;

			 //����������Ŀ
			 //��ɫ������
			 for each(ListViewItem^ item in lst_item_red)
			 {
				 ListViewItem^ item_ = (ListViewItem^)item->Clone();
				 item_->ForeColor = Color::HotPink;
				 lv_->Items[index] = item_;
				 index++;
			 }
			 //��ɫ���м�
			 for each(ListViewItem^ item in lst_item_blue)
			 {
				 ListViewItem^ item_ = (ListViewItem^)item->Clone();
				 lv_->Items[index] = item_;
				 index++;
			 }
			 //Ĭ����������
			 for each(ListViewItem^ item in lst_item_default)
			 {
				 ListViewItem^ item_ = (ListViewItem^)item->Clone();
				 lv_->Items[index] = item_;
				 index++;
			 }

			 lv_->EndUpdate();

			 this->Text = "[" + str_ +"]����ģ��" + "[" + ulCount.ToString()+ "]";

			 lv_->Enabled = true;
		 }

		 //У����������ǩ��
private: System::Void lv_sigverifAll_Click(System::Object^  sender, System::EventArgs^  e) {
			 lv_->Enabled = false;

			 //�����߳���֤
			 thread_sigverif= gcnew Thread(gcnew ThreadStart(this,&ProcessModuleForm::thread_lv__sigverifAll_func));
			 thread_sigverif->IsBackground = true;
			 thread_sigverif->Start();
		 }

		 //����ģ��·��
private: System::Void lv_copyAddr_Click(System::Object^  sender, System::EventArgs^  e) {
			 ListViewItem^ item_ = lv_->SelectedItems[0];
			 //���Ƶ����а�
			 Clipboard::SetDataObject(item_->SubItems[0]->Text, true);
		 }

		 //��λģ���ļ�
private: System::Void lv_explorer_Click(System::Object^  sender, System::EventArgs^  e) {
			 ListViewItem^ item_ = lv_->SelectedItems[0];
			 common_explorerFile(common_net::common_stringToHGlobalAnsi(item_->SubItems[0]->Text));
		 }

		 //�鿴ģ���ļ�����
private: System::Void lv_showProperty_Click(System::Object^  sender, System::EventArgs^  e) {
			 ListViewItem^ item_ = lv_->SelectedItems[0];
			 common_showFileProperty(common_net::common_stringToHGlobalUni(item_->SubItems[0]->Text));
		 }

		 //����
private: System::Void lv_export_Click(System::Object^  sender, System::EventArgs^  e) {
			 common_net::common_getListViewData(lv_);
		 }


};
}
