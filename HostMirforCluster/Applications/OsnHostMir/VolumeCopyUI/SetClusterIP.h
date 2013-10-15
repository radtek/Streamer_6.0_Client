#pragma once

#include <stdio.h>
#include <Winsock2.h>

#using  <mscorlib.dll>
#using <System.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Diagnostics;
using namespace Microsoft::Win32;
using namespace System::Net;
using namespace System::Runtime::InteropServices;
using namespace System::Runtime ::CompilerServices;


namespace VolumeCopyUI {



	/// <summary>
	/// Summary for SetClusterIP
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class SetClusterIP : public System::Windows::Forms::Form
	{
	public:

		SetClusterIP(void)
		{
			InitializeComponent();
			init();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~SetClusterIP()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  lbName;
	private: System::Windows::Forms::Label^  lbIPOne;
	private: System::Windows::Forms::Label^  lbIPTwo;
	private: System::Windows::Forms::TextBox^  tbName;
	private: System::Windows::Forms::TextBox^  tbIPOne;
	private: System::Windows::Forms::TextBox^  tbIPTwo;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  btnOK;
	private: System::Windows::Forms::Button^  btnCancle;
	private: System::Windows::Forms::Label^  label2;


	public:
		DWORD ipOne;
		DWORD ipTwo;

	public: 
	protected: 






	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->lbName = (gcnew System::Windows::Forms::Label());
			this->lbIPOne = (gcnew System::Windows::Forms::Label());
			this->lbIPTwo = (gcnew System::Windows::Forms::Label());
			this->tbName = (gcnew System::Windows::Forms::TextBox());
			this->tbIPOne = (gcnew System::Windows::Forms::TextBox());
			this->tbIPTwo = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->btnOK = (gcnew System::Windows::Forms::Button());
			this->btnCancle = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// lbName
			// 
			this->lbName->AutoSize = true;
			this->lbName->Location = System::Drawing::Point(12, 67);
			this->lbName->Name = L"lbName";
			this->lbName->Size = System::Drawing::Size(53, 12);
			this->lbName->TabIndex = 0;
			this->lbName->Text = L"主机名：";
			// 
			// lbIPOne
			// 
			this->lbIPOne->AutoSize = true;
			this->lbIPOne->Location = System::Drawing::Point(12, 103);
			this->lbIPOne->Name = L"lbIPOne";
			this->lbIPOne->Size = System::Drawing::Size(77, 12);
			this->lbIPOne->TabIndex = 1;
			this->lbIPOne->Text = L"心跳IP地址：";
			// 
			// lbIPTwo
			// 
			this->lbIPTwo->AutoSize = true;
			this->lbIPTwo->Location = System::Drawing::Point(12, 135);
			this->lbIPTwo->Name = L"lbIPTwo";
			this->lbIPTwo->Size = System::Drawing::Size(77, 12);
			this->lbIPTwo->TabIndex = 2;
			this->lbIPTwo->Text = L"公用IP地址：";
			// 
			// tbName
			// 
			this->tbName->Location = System::Drawing::Point(98, 58);
			this->tbName->Name = L"tbName";
			this->tbName->Size = System::Drawing::Size(163, 21);
			this->tbName->TabIndex = 3;
			// 
			// tbIPOne
			// 
			this->tbIPOne->Location = System::Drawing::Point(98, 94);
			this->tbIPOne->Name = L"tbIPOne";
			this->tbIPOne->Size = System::Drawing::Size(163, 21);
			this->tbIPOne->TabIndex = 4;
			// 
			// tbIPTwo
			// 
			this->tbIPTwo->Location = System::Drawing::Point(98, 126);
			this->tbIPTwo->Name = L"tbIPTwo";
			this->tbIPTwo->Size = System::Drawing::Size(163, 21);
			this->tbIPTwo->TabIndex = 5;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 186);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(269, 12);
			this->label1->TabIndex = 6;
			this->label1->Text = L"____________________________________________";
			// 
			// btnOK
			// 
			this->btnOK->Location = System::Drawing::Point(34, 211);
			this->btnOK->Name = L"btnOK";
			this->btnOK->Size = System::Drawing::Size(75, 23);
			this->btnOK->TabIndex = 7;
			this->btnOK->Text = L"应用";
			this->btnOK->UseVisualStyleBackColor = true;
			this->btnOK->Click += gcnew System::EventHandler(this, &SetClusterIP::btnOK_Click);
			// 
			// btnCancle
			// 
			this->btnCancle->Location = System::Drawing::Point(159, 211);
			this->btnCancle->Name = L"btnCancle";
			this->btnCancle->Size = System::Drawing::Size(75, 23);
			this->btnCancle->TabIndex = 8;
			this->btnCancle->Text = L"取消";
			this->btnCancle->UseVisualStyleBackColor = true;
			this->btnCancle->Click += gcnew System::EventHandler(this, &SetClusterIP::btnCancle_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(14, 13);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(257, 12);
			this->label2->TabIndex = 9;
			this->label2->Text = L"请设置集群另一台主机的计算机名和两个IP地址";
			// 
			// SetClusterIP
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(292, 270);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->btnCancle);
			this->Controls->Add(this->btnOK);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->tbIPTwo);
			this->Controls->Add(this->tbIPOne);
			this->Controls->Add(this->tbName);
			this->Controls->Add(this->lbIPTwo);
			this->Controls->Add(this->lbIPOne);
			this->Controls->Add(this->lbName);
			this->Name = L"SetClusterIP";
			this->Text = L"SetClusterIP";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: void init()
			 {
				 ipOne=0;
				 ipTwo=0;
				 ReadTheRegistry("SYSTEM\\CurrentControlSet\\Services\\OSNHBService");
			 }

