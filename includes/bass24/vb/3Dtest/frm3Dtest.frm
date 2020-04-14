VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form frm3Dtest 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "BASS - 3D Test"
   ClientHeight    =   4005
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   5415
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   4005
   ScaleWidth      =   5415
   StartUpPosition =   2  'CenterScreen
   Begin VB.Frame Frame6 
      Caption         =   "Doppler factor"
      Height          =   495
      Left            =   2640
      TabIndex        =   12
      Top             =   3360
      Width           =   2655
      Begin VB.HScrollBar ID_Doppler 
         Height          =   135
         Left            =   120
         Max             =   20
         TabIndex        =   14
         Top             =   240
         Value           =   10
         Width           =   2415
      End
   End
   Begin VB.Frame Frame5 
      Caption         =   "Rolloff factor"
      Height          =   495
      Left            =   2640
      TabIndex        =   11
      Top             =   2760
      Width           =   2655
      Begin VB.HScrollBar ID_Rolloff 
         Height          =   135
         Left            =   120
         Max             =   20
         TabIndex        =   13
         Top             =   240
         Value           =   10
         Width           =   2415
      End
   End
   Begin VB.Frame Frame4 
      Height          =   2655
      Left            =   2640
      TabIndex        =   3
      Top             =   0
      Width           =   2655
      Begin VB.PictureBox picDisplay 
         FillStyle       =   0  'Solid
         Height          =   2295
         Left            =   120
         ScaleHeight     =   149
         ScaleMode       =   3  'Pixel
         ScaleWidth      =   157
         TabIndex        =   4
         Top             =   240
         Width           =   2415
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "Channels (sample/music)"
      Height          =   2295
      Left            =   120
      TabIndex        =   2
      Top             =   0
      Width           =   2415
      Begin VB.Timer tmr3D 
         Enabled         =   0   'False
         Interval        =   50
         Left            =   1800
         Top             =   840
      End
      Begin MSComDlg.CommonDialog DLG 
         Left            =   1800
         Top             =   360
         _ExtentX        =   847
         _ExtentY        =   847
         _Version        =   393216
      End
      Begin VB.CommandButton cmdStop 
         Caption         =   "Stop"
         Enabled         =   0   'False
         Height          =   300
         Left            =   1320
         TabIndex        =   9
         Top             =   1920
         Width           =   975
      End
      Begin VB.CommandButton cmdPlay 
         Caption         =   "Play"
         Enabled         =   0   'False
         Height          =   300
         Left            =   120
         TabIndex        =   8
         Top             =   1920
         Width           =   975
      End
      Begin VB.CommandButton cmdRemove 
         Caption         =   "Remove"
         Enabled         =   0   'False
         Height          =   300
         Left            =   1320
         TabIndex        =   7
         Top             =   1560
         Width           =   975
      End
      Begin VB.CommandButton cmdAdd 
         Caption         =   "Add ..."
         Height          =   300
         Left            =   120
         TabIndex        =   6
         Top             =   1560
         Width           =   975
      End
      Begin VB.ListBox lstChannels 
         Height          =   1230
         Left            =   120
         TabIndex        =   5
         Top             =   240
         Width           =   2175
      End
   End
   Begin VB.Frame Frame2 
      Caption         =   "Movement"
      ClipControls    =   0   'False
      Height          =   765
      Left            =   120
      TabIndex        =   1
      Top             =   2280
      Width           =   2415
      Begin VB.CommandButton btnReset 
         Caption         =   "reset"
         Enabled         =   0   'False
         Height          =   255
         Left            =   1680
         TabIndex        =   19
         Top             =   310
         Width           =   615
      End
      Begin VB.TextBox txtX 
         Enabled         =   0   'False
         Height          =   285
         Left            =   360
         MaxLength       =   2
         TabIndex        =   16
         Top             =   300
         Width           =   375
      End
      Begin VB.TextBox txtZ 
         Enabled         =   0   'False
         Height          =   285
         Left            =   1080
         MaxLength       =   2
         TabIndex        =   15
         Top             =   300
         Width           =   375
      End
      Begin VB.Label lblZ 
         AutoSize        =   -1  'True
         Caption         =   "z:"
         Height          =   195
         Left            =   840
         TabIndex        =   18
         Top             =   310
         Width           =   120
      End
      Begin VB.Label lblX 
         AutoSize        =   -1  'True
         Caption         =   "x:"
         Height          =   195
         Left            =   120
         TabIndex        =   17
         Top             =   310
         Width           =   120
      End
   End
   Begin VB.Frame Frame3 
      Caption         =   "EAX Environment"
      ClipControls    =   0   'False
      Height          =   735
      Left            =   120
      TabIndex        =   0
      Top             =   3120
      Width           =   2415
      Begin VB.ComboBox cmbEAX 
         BackColor       =   &H00FFFFFF&
         Enabled         =   0   'False
         Height          =   315
         Left            =   120
         Style           =   2  'Dropdown List
         TabIndex        =   10
         Top             =   240
         Width           =   2175
      End
   End
