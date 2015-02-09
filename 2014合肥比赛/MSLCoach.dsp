# Microsoft Developer Studio Project File - Name="MSLCoach" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MSLCoach - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MSLCoach.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MSLCoach.mak" CFG="MSLCoach - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MSLCoach - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MSLCoach - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MSLCoach - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "MSLCoach - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "MSLCoach - Win32 Release"
# Name "MSLCoach - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ballshot.cpp
# End Source File
# Begin Source File

SOURCE=.\CampCBox.cpp
# End Source File
# Begin Source File

SOURCE=.\ClrStr.cpp
# End Source File
# Begin Source File

SOURCE=.\CoachUDP.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ConctDomain.cpp
# End Source File
# Begin Source File

SOURCE=.\DebugView.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRefInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\MSLCoach.cpp
# End Source File
# Begin Source File

SOURCE=.\MSLCoach.rc
# End Source File
# Begin Source File

SOURCE=.\MSLCoachDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MulticastSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\NumDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\OwnGoalCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\PositionCycBuf.cpp
# End Source File
# Begin Source File

SOURCE=.\RefClient.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TCPClient.cpp
# End Source File
# Begin Source File

SOURCE=.\Xml\tinystr.cpp
# End Source File
# Begin Source File

SOURCE=.\Xml\tinyxml.cpp
# End Source File
# Begin Source File

SOURCE=.\Xml\tinyxmlerror.cpp
# End Source File
# Begin Source File

SOURCE=.\Xml\tinyxmlparser.cpp
# End Source File
# Begin Source File

SOURCE=.\ToAgent.cpp
# End Source File
# Begin Source File

SOURCE=.\UDPServer.cpp
# End Source File
# Begin Source File

SOURCE=.\UPFullField.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ballshot.h
# End Source File
# Begin Source File

SOURCE=.\CampCBox.h
# End Source File
# Begin Source File

SOURCE=.\ClrStr.h
# End Source File
# Begin Source File

SOURCE=.\CoachUDP.h
# End Source File
# Begin Source File

SOURCE=.\ColorDlg.h
# End Source File
# Begin Source File

SOURCE=.\ConctDomain.h
# End Source File
# Begin Source File

SOURCE=.\DebugView.h
# End Source File
# Begin Source File

SOURCE=.\DlgDebug.h
# End Source File
# Begin Source File

SOURCE=.\DlgRefInfo.h
# End Source File
# Begin Source File

SOURCE=.\MSG_Control.h
# End Source File
# Begin Source File

SOURCE=.\MSL_Protocol.h
# End Source File
# Begin Source File

SOURCE=.\MSLCoach.h
# End Source File
# Begin Source File

SOURCE=.\MSLCoachDlg.h
# End Source File
# Begin Source File

SOURCE=.\MulticastSocket.h
# End Source File
# Begin Source File

SOURCE=.\NumDisplay.h
# End Source File
# Begin Source File

SOURCE=.\OwnGoalCombo.h
# End Source File
# Begin Source File

SOURCE=.\PositionCycBuf.h
# End Source File
# Begin Source File

SOURCE=.\RefClient.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TCPClient.h
# End Source File
# Begin Source File

SOURCE=.\Xml\tinystr.h
# End Source File
# Begin Source File

SOURCE=.\Xml\tinyxml.h
# End Source File
# Begin Source File

SOURCE=.\ToAgent.h
# End Source File
# Begin Source File

SOURCE=.\UDPServer.h
# End Source File
# Begin Source File

SOURCE=.\UPFullField.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\BlueFullField.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FullField.bmp
# End Source File
# Begin Source File

SOURCE=.\res\L2RFullField.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MSLCoach.ico
# End Source File
# Begin Source File

SOURCE=.\res\MSLCoach.rc2
# End Source File
# Begin Source File

SOURCE=.\res\Offline.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Online.bmp
# End Source File
# Begin Source File

SOURCE=.\res\R2LFullField.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\res\style.manifest
# End Source File
# End Target
# End Project
