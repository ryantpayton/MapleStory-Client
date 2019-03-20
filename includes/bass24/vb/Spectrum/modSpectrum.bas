Attribute VB_Name = "modSpectrum"
'/////////////////////////////////////////////////////////////////////////////////
' modSpectrum.bas - Copyright (c) 2002-2007 (: JOBnik! :) [Arthur Aminov, ISRAEL]
'                                                         [http://www.jobnik.org]
'                                                         [  jobnik@jobnik.org  ]
'
' Other source: frmSpectrum.frm
'
' Bass spectrum example
' Originally translated from - spectrum.c - Example of Ian Luck
'/////////////////////////////////////////////////////////////////////////////////

Option Explicit

Public Const BI_RGB = 0&
Public Const DIB_RGB_COLORS = 0&    ' color table in RGBs

Public Type BITMAPINFOHEADER
        biSize As Long
        biWidth As Long
        biHeight As Long
        biPlanes As Integer
        biBitCount As Integer
        biCompression As Long
        biSizeImage As Long
        biXPelsPerMeter As Long
        biYPelsPerMeter As Long
        biClrUsed As Long
        biClrImportant As Long
End Type

Public Type RGBQUAD
        rgbBlue As Byte
        rgbGreen As Byte
        rgbRed As Byte
        rgbReserved As Byte
End Type

Public Type BITMAPINFO
        bmiHeader As BITMAPINFOHEADER
        bmiColors(255) As RGBQUAD
End Type

Declare Sub FillMemory Lib "kernel32.dll" Alias "RtlFillMemory" (Destination As Any, ByVal length As Long, ByVal Fill As Byte)
Public Declare Function SetDIBitsToDevice Lib "gdi32" (ByVal hDC As Long, ByVal X As Long, ByVal Y As Long, ByVal dx As Long, ByVal dy As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal Scan As Long, ByVal NumScans As Long, Bits As Any, BitsInfo As BITMAPINFO, ByVal wUsage As Long) As Long

' NOTE: Using an API MM timer (may sometimes Crash your app in an IDE mode)
Public Const TIME_PERIODIC = 1  ' program for continuous periodic event
Public Declare Function timeSetEvent Lib "winmm.dll" (ByVal uDelay As Long, ByVal uResolution As Long, ByVal lpFunction As Long, ByVal dwUser As Long, ByVal uFlags As Long) As Long
Public Declare Function timeKillEvent Lib "winmm.dll" (ByVal uID As Long) As Long
Public timing As Long       ' an API timer Handle

Public Const SPECWIDTH As Long = 368  ' display width
Public Const SPECHEIGHT As Long = 127 ' height (changing requires palette adjustments too)

Public chan As Long         ' stream/music handle

Public specmode As Long, specpos As Long  ' spectrum mode (and marker pos for 2nd mode)
Public specbuf() As Byte    ' a pointer

Public bh As BITMAPINFO     ' bitmap header

' MATH Functions
Public Function Sqrt(ByVal num As Double) As Double
    Sqrt = num ^ 0.5
End Function

