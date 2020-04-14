Attribute VB_Name = "modSynth"
'//////////////////////////////////////////////////////////////////////////////
' modSynth.bas - Copyright (c) 2006-2007 (: JOBnik! :) [Arthur Aminov, ISRAEL]
'                                                      [http://www.jobnik.org]
'                                                      [  jobnik@jobnik.org  ]
'
' Other source: frmSynth.frm
'
' BASS Simple Synth
' Originally translated from - synth.c - Example of Ian Luck
'//////////////////////////////////////////////////////////////////////////////

Option Explicit

Public info As BASS_INFO

Public Const PI = 3.14159265358979
Public Const TABLESIZE = 2048
Public sinetable(TABLESIZE) As Long     ' sine table
Public Const KEYS_ = 20
Public keys As Variant
Public Const MAXVOL = 4000    ' higher value = longer fadeout

Public vol(KEYS_) As Long, pos(KEYS_) As Long   ' keys' volume & pos

Public Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (ByRef Destination As Any, ByRef Source As Any, ByVal length As Long)

' stream writer
Public Function WriteStream(ByVal handle As Long, ByVal buffer As Long, ByVal length As Long, ByVal user As Long) As Long
    Dim n As Long, s As Long, c As Long
    Dim f As Single
    Dim buf() As Integer
    ReDim buf(length) As Integer

    For n = 0 To KEYS_ - 1
        If (vol(n)) Then
            f = (2 ^ ((n + 3) / 12#)) * TABLESIZE * 440# / info.freq
            c = 0
            Do While (c < length / 4 And vol(n))
                s = sinetable((pos(n) * f) And (TABLESIZE - 1)) * vol(n) / MAXVOL
                pos(n) = pos(n) + 1
                s = s + buf(c * 2)
                ' clip
                If (s > 32767) Then s = 32767
                If (s < -32768) Then s = -32768
                ' left and right channels are the same
                buf(c * 2) = s
                buf(c * 2 + 1) = s
                If (vol(n) < MAXVOL) Then vol(n) = vol(n) - 1
                c = c + 1
            Loop
        End If
    Next n

    Call CopyMemory(ByVal buffer, buf(0), length)

    WriteStream = length
End Function
