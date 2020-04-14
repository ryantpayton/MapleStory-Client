VERSION 5.00
Begin VB.Form frmDevice 
   BorderStyle     =   1  'Fixed Single
   Caption         =   " "
   ClientHeight    =   1500
   ClientLeft      =   5160
   ClientTop       =   4530
   ClientWidth     =   3855
   ControlBox      =   0   'False
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1500
   ScaleWidth      =   3855
   Begin VB.CommandButton btnOK 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   300
      Left            =   2280
      TabIndex        =   1
      Top             =   1080
      Width           =   1335
   End
   Begin VB.ListBox lstDevices 
      Height          =   840
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   3615
   End
End
Attribute VB_Name = "frmDevice"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'///////////////////////////////////////////////////////////////////////////////
' frmDevice.frm - Copyright (c) 2003-2007 (: JOBnik! :) [Arthur Aminov, ISRAEL]
'                                                       [http://www.jobnik.org]
'                                                       [  jobnik@jobnik.org  ]
' Other sources: frmMulti.frm and modMulti.bas
'
' BASS output device selector
' Originally translated from - multi.c - example of Ian Luck
'///////////////////////////////////////////////////////////////////////////////

Option Explicit

Public device As Long       ' selected device

Public Sub SelectDevice(ByVal dev As Long)
    Dim c As Integer
    Dim i As BASS_DEVICEINFO

    c = 1      ' device 1 = 1st real device
    Me.Caption = "Select output device #" & dev
    While BASS_GetDeviceInfo(c, i)
        If (i.flags And BASS_DEVICE_ENABLED) Then  ' enabled, so add it...
            lstDevices.AddItem VBStrFromAnsiPtr(i.name)
            lstDevices.ItemData(lstDevices.NewIndex) = c    'store device #
        End If
        c = c + 1
    Wend
    
    If (lstDevices.ListCount) Then lstDevices.Selected(0) = True
End Sub

Private Sub btnOK_Click()
    Unload Me
End Sub

Private Sub lstDevices_DblClick()
    Call btnOK_Click
End Sub

Private Sub Form_Unload(Cancel As Integer)
    device = lstDevices.ItemData(lstDevices.ListIndex)  'get device #
End Sub