Function Log10(ByVal X As Double) As Double
    Log10 = Log(X) / Log(10#)
End Function

' update the spectrum display - the interesting bit :)
Public Sub UpdateSpectrum(ByVal uTimerID As Long, ByVal uMsg As Long, ByVal dwUser As Long, ByVal dw1 As Long, ByVal dw2 As Long)
    Dim X As Long, Y As Long, y1 As Long
    
    If (specmode = 3) Then ' waveform
        Dim c As Long, buf() As Single, ci As BASS_CHANNELINFO
        ReDim specbuf(SPECWIDTH * (SPECHEIGHT + 1)) As Byte ' clear display

        Call BASS_ChannelGetInfo(chan, ci) ' get number of channels
        ReDim buf(ci.chans * SPECWIDTH * LenB(buf(0))) As Single  ' allocate buffer for data

        Call BASS_ChannelGetData(chan, buf(0), (ci.chans * SPECWIDTH * LenB(buf(0))) Or BASS_DATA_FLOAT)  ' get the sample data (floating-point to avoid 8 & 16 bit processing)
        For c = 0 To ci.chans - 1
            For X = 0 To SPECWIDTH - 1
                Dim v As Long
                v = (1 - buf(X * ci.chans + c)) * SPECHEIGHT / 2 ' invert and scale to fit display
                If (v < 0) Then
                    v = 0
                ElseIf (v >= SPECHEIGHT) Then
                    v = SPECHEIGHT - 1
                End If
                If (X = 0) Then Y = v
                Do  ' draw line from previous sample...
                    If (Y < v) Then
                        Y = Y + 1
                    ElseIf (Y > v) Then
                        Y = Y - 1
                    End If
                    specbuf(Y * SPECWIDTH + X) = IIf(c And 1, 127, 1) ' left=green, right=red (could add more colours to palette for more chans)
                Loop While (Y <> v)
            Next X
        Next c
    Else
        Dim fft(1024) As Single     ' get the FFT data
        Call BASS_ChannelGetData(chan, fft(0), BASS_DATA_FFT2048)

        If (specmode = 0) Then ' "normal" FFT
            ReDim specbuf(SPECWIDTH * (SPECHEIGHT + 1)) As Byte ' clear display
            For X = 0 To (SPECWIDTH / 2) - 1
#If 1 Then
                Y = Sqrt(fft(X + 1)) * 3 * SPECHEIGHT - 4 ' scale it (sqrt to make low values more visible)
#Else
                Y = fft(X + 1) * 10 * SPECHEIGHT ' scale it (linearly)
#End If
                If (Y > SPECHEIGHT) Then Y = SPECHEIGHT ' cap it
                If (X) Then  ' interpolate from previous to make the display smoother
                    y1 = (Y + y1) / 2
                    y1 = y1 - 1
                    While (y1 >= 0)
                        specbuf(y1 * SPECWIDTH + X * 2 - 1) = y1 + 1
                        y1 = y1 - 1
                    Wend
                End If
                y1 = Y
                Y = Y - 1
                While (Y >= 0)
                    specbuf(Y * SPECWIDTH + X * 2) = Y + 1 ' draw level
                    Y = Y - 1
                Wend
            Next X
        ElseIf (specmode = 1) Then  ' logarithmic, acumulate & average bins
            ReDim specbuf(SPECWIDTH * (SPECHEIGHT + 1)) As Byte ' clear display
            Dim b0 As Long, BANDS As Integer
            b0 = 0
            BANDS = 28
            Dim sc As Long, b1 As Long
            Dim sum As Single
            For X = 0 To BANDS - 1
                sum = 0
                b1 = 2 ^ (X * 10# / (BANDS - 1))
                If (b1 > 1023) Then b1 = 1023
                If (b1 <= b0) Then b1 = b0 + 1 ' make sure it uses at least 1 FFT bin
                sc = 10 + b1 - b0
                Do
                    sum = sum + fft(1 + b0)
                    b0 = b0 + 1
                Loop While b0 < b1
                Y = (Sqrt(sum / Log10(sc)) * 1.7 * SPECHEIGHT) - 4 ' scale it
                If (Y > SPECHEIGHT) Then Y = SPECHEIGHT ' cap it
                Y = Y - 1
                While (Y >= 0)
                    Call FillMemory(specbuf(Y * SPECWIDTH + X * Int(SPECWIDTH / BANDS)), SPECWIDTH / BANDS - 2, Y + 1)
                    Y = Y - 1
                Wend
            Next X
        Else ' "3D"
            For X = 0 To SPECHEIGHT - 1
                Y = Sqrt(fft(X + 1)) * 3 * 127 ' scale it (sqrt to make low values more visible)
                If (Y > 127) Then Y = 127 ' cap it
                specbuf(X * SPECWIDTH + specpos) = 128 + Y ' plot it
            Next X
            ' move marker onto next position
            specpos = (specpos + 1) Mod SPECWIDTH
            For X = 0 To SPECHEIGHT - 1
                specbuf(X * SPECWIDTH + specpos) = 255
            Next X
        End If
    End If

    ' update the display
    ' to display in a PictureBox, simply change the .hDC to Picture1.hDC :)
    Call SetDIBitsToDevice(frmSpectrum.hDC, 0, 0, SPECWIDTH, SPECHEIGHT, 0, 0, 0, SPECHEIGHT, specbuf(0), bh, 0)
End Sub
