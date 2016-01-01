#pragma once

namespace Deianeira {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace Microsoft::Win32;

	/// <summary>
	/// reg_string_change_form ժҪ
	/// </summary>
	public ref class reg_string_change_form : public System::Windows::Forms::Form
	{
	public:
		reg_string_change_form(ListViewItem^ item_)
		{
			InitializeComponent();
			//
			//TODO: �ڴ˴���ӹ��캯������
			//
			this->item_ = item_;
		}

	protected:
		/// <summary>
		/// ������������ʹ�õ���Դ��
		/// </summary>
		~reg_string_change_form()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  txt_name;
	private: System::Windows::Forms::TextBox^  txt_value;
	protected: 


	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  btnOK;
	private: System::Windows::Forms::Button^  btnQuit;



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
			this->txt_name = (gcnew System::Windows::Forms::TextBox());
			this->txt_value = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->btnOK = (gcnew System::Windows::Forms::Button());
			this->btnQuit = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(77, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"��ֵ����(N):";
			// 
			// txt_name
			// 
			this->txt_name->Location = System::Drawing::Point(14, 25);
			this->txt_name->Name = L"txt_name";
			this->txt_name->ReadOnly = true;
			this->txt_name->Size = System::Drawing::Size(356, 21);
			this->txt_name->TabIndex = 3;
			// 
			// txt_value
			// 
			this->txt_value->Location = System::Drawing::Point(14, 69);
			this->txt_value->Name = L"txt_value";
			this->txt_value->Size = System::Drawing::Size(356, 21);
			this->txt_value->TabIndex = 0;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 53);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(77, 12);
			this->label2->TabIndex = 2;
			this->label2->Text = L"��ֵ����(V):";
			// 
			// btnOK
			// 
			this->btnOK->Location = System::Drawing::Point(203, 99);
			this->btnOK->Name = L"btnOK";
			this->btnOK->Size = System::Drawing::Size(75, 23);
			this->btnOK->TabIndex = 1;
			this->btnOK->Text = L"ȷ��";
			this->btnOK->UseVisualStyleBackColor = true;
			this->btnOK->Click += gcnew System::EventHandler(this, &reg_string_change_form::btnOK_Click);
			// 
			// btnQuit
			// 
			this->btnQuit->Location = System::Drawing::Point(295, 99);
			this->btnQuit->Name = L"btnQuit";
			this->btnQuit->Size = System::Drawing::Size(75, 23);
			this->btnQuit->TabIndex = 2;
			this->btnQuit->Text = L"ȡ��";
			this->btnQuit->UseVisualStyleBackColor = true;
			this->btnQuit->Click += gcnew System::EventHandler(this, &reg_string_change_form::btnQuit_Click);
			// 
			// reg_string_change_form
			// 
			this->AcceptButton = this->btnOK;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(382, 131);
			this->Controls->Add(this->btnQuit);
			this->Controls->Add(this->btnOK);
			this->Controls->Add(this->txt_value);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->txt_name);
			this->Controls->Add(this->label1);
			this->MaximizeBox = false;
			this->Name = L"reg_string_change_form";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"�༭�ַ���";
			this->Load += gcnew System::EventHandler(this, &reg_string_change_form::reg_string_change_form_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	
		 //
		 static ListViewItem^ item_ = nullptr;

		 //ȡ��
private: System::Void btnQuit_Click(System::Object^  sender, System::EventArgs^  e) {
			//
			 this->Close();
		 }

		 //ȷ��
private: System::Void btnOK_Click(System::Object^  sender, System::EventArgs^  e) {
			 try
			 {
				 //
				 RegistryKey^ rk_ = (RegistryKey^)item_->Tag;
				 //
				 if (item_->SubItems[1]->Text == "REG_EXPAND_SZ")
				 {
					 //
					 rk_->SetValue((txt_name->Text == "(Ĭ��)" ? "" : txt_name->Text),txt_value->Text,RegistryValueKind::ExpandString);
				 }
				 else
				 {
					 //
					 rk_->SetValue((txt_name->Text == "(Ĭ��)" ? "" : txt_name->Text),txt_value->Text);
				 }
			     //����item
				 item_->SubItems[2]->Text = txt_value->Text;
				 //
				 this->Close();
			 }
			 catch(Exception^ ex){ }		 
		 }

		 //
private: System::Void reg_string_change_form_Load(System::Object^  sender, System::EventArgs^  e) {
			 //
			 txt_name->Text = item_->Text;
			 //
			 RegistryKey^ rk_ = (RegistryKey^)item_->Tag;
			 //
			 Object^ obj_ = rk_->GetValue((txt_name->Text == "(Ĭ��)" ? "" : txt_name->Text),nullptr,RegistryValueOptions::DoNotExpandEnvironmentNames);
			 //
			 if (obj_ == nullptr)
			 {
				 //
				 txt_value->Text = "";
			 }
			 else
			 {
				 //
				 txt_value->Text = obj_->ToString();
			 }
		 }
};
}
