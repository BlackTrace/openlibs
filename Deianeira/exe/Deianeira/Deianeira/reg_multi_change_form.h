#pragma once

namespace Deianeira {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// reg_multi_change_form ժҪ
	/// </summary>
	public ref class reg_multi_change_form : public System::Windows::Forms::Form
	{
	public:
		reg_multi_change_form(ListViewItem^ item_)
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
		~reg_multi_change_form()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  btnQuit;
	protected: 
	private: System::Windows::Forms::Button^  btnOK;
	private: System::Windows::Forms::TextBox^  txt_value;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  txt_name;
	private: System::Windows::Forms::Label^  label1;

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
			this->btnQuit = (gcnew System::Windows::Forms::Button());
			this->btnOK = (gcnew System::Windows::Forms::Button());
			this->txt_value = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->txt_name = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// btnQuit
			// 
			this->btnQuit->Location = System::Drawing::Point(267, 241);
			this->btnQuit->Name = L"btnQuit";
			this->btnQuit->Size = System::Drawing::Size(75, 23);
			this->btnQuit->TabIndex = 2;
			this->btnQuit->Text = L"ȡ��";
			this->btnQuit->UseVisualStyleBackColor = true;
			this->btnQuit->Click += gcnew System::EventHandler(this, &reg_multi_change_form::btnQuit_Click);
			// 
			// btnOK
			// 
			this->btnOK->Location = System::Drawing::Point(180, 241);
			this->btnOK->Name = L"btnOK";
			this->btnOK->Size = System::Drawing::Size(75, 23);
			this->btnOK->TabIndex = 1;
			this->btnOK->Text = L"ȷ��";
			this->btnOK->UseVisualStyleBackColor = true;
			this->btnOK->Click += gcnew System::EventHandler(this, &reg_multi_change_form::btnOK_Click);
			// 
			// txt_value
			// 
			this->txt_value->Location = System::Drawing::Point(12, 72);
			this->txt_value->Multiline = true;
			this->txt_value->Name = L"txt_value";
			this->txt_value->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->txt_value->Size = System::Drawing::Size(330, 162);
			this->txt_value->TabIndex = 0;
			this->txt_value->WordWrap = false;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(10, 56);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(77, 12);
			this->label2->TabIndex = 8;
			this->label2->Text = L"��ֵ����(V):";
			// 
			// txt_name
			// 
			this->txt_name->Location = System::Drawing::Point(12, 28);
			this->txt_name->Name = L"txt_name";
			this->txt_name->ReadOnly = true;
			this->txt_name->Size = System::Drawing::Size(330, 21);
			this->txt_name->TabIndex = 3;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(10, 12);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(77, 12);
			this->label1->TabIndex = 5;
			this->label1->Text = L"��ֵ����(N):";
			// 
			// reg_multi_change_form
			// 
			this->AcceptButton = this->btnOK;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(354, 271);
			this->Controls->Add(this->btnQuit);
			this->Controls->Add(this->btnOK);
			this->Controls->Add(this->txt_value);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->txt_name);
			this->Controls->Add(this->label1);
			this->Name = L"reg_multi_change_form";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"�༭���ַ���";
			this->Load += gcnew System::EventHandler(this, &reg_multi_change_form::reg_multi_change_form_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	
		 //
		 static ListViewItem^ item_ = nullptr;

		 //
private: System::Void btnOK_Click(System::Object^  sender, System::EventArgs^  e) {
			 //

		 }

		 //
private: System::Void btnQuit_Click(System::Object^  sender, System::EventArgs^  e) {
			 //
			 this->Close();
		 }

		 //
private: System::Void reg_multi_change_form_Load(System::Object^  sender, System::EventArgs^  e) {
			 //
			 txt_name->Text = item_->Text;
		 }
};
}