End
Attribute VB_Name = "frm3Dtest"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'/////////////////////////////////////////////////////////////
' BASS 3D test,  copyright (c) 1999 Adam Hoult.
'
' Updated: 2003-2007 by (: JOBnik! :) [Arthur Aminov, ISRAEL]
'                                     [http://www.jobnik.org]
'                                     [  jobnik@jobnik.org  ]
'
' Other source: frmDevice.frm
'
' Originally translated from - 3dtest.c - example of Ian Luck
'/////////////////////////////////////////////////////////////

Option Explicit

' channel (sample/music) info structure
Private Type channel
    channel As Long         ' the channel
    pos As BASS_3DVECTOR    ' position
    vel As BASS_3DVECTOR    ' velocity
End Type

Dim chans() As channel      ' array of channels
Dim chanc As Long           ' number of Channels
Dim chan As Long            ' current Channel

Const TIMERPERIOD = 50      ' timer period (ms)
Const MAXDIST = 50          ' maximum distance of the channels (m)
Const SPEED = 12            ' speed of the channels' movement (m/s)

' display error messages
Sub Error_(ByVal es As String)
    Call MsgBox(es & vbCrLf & vbCrLf & "error code: " & BASS_ErrorGetCode, vbExclamation, "Error")
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
    
    With cmbEAX
        .AddItem "Off"
        .AddItem "Generic"
        .AddItem "Padded Cell"
        .AddItem "Room"
        .AddItem "Bathroom"
        .AddItem "Living Room"
        .AddItem "Stone Room"
        .AddItem "Auditorium"
        .AddItem "Concert Hall"
        .AddItem "Cave"
        .AddItem "Arena"
        .AddItem "Hangar"
        .AddItem "Carpeted Hallway"
        .AddItem "Hallway"
        .AddItem "Stone Corridor"
        .AddItem "Alley"
        .AddItem "Forest"
        .AddItem "City"
        .AddItem "Mountains"
        .AddItem "Quarry"
        .AddItem "Plain"
        .AddItem "Parking Lot"
        .AddItem "Sewer Pipe"
        .AddItem "Under Water"
        .AddItem "Drugged"
        .AddItem "Dizzy"
        .AddItem "Psychotic"
        .ListIndex = 0
    End With
    
    chanc = 0
    chan = -1

    ' Show the main window
    Me.Show

    ' Initialize the default output device with 3D support
    If (BASS_Init(-1, 44100, BASS_DEVICE_3D, Me.hWnd, 0) = 0) Then
        Call Error_("Can't initialize output device")
        End
    End If

    ' Use meters as distance unit, real world rolloff, real doppler effect
    Call BASS_Set3DFactors(1, 1, 1)

    ' Turn EAX off (volume=0), if error then EAX is not supported
    If BASS_SetEAXParameters(-1, 0, -1, -1) Then cmbEAX.Enabled = True

    Call UpdateButtons
    tmr3D.Enabled = True
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    Call BASS_Free
    Erase chans
End Sub

Sub Update()
    Dim c As Integer, X As Integer, Y As Integer, cx As Integer, cy As Integer

    cx = picDisplay.ScaleWidth / 2
    cy = picDisplay.ScaleHeight / 2

    ' Clear the display
    picDisplay.Cls

    ' Draw Center Circle
    picDisplay.FillColor = RGB(100, 100, 100)
    picDisplay.Circle (cx - 4, cy - 4), 4, RGB(0, 0, 0)

    For c = 0 To chanc - 1
        ' If the channel is playing, then update it's position
        If BASS_ChannelIsActive(chans(c).channel) = BASS_ACTIVE_PLAYING Then
            ' Check if channel has reached the max distance
            If chans(c).pos.z >= MAXDIST Or chans(c).pos.z <= -MAXDIST Then chans(c).vel.z = -chans(c).vel.z
            If chans(c).pos.X >= MAXDIST Or chans(c).pos.X <= -MAXDIST Then chans(c).vel.X = -chans(c).vel.X
            
            ' Update channel position
            chans(c).pos.z = chans(c).pos.z + chans(c).vel.z * TIMERPERIOD / 1000
            chans(c).pos.X = chans(c).pos.X + chans(c).vel.X * TIMERPERIOD / 1000
            Call BASS_ChannelSet3DPosition(chans(c).channel, chans(c).pos, 0, chans(c).vel)
        End If
        ' Draw the channel position indicator
        X = cx + Int((cx - 7) * chans(c).pos.X / MAXDIST)
        Y = cy - Int((cy - 7) * chans(c).pos.z / MAXDIST)

        If chan = c Then
            picDisplay.FillColor = RGB(255, 0, 0)
        Else
            picDisplay.FillColor = RGB(150, 0, 0)
        End If
        picDisplay.Circle (X - 4, Y - 4), 4, RGB(0, 0, 0)
    Next c

    ' Apply 3d changes
    Call BASS_Apply3D
