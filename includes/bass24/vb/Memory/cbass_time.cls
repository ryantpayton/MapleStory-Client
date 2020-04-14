VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "cbass_time"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'////////////////////////////////////////////////////////////////////////////////
' cbass_time.cls - Copyright (c) 2001-2007 (: JOBnik! :) [Arthur Aminov, ISRAEL]
'                                                        [http://www.jobnik.org]
'                                                        [  jobnik@jobnik.org  ]
'
' Other sources: frmMemory.frm & SYNCtest.bas
'
' This VB class module, shows how to get:
'  * Total playing time, in seconds, of any stream/music
'  * Playing position in seconds
'  * A function that will convert total seconds into:
'       Hours:Minutes:Seconds   [01:25:50]
'  * and much more... :)
'
'  * Some functions are based on Ian Lucks 'C' examples!
'////////////////////////////////////////////////////////////////////////////////

Option Explicit

Dim info As BASS_CHANNELINFO

' Returns - Total duration from seconds as Time format: HH:MM:SS
Public Function GetTime(ByVal seconds As Long) As String
    Dim hour As Single, min As Single, sec As Single

    hour = seconds / 60 / 60
    sec = seconds Mod 60
    min = (hour - Int(hour)) * 60

    GetTime = Format(Int(hour), "00") & ":" & Format(Int(min), "00") & ":" & Format(Int(sec), "00")
End Function

' Returns - Playing position in seconds
Public Function GetPlayingPos(ByVal handle As Long) As Single
    GetPlayingPos = BASS_ChannelBytes2Seconds(handle, BASS_ChannelGetPosition(handle, BASS_POS_BYTE))
End Function

' Returns - Total duration in seconds
Public Function GetDuration(ByVal handle As Long) As Single
    GetDuration = BASS_ChannelBytes2Seconds(handle, BASS_ChannelGetLength(handle, BASS_POS_BYTE))
End Function

' Returns - Bytes Per Second
Public Function GetBytesPerSecond(ByVal handle As Long) As Long
    Dim bps As Long

    Call BASS_ChannelGetInfo(handle, info) ' stereo/mono, 8/16 bit flags

    bps = info.freq * info.chans
    If (info.flags And BASS_SAMPLE_8BITS) = 0 Then bps = bps * 2

    GetBytesPerSecond = bps
End Function

' Returns - Kilo Bits Per Second
Public Function GetBitsPerSecond(ByVal handle As Long, ByVal FileLength As Long) As Long
    GetBitsPerSecond = CInt(((FileLength * 8) / GetDuration(handle)) / 1000)
End Function

' Returns - 'Stereo'/'Mono' or 'MultiChannel'
Public Function GetMode(ByVal handle As Long) As String
    Call BASS_ChannelGetInfo(handle, info)
    Select Case info.chans
        Case 1: GetMode = "Mono"
        Case 2: GetMode = "Stereo"
        Case Else: GetMode = info.chans & " MultiChannel"
    End Select
End Function

' Returns - 8/16/32-float bits
Public Function GetBits(ByVal handle As Long) As Byte
    Call BASS_ChannelGetInfo(handle, info)
        If (info.flags And BASS_SAMPLE_8BITS) Then
            GetBits = 8
        ElseIf (info.flags And BASS_SAMPLE_FLOAT) Then
            GetBits = 32
        Else
            GetBits = 16
        End If
End Function

' Returns - Sample Rate [Frequency]
Public Function GetFrequency(ByVal handle As Long) As Long
    Dim freq As Single
    Call BASS_ChannelGetAttribute(handle, BASS_ATTRIB_FREQ, freq)
    GetFrequency = freq
End Function

' Returns - DirectX version
Public Function GetDXver() As Byte
    Dim bi As BASS_INFO
    Call BASS_GetInfo(bi)
    GetDXver = bi.dsver
End Function
