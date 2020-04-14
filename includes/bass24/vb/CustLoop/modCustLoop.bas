Attribute VB_Name = "modCustLoop"
'/////////////////////////////////////////////////////////////////////////////////
' modCustLoop.bas - Copyright (c) 2004-2007 (: JOBnik! :) [Arthur Aminov, ISRAEL]
'                                                         [http://www.jobnik.org]
'                                                         [  jobnik@jobnik.org  ]
' Other source: frmCustLoop.frm
'
' BASS custom looping example
' Originally translated from - custloop.c - Example of Ian Luck
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

Public Declare Function SetDIBitsToDevice Lib "gdi32.dll" (ByVal hdc As Long, ByVal X As Long, ByVal Y As Long, ByVal dx As Long, ByVal dy As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal Scan As Long, ByVal NumScans As Long, Bits As Any, BitsInfo As BITMAPINFO, ByVal wUsage As Long) As Long

Public Const TRANSPARENT = 1
Public Const TA_LEFT = 0
Public Const TA_RIGHT = 2

Public Declare Function SetTextColor Lib "gdi32" (ByVal hdc As Long, ByVal crColor As Long) As Long
Public Declare Function SetBkMode Lib "gdi32" (ByVal hdc As Long, ByVal nBkMode As Long) As Long
Public Declare Function SetTextAlign Lib "gdi32" (ByVal hdc As Long, ByVal wFlags As Long) As Long
Public Declare Function TextOut Lib "gdi32" Alias "TextOutA" (ByVal hdc As Long, ByVal X As Long, ByVal Y As Long, ByVal lpString As String, ByVal nCount As Long) As Long

Public Const WIDTH_ = 600   ' display width
Public Const HEIGHT_ = 201  ' height (odd number for centre line)
Public bpp As Long          ' stream bytes per pixel
Public loop_(2) As Long     ' loop start & end
Public lsync As Long        ' looping sync
Public killscan As Boolean

Public wavebuf() As Byte    ' wave buffer
Public chan As Long         ' stream/music handle

Public bh As BITMAPINFO     ' bitmap header

' display error messages
Public Sub Error_(ByVal es As String)
    Call MsgBox(es & vbCrLf & vbCrLf & "error code: " & BASS_ErrorGetCode, vbExclamation, "Error")
End Sub

Sub LoopSyncProc(ByVal handle As Long, ByVal channel As Long, ByVal data As Long, ByVal user As Long)
    If (BASS_ChannelSetPosition(channel, loop_(0), BASS_POS_BYTE) = 0) Then ' try seeking to loop start
        Call BASS_ChannelSetPosition(channel, 0, BASS_POS_BYTE) ' failed, go to start of file instead
    End If
End Sub

Sub SetLoopStart(ByVal pos As Long)
    loop_(0) = pos
End Sub

Sub SetLoopEnd(ByVal pos As Long)
    loop_(1) = pos
    Call BASS_ChannelRemoveSync(chan, lsync) ' remove old sync
    lsync = BASS_ChannelSetSync(chan, BASS_SYNC_POS Or BASS_SYNC_MIXTIME, loop_(1), AddressOf LoopSyncProc, 0) ' set new sync
End Sub

' scan the peaks
Sub ScanPeaks(ByVal decoder As Long)
    ReDim wavebuf(-120600 To 120600) As Byte    ' set 'n clear the buffer (600 x 201 = 120600)
    Dim cpos As Long, peak(2) As Long

    Do While (Not killscan)
        Dim Level As Long, pos As Long
        Level = BASS_ChannelGetLevel(decoder)  ' scan peaks
        pos = BASS_ChannelGetPosition(decoder, BASS_POS_BYTE) / bpp
        If (peak(0) < LoWord(Level)) Then peak(0) = LoWord(Level) ' set left peak
        If (peak(1) < HiWord(Level)) Then peak(1) = HiWord(Level) ' set right peak
        If (BASS_ChannelIsActive(decoder) = 0) Then
            pos = -1 ' reached the end
        Else
            pos = BASS_ChannelGetPosition(decoder, BASS_POS_BYTE) / bpp
        End If
        If (pos > cpos) Then
            Dim a As Long
            For a = 0 To (peak(0) * (HEIGHT_ / 2) / 32768) - 1
                ' draw left peak
                wavebuf(IIf((HEIGHT_ / 2 - 1 - a) * WIDTH_ + cpos > 120600, 120600, (HEIGHT_ / 2 - 1 - a) * WIDTH_ + cpos)) = 1 + a
            Next a
            For a = 0 To (peak(1) * (HEIGHT_ / 2) / 32768) - 1
                ' draw right peak
                wavebuf(IIf((HEIGHT_ / 2 + 1 + a) * WIDTH_ + cpos > 120600, 120600, (HEIGHT_ / 2 + 1 + a) * WIDTH_ + cpos)) = 1 + a
            Next a
            If (pos >= WIDTH_) Then Exit Do ' gone off end of display
            cpos = pos
            peak(0) = 0
            peak(1) = 0
        End If
        DoEvents
    Loop
    Call BASS_StreamFree(decoder) ' free the decoder
