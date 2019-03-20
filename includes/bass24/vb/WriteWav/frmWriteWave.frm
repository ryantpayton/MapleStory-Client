VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form frmWriteWave 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "BASS WAVE writer example"
   ClientHeight    =   2295
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   4470
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2295
   ScaleWidth      =   4470
   StartUpPosition =   2  'CenterScreen
   Begin VB.CommandButton btnConvert 
      Caption         =   "Convert to ""BASS.WAV"""
      Enabled         =   0   'False
      Height          =   495
      Left            =   120
      TabIndex        =   3
      Top             =   1680
      Width           =   4215
   End
   Begin VB.CommandButton btnLoadFile 
      Caption         =   "Select File to Convert"
      Height          =   495
      Left            =   120
      TabIndex        =   2
      Top             =   1080
      Width           =   4215
   End
   Begin MSComDlg.CommonDialog cmd 
      Left            =   3840
      Top             =   480
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.Label lblTime 
      AutoSize        =   -1  'True
      Caption         =   "Time:"
      Height          =   195
      Left            =   120
      TabIndex        =   4
      Top             =   360
      Width           =   390
   End
   Begin VB.Label lblStrFile 
      AutoSize        =   -1  'True
      Caption         =   "Streaming File:"
      Height          =   195
      Left            =   120
      TabIndex        =   1
      Top             =   120
      Width           =   1035
   End
   Begin VB.Label lblPos 
      AutoSize        =   -1  'True
      Caption         =   "Pos:"
      Height          =   195
      Left            =   120
      TabIndex        =   0
      Top             =   600
      Width           =   315
   End
End
Attribute VB_Name = "frmWriteWave"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'//////////////////////////////////////////////////////////////////////////////////
' frmWriteWave.frm - Copyright (c) 2002-2007 (: JOBnik! :) [Arthur Aminov, ISRAEL]
'                                                          [http://www.jobnik.org]
'                                                          [  jobnik@jobnik.org  ]
'
' BASS WAVE writer example: MOD/MPx/OGG -> "BASS.WAV"
' Originally translated from - writewav.c - Example of Ian Luck
'//////////////////////////////////////////////////////////////////////////////////

Option Explicit

Dim info As BASS_CHANNELINFO
Dim chan As Long, p As Long
Dim pos As Long
Dim buf() As Byte

Private Type WAVEHEADER_RIFF        ' == 12 bytes ==
    RIFF As Long                    ' "RIFF" = &H46464952
    riffBlockSize As Long           ' pos + 44 - 8
    riffBlockType As Long           ' "WAVE" = &H45564157
End Type

Private Type WAVEHEADER_data        ' == 8 bytes ==
   dataBlockType As Long            ' "data" = &H61746164
   dataBlockSize As Long            ' pos
End Type

Private Type WAVEFORMAT             ' == 24 bytes ==
    wfBlockType As Long             ' "fmt " = &H20746D66
    wfBlockSize As Long
    ' == block size begins from here = 16 bytes
    wFormatTag As Integer
    nChannels As Integer
    nSamplesPerSec As Long
    nAvgBytesPerSec As Long
    nBlockAlign As Integer
    wBitsPerSample As Integer
End Type

Dim wr As WAVEHEADER_RIFF
Dim wf As WAVEFORMAT
Dim wd As WAVEHEADER_data

' display error message
Sub Error_(ByVal Message As String)
    Call MsgBox(Message & vbCrLf & vbCrLf & "Error Code : " & BASS_ErrorGetCode, vbExclamation, "Error")
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

    ' not playing anything, so don't need an update thread
    Call BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD, 0)

    ' setup output - "no sound" device, 44100hz, stereo, 16 bits
    If (BASS_Init(0, 44100, 0, Me.hWnd, 0) = 0) Then
        Call Error_("Can't initialize device")
        End
    End If
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Local Error Resume Next
    
    BASS_Free
    End
End Sub

