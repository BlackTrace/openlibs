#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Deianeira {

	/// <summary>
	/// CatchForm ժҪ
	///
	/// ����: ������Ĵ�������ƣ�����Ҫ����
	///          ����������������� .resx �ļ��������й���Դ���������ߵ�
	///          ����Դ�ļ��������ԡ�����
	///          �������������˴���Ĺ���
	///          ���ػ���Դ��ȷ������
	/// </summary>
	public ref class CatchForm : public System::Windows::Forms::Form
	{
	public:
		CatchForm(void)
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
		~CatchForm()
		{
			if (components)
			{
				delete components;
			}
		}

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
			this->SuspendLayout();
			// 
			// CatchForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(292, 266);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->Name = L"CatchForm";
			this->ShowInTaskbar = false;
			this->Text = L"CatchForm";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->Load += gcnew System::EventHandler(this, &CatchForm::CatchForm_Load);
			this->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &CatchForm::CatchForm_MouseClick);
			this->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &CatchForm::CatchForm_MouseDoubleClick);
			this->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &CatchForm::CatchForm_MouseDown);
			this->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &CatchForm::CatchForm_MouseMove);
			this->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &CatchForm::CatchForm_MouseUp);
			this->ResumeLayout(false);

		}
#pragma endregion

		//��¼��갴�����꣬����ȷ����ͼ���
		static Point DownPoint = Point::Empty;
		//������ʾ�Ƿ��ͼ���
		static bool CatchFinished = false;    
		//��ʾ��ͼ�Ƿ�ʼ
		static bool CatchStart = false;
		//��������ԭʼͼ��
		static Bitmap^ originBmp;             
		//���������ͼ�ľ���
		static System::Drawing::Rectangle CatchRect;          
		//���汻��ȡ��ͼƬ����Ҫ�Ǵ���������
	    public: Bitmap^ CatchedBmp;

			//��������¼�
private: System::Void CatchForm_Load(System::Object^  sender, System::EventArgs^  e) {


				 //BackgroundImageΪȫ��ͼƬ���������ñ���������ȫ��ͼƬ
				 //BackgroundImage����������õ�ʱ���Ѿ�Ϊ��ָ����
				 originBmp = gcnew Bitmap(this->BackgroundImage);
			 }

			 //����
private: System::Void CatchForm_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 //����㰴�µ�������Ҽ�
				 if (e->Button == System::Windows::Forms::MouseButtons::Right)
				 {
					 //����DialogResult
					 this->DialogResult = System::Windows::Forms::DialogResult::None;
					 //�رմ˴���
					 this->Close();
				 }
			 }

			 //�����¼�
private: System::Void CatchForm_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 //����㰴�µ���������
				 if (e->Button == System::Windows::Forms::MouseButtons::Left)
				 {
					 //�����׽û�п�ʼ
					 if (!CatchStart)
					 {					
						 CatchStart = true;
						 //������갴������
						 DownPoint = Point(e->X, e->Y);
					 }
				 }
			 }

			 //����ƶ�
private: System::Void CatchForm_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 //�����׽��ʼ
				 if (CatchStart)
				 {
					 //�½�һ��ͼƬ���󣬲�������ԭʼͼƬ��ͬ
					 Bitmap^ destBmp = (Bitmap^)originBmp->Clone();
					 //��ȡ��������
					 Point newPoint = Point(DownPoint.X, DownPoint.Y);

					 Graphics^ g = Graphics::FromImage(destBmp);
					 //�ڸղ��½���ͼƬ���½�һ������
					 Pen^ p = gcnew Pen(Color::Blue, 1);
					 //��ȡ���εĳ��Ϳ�
					 int width = Math::Abs(e->X - DownPoint.X);
					 int height = Math::Abs(e->Y - DownPoint.Y);
					 if (e->X < DownPoint.X)
					 {
						 newPoint.X = e->X;
					 }
					 if (e->Y < DownPoint.Y)
					 {
						 newPoint.Y = e->Y;
					 }
					 //�������
					 CatchRect = System::Drawing::Rectangle(newPoint,System::Drawing::Size(width, height));
					 //�����λ������������
					 g->DrawRectangle(p, CatchRect);
					 //�����½�һ��Graphics��
					 Graphics^ g1 = this->CreateGraphics();
					 ////������ȫ���������½�����
					 //g1 = this->CreateGraphics();
					 //���ղ�������ͼƬ�������������
					 g1->DrawImage(destBmp,Point(0, 0));
				 }
			 }

		 //��굯��
private: System::Void CatchForm_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 if (e->Button == System::Windows::Forms::MouseButtons::Left)
				 {
					 if (CatchStart)
					 {
						 CatchStart = false;
						 CatchFinished = true;
					 }
				 }
			 }

         //���˫��
private: System::Void CatchForm_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 if (e->Button == System::Windows::Forms::MouseButtons::Left && CatchFinished)
				 {
					 if (CatchRect.Contains(Point(e->X, e->Y)))
					 {
						 //�½�һ���ھ��εȴ�Ŀհ�ͼƬ
						 CatchedBmp = gcnew Bitmap(CatchRect.Width, CatchRect.Height);
						 Graphics^ g = Graphics::FromImage(CatchedBmp);
						 //��originBmp����ͼƬ��CatchedBmp
						 g->DrawImage(originBmp, System::Drawing::Rectangle(0, 0, CatchRect.Width, CatchRect.Height), CatchRect, GraphicsUnit::Pixel);
						 this->DialogResult = System::Windows::Forms::DialogResult::OK;
						 this->Close();
					 }
				 }
			 }
	};
}