End Sub

' Update the button states
Sub UpdateButtons()
    ' Disable/enable controls depending on chanc
    cmdRemove.Enabled = IIf(chan = -1, False, True)
    cmdPlay.Enabled = IIf(chan = -1, False, True)
    cmdStop.Enabled = IIf(chan = -1, False, True)
    txtX.Enabled = IIf(chan = -1, False, True)
    txtZ.Enabled = IIf(chan = -1, False, True)
    btnReset.Enabled = IIf(chan = -1, False, True)

    If (chan <> -1) Then
        txtX.Text = Abs(Int(chans(chan).vel.X))
        txtZ.Text = Abs(Int(chans(chan).vel.z))
    End If
End Sub

Private Sub cmbEAX_Click()
    ' Change the EAX Environment depending on which is selected
    Select Case cmbEAX.ListIndex
        Case 0: BASS_SetEAXParameters -1, 0, -1, -1
        Case 1: BASS_SetEAXPreset EAX_ENVIRONMENT_GENERIC
        Case 2: BASS_SetEAXPreset EAX_ENVIRONMENT_PADDEDCELL
        Case 3: BASS_SetEAXPreset EAX_ENVIRONMENT_ROOM
        Case 4: BASS_SetEAXPreset EAX_ENVIRONMENT_BATHROOM
        Case 5: BASS_SetEAXPreset EAX_ENVIRONMENT_LIVINGROOM
        Case 6: BASS_SetEAXPreset EAX_ENVIRONMENT_STONEROOM
        Case 7: BASS_SetEAXPreset EAX_ENVIRONMENT_AUDITORIUM
        Case 8: BASS_SetEAXPreset EAX_ENVIRONMENT_CONCERTHALL
        Case 9: BASS_SetEAXPreset EAX_ENVIRONMENT_CAVE
        Case 10: BASS_SetEAXPreset EAX_ENVIRONMENT_ARENA
        Case 11: BASS_SetEAXPreset EAX_ENVIRONMENT_HANGAR
        Case 12: BASS_SetEAXPreset EAX_ENVIRONMENT_CARPETEDHALLWAY
        Case 13: BASS_SetEAXPreset EAX_ENVIRONMENT_HALLWAY
        Case 14: BASS_SetEAXPreset EAX_ENVIRONMENT_STONECORRIDOR
        Case 15: BASS_SetEAXPreset EAX_ENVIRONMENT_ALLEY
        Case 16: BASS_SetEAXPreset EAX_ENVIRONMENT_FOREST
        Case 17: BASS_SetEAXPreset EAX_ENVIRONMENT_CITY
        Case 18: BASS_SetEAXPreset EAX_ENVIRONMENT_MOUNTAINS
        Case 19: BASS_SetEAXPreset EAX_ENVIRONMENT_QUARRY
        Case 20: BASS_SetEAXPreset EAX_ENVIRONMENT_PLAIN
        Case 21: BASS_SetEAXPreset EAX_ENVIRONMENT_PARKINGLOT
        Case 22: BASS_SetEAXPreset EAX_ENVIRONMENT_SEWERPIPE
        Case 23: BASS_SetEAXPreset EAX_ENVIRONMENT_UNDERWATER
        Case 24: BASS_SetEAXPreset EAX_ENVIRONMENT_DRUGGED
        Case 25: BASS_SetEAXPreset EAX_ENVIRONMENT_DIZZY
        Case 26: BASS_SetEAXPreset EAX_ENVIRONMENT_PSYCHOTIC
    End Select
End Sub

