#pragma once
#include "..\common\common.h"
#include "..\common\common_net.h"
#include "pe.h"

namespace Deianeira {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Text::RegularExpressions;

	/// <summary>
	/// other_table_infect_form ժҪ
	/// </summary>
	public ref class other_table_infect_form : public System::Windows::Forms::Form
	{
	public:
		other_table_infect_form(void)
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
		~other_table_infect_form()
		{
			if (components)
			{
				delete components;
			}
		}

	protected: 

	protected: 










	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::TextBox^  txtShellCode2;

	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tp_eat;
	private: System::Windows::Forms::TabPage^  tp_shellcode;



	private: System::Windows::Forms::GroupBox^  groupBox3;
	private: System::Windows::Forms::RadioButton^  rdoYes;
	private: System::Windows::Forms::RadioButton^  rdoNo;


	private: System::Windows::Forms::Button^  btnFileView;
	private: System::Windows::Forms::TextBox^  txtFuncName;


	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TextBox^  txtPath;

	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::TextBox^  txtShellCode1;
	private: System::Windows::Forms::Button^  btnFileView2;

	private: System::Windows::Forms::TextBox^  txtPath2;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  txtDllName;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::OpenFileDialog^  OFD;
	private: System::Windows::Forms::Button^  btnQuit;
	private: System::Windows::Forms::Button^  btnInfect;
	private: System::Windows::Forms::Button^  btnQuit2;
	private: System::Windows::Forms::Button^  btnInfect2;




	protected: 



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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(other_table_infect_form::typeid));
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->txtShellCode2 = (gcnew System::Windows::Forms::TextBox());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tp_eat = (gcnew System::Windows::Forms::TabPage());
			this->btnQuit = (gcnew System::Windows::Forms::Button());
			this->btnInfect = (gcnew System::Windows::Forms::Button());
			this->txtDllName = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->txtShellCode1 = (gcnew System::Windows::Forms::TextBox());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->rdoYes = (gcnew System::Windows::Forms::RadioButton());
			this->rdoNo = (gcnew System::Windows::Forms::RadioButton());
			this->btnFileView = (gcnew System::Windows::Forms::Button());
			this->txtFuncName = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->txtPath = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->tp_shellcode = (gcnew System::Windows::Forms::TabPage());
			this->btnQuit2 = (gcnew System::Windows::Forms::Button());
			this->btnInfect2 = (gcnew System::Windows::Forms::Button());
			this->btnFileView2 = (gcnew System::Windows::Forms::Button());
			this->txtPath2 = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->OFD = (gcnew System::Windows::Forms::OpenFileDialog());
			this->groupBox2->SuspendLayout();
			this->tabControl1->SuspendLayout();
			this->tp_eat->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->tp_shellcode->SuspendLayout();
			this->SuspendLayout();
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->txtShellCode2);
			this->groupBox2->Location = System::Drawing::Point(32, 48);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(389, 227);
			this->groupBox2->TabIndex = 5;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"�Զ���ShellCode";
			// 
			// txtShellCode2
			// 
			this->txtShellCode2->AllowDrop = true;
			this->txtShellCode2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->txtShellCode2->Location = System::Drawing::Point(3, 17);
			this->txtShellCode2->Multiline = true;
			this->txtShellCode2->Name = L"txtShellCode2";
			this->txtShellCode2->Size = System::Drawing::Size(383, 207);
			this->txtShellCode2->TabIndex = 3;
			this->txtShellCode2->Text = L"0x6A0x00\r\n0xB80x120xCB0x810x7C\r\n0xFF0xD0";
			this->txtShellCode2->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &other_table_infect_form::txtShellCode2_DragDrop);
			this->txtShellCode2->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &other_table_infect_form::txtShellCode2_DragEnter);
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tp_eat);
			this->tabControl1->Controls->Add(this->tp_shellcode);
			this->tabControl1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControl1->Location = System::Drawing::Point(0, 0);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(461, 357);
			this->tabControl1->TabIndex = 6;
			// 
			// tp_eat
			// 
			this->tp_eat->BackColor = System::Drawing::SystemColors::Window;
			this->tp_eat->Controls->Add(this->btnQuit);
			this->tp_eat->Controls->Add(this->btnInfect);
			this->tp_eat->Controls->Add(this->txtDllName);
			this->tp_eat->Controls->Add(this->label2);
			this->tp_eat->Controls->Add(this->groupBox1);
			this->tp_eat->Controls->Add(this->groupBox3);
			this->tp_eat->Controls->Add(this->btnFileView);
			this->tp_eat->Controls->Add(this->txtFuncName);
			this->tp_eat->Controls->Add(this->label3);
			this->tp_eat->Controls->Add(this->txtPath);
			this->tp_eat->Controls->Add(this->label4);
			this->tp_eat->Location = System::Drawing::Point(4, 21);
			this->tp_eat->Name = L"tp_eat";
			this->tp_eat->Padding = System::Windows::Forms::Padding(3);
			this->tp_eat->Size = System::Drawing::Size(453, 332);
			this->tp_eat->TabIndex = 0;
			this->tp_eat->Text = L"������Ⱦ";
			// 
			// btnQuit
			// 
			this->btnQuit->Location = System::Drawing::Point(235, 290);
			this->btnQuit->Name = L"btnQuit";
			this->btnQuit->Size = System::Drawing::Size(75, 23);
			this->btnQuit->TabIndex = 14;
			this->btnQuit->Text = L"ȡ��";
			this->btnQuit->UseVisualStyleBackColor = true;
			this->btnQuit->Click += gcnew System::EventHandler(this, &other_table_infect_form::btnQuit_Click);
			// 
			// btnInfect
			// 
			this->btnInfect->Location = System::Drawing::Point(343, 290);
			this->btnInfect->Name = L"btnInfect";
			this->btnInfect->Size = System::Drawing::Size(75, 23);
			this->btnInfect->TabIndex = 15;
			this->btnInfect->Text = L"��Ⱦ";
			this->btnInfect->UseVisualStyleBackColor = true;
			this->btnInfect->Click += gcnew System::EventHandler(this, &other_table_infect_form::btnInfect_Click);
			// 
			// txtDllName
			// 
			this->txtDllName->AllowDrop = true;
			this->txtDllName->Location = System::Drawing::Point(118, 51);
			this->txtDllName->Name = L"txtDllName";
			this->txtDllName->Size = System::Drawing::Size(110, 21);
			this->txtDllName->TabIndex = 2;
			this->txtDllName->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &other_table_infect_form::txtDllName_DragDrop);
			this->txtDllName->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &other_table_infect_form::txtDllName_DragEnter);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(41, 55);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 12);
			this->label2->TabIndex = 13;
			this->label2->Text = L"DLL����:";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->txtShellCode1);
			this->groupBox1->Location = System::Drawing::Point(43, 170);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(375, 102);
			this->groupBox1->TabIndex = 11;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Ĭ��ShellCode";
			// 
			// txtShellCode1
			// 
			this->txtShellCode1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->txtShellCode1->Location = System::Drawing::Point(3, 17);
			this->txtShellCode1->Multiline = true;
			this->txtShellCode1->Name = L"txtShellCode1";
			this->txtShellCode1->Size = System::Drawing::Size(369, 82);
			this->txtShellCode1->TabIndex = 6;
			this->txtShellCode1->Text = resources->GetString(L"txtShellCode1.Text");
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->rdoYes);
			this->groupBox3->Controls->Add(this->rdoNo);
			this->groupBox3->Location = System::Drawing::Point(43, 114);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(375, 45);
			this->groupBox3->TabIndex = 10;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"�Ƿ�ִ�е���������ת";
			// 
			// rdoYes
			// 
			this->rdoYes->AutoSize = true;
			this->rdoYes->Location = System::Drawing::Point(101, 20);
			this->rdoYes->Name = L"rdoYes";
			this->rdoYes->Size = System::Drawing::Size(35, 16);
			this->rdoYes->TabIndex = 4;
			this->rdoYes->Text = L"��";
			this->rdoYes->UseVisualStyleBackColor = true;
			// 
			// rdoNo
			// 
			this->rdoNo->AutoSize = true;
			this->rdoNo->Checked = true;
			this->rdoNo->Location = System::Drawing::Point(232, 20);
			this->rdoNo->Name = L"rdoNo";
			this->rdoNo->Size = System::Drawing::Size(35, 16);
			this->rdoNo->TabIndex = 5;
			this->rdoNo->TabStop = true;
			this->rdoNo->Text = L"��";
			this->rdoNo->UseVisualStyleBackColor = true;
			// 
			// btnFileView
			// 
			this->btnFileView->Location = System::Drawing::Point(375, 17);
			this->btnFileView->Name = L"btnFileView";
			this->btnFileView->Size = System::Drawing::Size(36, 23);
			this->btnFileView->TabIndex = 1;
			this->btnFileView->Text = L"...";
			this->btnFileView->UseVisualStyleBackColor = true;
			this->btnFileView->Click += gcnew System::EventHandler(this, &other_table_infect_form::btnFileView_Click);
			// 
			// txtFuncName
			// 
			this->txtFuncName->Location = System::Drawing::Point(118, 85);
			this->txtFuncName->Name = L"txtFuncName";
			this->txtFuncName->Size = System::Drawing::Size(157, 21);
			this->txtFuncName->TabIndex = 3;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(41, 88);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(59, 12);
			this->label3->TabIndex = 5;
			this->label3->Text = L"��������:";
			// 
			// txtPath
			// 
			this->txtPath->AllowDrop = true;
			this->txtPath->Location = System::Drawing::Point(118, 18);
			this->txtPath->Name = L"txtPath";
			this->txtPath->Size = System::Drawing::Size(251, 21);
			this->txtPath->TabIndex = 0;
			this->txtPath->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &other_table_infect_form::txtPath_DragDrop);
			this->txtPath->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &other_table_infect_form::txtPath_DragEnter);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(41, 22);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(71, 12);
			this->label4->TabIndex = 6;
			this->label4->Text = L"PE�ļ�·��:";
			// 
			// tp_shellcode
			// 
			this->tp_shellcode->BackColor = System::Drawing::SystemColors::Window;
			this->tp_shellcode->Controls->Add(this->btnQuit2);
			this->tp_shellcode->Controls->Add(this->btnInfect2);
			this->tp_shellcode->Controls->Add(this->btnFileView2);
			this->tp_shellcode->Controls->Add(this->txtPath2);
			this->tp_shellcode->Controls->Add(this->label1);
			this->tp_shellcode->Controls->Add(this->groupBox2);
			this->tp_shellcode->Location = System::Drawing::Point(4, 21);
			this->tp_shellcode->Name = L"tp_shellcode";
			this->tp_shellcode->Padding = System::Windows::Forms::Padding(3);
			this->tp_shellcode->Size = System::Drawing::Size(453, 332);
			this->tp_shellcode->TabIndex = 1;
			this->tp_shellcode->Text = L"ShellCodeע��";
			// 
			// btnQuit2
			// 
			this->btnQuit2->Location = System::Drawing::Point(235, 290);
			this->btnQuit2->Name = L"btnQuit2";
			this->btnQuit2->Size = System::Drawing::Size(75, 23);
			this->btnQuit2->TabIndex = 16;
			this->btnQuit2->Text = L"ȡ��";
			this->btnQuit2->UseVisualStyleBackColor = true;
			this->btnQuit2->Click += gcnew System::EventHandler(this, &other_table_infect_form::btnQuit2_Click);
			// 
			// btnInfect2
			// 
			this->btnInfect2->Location = System::Drawing::Point(343, 290);
			this->btnInfect2->Name = L"btnInfect2";
			this->btnInfect2->Size = System::Drawing::Size(75, 23);
			this->btnInfect2->TabIndex = 17;
			this->btnInfect2->Text = L"��Ⱦ";
			this->btnInfect2->UseVisualStyleBackColor = true;
			this->btnInfect2->Click += gcnew System::EventHandler(this, &other_table_infect_form::btnInfect2_Click);
			// 
			// btnFileView2
			// 
			this->btnFileView2->Location = System::Drawing::Point(375, 17);
			this->btnFileView2->Name = L"btnFileView2";
			this->btnFileView2->Size = System::Drawing::Size(36, 23);
			this->btnFileView2->TabIndex = 2;
			this->btnFileView2->Text = L"...";
			this->btnFileView2->UseVisualStyleBackColor = true;
			this->btnFileView2->Click += gcnew System::EventHandler(this, &other_table_infect_form::btnFileView2_Click);
			// 
			// txtPath2
			// 
			this->txtPath2->AllowDrop = true;
			this->txtPath2->Location = System::Drawing::Point(118, 18);
			this->txtPath2->Name = L"txtPath2";
			this->txtPath2->Size = System::Drawing::Size(251, 21);
			this->txtPath2->TabIndex = 1;
			this->txtPath2->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &other_table_infect_form::txtPath2_DragDrop);
			this->txtPath2->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &other_table_infect_form::txtPath2_DragEnter);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(41, 22);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(71, 12);
			this->label1->TabIndex = 10;
			this->label1->Text = L"PE�ļ�·��:";
			// 
			// other_table_infect_form
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(461, 357);
			this->Controls->Add(this->tabControl1);
			this->MaximizeBox = false;
			this->Name = L"other_table_infect_form";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"PE�ļ���Ⱦ";
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->tabControl1->ResumeLayout(false);
			this->tp_eat->ResumeLayout(false);
			this->tp_eat->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->tp_shellcode->ResumeLayout(false);
			this->tp_shellcode->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

	
	     //���ļ�
