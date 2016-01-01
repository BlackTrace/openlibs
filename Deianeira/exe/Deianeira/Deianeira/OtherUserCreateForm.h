#pragma once
#include "..\common\common_net.h"

namespace Deianeira {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Management;

	/// <summary>
	/// OtherUserCreateForm ժҪ
	/// </summary>
	public ref class OtherUserCreateForm : public System::Windows::Forms::Form
	{
	public:
		OtherUserCreateForm(void)
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
		~OtherUserCreateForm()
		{
			if (components)
			{
				delete components;
			}
		}

	protected: 
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Button^  btnFolderBrowse;
	private: System::Windows::Forms::Button^  btnOK;

	private: System::Windows::Forms::TextBox^  txt_desc;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  txt_name;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::FolderBrowserDialog^  FBD;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::TextBox^  txt_path;
	private: System::Windows::Forms::Button^  btnQuit;
	private: System::Windows::Forms::ComboBox^  cbo_group;

	private: System::Windows::Forms::TextBox^  txt_pass;

	private: System::Windows::Forms::Label^  label3;

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
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->btnFolderBrowse = (gcnew System::Windows::Forms::Button());
			this->btnOK = (gcnew System::Windows::Forms::Button());
			this->txt_desc = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->txt_name = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->FBD = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->txt_path = (gcnew System::Windows::Forms::TextBox());
			this->btnQuit = (gcnew System::Windows::Forms::Button());
			this->cbo_group = (gcnew System::Windows::Forms::ComboBox());
			this->txt_pass = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(37, 133);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(41, 12);
			this->label6->TabIndex = 26;
			this->label6->Text = L"������";
			// 
			// btnFolderBrowse
			// 
			this->btnFolderBrowse->Location = System::Drawing::Point(385, 54);
			this->btnFolderBrowse->Name = L"btnFolderBrowse";
			this->btnFolderBrowse->Size = System::Drawing::Size(31, 23);
			this->btnFolderBrowse->TabIndex = 3;
			this->btnFolderBrowse->Text = L"...";
			this->btnFolderBrowse->UseVisualStyleBackColor = true;
			this->btnFolderBrowse->Click += gcnew System::EventHandler(this, &OtherUserCreateForm::btnFolderBrowse_Click);
			// 
			// btnOK
			// 
			this->btnOK->Location = System::Drawing::Point(247, 232);
			this->btnOK->Name = L"btnOK";
			this->btnOK->Size = System::Drawing::Size(75, 23);
			this->btnOK->TabIndex = 6;
			this->btnOK->Text = L"ȷ��";
			this->btnOK->UseVisualStyleBackColor = true;
			this->btnOK->Click += gcnew System::EventHandler(this, &OtherUserCreateForm::btnOK_Click);
			// 
			// txt_desc
			// 
			this->txt_desc->Location = System::Drawing::Point(84, 133);
			this->txt_desc->Multiline = true;
			this->txt_desc->Name = L"txt_desc";
			this->txt_desc->Size = System::Drawing::Size(295, 90);
			this->txt_desc->TabIndex = 5;
			this->txt_desc->Text = L"(\"��\"��ʾû������)";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(31, 59);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 12);
			this->label2->TabIndex = 20;
			this->label2->Text = L"��Ŀ¼��";
			// 
			// txt_name
			// 
			this->txt_name->Location = System::Drawing::Point(84, 20);
			this->txt_name->Name = L"txt_name";
			this->txt_name->Size = System::Drawing::Size(100, 21);
			this->txt_name->TabIndex = 0;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(31, 24);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(53, 12);
			this->label1->TabIndex = 17;
			this->label1->Text = L"�û�����";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(31, 97);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(53, 12);
			this->label4->TabIndex = 24;
			this->label4->Text = L"�û��飺";
			// 
			// txt_path
			// 
			this->txt_path->Location = System::Drawing::Point(84, 55);
			this->txt_path->Name = L"txt_path";
			this->txt_path->Size = System::Drawing::Size(295, 21);
			this->txt_path->TabIndex = 2;
			// 
			// btnQuit
			// 
			this->btnQuit->Location = System::Drawing::Point(341, 232);
			this->btnQuit->Name = L"btnQuit";
			this->btnQuit->Size = System::Drawing::Size(75, 23);
			this->btnQuit->TabIndex = 7;
			this->btnQuit->Text = L"ȡ��";
			this->btnQuit->UseVisualStyleBackColor = true;
			this->btnQuit->Click += gcnew System::EventHandler(this, &OtherUserCreateForm::btnQuit_Click);
			// 
			// cbo_group
			// 
			this->cbo_group->FormattingEnabled = true;
			this->cbo_group->Location = System::Drawing::Point(84, 93);
			this->cbo_group->Name = L"cbo_group";
			this->cbo_group->Size = System::Drawing::Size(132, 20);
			this->cbo_group->TabIndex = 4;
			// 
			// txt_pass
			// 
			this->txt_pass->Location = System::Drawing::Point(247, 20);
			this->txt_pass->Name = L"txt_pass";
			this->txt_pass->Size = System::Drawing::Size(100, 21);
			this->txt_pass->TabIndex = 1;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(200, 24);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(41, 12);
			this->label3->TabIndex = 30;
			this->label3->Text = L"���룺";
			// 
			// OtherUserCreateForm
			// 
			this->AcceptButton = this->btnOK;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(443, 274);
			this->Controls->Add(this->txt_pass);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->cbo_group);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->btnFolderBrowse);
			this->Controls->Add(this->btnOK);
			this->Controls->Add(this->txt_desc);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->txt_name);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->txt_path);
			this->Controls->Add(this->btnQuit);
			this->MaximizeBox = false;
			this->Name = L"OtherUserCreateForm";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"�����û�";
			this->Load += gcnew System::EventHandler(this, &OtherUserCreateForm::OtherUserCreateForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	
		 //�����¼�
private: System::Void OtherUserCreateForm_Load(System::Object^  sender, System::EventArgs^  e) {
			 ManagementObjectSearcher^ mos = gcnew ManagementObjectSearcher("select Name from Win32_Group"); 
			 for each(ManagementObject^ mo in mos->Get())   
			 {
				 cbo_group->Items->Add(mo["Name"]->ToString());
			 }
		 }

		 //���
private: System::Void btnFolderBrowse_Click(System::Object^  sender, System::EventArgs^  e) {
			 FBD->ShowNewFolderButton = true;
			 if (FBD->ShowDialog()==System::Windows::Forms::DialogResult::OK)
			 {
				 txt_path->Text = FBD->SelectedPath;
			 }
		 }

		 //ȷ��
private: System::Void btnOK_Click(System::Object^  sender, System::EventArgs^  e) {
			 if (String::IsNullOrEmpty(txt_name->Text))
			 {
				 MessageBox::Show("�û�������Ϊ�գ�");
				 txt_name->Focus();
				 return;
			 }
			 if (String::IsNullOrEmpty(txt_path->Text))
			 {
				 MessageBox::Show("�û���Ŀ¼����Ϊ�գ�");
				 txt_path->Focus();
				 return;
			 }
			 //�����û�
			 if(common_net::common_createUser(txt_name->Text,txt_pass->Text,txt_path->Text,cbo_group->Text,txt_desc->Text))
			 {
				 if (MessageBox::Show("�����ɹ���")==System::Windows::Forms::DialogResult::OK)
				 {
					 this->Close();
				 }	 
			 }
			 else
			 {
				 MessageBox::Show("����δ֪����");
			 }
		 }

		 //ȡ��
private: System::Void btnQuit_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }

};
}
