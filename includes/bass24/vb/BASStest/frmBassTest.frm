VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Begin VB.Form frmBassTest 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "BASS - simple playback test"
   ClientHeight    =   3135
   ClientLeft      =   10425
   ClientTop       =   3825
   ClientWidth     =   7830
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   3135
   ScaleWidth      =   7830
   Begin MSComctlLib.Slider sldVol 
      Height          =   195
      Left            =   4560
      TabIndex        =   23
      Top             =   2760
      Width           =   1995
      _ExtentX        =   3519
      _ExtentY        =   344
      _Version        =   393216
      LargeChange     =   10
      Max             =   100
      SelStart        =   100
      TickFrequency   =   10
      Value           =   100
   End
   Begin VB.CommandButton cmdResumeAll 
      Caption         =   "Resume"
      Height          =   375
      Left            =   3120
      TabIndex        =   14
      Top             =   2580
      Width           =   1215
   End
   Begin VB.CommandButton cmdStopAll 
      Caption         =   "Stop Output"
      Height          =   375
      Left            =   1320
      TabIndex        =   13
      Top             =   2580
      Width           =   1695
   End
   Begin VB.Timer tmrBass 
      Enabled         =   0   'False
      Interval        =   250
      Left            =   600
      Top             =   2550
   End
   Begin VB.Frame frameStream 
      Caption         =   "Stream"
      Height          =   2415
      Left            =   120
      TabIndex        =   12
      Top             =   0
      Width           =   2415
      Begin MSComctlLib.Slider sldVolGlStr 
         Height          =   195
         Left            =   150
         TabIndex        =   25
         Top             =   2160
         Width           =   2115
         _ExtentX        =   3731
         _ExtentY        =   344
         _Version        =   393216
         LargeChange     =   1000
         SmallChange     =   100
         Max             =   10000
         SelStart        =   10000
         TickFrequency   =   1000
         Value           =   10000
      End
      Begin VB.CommandButton cmdStreamRemove 
         Caption         =   "Remove"
         Height          =   375
         Left            =   1200
         TabIndex        =   18
         Top             =   1560
         Width           =   1095
      End
      Begin VB.CommandButton cmdStreamRestart 
         Caption         =   "Restart"
         Height          =   375
         Left            =   1560
         TabIndex        =   20
         Top             =   1200
         Width           =   735
      End
      Begin VB.CommandButton cmdStreamAdd 
         Caption         =   "Add ..."
         Height          =   375
         Left            =   120
         TabIndex        =   19
         Top             =   1560
         Width           =   1095
      End
      Begin VB.CommandButton cmdStreamStop 
         Caption         =   "Stop"
         Height          =   375
         Left            =   840
         TabIndex        =   21
         Top             =   1200
         Width           =   735
      End
      Begin VB.ListBox lstStream 
         Height          =   840
         Left            =   120
         TabIndex        =   17
         Top             =   240
         Width           =   2175
      End
      Begin VB.CommandButton cmdStreamPlay 
         Caption         =   "Play"
         Height          =   375
         Left            =   120
         TabIndex        =   22
         Top             =   1200
         Width           =   735
      End
      Begin VB.Label Label2 
         Caption         =   "global volume"
         Height          =   195
         Left            =   180
         TabIndex        =   26
         Top             =   1965
         Width           =   1455
      End
   End
   Begin VB.Frame frameSamples 
      Caption         =   "Sample"
      Height          =   2415
      Left            =   5160
      TabIndex        =   7
      Top             =   0
      Width           =   2415
      Begin VB.CommandButton cmdSampleRemove 
         Caption         =   "Remove"
         Height          =   375
         Left            =   1200
         TabIndex        =   11
         Top             =   1560
         Width           =   1095
      End
      Begin VB.CommandButton cmdSampleAdd 
         Caption         =   "Add ..."
         Height          =   375
         Left            =   120
         TabIndex        =   10
         Top             =   1560
         Width           =   1095
      End
      Begin VB.ListBox lstSamples 
         Height          =   840
         Left            =   120
         TabIndex        =   9
         Top             =   240
         Width           =   2175
      End
      Begin VB.CommandButton cmdSamplePlay 
         Caption         =   "Play"
         Height          =   375
         Left            =   120
         TabIndex        =   8
         Top             =   1200
         Width           =   2175
      End
      Begin MSComctlLib.Slider sldVolglSam 
         Height          =   195
         Left            =   180
         TabIndex        =   29
         Top             =   2160
         Width           =   2115
         _ExtentX        =   3731
         _ExtentY        =   344
         _Version        =   393216
         LargeChange     =   1000
         SmallChange     =   100
         Max             =   10000
         SelStart        =   10000
         TickFrequency   =   1000
         Value           =   10000
      End
      Begin VB.Label Label4 
         Caption         =   "global volume"
         Height          =   195
         Left            =   240
         TabIndex        =   30
         Top             =   1965
         Width           =   1455
      End
   End
   Begin MSComDlg.CommonDialog DLG 
      Left            =   120
      Top             =   2520
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.Frame frameMusic 
      Caption         =   "MOD Music"
      Height          =   2415
      Left            =   2640
      TabIndex        =   0
      Top             =   0
      Width           =   2415
      Begin VB.CommandButton cmdMusicRemove 
         Caption         =   "Remove"
         Height          =   375
         Left            =   1200
         TabIndex        =   6
         Top             =   1560
         Width           =   1095
      End
      Begin VB.CommandButton cmdMusicAdd 
         Caption         =   "Add ..."
         Height          =   375
         Left            =   120
         TabIndex        =   5
         Top             =   1560
         Width           =   1095
      End
      Begin VB.CommandButton cmdMusicRestart 
         Caption         =   "Restart"
         Height          =   375
         Left            =   1560
         TabIndex        =   4
         Top             =   1200
         Width           =   735
      End
      Begin VB.CommandButton cmdMusicStop 
         Caption         =   "Stop"
         Height          =   375
         Left            =   840
         TabIndex        =   3
         Top             =   1200
         Width           =   735
      End
      Begin VB.CommandButton cmdMusicPlay 
         Caption         =   "Play"
         Height          =   375
         Left            =   120
         TabIndex        =   2
         Top             =   1200
         Width           =   735
      End
      Begin VB.ListBox lstMusic 
         Height          =   840
         Left            =   120
         TabIndex        =   1
         Top             =   240
         Width           =   2175
      End
      Begin MSComctlLib.Slider sldVolGlMus 
         Height          =   195
         Left            =   150
         TabIndex        =   27
         Top             =   2160
         Width           =   2115
         _ExtentX        =   3731
         _ExtentY        =   344
         _Version        =   393216
         LargeChange     =   1000
         SmallChange     =   100
         Max             =   10000
         SelStart        =   10000
         TickFrequency   =   1000
         Value           =   10000
      End
      Begin VB.Label Label3 
         Caption         =   "global volume"
         Height          =   195
         Left            =   210
         TabIndex        =   28
         Top             =   1965
         Width           =   1455
      End
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "Volume"
      Height          =   195
      Left            =   5280
      TabIndex        =   24
      Top             =   2580
      Width           =   525
   End
   Begin VB.Label lblCPUP 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "CPU%"
      Height          =   195
      Left            =   6780
      TabIndex        =   16
      Top             =   2700
      Width           =   450
   End
   Begin VB.Label lblCPU 
      Alignment       =   2  'Center
      AutoSize        =   -1  'True
      Caption         =   "0.0"
      Height          =   195
      Left            =   7320
      TabIndex        =   15
      Top             =   2700
      Width           =   240
   End
