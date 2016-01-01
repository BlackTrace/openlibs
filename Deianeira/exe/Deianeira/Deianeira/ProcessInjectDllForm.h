#pragma once
#include "..\common\process.h"
#include "..\common\common_net.h"

namespace Deianeira {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// ProcessInjectDllForm ժҪ
	/// </summary>
	public ref class ProcessInjectDllForm : public System::Windows::Forms::Form
	{
	public:
		ProcessInjectDllForm(ULONG ulPID,String^ str_name)
		{
			InitializeComponent();
			//
			//TODO: �ڴ˴���ӹ��캯������
			//
			this->ulPID = ulPID;
			this->str_name = str_name;
		}

	protected:
		/// <summary>
		/// ������������ʹ�õ���Դ��
		/// </summary>
		~ProcessInjectDllForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  txt_path;
	protected: 

	private: System::Windows::Forms::Button^  btnQuit;

	private: System::Windows::Forms::Button^  btnOK;

	private: System::Windows::Forms::Button^  btnBrowse;

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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->txt_path = (gcnew System::Windows::Forms::TextBox());
			this->btnQuit = (gcnew System::Windows::Forms::Button());
			this->btnOK = (gcnew System::Windows::Forms::Button());
			this->btnBrowse = (gcnew System::Windows::Forms::Button());
			this->OFD = (gcnew System::Windows::Forms::OpenFileDialog());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 27);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(53, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Dll·��:";
			// 
			// txt_path
			// 
			this->txt_path->Location = System::Drawing::Point(71, 23);
			this->txt_path->Name = L"txt_path";
			this->txt_path->Size = System::Drawing::Size(265, 21);
			this->txt_path->TabIndex = 1;
			// 
			// btnQuit
			// 
			this->btnQuit->Location = System::Drawing::Point(289, 57);
			this->btnQuit->Name = L"btnQuit";
			this->btnQuit->Size = System::Drawing::Size(75, 23);
			this->btnQuit->TabIndex = 2;
			this->btnQuit->Text = L"ȡ��";
			this->btnQuit->UseVisualStyleBackColor = true;
			this->btnQuit->Click += gcnew System::EventHandler(this, &ProcessInjectDllForm::btnQuit_Click);
			// 
			// btnOK
			// 
			this->btnOK->Location = System::Drawing::Point(192, 57);
			this->btnOK->Name = L"btnOK";
			this->btnOK->Size = System::Drawing::Size(75, 23);
			this->btnOK->TabIndex = 2;
			this->btnOK->Text = L"ȷ��";
			this->btnOK->UseVisualStyleBackColor = true;
			this->btnOK->Click += gcnew System::EventHandler(this, &ProcessInjectDllForm::btnOK_Click);
			// 
			// btnBrowse
			// 
			this->btnBrowse->Location = System::Drawing::Point(342, 22);
			this->btnBrowse->Name = L"btnBrowse";
			this->btnBrowse->Size = System::Drawing::Size(34, 23);
			this->btnBrowse->TabIndex = 3;
			this->btnBrowse->Text = L"...";
			this->btnBrowse->UseVisualStyleBackColor = true;
			this->btnBrowse->Click += gcnew System::EventHandler(this, &ProcessInjectDllForm::btnBrowse_Click);
			// 
			// ProcessInjectDllForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(392, 91);
			this->Controls->Add(this->btnBrowse);
			this->Controls->Add(this->btnOK);
			this->Controls->Add(this->btnQuit);
			this->Controls->Add(this->txt_path);
			this->Controls->Add(this->label1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"ProcessInjectDllForm";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"InjectDll-->>";
			this->Load += gcnew System::EventHandler(this, &ProcessInjectDllForm::ProcessInjectDllForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		 ULONG ulPID;
		 String^ str_name;

		 //��ý����ǳ�
private: System::Void ProcessInjectDllForm_Load(System::Object^  sender, System::EventArgs^  e) {
			 this->Text = this->Text+"["+str_name+"]";
		 }

		 //���
private: System::Void btnBrowse_Click(System::Object^  sender, System::EventArgs^  e) {
			 OFD->Filter="dll files(*.dll)|*.dll|all files(*.*)|*.*";
			 OFD->Multiselect=true;
			 //���ж�ѡ
			 if (OFD->ShowDialog()==System::Windows::Forms::DialogResult::OK)
			 {
				 //����ı���
				 txt_path->Clear();
				 for each(String^ str_ in OFD->FileNames)
				 {
					 txt_path->Text = txt_path->Text+str_+";";		 
				 }
			 }
		 }

		 //ȷ��
private: System::Void btnOK_Click(System::Object^  sender, System::EventArgs^  e) {
			 for each(String^ str_ in OFD->FileNames)
			 {
				 if (process_injectDll(common_net::common_stringToHGlobalUni(str_),ulPID))
				 {
					 MessageBox::Show(str_ + " ע��ɹ���");
				 }
				 else
				 {
					 MessageBox::Show(str_ + " ע��ʧ�ܣ�");
				 }
			 }
		 }

		 //ȷ��
private: System::Void btnQuit_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }

};
}
