#pragma once

namespace VolumeCopyUI
{
	using namespace System;
	using namespace System::Diagnostics;
	using namespace System::Threading;
	using namespace System::IO;

	public ref class CHostMirrorLog
	{
		
	    public:
			
			CHostMirrorLog(String ^strPath)
			{
                
				this->m_ImagePath = strPath;
				CreateEventLog("OSNHostMirror","EIM");
			}

			void CreateEventLog(String ^source,String ^logName)
			{
				if(!EventLog::SourceExists(source))
				{
					EventLog::CreateEventSource(source,logName);
				}
			}

			void OpenLogFile()
			{
				try
				{
				
					if(!m_ImagePath)
					{
						return;
					}

					String ^strFileName = String::Concat(m_ImagePath, "\\OSNHostMirror.log");
					FileInfo ^pFileHost = gcnew FileInfo(strFileName);
					FileStream ^fs;


					if(!pFileHost->Exists)
					{
						// if the log file is not exsit,create it
						fs =  pFileHost->Create();
						m_LogStreamWriter = gcnew StreamWriter(fs);

					}
					else
					{
						fs= pFileHost->Open(FileMode::Append,FileAccess::Write,FileShare::None);
						m_LogStreamWriter = gcnew StreamWriter(fs);
					}
				}
				catch(...)
				{
					return;
				}
				
			}

			void CloseLogFile()
			{
				if(m_LogStreamWriter)
				{
					m_LogStreamWriter->Close();
				}

			}

			void OsnHostMirrorLog(String ^strLogInformation)
			{
				DateTime time = DateTime::Now;
				
				// 
				// Month/day/Year hour:min:sec - 
				//
				String ^strTime = time.Month.ToString("d2");
				strTime = String::Concat(strTime, "/");
				strTime = String::Concat(strTime,time.Day.ToString("d2"));
				strTime = String::Concat(strTime, "/");
				strTime = String::Concat(strTime,time.Year.ToString());
				strTime = String::Concat(strTime," ");
				strTime = String::Concat(strTime,time.Hour.ToString("d2"));
				strTime = String::Concat(strTime,":");
				strTime = String::Concat(strTime,time.Minute.ToString("d2"));
				strTime = String::Concat(strTime,":");
				strTime = String::Concat(strTime,time.Second.ToString("d2"));

				strTime = String::Concat(strTime," - ");

				strTime = String::Concat(strTime,strLogInformation);

				try
				{
					OpenLogFile();

					if(m_LogStreamWriter)
					{
						m_LogStreamWriter->WriteLine(strTime);
					}

					CloseLogFile();
				}
				catch(...)
				{
					return;
				}

			}


			void OSNWriteEventLog(String ^source,String ^logName,String ^message,EventLogEntryType type,int eventID,short category)
			{
				//create the log 
				/*if(!EventLog::Exists(logName)||!EventLog::SourceExists(source))
				{
					//if the log name matches an existing event source name,
					//or the source name matches an existing event log name,
					//CreateEventSource will be failed,so delete them first.
					if(EventLog::Exists(source))
					{		
						EventLog::Delete(source);
					}
					if(EventLog::SourceExists(logName))
					{
						EventLog::DeleteEventSource(logName);
					}
					
					EventLog::CreateEventSource(source,logName);
					int i=0;
					while(!EventLog::Exists(logName)&&i++>20)
					{
						System::Threading::Thread::Sleep(100);
					}
				}
				//set maxmum of the log 
				array<EventLog^>^ eventLogs = EventLog::GetEventLogs();
				for(int i=0;i<eventLogs->Length;i++)
				{
					if(eventLogs[i]->Log->Equals(logName))
					{
						eventLogs[i]->MaximumKilobytes = 2048;
						eventLogs[i]->ModifyOverflowPolicy(OverflowAction::OverwriteAsNeeded,7);
					}
				}*/
				//write entry

				if(!EventLog::SourceExists(source))
				{
					EventLog::CreateEventSource(source,logName);
				}
				else
				{
					String^ currentLogName = EventLog::LogNameFromSourceName(source,".");
					if(!logName->Equals(currentLogName))
					{
						EventLog::DeleteEventSource(source);
						EventLog::CreateEventSource(source,logName);
					}
				}
				EventLog::WriteEntry(source,message,type,eventID,category);				

			}
			void OSNWriteEventLog(String ^message,EventLogEntryType type, int eventID)
			{
				OSNWriteEventLog("OSNHostMirror","EIM",message,type,eventID,0);
				OsnHostMirrorLog(message);

			}

	private:

		StreamWriter ^m_LogStreamWriter;
		String ^m_ImagePath;
	};
}