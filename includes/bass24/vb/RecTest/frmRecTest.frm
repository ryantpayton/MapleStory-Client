VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Begin VB.Form frmRecTest 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "BASS recording test"
   ClientHeight    =   990
   ClientLeft      =   600
   ClientTop       =   990
   ClientWidth     =   4965
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   990
   ScaleWidth      =   4965
   StartUpPosition =   2  'CenterScreen
   Begin MSComctlLib.Slider sldInputLevel 
      Height          =   255
      Left            =   120
      TabIndex        =   5
      Top             =   720
      Width           =   1455
      _ExtentX        =   2566
      _ExtentY        =   450
      _Version        =   393216
      Max             =   100
      SelectRange     =   -1  'True
      TickStyle       =   3
   End
   Begin VB.ComboBox cmbInput 
      Height          =   315
      Left            =   120
      Style           =   2  'Dropdown List
      TabIndex        =   3
      Top             =   160
      Width           =   1455
   End
   Begin MSComDlg.CommonDialog cmd 
      Left            =   4440
      Top             =   480
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.Timer tmrRecTest 
      Enabled         =   0   'False
      Interval        =   200
      Left            =   3960
      Top             =   480
   End
   Begin VB.CommandButton btnSave 
      Caption         =   "Save"
      Enabled         =   0   'False
      Height          =   300
      Left            =   4080
      TabIndex        =   2
      Top             =   170
      Width           =   735
   End
   Begin VB.CommandButton btnPlay 
      Caption         =   "Play"
      Enabled         =   0   'False
      Height          =   300
      Left            =   3120
      TabIndex        =   1
      Top             =   170
      Width           =   855
   End
   Begin VB.CommandButton btnRecord 
      Caption         =   "Record"
      Height          =   300
      Left            =   1680
      TabIndex        =   0
      Top             =   170
      Width           =   1335
   End
   Begin VB.Label lblPos 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      Height          =   285
      Left            =   1680
      TabIndex        =   6
      Top             =   600
      Width           =   3135
   End
   Begin VB.Label lblInputType 
      Alignment       =   2  'Center
      Height          =   195
      Left            =   120
      TabIndex        =   4
      Top             =   480
      Width           =   1440
   End
End
Attribute VB_Name = "frmRecTest"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'////////////////////////////////////////////////////////////////////////////////
' frmRecTest.frm - Copyright (c) 2002-2007 (: JOBnik! :) [Arthur Aminov, ISRAEL]
'                                                        [http://www.jobnik.org]
'                                                        [  jobnik@jobnik.org  ]
'
' Other source: modRecTest.bas
'
' BASS Recording example
' Originally translated from - rectest.c - Example of Ian Luck
'////////////////////////////////////////////////////////////////////////////////

Option Explicit

Private Sub Form_Load()
    ' change and set the current path, to prevent from VB not finding BASS.DLL
    ChDrive App.Path
    ChDir App.Path

    ' check the correct BASS was loaded
    If (HiWord(BASS_GetVersion) <> BASSVERSION) Then
        Call MsgBox("An incorrect version of BASS.DLL was loaded", vbCritical)
        End
    End If

    ' setup recording and output devices (using default devices)
    If (BASS_RecordInit(-1) = 0) Or (BASS_Init(-1, 44100, 0, Me.hwnd, 0) = 0) Then
        Call Error_("Can't initialize device")
        End
    Else
        ' get list of inputs
        Dim c As Integer
        input_ = -1
        While BASS_RecordGetInputName(c)
            cmbInput.AddItem VBStrFromAnsiPtr(BASS_RecordGetInputName(c))
            If (BASS_RecordGetInput(c, ByVal 0) And BASS_INPUT_OFF) = 0 Then
                cmbInput.ListIndex = c  ' this 1 is currently "on"
                input_ = c
                Call UpdateInputInfo    ' display info
            End If
            c = c + 1
        Wend
    End If

    tmrRecTest.Enabled = True   ' timer to update the position display (200ms)

    recPtr = 0
    reclen = 0
    BUFSTEP = 200000    ' memory allocation unit
End Sub

Private Sub Form_Unload(Cancel As Integer)
    ' release all BASS stuff
    Call GlobalFree(ByVal recPtr)
    Call BASS_RecordFree
    Call BASS_Free
End Sub

' input selection changed
Private Sub cmbInput_Click()
    input_ = cmbInput.ListIndex ' get the selection
    ' enable the selected input
    Dim i As Integer
    For i = 0 To cmbInput.ListCount - 1
        Call BASS_RecordSetInput(i, BASS_INPUT_OFF, -1) ' 1st disable all inputs, then...
    Next i
    Call BASS_RecordSetInput(input_, BASS_INPUT_ON, -1) ' enable the selected input
    Call UpdateInputInfo
End Sub

Private Sub btnPlay_Click()
    Call BASS_ChannelPlay(chan, BASSFALSE)  ' play the recorded data
End Sub

Private Sub btnRecord_Click()
    If (rchan = 0) Then
        Call StartRecording
    Else
        Call StopRecording
    End If
End Sub

Private Sub btnSave_Click()
    Call WriteToDisk
End Sub

' set input source level
Private Sub sldInputLevel_Scroll()
    If BASS_RecordSetInput(input_, 0, sldInputLevel.value / 100) = 0 Then ' failed to set input level
        Call BASS_RecordSetInput(-1, 0, sldInputLevel.value / 100) ' try master level instead
    End If
End Sub

Private Sub tmrRecTest_Timer()
    ' update the recording/playback counter
    If (rchan) Then ' recording/encoding
        lblPos.Caption = BASS_ChannelGetPosition(rchan, BASS_POS_BYTE)
    ElseIf (chan) Then
        If (BASS_ChannelIsActive(chan)) Then ' playing
            lblPos.Caption = BASS_ChannelGetPosition(chan, BASS_POS_BYTE) & " / " & BASS_ChannelGetLength(chan, BASS_POS_BYTE)
        Else
            lblPos.Caption = BASS_ChannelGetLength(chan, BASS_POS_BYTE)
        End If
    End If
End Sub
