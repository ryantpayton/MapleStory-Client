Attribute VB_Name = "SYNCtest"
'//////////////////////////////////////////////////////////////////////////////
' SYNCtest.bas - Copyright (c) 2001-2007 (: JOBnik! :) [Arthur Aminov, ISRAEL]
'                                                      [http://www.jobnik.org]
'                                                      [  jobnik@jobnik.org  ]
'
' Other sources: frmMemory.frm & CBASS_TIME.cls
'
' * Updates:
'    . Now uses only VB functions without any Memory APIs
'    . Threading
'
' SYNC callback example...
'//////////////////////////////////////////////////////////////////////////////

Option Explicit

Public Declare Function MessageBox Lib "user32" Alias "MessageBoxA" (ByVal hwnd As Long, ByVal lpText As String, ByVal lpCaption As String, ByVal wType As Long) As Long

' THREADING
Public cthread As Long
Public Declare Function CreateThread Lib "kernel32" (lpThreadAttributes As Any, ByVal dwStackSize As Long, ByVal lpStartAddress As Long, ByVal lpParameter As Long, ByVal dwCreationFlags As Long, lpThreadID As Long) As Long
Public Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Long) As Long

Public chan As Long             ' stream or music handle
Public SyncEnd As Long          ' sync at end handle

Public DataStore() As Byte      ' data array
Public bassTime As cbass_time   ' Class module Handle

' display error messages
Public Sub Error_(ByVal es As String)
    Call MessageBox(frmMemory.hwnd, es & vbCrLf & "(error code: " & BASS_ErrorGetCode() & ")", "Error", vbExclamation)
End Sub

Public Sub MemoryFileThread(ByVal DataLength As Long)
    If (DataLength) Then
        ' free old stream (if any) and create new one
        Call BASS_StreamFree(chan)
        Call BASS_MusicFree(chan)

        ' reallocate data array
        ReDim DataStore(DataLength) As Byte

        ' insert all the file data into a byte array
        Get 100, 1, DataStore

        ' close file handle
        Close #100

        ' read data from memory location (our data array)
        chan = BASS_StreamCreateFile(BASSTRUE, VarPtr(DataStore(0)), 0, DataLength, BASS_SAMPLE_LOOP)
        If (chan = 0) Then chan = BASS_MusicLoad(BASSTRUE, VarPtr(DataStore(0)), 0, DataLength, BASS_MUSIC_LOOP Or BASS_MUSIC_RAMP Or BASS_MUSIC_PRESCAN, 1)

        If (chan = 0) Then
            ' free stream and music (if any)
            Call BASS_StreamFree(chan)
            Call BASS_MusicFree(chan)

            ' free memory
            Erase DataStore()

            Call Error_("Couldn't Get Data from Memory")
            frmMemory.cmdOpenPlay.Caption = "Click here to open a file && play it"
        Else
            Call frmMemory.chkSYNC_Click

            frmMemory.cmdOpenPlay.Caption = "Playing... click to choose another file"

            Call BASS_ChannelPlay(chan, BASSFALSE)
            frmMemory.tmrBASS.Enabled = True

            With bassTime
                frmMemory.lblDur.Caption = "Total duration: " & Format(.GetDuration(chan), "0.0") & " seconds / " & .GetTime(.GetDuration(chan))
                frmMemory.lblFreq.Caption = "Frequency: " & .GetFrequency(chan) & " Hz, " & .GetBits(chan) & " bits, " & .GetMode(chan)
                frmMemory.lblBPS.Caption = "Bytes/s: " & .GetBytesPerSecond(chan)
                frmMemory.lblBitsPS.Caption = "Kbp/s: " & .GetBitsPerSecond(chan, DataLength) & " [average kbp/s for vbr mp3s]"
            End With
        End If
    End If

    ' close thread
    Call CloseHandle(cthread)
    cthread = 0
End Sub

Public Sub SyncEndTest(ByVal handle As Long, ByVal channel As Long, ByVal data As Long, ByVal user As Long)
    Call MessageBox(frmMemory.hwnd, "End...", "SYNCtest", vbInformation)
End Sub