private: System::Void btnFileView_Click(System::Object^  sender, System::EventArgs^  e) {
			OFD->Multiselect = false;
			OFD->Filter = "exe files|*.exe|all files|*.*";
			//
			if (OFD->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				txtPath->Text = OFD->FileName;
			}
		 }

		 //
private: System::Void btnFileView2_Click(System::Object^  sender, System::EventArgs^  e) {
			 OFD->Multiselect = false;
			 OFD->Filter = "exe files|*.exe|all files|*.*";
			 //
			 if (OFD->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			 {
				 txtPath2->Text = OFD->FileName;
			 }
		 }

		 //
private: System::Void btnQuit_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }

		 //��Ⱦ
private: System::Void btnInfect_Click(System::Object^  sender, System::EventArgs^  e) {
			 if (String::IsNullOrEmpty(txtPath->Text))
			 {
				 MessageBox::Show("PE�ļ�·������Ϊ�գ�");
				 txtPath->Focus();
				 return;
			 }

			 //
			 if (String::IsNullOrEmpty(txtDllName->Text))
			 {
				 MessageBox::Show("DLL���Ʋ���Ϊ�գ�");
				 txtDllName->Focus();
				 return;
			 }

			 //���Ը�ʽ
			 if (!Regex::IsMatch(txtDllName->Text,common_net::common_getRegexEscapeString("*.dll")))
			 {
				 MessageBox::Show("dll��ʽ�������������룡");
				 txtDllName->Focus();
				 return;
			 }

			 //
			 if (String::IsNullOrEmpty(txtFuncName->Text))
			 {
				 MessageBox::Show("�����������Ʋ���Ϊ�գ�");
				 txtFuncName->Focus();
				 return;
			 }

			 //�ж��ǲ���PE�ļ�
			 if (!common_isPEFile(common_net::common_stringToHGlobalUni(txtPath->Text)))
			 {
				 MessageBox::Show("���Ǳ�׼PE�ļ���");
				 txtPath->Focus();
				 return;
			 }

			 //���ȱ����ļ�
			 File::Copy(txtPath->Text,((gcnew FileInfo(txtPath->Text))->Directory->FullName) + "\\" + (gcnew FileInfo(txtPath->Text))->Name + ".bak",true);

			 //�ж��Ƿ���Ҫ�޸�OEP
			 if (rdoYes->Checked)
			 {
				 //�����￪ʼ��Ⱦ
				 ppe_retn_msg p_msg = new pe_retn_msg;

				 //���Ŀ��������Ѿ�������Ŀ��dll�Ļ���������Ͳ�Ҫ����dll�ˣ�ʡ�Ķ���
				 pe_findDllModule(common_net::common_stringToHGlobalUni(txtPath->Text),
					 common_net::common_stringToHGlobalAnsi(txtDllName->Text),p_msg);

				 //����ҵ���Ŀ��ģ��Ļ�
				 if (p_msg->isSuccessed)
				 {
					 //����ɹ�
					 if (p_msg->isSuccessed)
					 {
						 pe_infect(common_net::common_stringToHGlobalUni(txtPath->Text),
							 common_net::common_stringToHGlobalUni(txtDllName->Text),
							 common_net::common_stringToHGlobalAnsi(txtFuncName->Text),p_msg);

						 //
						 if (p_msg->isSuccessed)
						 {
							 MessageBox::Show("��Ⱦ�ɹ���");
							 return;
						 }
					 }
				 }
				 else if(gcnew String(p_msg->tsMsg) == "no error!")
				 {
					 //��������dll
					 pe_infect_eat(common_net::common_stringToHGlobalUni(txtPath->Text),
						 common_net::common_stringToHGlobalAnsi(txtDllName->Text),
						 common_net::common_stringToHGlobalAnsi(txtFuncName->Text),p_msg);

					 //����ɹ�
					 if (p_msg->isSuccessed)
					 {
						 pe_infect(common_net::common_stringToHGlobalUni(txtPath->Text),
							 common_net::common_stringToHGlobalUni(txtDllName->Text),
							 common_net::common_stringToHGlobalAnsi(txtFuncName->Text),p_msg);

						 //
						 if (p_msg->isSuccessed)
						 {
							 MessageBox::Show("��Ⱦ�ɹ���");
							 return;
						 }
					 }
				 }

				 //��Ⱦʧ�ܵ���������Ϣ
				 MessageBox::Show(gcnew String(p_msg->tsMsg));

				 //
				 free(p_msg);

				 return;
			 }
			 else
			 {
				 //�����￪ʼ��Ⱦ
				 ppe_retn_msg p_msg = new pe_retn_msg;

				 //����dll
				 pe_infect_eat(common_net::common_stringToHGlobalUni(txtPath->Text),
					 common_net::common_stringToHGlobalAnsi(txtDllName->Text),
					 common_net::common_stringToHGlobalAnsi(txtFuncName->Text),p_msg);

				 //����ɹ�
				 //
				 if (p_msg->isSuccessed)
				 {
					 MessageBox::Show("��Ⱦ�ɹ���");
					 return;
				 }

				 //��Ⱦʧ�ܵ���������Ϣ
				 MessageBox::Show(gcnew String(p_msg->tsMsg));

				 //
				 free(p_msg);

				 return;
			 }
		 }

		 //
