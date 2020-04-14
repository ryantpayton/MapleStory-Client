VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form frmSpectrum 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Bass spectrum example (click to toggle mode)"
   ClientHeight    =   1905
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   5520
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   127
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   368
   StartUpPosition =   2  'CenterScreen
   Begin VB.Timer tmrSpectrum 
      Enabled         =   0   'False
      Interval        =   25
      Left            =   4440
      Top             =   1440
   End
   Begin MSComDlg.CommonDialog cmd 
      Left            =   4920
      Top             =   1440
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
End
Attribute VB_Name = "frmSpectrum"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'/////////////////////////////////////////////////////////////////////////////////
' frmSpectrum.frm - Copyright (c) 2002-2007 (: JOBnik! :) [Arthur Aminov, ISRAEL]
'                                                         [http://www.jobnik.org]
'                                                         [  jobnik@jobnik.org  ]
'
' Other source: modSpectrum.bas
'
' Bass spectrum example
' Originally translated from - spectrum.c - Example of Ian Luck
'/////////////////////////////////////////////////////////////////////////////////

Option Explicit

' display error messages
Sub Error_(ByVal es As String)
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

    ' initialize BASS
    If (BASS_Init(-1, 44100, 0, Me.hWnd, 0) = 0) Then
        Call Error_("Can't initialize device")
        End
    End If

    If (Not PlayFile) Then       ' start a file playing
        BASS_Free
        End
    End If

    specpos = 0
    specmode = 0

    ' create bitmap to draw spectrum in - 8 bit for easy updating :)
    With bh.bmiHeader
        .biBitCount = 8
        .biPlanes = 1
        .biSize = Len(bh.bmiHeader)
        .biWidth = SPECWIDTH
        .biHeight = SPECHEIGHT  ' upside down (line 0=bottom)
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

    ' setup update timer (40hz)
#If 1 Then
    tmrSpectrum.Enabled = True
#Else
    timing = timeSetEvent(25, 25, AddressOf UpdateSpectrum, 0, TIME_PERIODIC)  ' API MM timer
#End If
End Sub

Private Sub Form_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    specmode = (specmode + 1) Mod 4  ' swap spectrum mode
    ReDim specbuf(SPECWIDTH * (SPECHEIGHT + 1)) As Byte ' clear display
End Sub

Private Sub Form_Unload(Cancel As Integer)
    If (timing) Then Call timeKillEvent(timing)
    tmrSpectrum.Enabled = False
    Call BASS_Free
    End
End Sub

Function PlayFile() As Boolean
    On Local Error Resume Next    ' if Cancel pressed...

    cmd.CancelError = True
    cmd.flags = cdlOFNExplorer Or cdlOFNFileMustExist Or cdlOFNHideReadOnly
    cmd.DialogTitle = "Select a file to play"
    cmd.Filter = "playable files|*.mo3;*.xm;*.mod;*.s3m;*.it;*.mtm;*.umx;*.mp3;*.mp2;*.mp1;*.ogg;*.wav;*.aif|All files|*.*"
    cmd.ShowOpen

    ' if cancel was pressed, exit the procedure
    If Err.Number = 32755 Then Exit Function

    Call BASS_StreamFree(chan)
    Call BASS_MusicFree(chan)

    chan = BASS_StreamCreateFile(BASSFALSE, StrPtr(cmd.filename), 0, 0, BASS_SAMPLE_LOOP)
    If chan = 0 Then chan = BASS_MusicLoad(BASSFALSE, cmd.filename, 0, 0, BASS_MUSIC_RAMP Or BASS_MUSIC_LOOP, 1)

    If chan = 0 Then
        Call Error_("Selected file couldn't be played!")
        PlayFile = False ' Can't load the file
        Exit Function
    End If

    Call BASS_ChannelPlay(chan, BASSFALSE)

    PlayFile = True
End Function

Private Sub tmrSpectrum_Timer()
    Call UpdateSpectrum(0, 0, 0, 0, 0)  ' the params are if using the API MM timer
End Sub
