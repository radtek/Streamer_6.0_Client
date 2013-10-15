#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace Microsoft::Win32;

namespace VolumeCopyUI {

	/// <summary>
	/// Summary for Setting
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Setting : public System::Windows::Forms::Form
	{
	public:
		Setting(INSTALLTYPE ty)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			Type=ty;

		    FormLoad();
		}

	public:

		unsigned int BitSize;
		unsigned int InitIO;
		unsigned int AutoInit;
		unsigned int DeleteFlag;
		unsigned int ContinueInit;

		unsigned int DiskBitSize;
		unsigned int DiskInitIO;
		unsigned int DiskAutoInit;
		unsigned int DiskDeleteFlag;
        unsigned int DiskContinueInit;


	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::RadioButton^  radioButton4;
	private: System::Windows::Forms::RadioButton^  radioButton3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::GroupBox^  groupBox3;
	private: System::Windows::Forms::CheckBox^  checkBox1;

	public: 
		INSTALLTYPE Type;


	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Setting()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::ComboBox^  comboBox1;



	private: System::Windows::Forms::Button^  ButtonOk;

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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->ButtonOk = (gcnew System::Windows::Forms::Button());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->radioButton4 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton3 = (gcnew System::Windows::Forms::RadioButton());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->BeginInit();
			this->groupBox2->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(7, 14);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(71, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"初始化IO数:";
			// 
			// numericUpDown1
			// 
			this->numericUpDown1->Location = System::Drawing::Point(84, 5);
			this->numericUpDown1->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {8, 0, 0, 0});
			this->numericUpDown1->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {2, 0, 0, 0});
			this->numericUpDown1->Name = L"numericUpDown1";
			this->numericUpDown1->ReadOnly = true;
			this->numericUpDown1->Size = System::Drawing::Size(110, 21);
			this->numericUpDown1->TabIndex = 1;
			this->numericUpDown1->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {2, 0, 0, 0});
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(7, 40);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(65, 12);
			this->label2->TabIndex = 2;
			this->label2->Text = L"位大小(K):";
			// 
			// comboBox1
			// 
			this->comboBox1->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"32", L"64", L"128"});
			this->comboBox1->Location = System::Drawing::Point(84, 32);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(110, 20);
			this->comboBox1->TabIndex = 3;
			// 
			// ButtonOk
			// 
			this->ButtonOk->Location = System::Drawing::Point(84, 233);
			this->ButtonOk->Name = L"ButtonOk";
			this->ButtonOk->Size = System::Drawing::Size(56, 22);
			this->ButtonOk->TabIndex = 5;
			this->ButtonOk->Text = L"应用";
			this->ButtonOk->UseVisualStyleBackColor = true;
			this->ButtonOk->Click += gcnew System::EventHandler(this, &Setting::ButtonOk_Click);
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->label4);
			this->groupBox2->Controls->Add(this->label3);
			this->groupBox2->Controls->Add(this->radioButton4);
			this->groupBox2->Controls->Add(this->radioButton3);
			this->groupBox2->Location = System::Drawing::Point(7, 58);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(220, 93);
			this->groupBox2->TabIndex = 6;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"删除镜像设置";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(6, 74);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(119, 12);
			this->label4->TabIndex = 3;
			this->label4->Text = L"建议选择重启后生效.";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(6, 62);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(173, 12);
			this->label3->TabIndex = 2;
			this->label3->Text = L"注意:立即生效有部分IO会丢失,";
			// 
			// radioButton4
			// 
			this->radioButton4->AutoSize = true;
			this->radioButton4->Location = System::Drawing::Point(7, 43);
			this->radioButton4->Name = L"radioButton4";
			this->radioButton4->Size = System::Drawing::Size(83, 16);
			this->radioButton4->TabIndex = 1;
			this->radioButton4->TabStop = true;
			this->radioButton4->Text = L"重启后生效";
			this->radioButton4->UseVisualStyleBackColor = true;
			this->radioButton4->CheckedChanged += gcnew System::EventHandler(this, &Setting::radioButton3_CheckedChanged);
			// 
			// radioButton3
			// 
			this->radioButton3->AutoSize = true;
			this->radioButton3->Location = System::Drawing::Point(7, 21);
			this->radioButton3->Name = L"radioButton3";
			this->radioButton3->Size = System::Drawing::Size(71, 16);
			this->radioButton3->TabIndex = 0;
			this->radioButton3->TabStop = true;
			this->radioButton3->Text = L"立即生效";
			this->radioButton3->UseVisualStyleBackColor = true;
			this->radioButton3->CheckedChanged += gcnew System::EventHandler(this, &Setting::radioButton3_CheckedChanged);
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->checkBox1);
			this->groupBox3->Location = System::Drawing::Point(7, 163);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(220, 53);
			this->groupBox3->TabIndex = 7;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"初始化设置";
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Location = System::Drawing::Point(8, 21);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(144, 16);
			this->checkBox1->TabIndex = 0;
			this->checkBox1->Text = L"遇到坏扇区继续初始化";
			this->checkBox1->UseVisualStyleBackColor = true;
			// 
			// Setting
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(239, 262);
			this->Controls->Add(this->groupBox3);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->ButtonOk);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->numericUpDown1);
			this->Controls->Add(this->label1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->Name = L"Setting";
			this->Text = L"系统设置";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->EndInit();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}

		void SetVolumeCopy(unsigned int initIO,unsigned int bitSize,unsigned int Auto,unsigned int Delete,unsigned int Init)
		{
			
			RegistryKey ^SubKey= Registry::LocalMachine;
			RegistryKey ^pParameter =nullptr;

			pParameter = SubKey->OpenSubKey("System\\CurrentControlSet\\Services\\OsnVSS\\Parameters",true);
			if(nullptr==pParameter)
				pParameter = SubKey->CreateSubKey("System\\CurrentControlSet\\Services\\OsnVSS\\Parameters");

			
  
			if(nullptr == pParameter)
			{
				MessageBox::Show("找不到相应的注册表值");
				return;

			}
            
			if(initIO!=this->InitIO)
			{
				pParameter->SetValue("InitIO",initIO,RegistryValueKind::DWord);
				this->InitIO=initIO;

			}

			if(bitSize!=this->BitSize)
			{
				pParameter->SetValue("BitSize",bitSize,RegistryValueKind::DWord);
				this->BitSize = bitSize;

			}

			if(Auto!=this->AutoInit)
			{
				pParameter->SetValue("AutoInit",Auto,RegistryValueKind::DWord);
				this->AutoInit = Auto;
			}

			if(Delete!=this->DeleteFlag)
			{
				unsigned int ErrorCode = 0;
				ErrorCode = OsnVolumeCopySetRemoveFlag(Delete);


				if(ErrorCode==0)
				{
				   pParameter->SetValue("DeleteFlag",Delete,RegistryValueKind::DWord);
				   DeleteFlag = Delete;
				  
				}
				else
				{
					MessageBox::Show(String::Concat("删除设置出错,代码=",ErrorCode.ToString()));
				}

			}

			if(Init != this->ContinueInit)
			{
				unsigned int ErrorCode =0;
				ErrorCode = OsnVolumeCopySetContinueInit(Init);
				if(0==ErrorCode)
				{
					pParameter->SetValue("ContinueInit",Init,RegistryValueKind::DWord);
					this->ContinueInit = Init;
				}
				else
				{
					MessageBox::Show(String::Concat("设置继续初始化失败，代码＝",ErrorCode.ToString()));
				}
			}

			pParameter->Close();

		}

		void SetDiskCopy(unsigned int initIO,unsigned int bitSize,unsigned int Auto,unsigned int Delete,unsigned int Init)
		{
			RegistryKey ^SubKey= Registry::LocalMachine;
			RegistryKey ^pParameter =nullptr;

			pParameter = SubKey->OpenSubKey("System\\CurrentControlSet\\Services\\OsnDSS\\Parameters",true);
			if(nullptr==pParameter)
				pParameter = SubKey->CreateSubKey("System\\CurrentControlSet\\Services\\OsnDSS\\Parameters");

			
  
			if(nullptr == pParameter)
			{
				MessageBox::Show("找不到相应的注册表值");
				return;

			}

			
            
			if(initIO!=this->DiskInitIO)
			{
				pParameter->SetValue("InitIO",initIO,RegistryValueKind::DWord);
				DiskInitIO= initIO;

			}

			if(bitSize!=this->DiskBitSize)
			{
				pParameter->SetValue("BitSize",bitSize,RegistryValueKind::DWord);
                DiskBitSize = bitSize;
			}

			if(Auto!=this->DiskAutoInit)
			{
				pParameter->SetValue("AutoInit",Auto,RegistryValueKind::DWord);
				DiskAutoInit = Auto;
			}

			if(Delete!=this->DiskDeleteFlag)
			{
				unsigned int ErrorCode = 0;
				ErrorCode = OsnDiskCopySetRemoveFlag(Delete);


				if(ErrorCode==0)
				{
				   pParameter->SetValue("DeleteFlag",Delete,RegistryValueKind::DWord);
				   DiskDeleteFlag = Delete;
				}
				else
				{
					MessageBox::Show(String::Concat("设置继续初始化出错,代码=",ErrorCode.ToString()));
				}


			}

			if(this->DiskContinueInit != Init)
			{
				unsigned int ErrorCode = 0;
				ErrorCode = OsnDiskCopySetContinueInit(Init);


				if(ErrorCode==0)
				{
				   pParameter->SetValue("ContinueInit",Init,RegistryValueKind::DWord);
				   DiskContinueInit = Init;
				}
				else
				{
					MessageBox::Show(String::Concat("出错,代码=",ErrorCode.ToString()));
				}

			}

			pParameter->Close();


		}
        
		void SetRegistryValue(unsigned int initIO,unsigned int bitSize,unsigned int Auto,
			                           unsigned int Delete,unsigned int CInit)
		{
			if(VolumeCopy == Type)
			{
               SetVolumeCopy(initIO,bitSize,Auto,Delete,CInit);
			}
			else if(DiskCopy == Type)
			{
               SetDiskCopy(initIO,bitSize,Auto,Delete,CInit);
			}
			else
			{
				SetVolumeCopy(initIO,bitSize,Auto,Delete,CInit);
				SetDiskCopy(initIO,bitSize,Auto,Delete,CInit);
			}
		}

		void LoadVolumeCopy()
		{
			Object ^pBitSize=nullptr;
			Object ^pInitIO=nullptr;
			Object ^pAutoInit=nullptr;
			Object ^pDeleteFlag = nullptr;
            Object ^pContinueInit = nullptr;

			RegistryKey ^SubKey= Registry::LocalMachine;
			RegistryKey ^pParamter =nullptr;

			
			pParamter = SubKey->OpenSubKey("System\\CurrentControlSet\\Services\\OsnVSS\\Parameters");
			

			if(pParamter==nullptr)
			{
				this->BitSize=32;
				this->InitIO = 2;
				this->AutoInit=0; // 0 is false,1 is true;
				this->DeleteFlag=0; // 0 is 立即生效,1 is 重启后生效;

				this->ContinueInit=1; // 1 is  继续初始化

			}
			else
			{
				pBitSize = pParamter->GetValue("BitSize");
				pInitIO= pParamter->GetValue("InitIO");
				pAutoInit = pParamter->GetValue("AutoInit");
				pDeleteFlag = pParamter->GetValue("DeleteFlag");
				pContinueInit = pParamter->GetValue("ContinueInit");

				if(pBitSize!=nullptr)
					BitSize=Convert::ToUInt32(pBitSize);
				else
					BitSize = -1;

				if(pInitIO!=nullptr)
					InitIO=Convert::ToUInt32(pInitIO);
				else
					InitIO = -1;

				if(pAutoInit!=nullptr)
					AutoInit=Convert::ToUInt32(pAutoInit);
				else
					AutoInit= -1;

				if(pDeleteFlag!=nullptr)
					DeleteFlag=Convert::ToUInt32(pDeleteFlag);
				else
					DeleteFlag = 1; // 1重启后删除

				if(nullptr!=pContinueInit )
					ContinueInit=Convert::ToUInt32(pContinueInit);
				else
				     this->ContinueInit = 1;

				
					
				pParamter->Close();
				SubKey->Close();
			}

            if(InitIO!=-1)
			   this->numericUpDown1->Value= InitIO;
			else
				this->numericUpDown1->Value= 2;

			if(BitSize!=-1)
			  this->comboBox1->Text= BitSize.ToString();
			else
				this->comboBox1->Text= Convert::ToString(32);


			/*if(AutoInit==-1||AutoInit==1)
				this->radioButton1->Checked = true;
			else
				this->radioButton2->Checked=true;*/

			if(1==DeleteFlag)
				this->radioButton4->Checked=true;
			else
				this->radioButton3->Checked=true;

				if(1 == this->ContinueInit)
					this->checkBox1->Checked=true;
				else
					this->checkBox1->Checked=false;

		}

		void LoadDiskCopy()
		{
			Object ^pBitSize=nullptr;
			Object ^pInitIO=nullptr;
			Object ^pAutoInit=nullptr;
			Object ^pDeleteFlag = nullptr;
			Object ^pContinueInit = nullptr;

			RegistryKey ^SubKey= Registry::LocalMachine;
			RegistryKey ^pParamter =nullptr;

			
			pParamter = SubKey->OpenSubKey("System\\CurrentControlSet\\Services\\OsnDSS\\Parameters");
			

			if(pParamter==nullptr)
			{
				this->DiskBitSize=32;
				this->DiskInitIO = 2;
				this->DiskAutoInit=0; // 0 is false,1 is true;
				this->DiskDeleteFlag=0; // 0 is 立即生效,1 is 重启后生效;
				this->DiskContinueInit = 1;

			}
			else
			{
				pBitSize = pParamter->GetValue("BitSize");
				pInitIO= pParamter->GetValue("InitIO");
				pAutoInit = pParamter->GetValue("AutoInit");
				pDeleteFlag = pParamter->GetValue("DeleteFlag");
				pContinueInit = pParamter->GetValue("ContinueInit");

				if(pBitSize!=nullptr)
					DiskBitSize=Convert::ToUInt32(pBitSize);
				else
					DiskBitSize = -1;

				if(pInitIO!=nullptr)
					DiskInitIO=Convert::ToUInt32(pInitIO);
				else
					DiskInitIO = -1;

				if(pAutoInit!=nullptr)
					DiskAutoInit=Convert::ToUInt32(pAutoInit);
				else
					DiskAutoInit= -1;

				if(pDeleteFlag!=nullptr)
					DiskDeleteFlag=Convert::ToUInt32(pDeleteFlag);
				else
					DiskDeleteFlag = 1; // 1重启后删除

				if(nullptr != pContinueInit)
					this->DiskContinueInit =Convert::ToUInt32(pContinueInit);
				else
					this->DiskContinueInit =1;
				
					
				pParamter->Close();
				SubKey->Close();
			}

            if(DiskInitIO!=-1)
			   this->numericUpDown1->Value= DiskInitIO;
			else
				this->numericUpDown1->Value= 2;

			if(DiskBitSize!=-1)
			  this->comboBox1->Text= DiskBitSize.ToString();
			else
				this->comboBox1->Text= Convert::ToString(32);


			/*if(DiskAutoInit==-1||DiskAutoInit==1)
				this->radioButton1->Checked = true;
			else
				this->radioButton2->Checked=true;*/

			if(1==DiskDeleteFlag)
				this->radioButton4->Checked=true;
			else
				this->radioButton3->Checked=true;

			if(1 == DiskContinueInit)
				this->checkBox1->Checked =true;
			else
				this->checkBox1->Checked =false;
		    
		}


		void FormLoad()
		{
			
			if(Type== VolumeCopy)
			{
				LoadVolumeCopy();
			}
			else if(Type == DiskCopy)
			{
				LoadDiskCopy();
			}
			else
			{
               LoadVolumeCopy();
			   LoadDiskCopy();

			}
		}
		
