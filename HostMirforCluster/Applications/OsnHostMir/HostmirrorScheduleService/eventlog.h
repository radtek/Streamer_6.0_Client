#pragma once


namespace HostmirrorScheduleService
{
	using namespace System;
	using namespace System::Diagnostics;
	//using  System::Diagnostics::EventLog;

	public ref class CEventLog
	{
	  private:
		static String^ m_LogName="EIM";
		static String^ m_Source="HostmirrorScheduleService";
	    
	public:
		static CEventLog()
		{
			//set maxmum of the log
			array<EventLog^>^ eventLogs = EventLog::GetEventLogs();
			for(int index=0;index<eventLogs->Length;index++)
			{
				if(!eventLogs[index]->Log->Equals(m_LogName))
					continue;
				eventLogs[index]->MaximumKilobytes = 1024*6;
				eventLogs[index]->ModifyOverflowPolicy(OverflowAction::OverwriteAsNeeded,7);
			}
		}
		static void OSNWriteEventLog(String^ message,EventLogEntryType type,int eventID)
		{
			try
			{
				if(!EventLog::SourceExists(m_Source))
				{
					EventLog::CreateEventSource(m_Source,m_LogName);
				}
				else
				{
					String^ currentLogName = EventLog::LogNameFromSourceName(m_Source,".");
					if(!m_LogName->Equals(currentLogName))
					{
						EventLog::DeleteEventSource(m_Source);
						EventLog::CreateEventSource(m_Source,m_LogName);
					}
				}

				EventLog::WriteEntry(m_Source,message,type,eventID,0);
			}
			catch(...)
			{
#ifdef TRACE
				Trace::WriteLine("SCA:Except to write log entry.");
#endif
			}
		}
	};
}