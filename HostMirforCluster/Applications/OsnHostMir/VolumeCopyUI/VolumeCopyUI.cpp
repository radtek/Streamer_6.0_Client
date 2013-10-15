// VolumeCopyUI.cpp : main project file.

#include "stdafx.h"
#include "stdlib.h"
#include "Configure.h"
#include "Globallist.h"
#include "SetClusterIP.h"
#include "ClusterResourceName.h"
#include<windows.h>
#include "..\OsnVolumeCopyApi\OsnVolumeCopyApi.h"
#include < vcclr.h >
#include "NewMirror.h"
#include "About.h"
#include "Setting.h"
#include "HostMirrorLog.h"
#include "MessageBoxShow.h"
#include "OsnMirrorCopyUI.h"

using namespace VolumeCopyUI;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 
    
	array<Process ^> ^Instance = Process::GetProcessesByName("MirrorCopyUI");
	if(Instance->Length>1)
    {
	
	  MessageBox::Show("已经有一个实例在运行.");
	  return  0;
    }

	// Create the main window and run it
	Application::Run(gcnew OsnMirrorCopyUI());
	return 0;
}
