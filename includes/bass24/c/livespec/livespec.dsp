# Microsoft Developer Studio Project File - Name="livespec" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=livespec - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "livespec.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "livespec.mak" CFG="livespec - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "livespec - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../bin"
# PROP BASE Intermediate_Dir "Release"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../bin"
# PROP Intermediate_Dir "Release"
# ADD BASE CPP /nologo /MD /GX /I ".." /D "WIN32" /D "NDEBUG" /FD /c
# ADD CPP /nologo /MD /GX /I ".." /D "WIN32" /D "NDEBUG" /FD /c
BSC32=bscmake.exe
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winmm.lib /nologo /subsystem:windows /pdb:none /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winmm.lib /nologo /subsystem:windows /pdb:none /machine:I386
# Begin Target

# Name "livespec - Win32 Release"
# Begin Source File

SOURCE=livespec.c
# End Source File
# Begin Source File

SOURCE=..\bass.lib
# End Source File
# End Target
# End Project