End Sub

' select a file to play, and start scanning it
Function PlayFile() As Boolean
    On Local Error Resume Next    ' if Cancel pressed...

    With frmCustLoop.cmdCustLoop
        .CancelError = True
        .flags = cdlOFNExplorer Or cdlOFNFileMustExist Or cdlOFNHideReadOnly
        .DialogTitle = "Select a file to play"
        .Filter = "Playable files|*.mp3;*.mp2;*.mp1;*.ogg;*.wav;*.aif;*.mo3;*.it;*.xm;*.s3m;*.mtm;*.mod;*.umx|All files|*.*"
        .ShowOpen

        ' if cancel was pressed, exit the procedure
        If Err.Number = 32755 Then Exit Function

        chan = BASS_StreamCreateFile(BASSFALSE, StrPtr(.filename), 0, 0, 0)
        If (chan = 0) Then chan = BASS_MusicLoad(BASSFALSE, StrPtr(.filename), 0, 0, BASS_MUSIC_RAMPS Or BASS_MUSIC_POSRESET Or BASS_MUSIC_PRESCAN, 1)

        If (chan = 0) Then
            Call Error_("Can't play file")
            PlayFile = False ' Can't load the file
            Exit Function
        End If
        
        frmCustLoop.Show   ' show form

        With bh.bmiHeader
            .biSize = Len(bh.bmiHeader)
            .biWidth = WIDTH_
            .biHeight = -HEIGHT_
            .biPlanes = 1
            .biBitCount = 8
            .biClrUsed = HEIGHT_ / 2 + 1
            .biClrImportant = HEIGHT_ / 2 + 1
        End With

        ' setup palette
        Dim a As Byte

        For a = 1 To HEIGHT_ / 2
            bh.bmiColors(a).rgbRed = (255 * a) / (HEIGHT_ / 2)
            bh.bmiColors(a).rgbGreen = 255 - bh.bmiColors(a).rgbRed
        Next a

        bpp = BASS_ChannelGetLength(chan, BASS_POS_BYTE) / WIDTH_ ' bytes per pixel
        If (bpp < BASS_ChannelSeconds2Bytes(chan, 0.02)) Then ' minimum 20ms per pixel (BASS_ChannelGetLevel scans 20ms)
            bpp = BASS_ChannelSeconds2Bytes(chan, 0.02)
        End If
        lsync = BASS_ChannelSetSync(chan, BASS_SYNC_END Or BASS_SYNC_MIXTIME, 0, AddressOf LoopSyncProc, 0) ' set sync to loop at end
        Call BASS_ChannelPlay(chan, BASSFALSE) ' start playing
        frmCustLoop.tmrCustLoop.Enabled = True ' timer's interval is 100ms (10Hz)

        Dim chan2 As Long
        chan2 = BASS_StreamCreateFile(BASSFALSE, StrPtr(.filename), 0, 0, BASS_STREAM_DECODE)
        If (chan2 = 0) Then chan2 = BASS_MusicLoad(BASSFALSE, StrPtr(.filename), 0, 0, BASS_MUSIC_DECODE, 1)
        Call ScanPeaks(chan2)    ' start scanning peaks
    End With
    PlayFile = True
End Function

Sub DrawTimeLine(ByVal dc As Long, ByVal pos As Long, ByVal col As Long, ByVal Y As Long)
    Dim wpos As Long
    wpos = pos / bpp
    Dim time As Long
    time = BASS_ChannelBytes2Seconds(chan, pos)
    Dim text As String
    text = time \ 60 & ":" & Format(time Mod 60, "00")
    frmCustLoop.CurrentX = wpos
    frmCustLoop.Line (wpos, 0)-(wpos, HEIGHT_ - 1), col
    Call SetTextColor(dc, col)
    Call SetBkMode(dc, TRANSPARENT)
    Call SetTextAlign(dc, IIf(wpos >= WIDTH_ / 2, TA_RIGHT, TA_LEFT))
    Call TextOut(dc, wpos, Y, text, Len(text))
End Sub
