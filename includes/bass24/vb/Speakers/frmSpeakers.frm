VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form frmSpeakers 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "BASS multi-speaker example"
   ClientHeight    =   3030
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   5175
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3030
   ScaleWidth      =   5175
   StartUpPosition =   2  'CenterScreen
   Begin VB.CommandButton btnSwap 
      Caption         =   "swap"
      Height          =   375
      Index           =   2
      Left            =   4320
      TabIndex        =   10
      Top             =   2040
      Width           =   615
   End
   Begin VB.CommandButton btnSwap 
      Caption         =   "swap"
      Height          =   375
      Index           =   1
      Left            =   4320
      TabIndex        =   9
      Top             =   1320
      Width           =   615
   End
   Begin VB.CommandButton btnSwap 
      Caption         =   "swap"
      Height          =   375
      Index           =   0
      Left            =   4320
      TabIndex        =   8
      Top             =   600
      Width           =   615
   End
   Begin MSComDlg.CommonDialog cmdOpenFile 
      Left            =   4440
      Top             =   2520
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.CommandButton btnOpen 
      Caption         =   "click here to open a file..."
      Height          =   375
      Index           =   0
      Left            =   240
      TabIndex        =   4
      Top             =   240
      Width           =   3975
   End
   Begin VB.Frame Frame 
      Caption         =   "1 - front "
      Height          =   735
      Index           =   0
      Left            =   120
      TabIndex        =   0
      Top             =   0
      Width           =   4935
   End
   Begin VB.Frame Frame 
      Caption         =   "2 - rear "
      Height          =   735
      Index           =   1
      Left            =   120
      TabIndex        =   1
      Top             =   720
      Width           =   4935
      Begin VB.CommandButton btnOpen 
         Caption         =   "click here to open a file..."
         Height          =   375
         Index           =   1
         Left            =   120
         TabIndex        =   5
         Top             =   240
         Width           =   3975
      End
   End
   Begin VB.Frame Frame 
      Caption         =   "3 - center/LFE "
      Height          =   735
      Index           =   2
      Left            =   120
      TabIndex        =   2
      Top             =   1440
      Width           =   4935
      Begin VB.CommandButton btnOpen 
         Caption         =   "click here to open a file..."
         Height          =   375
         Index           =   2
         Left            =   120
         TabIndex        =   6
         Top             =   240
         Width           =   3975
      End
   End
   Begin VB.Frame Frame 
      Caption         =   "4 - rear center "
      Height          =   735
      Index           =   3
      Left            =   120
      TabIndex        =   3
      Top             =   2160
      Width           =   4935
      Begin VB.CommandButton btnOpen 
         Caption         =   "click here to open a file..."
         Height          =   375
         Index           =   3
         Left            =   120
         TabIndex        =   7
         Top             =   240
         Width           =   3975
      End
   End
End
Attribute VB_Name = "frmSpeakers"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'/////////////////////////////////////////////////////////////////////////////////
' frmSpeakers.frm - Copyright (c) 2003-2007 (: JOBnik! :) [Arthur Aminov, ISRAEL]
'                                                         [http://www.jobnik.org]
'                                                         [  jobnik@jobnik.org  ]
'
' BASS multi-speaker example
' Originally translated from - speakers.c - Example of Ian Luck
'/////////////////////////////////////////////////////////////////////////////////

Option Explicit

Dim flags(4) As Long
Dim chan(4) As Long

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
    
    ' initialize BASS - default device
    If (BASS_Init(-1, 44100, 0, Me.hWnd, 0) = 0) Then
        Call Error_("Can't initialize device")
        End
    End If
    
    flags(0) = BASS_SPEAKER_FRONT
    flags(1) = BASS_SPEAKER_REAR
    flags(2) = BASS_SPEAKER_CENLFE
    flags(3) = BASS_SPEAKER_REAR2

    ' check how many speakers the device supports
    Dim i As BASS_INFO
    Call BASS_GetInfo(i)
    If (i.speakers < 4) Then ' no extra speakers detected, enable them anyway?
        If (MsgBox("Do you wish to enable ""speaker assignment"" anyway?", vbYesNo + vbQuestion, "No extra speakers detected") = vbYes) Then
            ' reinitialize BASS - forcing speaker assignment
            Call BASS_Free
            If (BASS_Init(-1, 44100, BASS_DEVICE_SPEAKERS, Me.hWnd, 0) = 0) Then
                Call Error_("Can't initialize device")
                End
            End If
            Call BASS_GetInfo(i) ' get info again
        End If
    End If
    If (i.speakers < 8) Then
        btnOpen(3).Enabled = False
        btnSwap(2).Enabled = False
    End If
    If (i.speakers < 6) Then
        btnOpen(2).Enabled = False
        btnSwap(1).Enabled = False
    End If
    If (i.speakers < 4) Then
        btnOpen(1).Enabled = False
        btnSwap(0).Enabled = False
    End If
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Call BASS_Free
    End
End Sub

Private Sub btnOpen_Click(index As Integer)
    On Local Error Resume Next    ' if Cancel pressed...
    
    Dim speaker As Long
    speaker = index
    
    cmdOpenFile.CancelError = True
    cmdOpenFile.flags = cdlOFNExplorer Or cdlOFNFileMustExist Or cdlOFNHideReadOnly
    cmdOpenFile.DialogTitle = "Open"
    cmdOpenFile.Filter = "streamable files|*.mp3;*.mp2;*.mp1;*.ogg;*.wav;*.aif|All files|*.*"
    cmdOpenFile.ShowOpen
    
    ' if cancel was pressed, exit the procedure
    If Err.Number = 32755 Then Exit Sub
    
    Call BASS_StreamFree(chan(speaker)) ' free old stream before opening new
    
    chan(speaker) = BASS_StreamCreateFile(BASSFALSE, StrPtr(cmdOpenFile.filename), 0, 0, flags(speaker) Or BASS_SAMPLE_LOOP)
    
    If (chan(speaker) = 0) Then
        btnOpen(index).Caption = "click here to open a file..."
        Call Error_("Can't play the file")
        Exit Sub
    End If
    
    btnOpen(speaker).Caption = cmdOpenFile.filename
    Call BASS_ChannelPlay(chan(speaker), BASSFALSE)
End Sub

Private Sub btnSwap_Click(index As Integer)
    Dim speaker As Long
    speaker = index

    ' swap handles
    Dim temp As Long

    temp = chan(speaker)
    chan(speaker) = chan(speaker + 1)
    chan(speaker + 1) = temp

    ' swap text
    Dim temp1 As String

    temp1 = btnOpen(index).Caption
    btnOpen(index).Caption = btnOpen(index + 1).Caption
    btnOpen(index + 1).Caption = temp1

    ' update speaker flags
    Call BASS_ChannelFlags(chan(speaker), flags(speaker), BASS_SPEAKER_FRONT)
    Call BASS_ChannelFlags(chan(speaker + 1), flags(speaker + 1), BASS_SPEAKER_FRONT)
End Sub
