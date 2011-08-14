# Microsoft Developer Studio Project File - Name="OpenMahjongClientDbg" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=OpenMahjongClientDbg - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "OpenMahjongClientDbg.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "OpenMahjongClientDbg.mak" CFG="OpenMahjongClientDbg - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "OpenMahjongClientDbg - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "OpenMahjongClientDbg - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OpenMahjongClientDbg - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\AILib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib /nologo /subsystem:windows /machine:I386 /libpath:"C:\Program Files\MSXML 4.0\lib"

!ELSEIF  "$(CFG)" == "OpenMahjongClientDbg - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\AILib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"C:\Program Files\MSXML 4.0\lib"

!ENDIF 

# Begin Target

# Name "OpenMahjongClientDbg - Win32 Release"
# Name "OpenMahjongClientDbg - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AgariDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DebugDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Komae.cpp
# End Source File
# Begin Source File

SOURCE=.\MahjongBtn.cpp
# End Source File
# Begin Source File

SOURCE=.\MahjongStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\MessageEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\MyButton.cpp
# End Source File
# Begin Source File

SOURCE=.\OpenMahjongClientDbg.cpp
# End Source File
# Begin Source File

SOURCE=.\OpenMahjongClientDbg.rc
# End Source File
# Begin Source File

SOURCE=.\OpenMahjongClientDbgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RuleDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\StateButton.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\VolumeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WaveFilePlayMethod.cpp
# End Source File
# Begin Source File

SOURCE=.\WavePlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\WavePlayMatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\WavePlayMethod.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AgariDialog.h
# End Source File
# Begin Source File

SOURCE=..\AILib\AILib.h
# End Source File
# Begin Source File

SOURCE=.\ConnectDlg.h
# End Source File
# Begin Source File

SOURCE=.\DebugDialog.h
# End Source File
# Begin Source File

SOURCE=.\Komae.h
# End Source File
# Begin Source File

SOURCE=.\MahjongBtn.h
# End Source File
# Begin Source File

SOURCE=.\MahjongStatic.h
# End Source File
# Begin Source File

SOURCE=.\MessageEdit.h
# End Source File
# Begin Source File

SOURCE=..\AILib\MJ0.h
# End Source File
# Begin Source File

SOURCE=.\MyButton.h
# End Source File
# Begin Source File

SOURCE=.\NakiMentsu.h
# End Source File
# Begin Source File

SOURCE=..\..\Qt\OpenMahjongClient\common\OMXMLCommon.h
# End Source File
# Begin Source File

SOURCE=.\OpenMahjongClientDbg.h
# End Source File
# Begin Source File

SOURCE=.\OpenMahjongClientDbgDlg.h
# End Source File
# Begin Source File

SOURCE=.\OpenMahjongDef.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RuleDialog.h
# End Source File
# Begin Source File

SOURCE=.\StateButton.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\VolumeDlg.h
# End Source File
# Begin Source File

SOURCE=.\WaveFilePlayMethod.h
# End Source File
# Begin Source File

SOURCE=.\WavePlayer.h
# End Source File
# Begin Source File

SOURCE=.\WavePlayMatrix.h
# End Source File
# Begin Source File

SOURCE=.\WavePlayMethod.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\OpenMahjongClientDbg.ico
# End Source File
# Begin Source File

SOURCE=.\res\OpenMahjongClientDbg.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
