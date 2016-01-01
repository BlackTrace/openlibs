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

	/// <summary>
	/// ProcessInjectDllByHookMsg2Form ժҪ
	/// </summary>
	public ref class ProcessInjectDllByHookMsg2Form : public System::Windows::Forms::Form
	{
	public:
		ProcessInjectDllByHookMsg2Form(void)
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
		~ProcessInjectDllByHookMsg2Form()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  txt_func;
	protected: 

	protected: 
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  btnBrowse;
	private: System::Windows::Forms::TextBox^  txt_path;


	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  btnQuit;
	private: System::Windows::Forms::Button^  btnOK;
	private: System::Windows::Forms::OpenFileDialog^  OFD;

	private:
		/// <summary>
		/// ����������������
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// �����֧������ķ��� - ��Ҫ
		/// ʹ�ô���༭���޸Ĵ˷��������ݡ�
		/// </summary>
		void InitializeComponent(void)
		{
			this->txt_func = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->btnBrowse = (gcnew System::Windows::Forms::Button());
			this->txt_path = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->btnQuit = (gcnew System::Windows::Forms::Button());
			this->btnOK = (gcnew System::Windows::Forms::Button());
			this->OFD = (gcnew System::Windows::Forms::OpenFileDialog());
			this->SuspendLayout();
			// 
			// txt_func
			// 
			this->txt_func->Location = System::Drawing::Point(15, 79);
			this->txt_func->Name = L"txt_func";
			this->txt_func->Size = System::Drawing::Size(167, 21);
			this->txt_func->TabIndex = 2;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(13, 64);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(137, 12);
			this->label2->TabIndex = 12;
			this->label2->Text = L"Dll��������(CallBack):";
			// 
			// btnBrowse
			// 
			this->btnBrowse->Location = System::Drawing::Point(317, 30);
			this->btnBrowse->Name = L"btnBrowse";
			this->btnBrowse->Size = System::Drawing::Size(30, 23);
			this->btnBrowse->TabIndex = 1;
			this->btnBrowse->Text = L"...";
			this->btnBrowse->UseVisualStyleBackColor = true;
			this->btnBrowse->Click += gcnew System::EventHandler(this, &ProcessInjectDllByHookMsg2Form::btnBrowse_Click);
			// 
			// txt_path
			// 
			this->txt_path->Location = System::Drawing::Point(15, 30);
			this->txt_path->Name = L"txt_path";
			this->txt_path->Size = System::Drawing::Size(296, 21);
			this->txt_path->TabIndex = 0;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(13, 15);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(53, 12);
			this->label1->TabIndex = 9;
			this->label1->Text = L"Dll·��:";
			// 
			// btnQuit
			// 
			this->btnQuit->Location = System::Drawing::Point(272, 112);
			this->btnQuit->Name = L"btnQuit";
			this->btnQuit->Size = System::Drawing::Size(75, 23);
			this->btnQuit->TabIndex = 4;
			this->btnQuit->Text = L"ȡ��";
			this->btnQuit->UseVisualStyleBackColor = true;
			this->btnQuit->Click += gcnew System::EventHandler(this, &ProcessInjectDllByHookMsg2Form::btnQuit_Click);
			// 
			// btnOK
			// 
			this->btnOK->Location = System::Drawing::Point(182, 112);
			this->btnOK->Name = L"btnOK";
			this->btnOK->Size = System::Drawing::Size(75, 23);
			this->btnOK->TabIndex = 3;
			this->btnOK->Text = L"ȷ��";
			this->btnOK->UseVisualStyleBackColor = true;
			this->btnOK->Click += gcnew System::EventHandler(this, &ProcessInjectDllByHookMsg2Form::btnOK_Click);
			// 
			// ProcessInjectDllByHookMsg2Form
			// 
			this->AcceptButton = this->btnOK;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(361, 150);
			this->Controls->Add(this->txt_func);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->btnBrowse);
			this->Controls->Add(this->txt_path);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->btnQuit);
			this->Controls->Add(this->btnOK);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"ProcessInjectDllByHookMsg2Form";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"Dll��Ϣ����ע��2";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		 //���
private: System::Void btnBrowse_Click(System::Object^  sender, System::EventArgs^  e) {
			 OFD->Filter="dll files(*.dll)|*.dll|all files(*.*)|*.*";
			 OFD->Multiselect=false;

			 //�򿪶Ի���
			 if (OFD->ShowDialog()==System::Windows::Forms::DialogResult::OK)
			 {
				 txt_path->Text = OFD->FileName;
			 }
		 }

		 //ȷ��
private: System::Void btnOK_Click(System::Object^  sender, System::EventArgs^  e) {
			 if (String::IsNullOrEmpty(txt_path->Text))
			 {
				 MessageBox::Show("DLL·������Ϊ�գ�");
				 return;
			 }
			 if (String::IsNullOrEmpty(txt_func->Text))
			 {
				 MessageBox::Show("DLL������������Ϊ�գ�");
				 return;
			 }

			 //����Ŀ��DLL
			 HMODULE hModule = LoadLibrary(common_net::common_stringToHGlobalUni(txt_path->Text));
			 //�������ʧ��
			 if (hModule==NULL)
			 {
				 MessageBox::Show("��װ��Ϣ����ʧ�ܣ�");
				 return;
			 }
			 //��ú�����ַ
			 FARPROC farproc_func = GetProcAddress(hModule,common_net::common_stringToHGlobalAnsi(txt_func->Text));
			 if (farproc_func==NULL)
			 {
				 MessageBox::Show("��װ��Ϣ����ʧ�ܣ�");
				 return;
			 }
			 
			 //��װ��Ϣ����
			 HHOOK hHook = SetWindowsHookEx(WH_GETMESSAGE,(HOOKPROC)farproc_func,hModule,0);
			 if (hHook==NULL)
			 {
				 MessageBox::Show("��װ��Ϣ����ʧ�ܣ�");
				 return;
			 }
		 }

		 //ȡ��
private: System::Void btnQuit_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }

};
}