private: System::Void btnQuit2_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }

		 //
private: System::Void btnInfect2_Click(System::Object^  sender, System::EventArgs^  e) {
			 if (String::IsNullOrEmpty(txtShellCode2->Text))
			 {
				 MessageBox::Show("shellcode����Ϊ�գ�");
				 txtShellCode2->Focus();
				 return;
			 }

			 if (String::IsNullOrEmpty(txtPath2->Text))
			 {
				 MessageBox::Show("PE�ļ�·������Ϊ�գ�");
				 txtPath2->Focus();
				 return;
			 }

			 //���ȱ����ļ�
			 File::Copy(txtPath2->Text,((gcnew FileInfo(txtPath2->Text))->Directory->FullName) + "\\" + (gcnew FileInfo(txtPath2->Text))->Name + ".bak",true);

			 //�����￪ʼ��Ⱦ
			 ppe_retn_msg p_msg = new pe_retn_msg;

			 //���shellcode
			 String^ str_shellcode = String::Empty;

			 for each(String^ str_ in txtShellCode2->Lines)
			 {
				 str_shellcode = str_shellcode + str_;
			 }

			 //ת��
			 array<String^>^ strs_code = str_shellcode->ToLower()->Split(gcnew array<String^>{ "0x" },StringSplitOptions::RemoveEmptyEntries);

			 //code
			 PBYTE code = new byte[strs_code->Length];
			 memset(code,0,strs_code->Length);

			 for(int i = 0; i < strs_code->Length; i++)
			 {
				 code[i] = Convert::ToByte(strs_code[i],16);
			 }

			 //����dll
			 pe_infect(common_net::common_stringToHGlobalUni(txtPath2->Text),
				 code,p_msg,strs_code->Length);

			 //����ɹ�
			 if (p_msg->isSuccessed)
			 {
				 MessageBox::Show("��Ⱦ�ɹ���");
				 return;
			 }

			 //��Ⱦʧ�ܵ���������Ϣ
			 MessageBox::Show(gcnew String(p_msg->tsMsg));

			 //
			 free(p_msg);

			 return;
		 }

		 //
