VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Begin VB.Form frmPlugins 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "BASS plugin test"
   ClientHeight    =   3105
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   4560
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3105
   ScaleWidth      =   4560
   StartUpPosition =   2  'CenterScreen
   Begin VB.Timer tmrPlugins 
      Interval        =   500
      Left            =   0
      Top             =   2400
   End
   Begin MSComctlLib.Slider sldPosition 
      Height          =   435
      Left            =   360
      TabIndex        =   1
      Top             =   2640
      Width           =   3855
      _ExtentX        =   6800
      _ExtentY        =   767
      _Version        =   393216
      TickStyle       =   3
      TickFrequency   =   0
   End
   Begin VB.Frame framePlugins 
      Caption         =   " Loaded plugins "
      Height          =   1455
      Left            =   120
      TabIndex        =   2
      Top             =   120
      Width           =   4335
      Begin VB.ListBox lstPlugins 
         Height          =   1035
         Left            =   120
         TabIndex        =   3
         TabStop         =   0   'False
         Top             =   240
         Width           =   4095
      End
   End
   Begin VB.CommandButton btnOpen 
      Caption         =   "click here to open a file..."
      Height          =   375
      Left            =   120
      TabIndex        =   0
      Top             =   1680
      Width           =   4335
   End
   Begin MSComDlg.CommonDialog cmdOpenFile 
      Left            =   4080
      Top             =   2400
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.Label lblInfo 
      Alignment       =   2  'Center
      Height          =   435
      Left            =   120
      TabIndex        =   4
      Top             =   2160
      Width           =   4320
   End
End
Attribute VB_Name = "frmPlugins"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'////////////////////////////////////////////////////////////////////////////////
' frmPlugins.frm - Copyright (c) 2006-2007 (: JOBnik! :) [Arthur Aminov, ISRAEL]
'                                                        [http://www.jobnik.org]
'                                                        [  jobnik@jobnik.org  ]
'
' BASS plugin test
' Originally translated from - plugins.c - Example of Ian Luck
'////////////////////////////////////////////////////////////////////////////////

Option Explicit

Dim chan As Long    ' the channel
Dim filter As String

' display error messages
Sub Error_(ByVal mes As String)
    Call MsgBox(mes & vbCrLf & vbCrLf & "(error code: " & BASS_ErrorGetCode & ")", vbExclamation, "Error!")
End Sub

' translate a CTYPE value to text
Public Function GetCTypeString(ByVal ctype As Long, ByVal plugin As Long) As String
    If (plugin) Then ' using a plugin
        Dim pinfo As BASS_PLUGININFO, a As Long

        pinfo = BASS_PluginGetInfo(plugin)  ' get plugin info

        For a = 0 To pinfo.formatc - 1
            If (BASS_PluginGetInfoFormat(plugin, a).ctype = ctype) Then   ' found a "ctype" match...
                GetCTypeString = VBStrFromAnsiPtr(BASS_PluginGetInfoFormat(plugin, a).name)  ' return it's name
                Exit Function
            End If
        Next a
    End If

    ' check built-in stream formats...
    Select Case (ctype)
        Case (BASS_CTYPE_STREAM_OGG):   GetCTypeString = "Ogg Vorbis"
        Case (BASS_CTYPE_STREAM_MP1): GetCTypeString = "MPEG layer 1"
        Case (BASS_CTYPE_STREAM_MP2): GetCTypeString = "MPEG layer 2"
        Case (BASS_CTYPE_STREAM_MP3): GetCTypeString = "MPEG layer 3"
        Case (BASS_CTYPE_STREAM_AIFF): GetCTypeString = "Audio IFF"
        Case (BASS_CTYPE_STREAM_WAV_PCM): GetCTypeString = "PCM WAVE"
        Case (BASS_CTYPE_STREAM_WAV_FLOAT): GetCTypeString = "Floating-point WAVE"
        Case Else: GetCTypeString = "?"
    End Select

    ' other WAVE codec, could use acmFormatTagDetails to get its name, but...
    If (ctype And BASS_CTYPE_STREAM_WAV) Then GetCTypeString = "WAVE"
End Function

