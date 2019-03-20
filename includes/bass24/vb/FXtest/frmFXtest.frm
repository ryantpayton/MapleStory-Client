VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form frmFXtest 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "BASS DX8 effects test"
   ClientHeight    =   2025
   ClientLeft      =   45
   ClientTop       =   405
   ClientWidth     =   4455
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2025
   ScaleWidth      =   4455
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin MSComctlLib.Slider sldEQR 
      Height          =   1095
      Index           =   0
      Left            =   360
      TabIndex        =   5
      Top             =   600
      Width           =   675
      _ExtentX        =   1191
      _ExtentY        =   1931
      _Version        =   393216
      Orientation     =   1
      Max             =   20
      TickStyle       =   2
      TickFrequency   =   0
      Value           =   10
   End
   Begin MSComDlg.CommonDialog CMD 
      Left            =   3720
      Top             =   2160
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.CommandButton cmdOpenFP 
      Caption         =   "click here to open a file..."
      Height          =   375
      Left            =   120
      Style           =   1  'Graphical
      TabIndex        =   0
      Top             =   120
      Width           =   4215
   End
   Begin MSComctlLib.Slider sldEQR 
      Height          =   1095
      Index           =   1
      Left            =   1200
      TabIndex        =   6
      Top             =   600
      Width           =   675
      _ExtentX        =   1191
      _ExtentY        =   1931
      _Version        =   393216
      Orientation     =   1
      Max             =   20
      TickStyle       =   2
      TickFrequency   =   0
      Value           =   10
   End
   Begin MSComctlLib.Slider sldEQR 
      Height          =   1095
      Index           =   2
      Left            =   2040
      TabIndex        =   7
      Top             =   600
      Width           =   675
      _ExtentX        =   1191
      _ExtentY        =   1931
      _Version        =   393216
      Orientation     =   1
      Max             =   20
      SelStart        =   10
      TickStyle       =   2
      TickFrequency   =   0
      Value           =   10
   End
   Begin MSComctlLib.Slider sldEQR 
      Height          =   1095
      Index           =   3
      Left            =   3240
      TabIndex        =   8
      Top             =   600
      Width           =   675
      _ExtentX        =   1191
      _ExtentY        =   1931
      _Version        =   393216
      Orientation     =   1
      Max             =   20
      SelStart        =   20
      TickStyle       =   2
      TickFrequency   =   0
      Value           =   20
   End
   Begin VB.Label lblEQR 
      AutoSize        =   -1  'True
      Caption         =   "reverb"
      Height          =   195
      Index           =   3
      Left            =   3360
      TabIndex        =   4
      Top             =   1770
      Width           =   450
   End
   Begin VB.Label lblEQR 
      AutoSize        =   -1  'True
      Caption         =   "8 khz"
      Height          =   195
      Index           =   2
      Left            =   2160
      TabIndex        =   3
      Top             =   1770
      Width           =   480
   End
   Begin VB.Label lblEQR 
      AutoSize        =   -1  'True
      Caption         =   "1 khz"
      Height          =   195
      Index           =   1
      Left            =   1320
      TabIndex        =   2
      Top             =   1770
      Width           =   390
   End
   Begin VB.Label lblEQR 
      AutoSize        =   -1  'True
      Caption         =   "125 hz"
      Height          =   195
      Index           =   0
      Left            =   480
      TabIndex        =   1
      Top             =   1770
      Width           =   480
   End
End
Attribute VB_Name = "frmFXtest"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'///////////////////////////////////////////////////////////////////////////////
' frmFXtest.frm - Copyright (c) 2001-2007 (: JOBnik! :) [Arthur Aminov, ISRAEL]
'                                                       [http://www.jobnik.org]
'                                                       [  jobnik@jobnik.org  ]
'
' BASS DX8 effects test
' Originally translated from - fxtest.c - example of Ian Luck
'///////////////////////////////////////////////////////////////////////////////

Option Explicit

Dim chan As Long         ' channel handle
Dim fx(3) As Long        ' 3 eq band + reverb

' display error messages
Public Sub Error_(ByVal es As String)
    Call MsgBox(es & vbCrLf & vbCrLf & "error code: " & BASS_ErrorGetCode, vbExclamation, "Error")
End Sub

Private Sub Form_Initialize()
    ' change and set the current path, to prevent from VB not finding BASS.DLL
    ChDrive App.Path
    ChDir App.Path

    ' check the correct BASS was loaded
    If (HiWord(BASS_GetVersion) <> BASSVERSION) Then
        Call MsgBox("An incorrect version of BASS.DLL was loaded", vbCritical)
        End
    End If

    ' setup output - default device
    If (BASS_Init(-1, 44100, 0, Me.hWnd, 0) = 0) Then
        Call Error_("Can't initialize device")
        End
    End If

    ' check that DX8 features are available
    Dim bi As BASS_INFO
    Call BASS_GetInfo(bi)
    If (bi.dsver < 8) Then
        Call BASS_Free
        Call Error_("DirectX 8 is not installed")
        End
    End If
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Call BASS_Free
End Sub

