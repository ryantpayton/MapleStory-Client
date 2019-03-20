Attribute VB_Name = "modLiveFX"
'///////////////////////////////////////////////////////////////////////////////
' modLiveFX.bas - Copyright (c) 2002-2007 (: JOBnik! :) [Arthur Aminov, ISRAEL]
'                                                       [http://www.jobnik.org]
'                                                       [  jobnik@jobnik.org  ]
'
' Other source: frmLiveFX.frm
'
' BASS full-duplex recording test with effects
' Originally translated from - livefx.c - Example of Ian Luck
'///////////////////////////////////////////////////////////////////////////////

Option Explicit

Declare Sub FillMemory Lib "kernel32.dll" Alias "RtlFillMemory" (Destination As Any, ByVal length As Long, ByVal Fill As Byte)
Declare Sub Sleep Lib "kernel32.dll" (ByVal dwMilliseconds As Long)

Public rchan As Long    ' recording channel
Public chan As Long     ' playback stream
Public fx(4) As Long    ' FX handles
Public chunk As Long    ' recording chunk size
Public input_ As Long   ' current input source
Public latency As Long  ' current latency

' Display error message
Public Sub Error_(ByVal es As String)
    Call MsgBox(es & vbCrLf & vbCrLf & "error code: " & BASS_ErrorGetCode, vbExclamation, "Error")
End Sub

Public Function RecordingCallback(ByVal handle As Long, ByVal buffer As Long, ByVal length As Long, ByVal user As Long) As Long
    Call BASS_StreamPutData(chan, ByVal buffer, length) ' feed recorded data to output stream
    RecordingCallback = BASSTRUE ' continue recording
End Function

Static Function Initialize() As Boolean
    Dim bi As BASS_INFO

    ' initialize output, get latency
    If (BASS_Init(-1, 44100, BASS_DEVICE_LATENCY, frmLiveFX.hWnd, 0) = 0) Then
        Call Error_("Can't initialize output")
        Initialize = False
        Exit Function
    End If

    Call BASS_GetInfo(bi)
    If (bi.dsver < 8) Then ' no DX8, so disable effect buttons
        With frmLiveFX
            .chkChorus.Enabled = False
            .chkFlanger.Enabled = False
            .chkGargle.Enabled = False
            .chkReverb.Enabled = False
        End With
    End If

    ' create a stream to play the recording
    chan = BASS_StreamCreate(44100, 2, 0, STREAMPROC_PUSH, 0)

    ' start recording with 10ms period
    Dim rinit As Long
    rinit = BASS_RecordInit(-1)
    rchan = BASS_RecordStart(44100, 2, MakeLong(0, 10), AddressOf RecordingCallback, 0)
    If (rinit = 0 Or rchan = 0) Then
        Call BASS_RecordFree
        Call BASS_Free
        Call Error_("Can't initialize recording")
        Initialize = False
        Exit Function
    End If
    
    ' get list of inputs
    Dim c As Integer
    Dim level As Single
    While BASS_RecordGetInputName(c) <> 0
        frmLiveFX.cmbSelChange.AddItem VBStrFromAnsiPtr(BASS_RecordGetInputName(c))
        If (BASS_RecordGetInput(c, level) And BASS_INPUT_OFF) = 0 Then
            frmLiveFX.cmbSelChange.ListIndex = c  ' this 1 is currently "on"
            input_ = c
            frmLiveFX.sLevel.SelStart = level * 100 ' set level slider
        End If
        c = c + 1
    Wend

    ' prebuffer at least "minbuf" amount of data before starting playback
    Dim prebuf As Long
    prebuf = BASS_ChannelSeconds2Bytes(chan, CSng(bi.minbuf) / 1000)
    While BASS_ChannelGetData(chan, 0, BASS_DATA_AVAILABLE) < prebuf
        Call Sleep(1)
    Wend
    Call BASS_ChannelPlay(chan, BASSFALSE)

    Initialize = True
End Function
