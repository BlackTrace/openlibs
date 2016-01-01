#pragma once
#include "..\common\common.h"
#include "..\common\common_net.h"

namespace Deianeira {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;

	/// <summary>
	/// ProcessThreadForm ժҪ
	/// </summary>
	public ref class ProcessThreadForm : public System::Windows::Forms::Form
	{
	public:
		ProcessThreadForm(ULONG ulPID,String^ str_,HANDLE hDriver)
		{
			InitializeComponent();
			//
			//TODO: �ڴ˴���ӹ��캯������
			//
			//�����ﴦ�����
			this->ulPID = ulPID;
			this->Text = "[" + str_ +"]�����߳�";
			this->str_ = str_;
			this->hDriver = hDriver;
		}

	protected:
		/// <summary>
		/// ������������ʹ�õ���Դ��
		/// </summary>
		~ProcessThreadForm()
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

	private: System::Windows::Forms::ColumnHeader^  columnHeader6;
	private: System::Windows::Forms::ContextMenuStrip^  cms_;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_refresh;

	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;

	private: System::Windows::Forms::ToolStripMenuItem^  cms_forceKill;


	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_suspend;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_resume;


	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator3;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_export;
	private: System::Windows::Forms::ColumnHeader^  columnHeader5;


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
			this->columnHeader5 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader6 = (gcnew System::Windows::Forms::ColumnHeader());
			this->cms_ = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->cms_refresh = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->cms_forceKill = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->cms_suspend = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cms_resume = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator3 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->cms_export = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cms_->SuspendLayout();
			this->SuspendLayout();
			// 
			// lv_
			// 
			this->lv_->AllowColumnReorder = true;
			this->lv_->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(6) {this->columnHeader1, this->columnHeader2, 
				this->columnHeader3, this->columnHeader4, this->columnHeader5, this->columnHeader6});
			this->lv_->ContextMenuStrip = this->cms_;
			this->lv_->Dock = System::Windows::Forms::DockStyle::Fill;
			this->lv_->FullRowSelect = true;
			this->lv_->Location = System::Drawing::Point(0, 0);
			this->lv_->MultiSelect = false;
			this->lv_->Name = L"lv_";
			this->lv_->ShowItemToolTips = true;
			this->lv_->Size = System::Drawing::Size(597, 283);
			this->lv_->TabIndex = 1;
			this->lv_->UseCompatibleStateImageBehavior = false;
			this->lv_->View = System::Windows::Forms::View::Details;
			this->lv_->ColumnClick += gcnew System::Windows::Forms::ColumnClickEventHandler(this, &ProcessThreadForm::lv__ColumnClick);
			this->lv_->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &ProcessThreadForm::lv__MouseUp);
			// 
			// columnHeader1
			// 
			this->columnHeader1->Text = L"�߳�ID";
			this->columnHeader1->Width = 69;
			// 
			// columnHeader2
			// 
			this->columnHeader2->Text = L"���ȼ�";
			this->columnHeader2->Width = 63;
			// 
			// columnHeader3
			// 
			this->columnHeader3->Text = L"�̻߳�ַ";
			this->columnHeader3->Width = 91;
			// 
			// columnHeader4
			// 
			this->columnHeader4->Text = L"����ģ��";
			this->columnHeader4->Width = 146;
			// 
			// columnHeader5
			// 
			this->columnHeader5->Text = L"����ʱ��";
			this->columnHeader5->Width = 137;
			// 
			// columnHeader6
			// 
			this->columnHeader6->Text = L"״̬";
			this->columnHeader6->Width = 77;
			// 
			// cms_
			// 
			this->cms_->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(8) {this->cms_refresh, this->toolStripSeparator1, 
				this->cms_forceKill, this->toolStripSeparator2, this->cms_suspend, this->cms_resume, this->toolStripSeparator3, this->cms_export});
			this->cms_->Name = L"cms_";
			this->cms_->Size = System::Drawing::Size(153, 154);
			// 
			// cms_refresh
			// 
			this->cms_refresh->Name = L"cms_refresh";
			this->cms_refresh->Size = System::Drawing::Size(148, 22);
			this->cms_refresh->Text = L"ˢ��";
			this->cms_refresh->Click += gcnew System::EventHandler(this, &ProcessThreadForm::cms_refresh_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(145, 6);
			// 
			// cms_forceKill
			// 
			this->cms_forceKill->Name = L"cms_forceKill";
			this->cms_forceKill->Size = System::Drawing::Size(152, 22);
			this->cms_forceKill->Text = L"�����߳�";
			this->cms_forceKill->Click += gcnew System::EventHandler(this, &ProcessThreadForm::cms_forceKill_Click);
			// 
			// toolStripSeparator2
			// 
			this->toolStripSeparator2->Name = L"toolStripSeparator2";
			this->toolStripSeparator2->Size = System::Drawing::Size(145, 6);
			// 
			// cms_suspend
			// 
			this->cms_suspend->Name = L"cms_suspend";
			this->cms_suspend->Size = System::Drawing::Size(148, 22);
			this->cms_suspend->Text = L"��ͣ�߳�����";
			this->cms_suspend->Click += gcnew System::EventHandler(this, &ProcessThreadForm::cms_suspend_Click);
			// 
			// cms_resume
			// 
			this->cms_resume->Name = L"cms_resume";
			this->cms_resume->Size = System::Drawing::Size(148, 22);
			this->cms_resume->Text = L"�ָ��߳�����";
			this->cms_resume->Click += gcnew System::EventHandler(this, &ProcessThreadForm::cms_resume_Click);
			// 
			// toolStripSeparator3
			// 
			this->toolStripSeparator3->Name = L"toolStripSeparator3";
			this->toolStripSeparator3->Size = System::Drawing::Size(145, 6);
			// 
			// cms_export
			// 
			this->cms_export->Name = L"cms_export";
			this->cms_export->Size = System::Drawing::Size(148, 22);
			this->cms_export->Text = L"����";
			this->cms_export->Click += gcnew System::EventHandler(this, &ProcessThreadForm::cms_export_Click);
			// 
			// ProcessThreadForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(597, 283);
			this->Controls->Add(this->lv_);
			this->MaximizeBox = false;
			this->Name = L"ProcessThreadForm";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"ProcessThreadForm";
			this->Load += gcnew System::EventHandler(this, &ProcessThreadForm::ProcessThreadForm_Load);
			this->cms_->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
		 
	     //
		 ULONG ulPID;
		 ULONG ulCount;
		 String^ str_;

		 HANDLE hDriver;

		 //ListViewItem�������裬��¼��ǰ��
		 int lv_item_curColumn; 
		 //��ʾ����������
		 bool lv_item_bAsc; 

		 Thread^ thread_sigverif;


		 //��������¼�
private: System::Void ProcessThreadForm_Load(System::Object^  sender, System::EventArgs^  e) {
			 try
			 {
				 ULONG ulBaseAddress;
				 CLIENT_ID clientID;
				 DWORD dwRet;
				 String^ str_fileName;
			     String^ str_CompName;

				 Process^ p = Process::GetProcessById(ulPID);
				 ProcessThreadCollection^ ptc_ = p->Threads;

				 ulCount =0;

				 //�õ�ģ����Ϣ
				 common_net::common_getProcessModuleInfo(ulPID);

				 for each(ProcessThread^ pt_ in ptc_)
				 {
					 try
					 {
						 ListViewItem^ item_ = gcnew ListViewItem(pt_->Id.ToString());

						 clientID.ulPID = ulPID;
						 clientID.ulTID = pt_->Id;

						 //��û�ַ
						 if(DeviceIoControl(hDriver, IOCTL_PROCESS_THREAD_GETBASEADDR, &clientID, sizeof(CLIENT_ID), &ulBaseAddress, sizeof(ULONG), &dwRet, NULL))
						 {
							 PPROCESS_MODULE_INFO p = common_net::common_getProcessModuleName(ulBaseAddress);

							 if (p)
							 {
								 str_fileName = gcnew String(p->FileName);
								 str_CompName = common_net::common_getFileCompanyName(gcnew String(p->FilePath));

								 //�������ж��ļ�����
								 if (!str_CompName->Contains("Microsoft Cor"))
								 {
								  item_->ForeColor=Color::Blue;
								 }
							 }		 
						 }

						 item_->SubItems->AddRange(gcnew array<String^>{pt_->CurrentPriority.ToString(),"0x" + ulBaseAddress.ToString("X8"),
							 str_fileName,pt_->StartTime.ToString(),pt_->ThreadState.ToString()});
						 lv_->Items->Add(item_);

						 item_->Tag = pt_;

						 ulCount++;
					 }
					 catch(Exception^ ex){ }
				 }	 

				 this->Text = "[" + str_ +"]�����߳�" + "[" + ulCount.ToString()+ "]";
			 }
			 catch(Exception^ ex){ }

			 //ListViewItem�������裬��¼��ǰ��
			 lv_item_curColumn = -1; 
			 //��ʾ����������
			 lv_item_bAsc = false; 
		 }

		 //��������Ҽ�
private: System::Void lv__MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if (lv_->Items->Count ==0)
			 {
				 cms_forceKill->Enabled = false;
				 cms_suspend->Enabled = false;
				 cms_resume->Enabled = false;
			 }
			 else if (lv_->SelectedItems->Count ==0)
			 {
				 cms_forceKill->Enabled = false;
				 cms_suspend->Enabled = false;
				 cms_resume->Enabled = false;
			 }
			 else
			 {
				 cms_forceKill->Enabled = true;

				 ProcessThread^ pt_ = (ProcessThread^)lv_->SelectedItems[0]->Tag;
				 if (pt_->WaitReason == ThreadWaitReason::Suspended)
				 {
					 cms_suspend->Enabled = false;
					 cms_resume->Enabled = true;
				 }
				 else
				 {
					 cms_suspend->Enabled = true;
					 cms_resume->Enabled = false;
				 }									 
			 }
		 }

		 //
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

				 if(e->Column ==0 || e->Column == 1)
				 {
					 //��������
					 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,0,10);
				 }
				 else if(e->Column == 2)
				 {
					 //��������
					 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,0,16);
				 }
				 else if(e->Column == 4)
				 {
					 //ʱ������
					 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,2,0);
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

				 if(e->Column ==0 || e->Column == 1)
				 {
					 //��������
					 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,0,10);
				 }
				 else if(e->Column == 2)
				 {
					 //��������
					 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,0,16);
				 }
				 else if(e->Column == 4)
				 {
					 //ʱ������
					 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,2,0);
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
			 ProcessThreadForm_Load(sender,e);
		 }

		 //ǿ�ƽ���
