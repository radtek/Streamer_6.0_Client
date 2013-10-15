#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace VolumeCopyUI {

	/// <summary>
	/// Summary for AccountLogin
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class AccountLogin : public System::Windows::Forms::Form
	{
	public:
		AccountLogin(void)
		{
			InitializeComponent();
			
			FillinAccountTbox();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~AccountLogin()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  btnCancel;
	protected: 
	private: System::Windows::Forms::Button^  btnOK;
	private: System::Windows::Forms::TextBox^  tboxCheckPassword;
	private: System::Windows::Forms::TextBox^  tboxPassword;
	private: System::Windows::Forms::TextBox^  tboxAccountName;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label1;
	
	public:   static String  ^accountName;
	public:	static String  ^password;

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
			this->btnCancel = (gcnew System::Windows::Forms::Button());
			this->btnOK = (gcnew System::Windows::Forms::Button());
			this->tboxCheckPassword = (gcnew System::Windows::Forms::TextBox());
			this->tboxPassword = (gcnew System::Windows::Forms::TextBox());
			this->tboxAccountName = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// btnCancel
			// 
			this->btnCancel->Location = System::Drawing::Point(194, 225);
			this->btnCancel->Name = L"btnCancel";
			this->btnCancel->Size = System::Drawing::Size(74, 21);
			this->btnCancel->TabIndex = 15;
			this->btnCancel->Text = L"取消";
			this->btnCancel->UseVisualStyleBackColor = true;
			this->btnCancel->Click += gcnew System::EventHandler(this, &AccountLogin::btnCancel_Click);
			// 
			// btnOK
			// 
			this->btnOK->Location = System::Drawing::Point(49, 225);
			this->btnOK->Name = L"btnOK";
			this->btnOK->Size = System::Drawing::Size(74, 21);
			this->btnOK->TabIndex = 14;
			//this->btnOK->Text = L"OK";
			this->btnOK->Text = L"确定";
			this->btnOK->UseVisualStyleBackColor = true;
			this->btnOK->Click += gcnew System::EventHandler(this, &AccountLogin::btnOK_Click);
			// 
			// tboxCheckPassword
			// 
			this->tboxCheckPassword->Location = System::Drawing::Point(96, 130);
			this->tboxCheckPassword->Name = L"tboxCheckPassword";
			this->tboxCheckPassword->PasswordChar = '*';
			this->tboxCheckPassword->Size = System::Drawing::Size(207, 21);
			this->tboxCheckPassword->TabIndex = 13;
			// 
			// tboxPassword
			// 
			this->tboxPassword->Location = System::Drawing::Point(96, 97);
			this->tboxPassword->Name = L"tboxPassword";
			this->tboxPassword->PasswordChar = '*';
			this->tboxPassword->Size = System::Drawing::Size(207, 21);
			this->tboxPassword->TabIndex = 12;
			// 
			// tboxAccountName
			// 
			this->tboxAccountName->Location = System::Drawing::Point(96, 65);
			this->tboxAccountName->Name = L"tboxAccountName";
			this->tboxAccountName->Size = System::Drawing::Size(207, 21);
			this->tboxAccountName->TabIndex = 11;
			// 
			// label4
			// 
			this->label4->Location = System::Drawing::Point(11, 125);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(82, 28);
			this->label4->TabIndex = 10;
			this->label4->Text = L"Confirm Password:";
			this->label4->Text = L"确认密码：";
			this->label4->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// label3
			// 
			this->label3->Location = System::Drawing::Point(11, 92);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(82, 28);
			this->label3->TabIndex = 5;
			//this->label3->Text = L"Password:";
			this->label3->Text = L"输入密码：";
			this->label3->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// label2
			// 
			this->label2->Location = System::Drawing::Point(11, 60);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(82, 28);
			this->label2->TabIndex = 7;
			//this->label2->Text = L"Account Name:";
			this->label2->Text = L"输入用户名：";
			this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// label6
			// 
			this->label6->Location = System::Drawing::Point(11, 201);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(310, 18);
			this->label6->TabIndex = 9;
			this->label6->Text = L"__________________________________________________";
			this->label6->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// label5
			// 
			this->label5->Location = System::Drawing::Point(11, 161);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(310, 41);
			this->label5->TabIndex = 8;
			//this->label5->Text = L"If you don\'t input the administrator account\'s password,the task may have no the " 
				//L"licence to be executed.";
			this->label5->Text = L"如果不输入管理者权限用户的密码，计划的任务可能不能运行。";
			this->label5->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// label1
			// 
			this->label1->Location = System::Drawing::Point(11, 7);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(310, 39);
			this->label1->TabIndex = 6;
			//this->label1->Text = L"Please input account name and password,and then the task will be executed automat" 
				//L"ically.";
            this->label1->Text = L"请输入用户名及密码。这样任务会自动运行，就好像用户亲自运行一样。";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// AccountLogin
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(313, 251);
			this->Controls->Add(this->btnCancel);
			this->Controls->Add(this->btnOK);
			this->Controls->Add(this->tboxCheckPassword);
			this->Controls->Add(this->tboxPassword);
			this->Controls->Add(this->tboxAccountName);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label1);
			this->Name = L"AccountLogin";
			this->Text = L"AccountLogin";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

private: System::Void FillinAccountTbox()
		 {
			 String ^domainName = System::Environment::UserDomainName;
			 String ^userName = System::Environment::UserName;
			 tboxAccountName->Text = String::Concat(domainName,"\\",userName);
		 }
private: System::Void btnOK_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(tboxAccountName->Text->Equals("") || tboxPassword->Text->Equals(""))
			 {
				 //String *meg = "Please input the user's account and password.";
				
				 MessageBox::Show("请先输入执行任务计划的用户名和密码！");
				 return;
			 }
			 if(tboxCheckPassword->Text->Equals(""))
			 {
				 //String *meg = "Please input the password again.";
				 
				 MessageBox::Show("请再次输入用户的密码！");
				 return;
			 }
			 else if(!tboxCheckPassword->Text->Equals(tboxPassword->Text))
			 {
				 //String *meg = "The new password is not equal to the original one,please input it again.";
				
				 MessageBox::Show("两次输入的密码不相等，请确认后重新输入密码！");
				 tboxPassword->Text = "";
				 tboxCheckPassword->Text = "";
				 return;
			 }
			 accountName = tboxAccountName->Text;
			 password = tboxPassword->Text;
			 this->DialogResult = System::Windows::Forms::DialogResult::Yes;
		 }
private: System::Void btnCancel_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->DialogResult = System::Windows::Forms::DialogResult::No;
		 }
};
}
