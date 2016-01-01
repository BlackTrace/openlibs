// Deianeira.cpp: ����Ŀ�ļ���

#include "stdafx.h"
#include "Form1.h"

using namespace Deianeira;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	DWORD dwCheckSumHead;
	DWORD dwCheckSum;

	if(MapFileAndCheckSum(common_net::common_stringToHGlobalUni(Application::ExecutablePath),&dwCheckSumHead,&dwCheckSum) != CHECKSUM_SUCCESS)
	{
		MessageBox::Show("Ӧ�ó���ִ����У��ʱʧ�ܣ�","���棡",MessageBoxButtons::OK,MessageBoxIcon::Warning);
		return 0;
	}

	//
	if(dwCheckSumHead != dwCheckSum)
	{
		if(MessageBox::Show("Ӧ�ó�����ܱ���Ⱦ����Ϊ�޸ģ���ȷ��Ҫִ����","���棡",MessageBoxButtons::OKCancel,MessageBoxIcon::Warning,
			MessageBoxDefaultButton::Button2) != System::Windows::Forms::DialogResult::OK)
		{
			return 0;
		}
	}

	// �ڴ����κοؼ�֮ǰ���� Windows XP ���ӻ�Ч��
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// ���������ڲ�������
	Application::Run(gcnew Form1());
	return 0;
}