private: System::Void cms_forceKill_Click(System::Object^  sender, System::EventArgs^  e) {
			 KILLTHREAD_CLIENT_ID clientID;
			 BOOL bRet=FALSE;
			 DWORD dwRet=0;

			 ListViewItem^ item_ = lv_->SelectedItems[0];

			 clientID.ulPID = ulPID;
			 clientID.ulTID = int::Parse(item_->SubItems[0]->Text);

			 bRet = DeviceIoControl(hDriver, IOCTL_PROCESS_KILLTHREAD, &clientID, sizeof(KILLTHREAD_CLIENT_ID), NULL, 0, &dwRet, NULL);
			 if (!bRet)
			 {
				 MessageBox::Show("ǿ�ƽ���ʧ�ܣ�");
				 return;
			 }

			 //ˢ���б�
			 cms_refresh_Click(sender,e);
		 }

		 //����
private: System::Void cms_suspend_Click(System::Object^  sender, System::EventArgs^  e) {
			 BOOL bRet=FALSE;
			 DWORD dwRet=0;
			 ULONG ulTID;

			 ulTID = ((ProcessThread^)lv_->SelectedItems[0]->Tag)->Id;

			 bRet = DeviceIoControl(hDriver, IOCTL_PROCESS_THREAD_SUSPEND, &ulTID, sizeof(ULONG), NULL, 0, &dwRet, NULL);
			 if (!bRet)
			 {
				 MessageBox::Show("�����߳�ʧ�ܣ�");
				 return;
			 }

			 //ˢ���б�
			 cms_refresh_Click(sender,e);
		 }

		 //�ָ�
private: System::Void cms_resume_Click(System::Object^  sender, System::EventArgs^  e) {
			 BOOL bRet=FALSE;
			 DWORD dwRet=0;
			 ULONG ulTID;

			 ulTID = ((ProcessThread^)lv_->SelectedItems[0]->Tag)->Id;

			 bRet = DeviceIoControl(hDriver, IOCTL_PROCESS_THREAD_RESUME, &ulTID, sizeof(ULONG), NULL, 0, &dwRet, NULL);
			 if (!bRet)
			 {
				 MessageBox::Show("�ָ��߳�ʧ�ܣ�");
				 return;
			 }

			 //ˢ���б�
			 cms_refresh_Click(sender,e);
		 }

		 //����
private: System::Void cms_export_Click(System::Object^  sender, System::EventArgs^  e) {
			 common_net::common_getListViewData(lv_);
		 }


};
}