End
Attribute VB_Name = "frmBassTest"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'*****************************************************************
'* BASS Simple test (rev .1), copyright (c) 1999 Adam Hoult.     *
'*                                                               *
'* Updated: 2003-2007 by (: JOBnik! :) [Arthur Aminov, ISRAEL]   *
'*                                     [http://www.jobnik.org]   *
'*                                     [  jobnik@jobnik.org  ]   *
'*                                                               *
'* Originally translated from - basstest.c - example of Ian Luck *
'*****************************************************************

Option Explicit

' display error messages
Public Sub Error_(ByVal es As String)
    Call MsgBox(es & vbCrLf & "(error code: " & BASS_ErrorGetCode() & ")", vbExclamation, "Error")
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

    ' Initialize output - default device, 44100hz, stereo, 16 bits
    If BASS_Init(-1, 44100, 0, Me.hWnd, 0) = BASSFALSE Then
        Call Error_("Can't initialize digital sound system")
        End
    End If

    ' Start the timer
    tmrBass.Enabled = True

    DLG.filename = ""
    DLG.CancelError = True
    DLG.flags = cdlOFNExplorer Or cdlOFNFileMustExist Or cdlOFNHideReadOnly
End Sub

Private Sub Form_Unload(Cancel As Integer)
    ' stop timer
    tmrBass.Enabled = False

    ' Close sound system and release everything
    Call BASS_Free
