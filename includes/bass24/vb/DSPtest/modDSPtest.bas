Attribute VB_Name = "modDSPtest"
'////////////////////////////////////////////////////////////////////////////////
' modDSPtest.bas - Copyright (c) 2003-2007 (: JOBnik! :) [Arthur Aminov, ISRAEL]
'                                                        [http://www.jobnik.org]
'                                                        [  jobnik@jobnik.org  ]
' Other source: frmDSPtest.frm
'
' BASS simple DSP test
' Originally translated from - dsptest.c - Example of Ian Luck
'////////////////////////////////////////////////////////////////////////////////

Option Explicit

Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (ByRef Destination As Any, ByRef Source As Any, ByVal length As Long)

Public floatable As Long   ' floating-point channel support?
Public chan As Long        ' the channel... HMUSIC or HSTREAM

Public Const PI = 3.1415927

'**********************************************************************************************
'       GLOBAL DSP Variables
'**********************************************************************************************

' "rotate"
Public rotdsp As Long                    ' DSP handle
Public rotpos As Single                  ' cur.pos

' "echo"
Public echdsp As Long                    ' DSP handle
Public Const ECHBUFLEN = 1200            ' buffer length
Public echbuf(ECHBUFLEN, 2) As Single    ' buffer
Public echpos As Long                    ' cur.pos

' "flanger"
Public fladsp As Long                    ' DSP handle
Public Const FLABUFLEN = 350             ' buffer length
Public flabuf(FLABUFLEN, 2) As Single    ' buffer
Public flapos As Long                    ' cur.pos
Public flas As Single, flasinc As Single ' sweep pos/increment

' "swapper"
Public swpdsp As Long                    ' DSP handle

Function fmod(ByVal a As Single, b As Single) As Single
   fmod = a - Fix(a / b) * b
End Function


'**********************************************************************************************
'       DSP Functions
'**********************************************************************************************

' "rotate"
Public Sub Rotate(ByVal handle As Long, ByVal channel As Long, ByVal buffer As Long, ByVal length As Long, ByVal user As Long)
    Dim d() As Single, a As Long
    ReDim d(length / 4) As Single

    Call CopyMemory(d(0), ByVal buffer, length)

    For a = 0 To (length / 4) - 1 Step 2
        d(a) = d(a) * CSng(Abs(Sin(rotpos)))
        d(a + 1) = d(a + 1) * CSng(Abs(Cos(rotpos)))
        rotpos = fmod(rotpos + 0.00003, PI)
    Next a

    Call CopyMemory(ByVal buffer, d(0), length)
End Sub

' "echo"
Public Sub Echo(ByVal handle As Long, ByVal channel As Long, ByVal buffer As Long, ByVal length As Long, ByVal user As Long)
    Dim d() As Single, a As Long
    ReDim d(length / 4) As Single

    Call CopyMemory(d(0), ByVal buffer, length)

    For a = 0 To (length / 4) - 1 Step 2
        Dim l As Single, r As Single
        l = d(a) + (echbuf(echpos, 1) / 2)
        r = d(a + 1) + (echbuf(echpos, 0) / 2)
#If 1 Then  ' 0=echo, 1=basic "bathroom" reverb
        echbuf(echpos, 0) = l
        d(a) = l
        echbuf(echpos, 1) = r
        d(a + 1) = r
#Else
        echbuf(echpos, 0) = d(a)
        echbuf(echpos, 1) = d(a + 1)
        d(a) = l
        d(a + 1) = r
#End If
        echpos = echpos + 1
        If (echpos = ECHBUFLEN) Then echpos = 0
    Next a

    Call CopyMemory(ByVal buffer, d(0), length)
End Sub

' "flanger"
Public Sub Flange(ByVal handle As Long, ByVal channel As Long, ByVal buffer As Long, ByVal length As Long, ByVal user As Long)
    Dim d() As Single, a As Long
    ReDim d(length / 4) As Single

    Call CopyMemory(d(0), ByVal buffer, length)

    For a = 0 To (length / 4) - 1 Step 2
        Dim p1 As Long, p2 As Long
        p1 = (flapos + Int(flas)) Mod FLABUFLEN
        p2 = (p1 + 1) Mod FLABUFLEN
        Dim f As Single, s As Single
        f = fmod(flas, 1)

        s = d(a) + ((flabuf(p1, 0) * (1 - f)) + (flabuf(p2, 0) * f))
        flabuf(flapos, 0) = d(a)
        d(a) = s

        s = d(a + 1) + ((flabuf(p1, 1) * (1 - f)) + (flabuf(p2, 1) * f))
        flabuf(flapos, 1) = d(a + 1)
        d(a + 1) = s

        flapos = flapos + 1
        If (flapos = FLABUFLEN) Then flapos = 0
        flas = flas + flasinc
        If ((flas < 0#) Or (flas > FLABUFLEN)) Then flasinc = -flasinc
    Next a

    Call CopyMemory(ByVal buffer, d(0), length)
End Sub

' "swap between channels"
Public Sub Swapper(ByVal handle As Long, ByVal channel As Long, ByVal buffer As Long, ByVal length As Long, ByVal user As Long)
    Dim d() As Single, tmp As Single, a As Long
    ReDim d(length / 4) As Single

    Call CopyMemory(d(0), ByVal buffer, length)

    For a = 0 To (length / 4) - 1 Step 2
        tmp = d(a)
        d(a) = d(a + 1)
        d(a + 1) = tmp
    Next a

    Call CopyMemory(ByVal buffer, d(0), length)
End Sub
