VERSION 5.00
Begin VB.Form frmSynth 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "BASS Simple Sinewave Synth"
   ClientHeight    =   3375
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   4350
   BeginProperty Font 
      Name            =   "Arial"
      Size            =   9.75
      Charset         =   177
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   225
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   290
   StartUpPosition =   2  'CenterScreen
   Begin VB.Label lblWinTxt 
      Height          =   855
      Left            =   0
      TabIndex        =   0
      Top             =   2520
      Width           =   4335
   End
End
Attribute VB_Name = "frmSynth"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'//////////////////////////////////////////////////////////////////////////////
' frmSynth.frm - Copyright (c) 2006-2007 (: JOBnik! :) [Arthur Aminov, ISRAEL]
'                                                      [http://www.jobnik.org]
'                                                      [  jobnik@jobnik.org  ]
'
' Other source: modSynth.bas
'
' BASS Simple Synth
' Originally translated from - synth.c - Example of Ian Luck
'//////////////////////////////////////////////////////////////////////////////

Option Explicit

Dim str As Long
Dim fx(9) As Long ' effect handles
Dim r As Long, buflen As Long
Dim fxname As Variant

' display error messages
Sub Error_(ByVal es As String)
    Call MsgBox(es & vbCrLf & vbCrLf & "error code: " & BASS_ErrorGetCode, vbExclamation, "Error")
End Sub

Private Sub Form_Load()
    ' change and set the current path, to prevent from VB not finding BASS.DLL
    ChDrive App.Path
    ChDir App.Path

    fxname = Array("CHORUS", "COMPRESSOR", "DISTORTION", "ECHO", "FLANGER", "GARGLE", "I3DL2REVERB", "PARAMEQ", "REVERB")
    keys = Array("Q", "2", "W", "3", "E", "R", "5", "T", "6", "Y", "7", "U", "I", "9", "O", "0", "P", 219, 187, 221)

    ' check the correct BASS was loaded
    If (HiWord(BASS_GetVersion()) <> BASSVERSION) Then
        Call Error_("An incorrect version of BASS.DLL was loaded")
        End
    End If

    ' 10ms update period
    Call BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD, 10)

    ' setup output - get latency
    If (BASS_Init(-1, 44100, BASS_DEVICE_LATENCY, 0, 0) = 0) Then
        Call Error_("Can't initialize device")
        End
    End If

    ' build sine table
    For r = 0 To TABLESIZE - 1
        sinetable(r) = CLng(Sin(2# * PI * CDbl(r) / TABLESIZE) * 7000#)
    Next r

    Call BASS_GetInfo(info)
    ' default buffer size = update period + 'minbuf'
    Call BASS_SetConfig(BASS_CONFIG_BUFFER, 10 + info.minbuf)
    buflen = BASS_GetConfig(BASS_CONFIG_BUFFER)
    ' if the device's output rate is unknown default to 44100 Hz
    If (info.freq = 0) Then info.freq = 44100
    ' create a stream, stereo so that effects sound nice
    str = BASS_StreamCreate(info.freq, 2, 0, AddressOf WriteStream, 0)

    Me.AutoRedraw = True
    Me.KeyPreview = True

    Print "device latency: " & info.latency & "ms"
    Print "device minbuf : " & info.minbuf & "ms"
    Print "ds version: " & info.dsver & " (effects " & IIf(info.dsver < 8, "disabled", "enabled") & ")"
    Print "press these keys to play:" & vbCrLf
    Print "  2 3  5 6 7  9 0  ="
    Print " Q W ER T Y UI O P[ ]" & vbCrLf
    Print "press -/+ to de/increase the buffer"
    Print "press spacebar to quit" & vbCrLf
    If (info.dsver >= 8) Then ' DX8 effects available
        Print "press F1-F9 to toggle effects" & vbCrLf
    End If
    lblWinTxt.Caption = "using a " & buflen & "ms buffer"

    Call BASS_ChannelPlay(str, BASSFALSE)
End Sub

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
    If (KeyCode = vbKeySpace) Then Call Unload(Me)

    If (KeyCode = vbKeySubtract Or KeyCode = vbKeyAdd) Then
        ' recreate stream with smaller/larger buffer
        Call BASS_StreamFree(str)
        If (KeyCode = vbKeySubtract) Then
            Call BASS_SetConfig(BASS_CONFIG_BUFFER, buflen - 1) ' smaller buffer
        Else
            Call BASS_SetConfig(BASS_CONFIG_BUFFER, buflen + 1) ' larger buffer
        End If
        buflen = BASS_GetConfig(BASS_CONFIG_BUFFER)
        lblWinTxt.Caption = "using a " & buflen & "ms buffer"
        str = BASS_StreamCreate(info.freq, 2, 0, AddressOf WriteStream, 0)
        ' set effects on the new stream
        For r = 0 To 9
            If (fx(r)) Then fx(r) = BASS_ChannelSetFX(str, BASS_FX_DX8_CHORUS + r, 0)
        Next r
        Call BASS_ChannelPlay(str, BASSFALSE)
    End If

    If (KeyCode >= vbKeyF1 And KeyCode <= vbKeyF9) Then
        r = KeyCode - vbKeyF1
        If (fx(r)) Then
            Call BASS_ChannelRemoveFX(str, fx(r))
            fx(r) = 0
            lblWinTxt.Caption = "effect " & fxname(r) & " = OFF"
        Else
            ' set the effect, not bothering with parameters (use defaults)
            fx(r) = BASS_ChannelSetFX(str, BASS_FX_DX8_CHORUS + r, 0)
            If (fx(r)) Then lblWinTxt.Caption = "effect " & fxname(r) & " = ON"
        End If
    End If

    Dim key As Long
    For key = 0 To KEYS_ - 1
        If (KeyCode = keys(key) Or KeyCode = Asc(keys(key))) Then Exit For
    Next key
    If (key <> KEYS_) Then
        If (KeyCode And (vol(key) <> MAXVOL)) Then
            pos(key) = 0
            vol(key) = MAXVOL  ' start key
        ElseIf ((KeyCode = 0) And vol(key)) Then
            vol(key) = vol(key) - 1 ' trigger key fadeout
        End If
    End If
End Sub

Private Sub Form_KeyUp(KeyCode As Integer, Shift As Integer)
    Dim key As Long
    For key = 0 To KEYS_ - 1
        If (KeyCode = keys(key) Or KeyCode = Asc(keys(key))) Then Exit For
    Next key
    If (key <> KEYS_) Then vol(key) = vol(key) - 1        ' trigger key fadeout
End Sub

Private Sub Form_Unload(Cancel As Integer)
    BASS_Free
End Sub
