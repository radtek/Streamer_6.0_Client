# Microsoft Developer Studio Project File - Name="OSNService" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=OSNService - Win32 Debug Win2k
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OSNService.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OSNService.mak" CFG="OSNService - Win32 Debug Win2k"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OSNService - Win32 Release Win2k" (based on "Win32 (x86) Console Application")
!MESSAGE "OSNService - Win32 Debug Win2k" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/OSNTree/OSNService", EBAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OSNService - Win32 Release Win2k"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "OSNService___Win32_Release_Win2k"
# PROP BASE Intermediate_Dir "OSNService___Win32_Release_Win2k"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Target\win2k\fre\storagetrust\i386"
# PROP Intermediate_Dir "..\..\Intermediate\win2k\fre\storagetrust\i386\OSNService"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\..\common" /I "$(IntDir)" /I "$(IntDir)\..\Common" /I "..\..\..\Tools\DDK\inc" /I "..\..\..\Tools\DDK\src\storage\inc" /I "..\..\..\Tools\Platform SDK\Include" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_DCS_USER_SPACE_" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\..\common" /I "$(IntDir)" /I "$(IntDir)\..\Common" /I "..\..\Tools\DDK\inc" /I "..\..\Tools\DDK\src\storage\inc" /I "..\..\Tools\Platform SDK\Include" /D _WIN32_WINNT=0x500 /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_DCS_USER_SPACE_" /D "_AFXDLL" /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /i "$(IntDir)" /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "$(IntDir)" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ws2_32.lib OSNApi.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt" /libpath:"..\..\..\Target\nt40\i386\Release\DataCore"
# ADD LINK32 ws2_32.lib OSNApi.lib OSNLicence.lib OsnDmApi.lib Rpcrt4.lib  wbemuuid.lib /nologo /subsystem:console /incremental:yes /debug /machine:I386 /nodefaultlib:"nafxcw.lib" /nodefaultlib:"libc.lib" /libpath:"..\..\target\win2k\fre\storagetrust\i386" /libpath:"..\..\tools\platform sdk\lib"
# Begin Special Build Tool
IntDir=.\..\..\Intermediate\win2k\fre\storagetrust\i386\OSNService
OutDir=.\..\..\Target\win2k\fre\storagetrust\i386
TargetPath=\eim_workspace\osn4.0\Target\win2k\fre\storagetrust\i386\OSNService.exe
TargetName=OSNService
SOURCE="$(InputPath)"
PostBuild_Desc=Extracting Symbols...
PostBuild_Cmds=..\..\tools\bin\nmsym /TRANS:source,package,always "/SOURCE:.;..\\Common;..\\..\\Common;$(IntDir);$(Include)" "/OUT:$(OutDir)\Symbols\$(TargetName).NMS" "$(TargetPath)"	move $(OutDir)\$(TargetName).pdb $(OutDir)\Symbols
# End Special Build Tool

!ELSEIF  "$(CFG)" == "OSNService - Win32 Debug Win2k"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "OSNService___Win32_Debug_Win2k"
# PROP BASE Intermediate_Dir "OSNService___Win32_Debug_Win2k"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\Target\win2k\chk\storagetrust\i386"
# PROP Intermediate_Dir "..\..\Intermediate\win2k\chk\storagetrust\i386\OSNService"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\common" /I "$(IntDir)" /I "$(IntDir)\..\Common" /I "..\..\..\Tools\DDK\inc" /I "..\..\..\Tools\DDK\src\storage\inc" /I "..\..\..\Tools\Platform SDK\Include" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_DCS_USER_SPACE_" /D "_AFXDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\common" /I "$(IntDir)" /I "$(IntDir)\..\Common" /I "..\..\Tools\DDK\inc" /I "..\..\Tools\DDK\src\storage\inc" /I "..\..\Tools\Platform SDK\Include" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_DCS_USER_SPACE_" /D "_AFXDLL" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /i "$(IntDir)" /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "$(IntDir)" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ws2_32.lib OSNApi.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /nodefaultlib:"libcmtd" /libpath:"..\..\..\Target\nt40\i386\Debug\DataCore"
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 ws2_32.lib OSNApi.lib OSNLicence.lib OsnDmApi.lib Rpcrt4.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmtd" /nodefaultlib:"nafxcwd" /nodefaultlib:"libcd" /libpath:"..\..\target\win2k\chk\storagetrust\i386" /libpath:"..\..\tools\platform sdk\lib"
# SUBTRACT LINK32 /incremental:no /nodefaultlib
# Begin Special Build Tool
IntDir=.\..\..\Intermediate\win2k\chk\storagetrust\i386\OSNService
OutDir=.\..\..\Target\win2k\chk\storagetrust\i386
TargetPath=\eim_workspace\osn4.0\Target\win2k\chk\storagetrust\i386\OSNService.exe
TargetName=OSNService
SOURCE="$(InputPath)"
PostBuild_Desc=Extracting Symbols...
PostBuild_Cmds=..\..\tools\bin\nmsym /TRANS:source,package,always "/SOURCE:.;..\\Common;..\\..\\Common;$(IntDir);$(Include)" "/OUT:$(OutDir)\Symbols\$(TargetName).NMS" "$(TargetPath)"	move $(OutDir)\$(TargetName).pdb $(OutDir)\Symbols
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "OSNService - Win32 Release Win2k"
# Name "OSNService - Win32 Debug Win2k"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\OSNRpcServer.cpp
# End Source File
# Begin Source File

SOURCE=.\OSNRpcSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\OSNService.cpp
# End Source File
# Begin Source File

SOURCE=.\OSNService.rc
# End Source File
# Begin Source File

SOURCE=.\RemoteCmd.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Configure.h
# End Source File
# Begin Source File

SOURCE=.\OSNRpcCommon.h
# End Source File
# Begin Source File

SOURCE=.\OSNRpcServer.h
# End Source File
# Begin Source File

SOURCE=.\OSNRpcSocket.h
# End Source File
# Begin Source File

SOURCE=.\OSNService.h
# End Source File
# Begin Source File

SOURCE=.\RemoteCmd.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\OSNClient.cfg
# End Source File
# Begin Source File

SOURCE=.\OSNServer.cfg
# End Source File
# Begin Source File

SOURCE=.\OSNService.reg

!IF  "$(CFG)" == "OSNService - Win32 Release Win2k"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Copying Registry File...
TargetDir=\eim_workspace\osn4.0\Target\win2k\fre\storagetrust\i386
InputPath=.\OSNService.reg
InputName=OSNService

"$(TargetDir)\$(InputName).reg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "$(InputPath)" "$(TargetDir)" >nul:

# End Custom Build

!ELSEIF  "$(CFG)" == "OSNService - Win32 Debug Win2k"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Copying Registry File...
TargetDir=\eim_workspace\osn4.0\Target\win2k\chk\storagetrust\i386
InputPath=.\OSNService.reg
InputName=OSNService

"$(TargetDir)\$(InputName).reg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "$(InputPath)" "$(TargetDir)" >nul:

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