End Sub

' Pause output
Private Sub cmdStopAll_Click()
    Call BASS_Pause
End Sub

' Resume output
Private Sub cmdResumeAll_Click()
    Call BASS_Start
End Sub

Private Sub cmdStreamAdd_Click()
    On Local Error Resume Next    ' incase Cancel is pressed

    DLG.Filter = "Streamable Files (wav/aif/mp3/mp2/mp1/ogg)|*.wav;*.aif;*.mp3;*.mp2;*.mp1;*.ogg|All Files (*.*)|*.*|"
    DLG.ShowOpen

    ' if cancel was pressed, exit the procedure
    If Err.Number = 32755 Then Exit Sub

    Dim StreamHandle As Long
    
    StreamHandle = BASS_StreamCreateFile(BASSFALSE, StrPtr(DLG.filename), 0, 0, 0)

    If StreamHandle = 0 Then
        Call Error_("Can't open stream")
    Else
        lstStream.AddItem GetFileName(DLG.filename)
        lstStream.ItemData(lstStream.ListCount - 1) = StreamHandle
    End If
End Sub

' Free the selected stream resource
' Remove the selected list
Private Sub cmdStreamRemove_Click()
    If (lstStream.ListIndex >= 0) Then
        Call BASS_StreamFree(lstStream.ItemData(lstStream.ListIndex))
        lstStream.RemoveItem lstStream.ListIndex
    End If
End Sub

' Play the stream (continue from current position)
Private Sub cmdStreamPlay_Click()
    If (lstStream.ListIndex >= 0) Then _
            If (BASS_ChannelPlay(lstStream.ItemData(lstStream.ListIndex), BASSFALSE) = 0) Then _
            Call Error_("Can't play stream")
End Sub

' Stop the stream
Private Sub cmdStreamStop_Click()
    If (lstStream.ListIndex >= 0) Then _
            Call BASS_ChannelStop(lstStream.ItemData(lstStream.ListIndex))
End Sub

' Play the stream from the start
Private Sub cmdStreamRestart_Click()
    If (lstStream.ListIndex >= 0) Then _
            Call BASS_ChannelPlay(lstStream.ItemData(lstStream.ListIndex), BASSTRUE)
End Sub

Private Sub cmdMusicAdd_Click()
    On Local Error Resume Next

    DLG.Filter = "MOD Music Files (mo3/xm/mod/s3m/it/mtm/umx)|*.mo3;*.xm;*.mod;*.s3m;*.it;*.mtm;*.umx|All Files (*.*)|*.*|"
    DLG.ShowOpen

    ' if cancel was pressed, exit the procedure
    If Err.Number = 32755 Then Exit Sub

    Dim ModHandle As Long

    ' Load a music from "DLG.FileName" and make it use ramping
    ModHandle = BASS_MusicLoad(BASSFALSE, StrPtr(DLG.filename), 0, 0, BASS_MUSIC_RAMPS, 1)

    If ModHandle = 0 Then
        Call Error_("Can't Load Music")
    Else
        lstMusic.AddItem GetFileName(DLG.filename)
        lstMusic.ItemData(lstMusic.ListCount - 1) = ModHandle
    End If
End Sub

