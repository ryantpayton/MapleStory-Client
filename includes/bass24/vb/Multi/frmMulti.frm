VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form frmMulti 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "BASS multiple output example"
   ClientHeight    =   1635
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   6030
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1635
   ScaleWidth      =   6030
   StartUpPosition =   2  'CenterScreen
   Begin VB.CommandButton cmdSwap 
      Caption         =   "swap"
      Height          =   375
      Left            =   5040
      TabIndex        =   4
      Top             =   660
      Width           =   615
   End
   Begin VB.Frame frameMulti 
      Caption         =   " device 1 "
      Height          =   735
      Index           =   0
      Left            =   120
      TabIndex        =   0
      Top             =   0
      Width           =   5775
      Begin VB.CommandButton cmdClone 
         Caption         =   "clone #2"
         Height          =   375
         Index           =   0
         Left            =   4200
         TabIndex        =   5
         Top             =   240
         Width           =   975
      End
      Begin VB.CommandButton cmdOpen 
         Caption         =   "click here to open a file..."
         Height          =   375
         Index           =   0
         Left            =   120
         TabIndex        =   1
         Top             =   240
         Width           =   3975
      End
   End
   Begin MSComDlg.CommonDialog cmd 
      Left            =   3720
      Top             =   960
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.Frame frameMulti 
      Caption         =   " device 2 "
      Height          =   735
      Index           =   1
      Left            =   120
      TabIndex        =   2
      Top             =   840
      Width           =   5775
      Begin VB.CommandButton cmdClone 
         Caption         =   "clone #1"
         Height          =   375
         Index           =   1
         Left            =   4200
         TabIndex        =   6
         Top             =   240
         Width           =   975
      End
      Begin VB.CommandButton cmdOpen 
         Caption         =   "click here to open a file..."
         Height          =   375
         Index           =   1
         Left            =   120
         TabIndex        =   3
         Top             =   240
         Width           =   3975
      End
   End
End
Attribute VB_Name = "frmMulti"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'//////////////////////////////////////////////////////////////////////////////
' frmMulti.frm - Copyright (c) 2003-2007 (: JOBnik! :) [Arthur Aminov, ISRAEL]
'                                                      [http://www.jobnik.org]
'                                                      [  jobnik@jobnik.org  ]
' Other sources: frmDevice.frm and modMulti.bas
'
' BASS Multiple output example
' Originally translated from - multi.c - Example of Ian Luck
'//////////////////////////////////////////////////////////////////////////////
 
Option Explicit

Dim outdev(2) As Long   ' output devices
Dim chan(2) As Long     ' the streams

' display error messages
Sub Error_(ByVal es As String)
    Call MsgBox(es & vbCrLf & vbCrLf & "Error Code : " & BASS_ErrorGetCode, vbExclamation, "Error")
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

    ' Let the user choose the output devices
    With frmDevice
        .SelectDevice 1
        .Show vbModal, Me
        outdev(0) = .device
        .SelectDevice 2
        .Show vbModal, Me
        outdev(1) = .device
    End With

    ' setup output devices
    If (BASS_Init(outdev(0), 44100, 0, Me.hWnd, 0) = 0) Then
        Call Error_("Can't initialize device 1")
        Unload Me
    End If

    If (BASS_Init(outdev(1), 44100, 0, Me.hWnd, 0) = 0) Then
        Call Error_("Can't initialize device 2")
        Unload Me
    End If

    Dim i As BASS_DEVICEINFO
    
    Call BASS_GetDeviceInfo(outdev(0), i)
    frameMulti(0).Caption = " " & VBStrFromAnsiPtr(i.name) & " "

    Call BASS_GetDeviceInfo(outdev(1), i)
    frameMulti(1).Caption = " " & VBStrFromAnsiPtr(i.name) & " "
End Sub

Private Sub Form_Unload(Cancel As Integer)
    ' release both devices
    Call BASS_SetDevice(outdev(0))
    Call BASS_Free
    Call BASS_SetDevice(outdev(1))
    Call BASS_Free
    End
End Sub

Private Sub cmdOpen_Click(index As Integer)
    On Local Error Resume Next    ' if Cancel pressed...

    ' open a file to play on selected device
    cmd.CancelError = True
    cmd.flags = cdlOFNExplorer Or cdlOFNFileMustExist Or cdlOFNHideReadOnly
    cmd.DialogTitle = "Open"
    cmd.Filter = "streamable files|*.mp3;*.mp2;*.mp1;*.ogg;*.wav;*.aif|All files|*.*"
    cmd.ShowOpen

    ' if cancel was pressed, exit the procedure
    If Err.Number = 32755 Then Exit Sub

    Call BASS_StreamFree(chan(index))
    Call BASS_SetDevice(outdev(index)) ' set the device to create stream on

    chan(index) = BASS_StreamCreateFile(BASSFALSE, StrPtr(cmd.filename), 0, 0, BASS_SAMPLE_LOOP)

    If (chan(index) = 0) Then
        cmdOpen(index).Caption = "click here to open a file..."
        Call Error_("Can't play the file")
        Exit Sub
    End If

    Call BASS_ChannelPlay(chan(index), BASSFALSE)   ' play new stream
    cmdOpen(index).Caption = cmd.filename
End Sub

' swap channel devices
Private Sub cmdSwap_Click()
    ' swap handles
    Dim temp As Long
    temp = chan(0)
    chan(0) = chan(1)
    chan(1) = temp

    ' swap text
    Dim temp2 As String
    temp2 = cmdOpen(0).Caption
    cmdOpen(0).Caption = cmdOpen(1).Caption
    cmdOpen(1).Caption = temp2

    ' update the channel devices
    Call BASS_ChannelSetDevice(chan(0), outdev(0))
    Call BASS_ChannelSetDevice(chan(1), outdev(1))
End Sub

' clone on device #1 / #2
Private Sub cmdClone_Click(index As Integer)
    Dim devn As Integer
    devn = index

    Dim i As BASS_CHANNELINFO
    If (BASS_ChannelGetInfo(chan(devn Xor 1), i) = 0) Then
        Call Error_("Nothing to clone")
        Exit Sub
    End If

    Call BASS_StreamFree(chan(devn)) ' free old stream
    Call BASS_SetDevice(outdev(devn)) ' set the device to create stream on

    chan(devn) = BASS_StreamCreate(i.freq, i.chans, i.flags, STREAMPROC_PUSH, 0)

    If (chan(devn) = 0) Then  ' create a "push" stream
        cmdOpen(devn).Caption = "click here to open a file..."
        Call Error_("Can't create clone")
        Exit Sub
    End If

    Call BASS_ChannelLock(chan(devn Xor 1), BASSTRUE) ' lock source stream to synchonise buffer contents
    Call BASS_ChannelSetDSP(chan(devn Xor 1), AddressOf CloneDSP, chan(devn), 0) ' set DSP to feed data to clone
    ' copy buffered data to clone
    Dim c As Long
    c = BASS_ChannelGetData(chan(devn Xor 1), ByVal 0, BASS_DATA_AVAILABLE)
    Dim buf() As Long
    ReDim buf(c) As Long
    c = BASS_ChannelGetData(chan(devn Xor 1), buf(0), c)
    Call BASS_StreamPutData(chan(devn), buf(0), c)
    Erase buf

    Call BASS_ChannelLock(chan(devn Xor 1), BASSFALSE) ' unlock source stream
    Call BASS_ChannelPlay(chan(devn), BASSFALSE) ' play clone
    cmdOpen(devn).Caption = "clone"
End Sub