Private Sub cmdAdd_Click()
    On Local Error Resume Next

    DLG.filename = ""
    DLG.CancelError = True
    DLG.flags = cdlOFNExplorer Or cdlOFNFileMustExist Or cdlOFNHideReadOnly
    DLG.Filter = "wav/aif/mo3/xm/mod/s3m/it/mtm/umx|*.wav;*.aif;*.mo3;*.xm;*.mod;*.s3m;*.it;*.mtm;*.umx|All Files (*.*)|*.*|"
    DLG.ShowOpen
    
    ' if cancel was pressed, exit the procedure
    If Err.Number = 32755 Then Exit Sub
    
    Dim newchan As Long

    ' Load a music or sample from "DLG.FileName"
    newchan = BASS_MusicLoad(BASSFALSE, StrPtr(DLG.filename), 0, 0, BASS_MUSIC_RAMP Or BASS_MUSIC_LOOP Or BASS_SAMPLE_3D, 1)
    If newchan = 0 Then newchan = BASS_SampleLoad(BASSFALSE, StrPtr(DLG.filename), 0, 0, 1, BASS_SAMPLE_LOOP Or BASS_SAMPLE_3D)

    If newchan Then
        ReDim Preserve chans(chanc) As channel
        chans(chanc).channel = newchan
        lstChannels.AddItem GetFileName(DLG.filename)
        Call BASS_SampleGetChannel(newchan, BASSFALSE) ' initialize sample channel
        chanc = chanc + 1
    Else
        Call Error_("Can't load file (note samples must be mono)")
    End If
End Sub

' Play the select sample/music
Private Sub cmdPlay_Click()
    Call BASS_ChannelPlay(chans(chan).channel, BASSFALSE)
End Sub

Private Sub cmdRemove_Click()
    Call BASS_SampleFree(chans(chan).channel)
    Call BASS_MusicFree(chans(chan).channel)

    ' remove the item from the array
    Dim TempChans() As channel, Counter As Integer
    ReDim TempChans(chanc) As channel

    Counter = 0

    Dim i As Integer

    For i = 0 To chanc - 1
        If i <> chan Then
            TempChans(Counter) = chans(i)
            Counter = Counter + 1
        End If
    Next i

    chanc = chanc - 1

    ReDim chans(chanc) As channel

    For i = 0 To chanc - 1
        chans(i) = TempChans(i)
    Next i

    Erase TempChans

    lstChannels.RemoveItem lstChannels.ListIndex
    chan = -1
    Call UpdateButtons
End Sub

' stop playing music/sample
Private Sub cmdStop_Click()
    Call BASS_ChannelPause(chans(chan).channel)
End Sub

' Change the rolloff factor
Private Sub ID_Rolloff_Scroll()
    Call BASS_Set3DFactors(-1#, 2# ^ ((ID_Rolloff.value - 10) / 5#), -1#)
End Sub

' Change the doppler factor
Private Sub ID_Doppler_Scroll()
    Call BASS_Set3DFactors(-1#, -1#, 2# ^ ((ID_Doppler.value - 10) / 5#))
End Sub

' Change the selected channel
Private Sub lstChannels_Click()
    chan = lstChannels.ListIndex
    Call UpdateButtons
End Sub

' X velocity
Private Sub txtX_Change()
    Dim v As Integer
    v = Val(txtX.Text)
    If (Abs(Int(chans(chan).vel.X)) <> v) Then chans(chan).vel.X = v
End Sub

Private Sub txtX_KeyPress(keyascii As Integer)
    keyascii = numbersOnly(keyascii)
End Sub

' Z velocity
Private Sub txtZ_Change()
    Dim v As Integer
    v = Val(txtZ.Text)
    If (Abs(Int(chans(chan).vel.z)) <> v) Then chans(chan).vel.z = v
End Sub

Private Sub txtZ_KeyPress(keyascii As Integer)
    keyascii = numbersOnly(keyascii)
End Sub

Private Sub tmr3D_Timer()
    Call Update
End Sub

' reset the position and velocity to 0
Private Sub btnReset_Click()
    Dim tmp As BASS_3DVECTOR    ' VB's default value is 0 ;)
    chans(chan).pos = tmp
    chans(chan).vel = tmp
    Call UpdateButtons
End Sub

'--------------------
' useful function :)
'--------------------

' get file name from file path
Public Function GetFileName(ByVal fp As String) As String
    GetFileName = Mid(fp, InStrRev(fp, "\") + 1)
End Function

' checks if keyascii is a number or a backspace
Public Function numbersOnly(ByVal keyascii As Integer) As Integer
    If (keyascii < vbKey0 Or keyascii > vbKey9) Then keyascii = IIf(keyascii = vbKeyBack, keyascii, 0)
    numbersOnly = keyascii
End Function
