VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form frmCustLoop 
   AutoRedraw      =   -1  'True
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "BASS custom looping example (left-click to set loop start, right-click to set end)"
   ClientHeight    =   3015
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   9000
   BeginProperty Font 
      Name            =   "MS Sans Serif"
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
   ScaleHeight     =   201
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   600
   StartUpPosition =   2  'CenterScreen
   Begin VB.Timer tmrCustLoop 
      Enabled         =   0   'False
      Interval        =   100
      Left            =   7800
      Top             =   2400
   End
   Begin MSComDlg.CommonDialog cmdCustLoop 
      Left            =   8400
      Top             =   2400
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
End
Attribute VB_Name = "frmCustLoop"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'/////////////////////////////////////////////////////////////////////////////////
' frmCustLoop.frm - Copyright (c) 2004-2007 (: JOBnik! :) [Arthur Aminov, ISRAEL]
'                                                         [http://www.jobnik.org]
'                                                         [  jobnik@jobnik.org  ]
' Other source: modCustLoop.bas
'
' BASS custom looping example
' Originally translated from - custloop.c - Example of Ian Luck
'/////////////////////////////////////////////////////////////////////////////////

Option Explicit

Private Sub Form_Load()
    ' change and set the current path, to prevent from VB not finding BASS.DLL
    Call ChDrive(App.Path)
    Call ChDir(App.Path)

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

    If (Not PlayFile) Then ' start a file playing
        Call BASS_Free
        End
    End If
End Sub

Private Sub Form_Unload(Cancel As Integer)
    killscan = True
    tmrCustLoop.Enabled = False
    Call BASS_Free
    End
End Sub

Private Sub Form_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    If (Button = vbLeftButton) Then ' set loop start
        Call SetLoopStart(X * bpp)
        Call DrawTimeLine(Me.hdc, loop_(0), &HFFFF00, 12)  ' loop start
    ElseIf (Button = vbRightButton) Then    ' set loop end
        Call SetLoopEnd(X * bpp)
        Call DrawTimeLine(Me.hdc, loop_(1), vbYellow, 24) ' loop end
    End If
End Sub

Private Sub Form_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    If ((X >= 0) And (X < WIDTH_)) Then
        If (Button = vbLeftButton) Then
            Call SetLoopStart(X * bpp)
        ElseIf (Button = vbRightButton) Then
            Call SetLoopEnd(X * bpp)
        End If
    End If
End Sub

Private Sub tmrCustLoop_Timer()
    With Me
        ' draw buffered peak waveform
        Call SetDIBitsToDevice(.hdc, 0, 0, WIDTH_, HEIGHT_, 0, 0, 0, HEIGHT_, wavebuf(-(WIDTH_ / 2)), bh, 0)
        Call DrawTimeLine(.hdc, BASS_ChannelGetPosition(chan, BASS_POS_BYTE), &HFFFFFF, 0) ' current pos
        Call DrawTimeLine(.hdc, loop_(0), &HFFFF00, 12) ' loop start
        Call DrawTimeLine(.hdc, loop_(1), vbYellow, 24) ' loop end
    End With
End Sub
