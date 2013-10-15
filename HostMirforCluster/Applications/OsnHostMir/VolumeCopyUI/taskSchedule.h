#pragma once

using namespace System;
using namespace System::Windows::Forms;
using namespace TaskScheduler;


namespace VolumeCopyUI
{
	public ref class OSNTaskSchedule
	{
	private: static	 ScheduledTasks ^st = nullptr;
	public: static int GoComputer(String ^machineName)
			{
				try
				{
					if(machineName->Equals(""))
					{
						st = gcnew ScheduledTasks();
					}
					else
					{
						st = gcnew ScheduledTasks(machineName);
					}
				}
				catch(Exception ^ex)
				{
					st = nullptr;
					MessageBox::Show(ex->ToString());
					return -1;
				}
				return 0;
			}
	public: static int CreateTask(String ^args)
			{
				System::Diagnostics::Process ^schtaskProcess = gcnew System::Diagnostics::Process();
				schtaskProcess->StartInfo->FileName = "schtasks.exe";
				schtaskProcess->StartInfo->RedirectStandardOutput = true;
				schtaskProcess->StartInfo->RedirectStandardInput = true;
				schtaskProcess->StartInfo->CreateNoWindow = true;
				schtaskProcess->StartInfo->UseShellExecute = false;
				schtaskProcess->StartInfo->Arguments = args;

				String ^ReturnString = "";
				int retcode = 0;
				try
				{
					schtaskProcess->Start();
					ReturnString = schtaskProcess->StandardOutput->ReadToEnd();
					schtaskProcess->WaitForExit();
				}
				catch (Exception ^ex)
				{
					MessageBox::Show(ex->ToString());

					if (schtaskProcess)
						schtaskProcess->Close();
					return -1;
				}
				__finally
				{
					if (schtaskProcess)
						schtaskProcess->Close();
				}
				return retcode;
			}
	public: static Task^ CreateTask(String ^taskName, String ^appName, String ^parameters, String ^comment,
				                                 String ^creator, String ^workDir, String ^acct, String ^password)
			{
				Task ^t = nullptr;
				st = gcnew ScheduledTasks();
				try
				{
					t = st->CreateTask(taskName);
				}
				catch(ArgumentException^)
				{
					//String* strMsg = "Task already exists!";
					MessageBox::Show("此任务已存在。");
					return nullptr;
				}
				t->ApplicationName = appName;
				t->Parameters = parameters;
				t->Comment = comment;
				t->Creator = creator;
				t->WorkingDirectory = workDir;
				if(acct->Equals(""))
				{
					t->SetAccountInformation(acct,nullptr);
				}
				else if(acct->Equals(System::Environment::UserName))
				{
					t->Flags = TaskFlags::RunOnlyIfLoggedOn;
					t->SetAccountInformation(acct,nullptr);
				}
				else
				{
					t->SetAccountInformation(acct,password);
				}

				t->IdleWaitDeadlineMinutes = 20;
				t->IdleWaitMinutes = 10;
				t->MaxRunTime = TimeSpan(1,0,0);
				t->Priority = System::Diagnostics::ProcessPriorityClass::High;
				t->Triggers->Add(gcnew RunOnceTrigger(DateTime::Now + TimeSpan::FromMinutes(3.0)));
				/*t->Triggers->Add(new DailyTrigger(8,30,1));
				t->Triggers->Add(new WeeklyTrigger(6,0,DaysOfTheWeek::Sunday));
				t->Triggers->Add(new MonthlyDOWTrigger(8,0,DaysOfTheWeek::Monday,WhichWeek::FirstWeek));
				t->Triggers->Add(new OnIdleTrigger());
				t->Triggers->Add(new OnLogonTrigger());
				t->Triggers->Add(new OnSystemStartTrigger());*/
				try
				{
				    t->Save();
				}
				catch(Exception ^ex)
				{
					if(ex->Message->Contains("任务计划程序服务没有运行。") 
					  || ex->Message->ToUpper()->Contains("0X80041315")
					  || ex->Message->Contains("接口未知")
					  || ex->Message->ToUpper()->Contains("0X800706B5"))
					{
						//String* strMsg = "Task schedule service isn't running!";
						MessageBox::Show("TaskSchedule服务没有启动。");
					}
					else if(ex->Message->ToUpper()->Contains("0X80070005"))
					{
						//String* strMsg = "The task schedule can not run due to invalid password!";
						MessageBox::Show("密码不正确，任务计划无法运行。");
					}

				}
				return t;
			}
	public: static Task^ GetTaskByTaskName(String ^strSrcVolume)
			{
				st = gcnew ScheduledTasks();
				array<String ^>^ taskNames = st->GetTaskNames();
				for(int i=0; i<taskNames->Length; i++)
				{
					if(taskNames[i]->EndsWith(".job"))
					{
						int len = taskNames[i]->Length;
						taskNames[i] = taskNames[i]->Substring(0,len-4);
					}
					/*if(taskNames[i]->Equals(taskName))
					{
						Task* t = st->OpenTask(taskName);
						return t;
					}*/

					if(taskNames[i]->StartsWith("VSSAgent") || taskNames[i]->StartsWith("OracleAndVSSAgent")
					   || taskNames[i]->StartsWith("SyncAgent") || taskNames[i]->StartsWith("OSNCDPAgent")
					   || taskNames[i]->StartsWith("OSNHostMirrorCDPAgent"))
					{
						if(taskNames[i]->Contains(strSrcVolume))
						{
							Task ^t = st->OpenTask(taskNames[i]);
							return t;
						}
					}
				}
				return nullptr;
			}
    public: static bool DeleteTaskSchedule(String ^taskName)
			{
				 st = gcnew ScheduledTasks();
				 bool bRet = false;
				 if(!taskName->Equals(""))
				 {
					bRet = st->DeleteTask(taskName);
				 }
				 return bRet;				 
			}
	};
}