private: System::Void txtPath_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) {
			 if (e->Data->GetDataPresent(DataFormats::FileDrop))
			 {
				 e->Effect = DragDropEffects::Link;
			 }
			 else
			 {
				 e->Effect = DragDropEffects::None;
			 }
		 }

		 //
private: System::Void txtPath_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) {
			 array<String^>^ fileNames = (array<String^>^)e->Data->GetData(DataFormats::FileDrop);
			 txtPath->Text = fileNames[0]; 
		 }

		 //
private: System::Void txtDllName_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) {
			 txtPath_DragEnter(sender,e);
		 }

		 //
private: System::Void txtDllName_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) {
			 array<String^>^ fileNames = (array<String^>^)e->Data->GetData(DataFormats::FileDrop);
			 FileInfo^ fi_ = gcnew FileInfo(fileNames[0]);
			 txtDllName->Text = fi_->Name; 
		 }

		 //
private: System::Void txtPath2_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) {
			 txtPath_DragEnter(sender,e);
		 }

		 //
private: System::Void txtPath2_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) {
			 array<String^>^ fileNames = (array<String^>^)e->Data->GetData(DataFormats::FileDrop);
			 txtPath2->Text = fileNames[0]; 
		 }

		 //
private: System::Void txtShellCode2_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) {
			 txtPath_DragEnter(sender,e);
		 }

		 //
private: System::Void txtShellCode2_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) {
			 array<String^>^ fileNames = (array<String^>^)e->Data->GetData(DataFormats::FileDrop);
			 StreamReader^ sr_ = gcnew StreamReader(fileNames[0],Encoding::Default);
			 txtShellCode2->Text = sr_->ReadToEnd();
			 sr_->Close();
		 }

};
}