Private Sub Form_Load()
    ' change and set the current path, to prevent from VB not finding BASS.DLL
    ChDrive App.Path
    ChDir App.Path

    ' check the correct BASS was loaded
    If (HiWord(BASS_GetVersion) <> BASSVERSION) Then
        Call MsgBox("An incorrect version of BASS.DLL was loaded", vbCritical)
        End
    End If

    ' initialize default output device
    If (BASS_Init(-1, 44100, 0, Me.hWnd, 0) = 0) Then
        Call Error_("Can't initialize device")
        End
    End If

    ' initialize file selector
    cmdOpenFile.CancelError = True
    cmdOpenFile.flags = cdlOFNExplorer Or cdlOFNFileMustExist Or cdlOFNHideReadOnly
    cmdOpenFile.DialogTitle = "Open"
    cmdOpenFile.filter = filter
    filter = "BASS built-in (*.mp3;*.mp2;*.mp1;*.ogg;*.wav;*.aif)|*.mp3;*.mp2;*.mp1;*.ogg;*.wav;*.aif"

    ' look for plugins (in the executable's directory)
    Dim fh As String
    fh = Dir("bass*.dll")   ' find 1st file

    Do While (fh <> "")
        Dim plug As Long
        plug = BASS_PluginLoad(fh, 0)   ' plugin loaded...
        If (plug) Then
            Dim pinfo As BASS_PLUGININFO
            pinfo = BASS_PluginGetInfo(plug) ' get plugin info to add to the file selector filter...
            Dim a As Long
            For a = 0 To pinfo.formatc - 1
                filter = filter & "|" & VBStrFromAnsiPtr(BASS_PluginGetInfoFormat(plug, a).name) & " (" & VBStrFromAnsiPtr(BASS_PluginGetInfoFormat(plug, a).exts) & ")" & " - " & fh ' format description
                filter = filter & "|" & VBStrFromAnsiPtr(BASS_PluginGetInfoFormat(plug, a).exts)  ' extension filter
            Next a
            
            ' add plugin to the list
            lstPlugins.AddItem fh
        End If
        fh = Dir()  ' get next file
    Loop

    ' no plugins...
    If (lstPlugins.ListCount = 0) Then _
        lstPlugins.AddItem "no plugins - visit the BASS webpage to get some"

    filter = filter & "|" & "All files|*.*"
    cmdOpenFile.filter = filter
End Sub

Private Sub Form_Unload(Cancel As Integer)
    ' "free" the output device and all plugins
    Call BASS_Free
    Call BASS_PluginFree(0)
    End
End Sub

Private Sub btnOpen_Click()
    On Local Error Resume Next    ' if Cancel pressed...

    cmdOpenFile.ShowOpen

    ' if cancel was pressed, exit the procedure
    If Err.Number = 32755 Then Exit Sub

    Call BASS_StreamFree(chan)  ' free the old stream

    chan = BASS_StreamCreateFile(BASSFALSE, StrPtr(cmdOpenFile.filename), 0, 0, BASS_SAMPLE_LOOP)

    ' it ain't playable
    If (chan = 0) Then
        btnOpen.Caption = "click here to open a file..."
        lblInfo.Caption = ""
        Call Error_("Can't play the file")
        Exit Sub
    End If
    
    btnOpen.Caption = cmdOpenFile.filename

    ' display the file type and length
    Dim bytes As Long
    bytes = BASS_ChannelGetLength(chan, BASS_POS_BYTE)

    Dim time As Long
    time = BASS_ChannelBytes2Seconds(chan, bytes)
    
    Dim info As BASS_CHANNELINFO
    Call BASS_ChannelGetInfo(chan, info)
    
    lblInfo.Caption = "channel type = " & Hex(info.ctype) & " (" & GetCTypeString(info.ctype, info.plugin) _
                    & ")" & vbCrLf & "length = " & bytes & " (" & time \ 60 & ":" & Format(time Mod 60, "00") & ")"

    sldPosition.max = time ' update scroller range

    Call BASS_ChannelPlay(chan, BASSFALSE)
End Sub

Private Sub sldPosition_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    tmrPlugins.Enabled = False
End Sub

Private Sub sldPosition_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Call BASS_ChannelSetPosition(chan, BASS_ChannelSeconds2Bytes(chan, sldPosition.value), BASS_POS_BYTE)  ' set the position
    tmrPlugins.Enabled = True
End Sub

Private Sub tmrPlugins_Timer()
    sldPosition.value = BASS_ChannelBytes2Seconds(chan, BASS_ChannelGetPosition(chan, BASS_POS_BYTE)) ' update position
End Sub