	private:    unsigned int SetRegistryKey(String ^ServiceKeyName)
				{
					RegistryKey ^pRegKey=Registry::LocalMachine;
					RegistryKey ^pParmeterKey;

					pParmeterKey=pRegKey->OpenSubKey(String::Concat(ServiceKeyName,"\\Parameters"),true);
					if(pParmeterKey==nullptr)
					{
						pParmeterKey=pRegKey->CreateSubKey(String::Concat(ServiceKeyName,"\\Parameters"));
						if(pParmeterKey==nullptr)
						{
							return 1;
						}
					}

					pParmeterKey->SetValue("PartnerName",this->tbName->Text,RegistryValueKind::String);
					pParmeterKey->SetValue("PriIPAddress",ipOne,RegistryValueKind::QWord);
					pParmeterKey->SetValue("SecIPAddress",ipTwo,RegistryValueKind::QWord);

					pParmeterKey->Close();

					return 0;
				}

	private:void ReadTheRegistry(String ^ServiceKeyName)
			{
				struct	in_addr sin_addr;
				char *ptr;
				String ^temp;
				RegistryKey ^pRegKey=Registry::LocalMachine;
				RegistryKey ^pParmeterKey;

					pParmeterKey=pRegKey->OpenSubKey(String::Concat(ServiceKeyName,"\\Parameters"),true);
					if(pParmeterKey!=nullptr)
					{
						this->tbName->Text=static_cast<String^>(pParmeterKey->GetValue("PartnerName"));

						sin_addr.S_un.S_addr= Convert::ToUInt32(pParmeterKey->GetValue("PriIPAddress"));
						ptr=inet_ntoa(sin_addr);
						temp=Marshal::PtrToStringAnsi((IntPtr)ptr); 
						this->tbIPOne->Text = temp;

						sin_addr.S_un.S_addr =Convert::ToUInt32(pParmeterKey->GetValue("SecIPAddress"));
						ptr=inet_ntoa(sin_addr);
						temp=Marshal::PtrToStringAnsi((IntPtr)ptr);
						this->tbIPTwo->Text = temp;
				}
			}

	private: bool SetTheNameAndIP()
			 {
				 int ErrorCode=0;

				 if(tbName->Text->Equals(""))
				 {
					 MessageBox::Show("请输入主机名！");
					 return false;
				 }

				 if(tbIPOne->Text->Equals(""))
				 {
					 MessageBox::Show("请输入心跳IP地址!");
					 return false;
				 }

				 if(tbIPTwo->Text->Equals(""))
				 {
					 MessageBox::Show("请输入公用IP地址!");
					 return false;
				 }

				 if(!IsIP(tbIPOne->Text))
				 {
					 MessageBox::Show("请输入正确格式的心跳IP地址！");
					 return false;
				 }

				 if(!IsIP(tbIPTwo->Text))
				 {
					 MessageBox::Show("请输入正确格式的公用IP地址！");
					 return false;
				 }

				 ChangeIPFormat();
				 ErrorCode = SetRegistryKey("SYSTEM\\CurrentControlSet\\Services\\OSNHBService");

				 if(ErrorCode!=0)
				 {
					 MessageBox::Show("设置注册表失败！");
					 return false;
				 }

				 return true;
			 }

	private: void ChangeIPFormat()
			 {
				 IntPtr p1=Marshal::StringToHGlobalAnsi(tbIPOne->Text);
				 const char *c0=static_cast<char*>(p1.ToPointer());
				 ipOne=inet_addr(c0);
				  p1=Marshal::StringToHGlobalAnsi(tbIPTwo->Text);
				 const char *c1=static_cast<char*>(p1.ToPointer());
				 ipTwo=inet_addr(c1);
				 Marshal::FreeHGlobal(p1);
			 }

	private:  bool IsIP(String ^str)
			 {
				 bool isIpResult=false;

				 try
				 {
					 IPAddress^ ipAdd=IPAddress::Parse(str);
					 isIpResult=true;
				 }
				 catch(...)
				 {
					 isIpResult=false;
				 }

				 return isIpResult;
			 }

	private: System::Void btnOK_Click(System::Object^  sender, System::EventArgs^  e) {

				 if(SetTheNameAndIP())
				 {
					 this->DialogResult = System::Windows::Forms::DialogResult::Yes;
					 return ;
				 }

			 }

	private: System::Void btnCancle_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->DialogResult = System::Windows::Forms::DialogResult::No;
			 }
	};
}