Private Sub btnLoadFile_Click()
    On Local Error Resume Next    ' if Cancel pressed...

    cmd.CancelError = True
    cmd.flags = cdlOFNExplorer Or cdlOFNFileMustExist Or cdlOFNHideReadOnly
    cmd.DialogTitle = "Select a file to Convert"
    cmd.Filter = "Convertable files (*.mo3;*.xm;*.mod;*.s3m;*.it;*.mtm;*.umx;*.mp3;*.mp2;*.mp1;*.ogg)|*.mo3;*.xm;*.mod;*.s3m;*.it;*.mtm;*.umx;*.mp3;*.mp2;*.mp1;*.ogg"
    cmd.ShowOpen

    ' if cancel was pressed, exit the procedure
    If Err.Number = 32755 Then Exit Sub

    ' try streaming the file
    chan = BASS_StreamCreateFile(BASSFALSE, StrPtr(cmd.filename), 0, 0, BASS_STREAM_DECODE)
    If chan Then
        pos = BASS_ChannelGetLength(chan, BASS_POS_BYTE)
        lblStrFile.Caption = "Streaming file: " & GetFileName(cmd.filename) & " [" & pos & " bytes]"
    End If

    ' try loading the MOD (with sensitive ramping, and calculate the duration)
    If chan = 0 Then
        chan = BASS_MusicLoad(BASSFALSE, StrPtr(cmd.filename), 0, 0, BASS_MUSIC_DECODE Or BASS_MUSIC_RAMP Or BASS_MUSIC_PRESCAN, 0)
        If chan = 0 Then
            ' not a MOD either
            Call Error_("Can't play the file")
            Exit Sub
        Else
            Dim dummy As Single
            p = 0
            While (BASS_ChannelGetAttribute(chan, BASS_ATTRIB_MUSIC_VOL_CHAN + p, dummy)) ' count channels
                p = p + 1
            Wend
            lblStrFile.Caption = "MOD music """ & VBStrFromAnsiPtr(BASS_ChannelGetTags(chan, BASS_TAG_MUSIC_NAME)) & _
                                    """ [" & p & " chans, " & BASS_ChannelGetLength(chan, BASS_POS_MUSIC_ORDER) & " orders]"
            pos = BASS_ChannelGetLength(chan, BASS_POS_BYTE)
        End If
    End If

    ' display the time length
    If (pos) Then
        p = CLng(BASS_ChannelBytes2Seconds(chan, pos))
        lblTime.Caption = "Time: " & CInt(p \ 60) & ":" & Format(CInt(p Mod 60), "00")
    Else ' no time length available
        lblPos.Caption = ""
    End If

    lblPos.Caption = "Pos:"
    btnConvert.Enabled = True
End Sub

Private Sub btnConvert_Click()
    Static convert As Boolean

    convert = Not convert

    If (convert) Then
        btnLoadFile.Enabled = False
        btnConvert.Caption = "Stop conversion..."
    
        Call BASS_ChannelGetInfo(chan, info)

        ' Set WAV Format
        wf.wFormatTag = 1
        wf.nChannels = info.chans
        wf.wBitsPerSample = IIf(info.flags And BASS_SAMPLE_8BITS, 8, 16)
        wf.nBlockAlign = wf.nChannels * wf.wBitsPerSample / 8
        wf.nSamplesPerSec = info.freq
        wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign
    
        ' Set WAV "fmt " header
        wf.wfBlockType = &H20746D66      ' "fmt "
        wf.wfBlockSize = 16
    
        ' Set WAV "RIFF" header
        wr.RIFF = &H46464952             ' "RIFF"
        wr.riffBlockSize = 0             ' after conversion
        wr.riffBlockType = &H45564157    ' "WAVE"
    
        ' set WAV "data" header
        wd.dataBlockType = &H61746164    ' "data"
        wd.dataBlockSize = 0             ' after conversion
    
        ' create a file BASS.WAV
        If (FileExists(RPP(App.Path) & "BASS.WAV")) Then _
            Call Kill(RPP(App.Path) & "BASS.WAV") ' delete if already created and create a new one
        Open RPP(App.Path) & "BASS.WAV" For Binary Lock Read Write As #1
    
        ' Write WAV Header to file
        Put #1, , wr    ' RIFF
        Put #1, , wf    ' Format
        Put #1, , wd    ' data
    
        ReDim buf(19999) As Byte
    
        Do While BASS_ChannelIsActive(chan)
            If Not convert Then Exit Do
            Dim c As Long
            c = BASS_ChannelGetData(chan, buf(0), 20000)
            ' write data to WAV file
            Put #1, , buf
            pos = BASS_ChannelGetPosition(chan, BASS_POS_BYTE)
            lblPos.Caption = "Pos: " & pos
            DoEvents        ' in case you want to stop/exit...
        Loop
    End If

    convert = False

    btnLoadFile.Enabled = True
    btnConvert.Caption = "Convert to ""BASS.WAV"""
    Call CompleteWAVHeader
    
    ' start next conversion from the beginning
    pos = 0
    Call BASS_ChannelSetPosition(chan, 0, BASS_POS_BYTE)
End Sub

Private Sub CompleteWAVHeader()
    ' complete WAV header
    wr.riffBlockSize = pos + 44 - 8
    wd.dataBlockSize = pos

    On Local Error Resume Next

    Put #1, 5, wr.riffBlockSize
    Put #1, 41, wd.dataBlockSize

    Close #1
End Sub

'--------------------------
' some useful functions :)
'--------------------------

' check if any file exists
Public Function FileExists(ByVal fp As String) As Boolean
    FileExists = (Dir(fp) <> "")
End Function

' RPP = Return Proper Path
Function RPP(ByVal fp As String) As String
    RPP = IIf(Mid(fp, Len(fp), 1) <> "\", fp & "\", fp)
End Function

' get file name from file path
Public Function GetFileName(ByVal fp As String) As String
    GetFileName = Mid(fp, InStrRev(fp, "\") + 1)
End Function