#pragma endregion
	private: System::Void radioButton1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
			 {
				 /*if(this->radioButton1->Checked)
					 this->radioButton2->Checked=false;

				 if(this->radioButton2->Checked)
					 this->radioButton1->Checked = false;*/
			 }
private: System::Void ButtonOk_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 unsigned int initIO = Convert::ToUInt32(this->numericUpDown1->Value);

			 if(this->comboBox1->Text->Equals(""))
			 {
				 MessageBox::Show("请选择位大小");
				 return;
			 }

			unsigned int bitSize = Convert::ToUInt32(this->comboBox1->Text);

			int Auto=0;
			int DeleteFlag = 1;
			int init=1;

			/*if(this->radioButton1->Checked)
				Auto=1;*/
            
			if(initIO>8)
				initIO=8;

			if(this->radioButton3->Checked)
				DeleteFlag=0;
			else
				DeleteFlag=1;

			if(this->checkBox1->Checked)
				init = 1;
			else
				init =0;

			SetRegistryValue(initIO,bitSize,Auto,DeleteFlag,init);
			
			this->DialogResult = System::Windows::Forms::DialogResult::Yes;
			return;

		 }
private: System::Void radioButton3_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(radioButton3->Checked)
				 this->radioButton4->Checked = false;

			 if(radioButton4->Checked)
				 this->radioButton3->Checked = false;
		 }

};
}
