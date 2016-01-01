#ifndef _LISTVIEWSORTER_
#define _LISTVIEWSORTER_
#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Text;

namespace Plugin {

	/// <summary>
	/// config ժҪ
	/// </summary>
	public ref class ListViewItemComparer : public System::Collections::IComparer
	{
	private: 

		int Column;

		//��������
		int Type;

		//��ʾ������߽���
		bool bAsc;

		int param1;

	public: 

		//Ĭ�Ϲ��캯��
		ListViewItemComparer(void)
		{
			//
			//TODO: �ڴ˴���ӹ��캯������
			//
		}

		//Column�ֶ�
		//bAsc��ʾ�����ǽ���
		//Type���ͣ�0��ʾ��������1��ʾ�ַ�������
		//param1���ֱȽ�ʱҪ�õģ��������
		ListViewItemComparer(int Column,bool bAsc, int Type,int param1)
		{
			this->Column = Column;
			this->Type = Type;
			this->bAsc = bAsc;
			this->param1 = param1;
		}

		//����ʵ��
		virtual int Compare(Object^ x, Object^ y)
		{
			//��������
			switch(Type)
			{
			case 0:
				{
					int item1;
					int item2;

					try
					{
						if (bAsc)
						{
							switch(param1)
							{
								//ʮ���ƱȽ�
							case 10:
								item1 = Convert::ToInt32((dynamic_cast<ListViewItem^>(x))->SubItems[Column]->Text);
								item2 = Convert::ToInt32((dynamic_cast<ListViewItem^>(y))->SubItems[Column]->Text);
								break;

								//16���ƱȽ�
							case 16:
								item1 = Convert::ToInt32((dynamic_cast<ListViewItem^>(x))->SubItems[Column]->Text,16);
								item2 = Convert::ToInt32((dynamic_cast<ListViewItem^>(y))->SubItems[Column]->Text,16);
								break;
							}			
						}
						else
						{
							switch(param1)
							{
								//ʮ���ƱȽ�
							case 10:
								item1 = Convert::ToInt32((dynamic_cast<ListViewItem^>(y))->SubItems[Column]->Text);
								item2 = Convert::ToInt32((dynamic_cast<ListViewItem^>(x))->SubItems[Column]->Text);
								break;

								//16���ƱȽ�
							case 16:
								item1 = Convert::ToInt32((dynamic_cast<ListViewItem^>(y))->SubItems[Column]->Text,16);
								item2 = Convert::ToInt32((dynamic_cast<ListViewItem^>(x))->SubItems[Column]->Text,16);
								break;
							}
						}
					}
					catch(Exception^ ex)
					{
						//ת������
						return 0;
					}

					if(item1 >= item2)
					{
						return 0;
					}
					else
					{			
						return 1;
					}
				}
				break;
			case 1:
				{
					if (!bAsc)
					{
						try
						{
							return String::Compare( 
								(dynamic_cast<ListViewItem^>(x))->SubItems[Column]->Text,
								(dynamic_cast<ListViewItem^>(y))->SubItems[Column]->Text 
								);
						}
						catch(Exception^ ex)
						{
							return 0;
						}

					}
					else
					{
						try
						{
							return String::Compare( 
								(dynamic_cast<ListViewItem^>(y))->SubItems[Column]->Text,
								(dynamic_cast<ListViewItem^>(x))->SubItems[Column]->Text 
								);
						}
						catch(Exception^ ex)
						{
							return 1;
						}			
					}	
				}
				break;
			case 2:
				{
					if (bAsc)
					{
						try
						{
							return DateTime::Compare( 
								DateTime::Parse((dynamic_cast<ListViewItem^>(y))->SubItems[Column]->Text),
								DateTime::Parse((dynamic_cast<ListViewItem^>(x))->SubItems[Column]->Text) 
								);
						}
						catch(Exception^ ex)
						{
							return 0;
						}

					}
					else
					{
						try
						{
							return DateTime::Compare( 
								DateTime::Parse((dynamic_cast<ListViewItem^>(x))->SubItems[Column]->Text),
								DateTime::Parse((dynamic_cast<ListViewItem^>(y))->SubItems[Column]->Text) 
								);
						}
						catch(Exception^ ex)
						{
							return 1;
						}			
					}
				}
				break;
			}

			//����ʧ��
			return 0;
		}

	};
}

#endif //_LISTVIEWSORTER_