' Free the selected mod resource
' Remove the selected list
Private Sub cmdMusicRemove_Click()
    If (lstMusic.ListIndex >= 0) Then
        Call BASS_MusicFree(lstMusic.ItemData(lstMusic.ListIndex))
        lstMusic.RemoveItem lstMusic.ListIndex
    End If
End Sub

' Play the music (continue from current position)
Private Sub cmdMusicPlay_Click()
    If (lstMusic.ListIndex >= 0) Then _
            If (BASS_ChannelPlay(lstMusic.ItemData(lstMusic.ListIndex), BASSFALSE) = 0) Then _
            Call Error_("Can't play music")
End Sub

' Stop the music
Private Sub cmdMusicStop_Click()
    If (lstMusic.ListIndex >= 0) Then _
            Call BASS_ChannelStop(lstMusic.ItemData(lstMusic.ListIndex))
End Sub

' Play the music from the start
Private Sub cmdMusicRestart_Click()
    If (lstMusic.ListIndex >= 0) Then _
            Call BASS_ChannelPlay(lstMusic.ItemData(lstMusic.ListIndex), BASSTRUE)
End Sub

Private Sub cmdSampleAdd_Click()
    On Local Error Resume Next

    DLG.Filter = "Sample files (wav/aif)|*.wav;*.aif|All Files (*.*)|*.*|"
    DLG.ShowOpen

    ' if cancel was pressed, exit the procedure
    If Err.Number = 32755 Then Exit Sub

    Dim SampleHandle As Long

    ' Load a sample from "DLG.FileName" and give it a max of 3 simultaneous
    ' playings using playback position as override decider
    SampleHandle = BASS_SampleLoad(BASSFALSE, StrPtr(DLG.filename), 0, 0, 3, BASS_SAMPLE_OVER_POS)

    If SampleHandle = 0 Then
        Call Error_("Can't Load Sample")
    Else
        lstSamples.AddItem GetFileName(DLG.filename)
        lstSamples.ItemData(lstSamples.ListCount - 1) = SampleHandle
    End If
End Sub

' Play the sample at default rate, volume=50%, random pan position
Private Sub cmdSamplePlay_Click()
    If (lstSamples.ListIndex >= 0) Then
        Dim ch As Long
        ch = BASS_SampleGetChannel(lstSamples.ItemData(lstSamples.ListIndex), BASSFALSE)
        Call BASS_ChannelSetAttribute(ch, BASS_ATTRIB_VOL, 0.5)
        Call BASS_ChannelSetAttribute(ch, BASS_ATTRIB_PAN, ((201 * Rnd) - 100) / 100)
        If (BASS_ChannelPlay(ch, BASSFALSE) = 0) Then Error_ ("Can't play sample")
    End If
End Sub

' Free the selected sample resource
' Remove the selected list item
Private Sub cmdSampleRemove_Click()
    If (lstSamples.ListIndex >= 0) Then
        Call BASS_SampleFree(lstSamples.ItemData(lstSamples.ListIndex))
        lstSamples.RemoveItem lstSamples.ListIndex
    End If
End Sub

Private Sub sldVol_Scroll()
    Call BASS_SetVolume(sldVol.value / 100)
End Sub

Private Sub sldVolGlMus_Scroll()
    Call BASS_SetConfig(BASS_CONFIG_GVOL_MUSIC, sldVolGlMus.value)
End Sub

Private Sub sldVolglSam_Scroll()
    Call BASS_SetConfig(BASS_CONFIG_GVOL_SAMPLE, sldVolglSam.value)
End Sub

Private Sub sldVolGlStr_Scroll()
    Call BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, sldVolGlStr.value)
End Sub

' Main timer, to update all info needed.
Private Sub tmrBass_Timer()
    ' update the CPU usage % display
    lblCPU.Caption = Format(BASS_GetCPU, "0.00")
End Sub

'--------------------
' useful function :)
'--------------------

'get file name from file path
Public Function GetFileName(ByVal fp As String) As String
    GetFileName = Mid(fp, InStrRev(fp, "\") + 1)
End Function