Public Sub UpdateFX(ByVal b As Integer)
    Dim v As Integer

    v = sldEQR(b).value

    If (b < 3) Then
        Dim p As BASS_DX8_PARAMEQ
        Call BASS_FXGetParameters(fx(b), p)
        p.fGain = 10# - v
        Call BASS_FXSetParameters(fx(b), p)
    Else
        Dim p1 As BASS_DX8_REVERB
        Call BASS_FXGetParameters(fx(b), p1)
        p1.fReverbMix = -0.012 * v * v * v
        Call BASS_FXSetParameters(fx(b), p1)
    End If
End Sub


Private Sub cmdOpenFP_Click()
    On Local Error Resume Next  ' incase Cancel is pressed

    CMD.filename = ""
    CMD.CancelError = True
    CMD.flags = cdlOFNExplorer Or cdlOFNFileMustExist Or cdlOFNHideReadOnly
    CMD.Filter = "playable files|*.mo3;*.xm;*.mod;*.s3m;*.it;*.mtm;*.umx;*.mp3;*.mp2;*.mp1;*.ogg;*.wav;*.aif|All Files|*.*|"
    CMD.ShowOpen

    ' if cancel was pressed, exit the procedure
    If Err.Number = 32755 Then Exit Sub

    ' free both MOD and stream, it must be one of them! :)
    Call BASS_MusicFree(chan)
    Call BASS_StreamFree(chan)

#If 1 Then ' with FX flag

    chan = BASS_StreamCreateFile(BASSFALSE, StrPtr(CMD.filename), 0, 0, BASS_SAMPLE_LOOP Or BASS_SAMPLE_FX)
    If (chan = 0) Then chan = BASS_MusicLoad(BASSFALSE, StrPtr(CMD.filename), 0, 0, BASS_MUSIC_LOOP Or BASS_MUSIC_RAMP Or BASS_SAMPLE_FX, 1)

#Else   ' without FX flag

    chan = BASS_StreamCreateFile(BASSFALSE, StrPtr(CMD.filename), 0, 0, BASS_SAMPLE_LOOP)
    If (chan = 0) Then chan = BASS_MusicLoad(BASSFALSE, StrPtr(CMD.filename), 0, 0, BASS_MUSIC_LOOP Or BASS_MUSIC_RAMP, 1)

#End If

    If (chan = 0) Then  ' whatever it is, it ain't playable
        cmdOpenFP.Caption = "click here to open a file..."
        Call Error_("Can't play the file")
        Exit Sub
    End If
    
    cmdOpenFP.Caption = GetFileName(CMD.filename)

    ' setup the effects
    Dim p As BASS_DX8_PARAMEQ

    fx(0) = BASS_ChannelSetFX(chan, BASS_FX_DX8_PARAMEQ, 0) ' bass
    fx(1) = BASS_ChannelSetFX(chan, BASS_FX_DX8_PARAMEQ, 0) ' mid
    fx(2) = BASS_ChannelSetFX(chan, BASS_FX_DX8_PARAMEQ, 0) ' treble
    fx(3) = BASS_ChannelSetFX(chan, BASS_FX_DX8_REVERB, 0)  ' reverb

    p.fGain = 0
    p.fBandwidth = 18

    p.fCenter = 125                     ' bass   [125hz]
    Call BASS_FXSetParameters(fx(0), p)

    p.fCenter = 1000                    ' mid    [1khz]
    Call BASS_FXSetParameters(fx(1), p)

    p.fCenter = 8000                    ' treble [8khz]
    Call BASS_FXSetParameters(fx(2), p)

    ' you can add more EQ bands with changing:
    ' p.fCenter = N [Hz] N>=80 and N<=16000
    
    Call UpdateFX(0) ' bass
    Call UpdateFX(1) ' mid
    Call UpdateFX(2) ' treble
    Call UpdateFX(3) ' reverb

    Call BASS_ChannelPlay(chan, BASSFALSE)
End Sub

Private Sub sldEQR_Scroll(index As Integer)
    sldEQR(index).Text = IIf(index < 3, 10 - sldEQR(index).value, 20 - sldEQR(index).value)
    Call UpdateFX(index)
End Sub

'--------------------
' useful function :)
'--------------------

' get file name from file path
Public Function GetFileName(ByVal fp As String) As String
    GetFileName = Mid(fp, InStrRev(fp, "\") + 1)
End Function
