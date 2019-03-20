Attribute VB_Name = "modMulti"
'//////////////////////////////////////////////////////////////////////////////
' modMulti.bas - Copyright (c) 2003-2007 (: JOBnik! :) [Arthur Aminov, ISRAEL]
'                                                      [http://www.jobnik.org]
'                                                      [  jobnik@jobnik.org  ]
' Other sources: frmDevice.frm and frmMulti.frm
'
' BASS Multiple output example
' Originally translated from - multi.c - Example of Ian Luck
'//////////////////////////////////////////////////////////////////////////////

Option Explicit

' Cloning DSP function
Sub CloneDSP(ByVal handle As Long, ByVal channel As Long, ByVal buffer As Long, ByVal length As Long, ByVal user As Long)
    Call BASS_StreamPutData(user, ByVal buffer, length)   ' user = clone
End Sub
