VERSION 5.00
Begin VB.Form frmLiveSpec 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "BASS ""live"" spectrum (click to toggle mode)"
   ClientHeight    =   1905
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   5520
   BeginProperty Font 
      Name            =   "Arial"
      Size            =   9.75
      Charset         =   177
      Weight          =   700
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   127
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   368
   StartUpPosition =   2  'CenterScreen
   Begin VB.Timer tmrLiveSpec 
      Enabled         =   0   'False
      Interval        =   25
      Left            =   5040
      Top             =   1440
   End
End
Attribute VB_Name = "frmLiveSpec"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'/////////////////////////////////////////////////////////////////////////////////
' frmLiveSpec.frm - Copyright (c) 2002-2007 (: JOBnik! :) [Arthur Aminov, ISRAEL]
'                                                         [http://www.jobnik.org]
'                                                         [  jobnik@jobnik.org  ]
' Other source: modLiveSpec.bas
'
' BASS "Live" spectrum analyser example
' Originally translated from - livespec.c - Example of Ian Luck
'/////////////////////////////////////////////////////////////////////////////////

Option Explicit

' display error messages
Public Sub Error_(ByVal es As String)
    Call MsgBox(es & vbCrLf & vbCrLf & "error code: " & BASS_ErrorGetCode, vbExclamation, "Error")
End Sub

Private Sub Form_Load()
    ' change and set the current path, to prevent from VB not finding BASS.DLL
    ChDrive App.Path
    ChDir App.Path

    ' check the correct BASS was loaded
    If (HiWord(BASS_GetVersion) <> BASSVERSION) Then
        Call MsgBox("An incorrect version of BASS.DLL was loaded", vbCritical)
        End
    End If

    ' initialize BASS recording (default device)
    If (BASS_RecordInit(-1) = 0) Then
        Call Error_("Can't initialize device")
        End
    End If

    ' start recording (44100hz mono 16-bit)
    chan = BASS_RecordStart(44100, 1, 0, AddressOf DuffRecording, 0)
    If chan = 0 Then
        Call Error_("Can't start recording")
        End
    End If

    specpos = 0
    specmode = 0

    ' create bitmap to draw spectrum in - 8 bit for easy updating :)
    With bh.bmiHeader
        .biSize = Len(bh.bmiHeader)
        .biWidth = SPECWIDTH
        .biHeight = SPECHEIGHT  ' upside down (line 0=bottom)
        .biPlanes = 1
        .biBitCount = 8
        .biClrUsed = 256
        .biClrImportant = 256
    End With

    Dim a As Byte

    ' setup palette
    For a = 1 To 127
        bh.bmiColors(a).rgbGreen = 256 - 2 * a
        bh.bmiColors(a).rgbRed = 2 * a
    Next a
    For a = 0 To 31
        bh.bmiColors(128 + a).rgbBlue = 8 * a
        bh.bmiColors(128 + 32 + a).rgbBlue = 255
        bh.bmiColors(128 + 32 + a).rgbRed = 8 * a
        bh.bmiColors(128 + 64 + a).rgbRed = 255
        bh.bmiColors(128 + 64 + a).rgbBlue = 8 * (31 - a)
        bh.bmiColors(128 + 64 + a).rgbGreen = 8 * a
        bh.bmiColors(128 + 96 + a).rgbRed = 255
        bh.bmiColors(128 + 96 + a).rgbGreen = 255
        bh.bmiColors(128 + 96 + a).rgbBlue = 8 * a
    Next a

    ' update timer 25ms (40hz)
    tmrLiveSpec.Enabled = True
End Sub

Private Sub Form_Unload(Cancel As Integer)
    tmrLiveSpec.Enabled = False
    Call BASS_RecordFree
End Sub

Private Sub Form_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    specmode = (specmode + 1) Mod 4  ' swap spectrum mode
    ReDim specbuf(SPECWIDTH * (SPECHEIGHT + 1)) As Byte ' clear display
End Sub

Private Sub tmrLiveSpec_Timer()
    Call UpdateSpectrum
End Sub
