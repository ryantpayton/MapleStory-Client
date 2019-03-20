Attribute VB_Name = "modRecTest"
'////////////////////////////////////////////////////////////////////////////////
' modRecTest.bas - Copyright (c) 2002-2007 (: JOBnik! :) [Arthur Aminov, ISRAEL]
'                                                        [http://www.jobnik.org]
'                                                        [  jobnik@jobnik.org  ]
'
' Other source: frmRecTest.frm
'
' BASS Recording example
' Originally translated from - rectest.c - Example of Ian Luck
'////////////////////////////////////////////////////////////////////////////////

Option Explicit

' MEMORY
Public Const GMEM_FIXED = &H0
Public Const GMEM_MOVEABLE = &H2
Public Declare Function GlobalAlloc Lib "kernel32" (ByVal wFlags As Long, ByVal dwBytes As Long) As Long
Public Declare Function GlobalReAlloc Lib "kernel32" (ByVal hMem As Long, ByVal dwBytes As Long, ByVal wFlags As Long) As Long
Public Declare Function GlobalFree Lib "kernel32" (ByVal hMem As Long) As Long
Public Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (lpvDest As Any, lpvSource As Any, ByVal cbCopy As Long)

' FILE
Const OFS_MAXPATHNAME = 128
Const OF_CREATE = &H1000
Const OF_READ = &H0
Const OF_WRITE = &H1

Private Type OFSTRUCT
        cBytes As Byte
        fFixedDisk As Byte
        nErrCode As Integer
        Reserved1 As Integer
        Reserved2 As Integer
        szPathName(OFS_MAXPATHNAME) As Byte
End Type

Private Declare Function OpenFile Lib "kernel32" (ByVal lpFileName As String, lpReOpenBuff As OFSTRUCT, ByVal wStyle As Long) As Long
Private Declare Function WriteFile Lib "kernel32" (ByVal hFile As Long, lpBuffer As Any, ByVal nNumberOfBytesToWrite As Long, lpNumberOfBytesWritten As Long, ByVal lpOverlapped As Any) As Long
Private Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Long) As Long

' WAV Header
Private Type WAVEHEADER_RIFF    ' == 12 bytes ==
    RIFF As Long                ' "RIFF" = &H46464952
    riffBlockSize As Long       ' reclen - 8
    riffBlockType As Long       ' "WAVE" = &H45564157
End Type

Private Type WAVEFORMAT         ' == 24 bytes ==
    wfBlockType As Long         ' "fmt " = &H20746D66
    wfBlockSize As Long
    ' == block size begins from here = 16 bytes
    wFormatTag As Integer
    nChannels As Integer
    nSamplesPerSec As Long
    nAvgBytesPerSec As Long
    nBlockAlign As Integer
    wBitsPerSample As Integer
End Type

Private Type WAVEHEADER_data    ' == 8 bytes ==
   dataBlockType As Long        ' "data" = &H61746164
   dataBlockSize As Long        ' reclen - 44
End Type

Dim wr As WAVEHEADER_RIFF
Dim wf As WAVEFORMAT
Dim wd As WAVEHEADER_data

Public Declare Function MessageBox Lib "user32" Alias "MessageBoxA" (ByVal hwnd As Long, ByVal lpText As String, ByVal lpCaption As String, ByVal wType As Long) As Long

Public BUFSTEP As Long        ' memory allocation unit
Public input_ As Long         ' current input source
Public recPtr As Long         ' a recording pointer to a memory location
Public reclen As Long         ' buffer length

Public rchan As Long          ' recording channel
Public chan As Long           ' playback channel

' display error messages
Public Sub Error_(ByVal es As String)
    Call MessageBox(frmRecTest.hwnd, es & vbCrLf & vbCrLf & "error code: " & BASS_ErrorGetCode, "Error", vbExclamation)
End Sub

' buffer the recorded data
Public Function RecordingCallback(ByVal handle As Long, ByVal buffer As Long, ByVal length As Long, ByVal user As Long) As Long
    ' increase buffer size if needed
    If ((reclen Mod BUFSTEP) + length >= BUFSTEP) Then
        recPtr = GlobalReAlloc(ByVal recPtr, ((reclen + length) / BUFSTEP + 1) * BUFSTEP, GMEM_MOVEABLE)
        If recPtr = 0 Then
            rchan = 0
            Call Error_("Out of memory!")
            frmRecTest.btnRecord.Caption = "Record"
            RecordingCallback = BASSFALSE ' stop recording
            Exit Function
        End If
    End If
    ' buffer the data
    Call CopyMemory(ByVal recPtr + reclen, ByVal buffer, length)
    reclen = reclen + length
    RecordingCallback = BASSTRUE ' continue recording
End Function

Public Sub StartRecording()
    ' free old recording
    If (recPtr) Then
        Call BASS_StreamFree(chan)
        Call GlobalFree(ByVal recPtr)
        recPtr = 0
        chan = 0
        frmRecTest.btnPlay.Enabled = False
        frmRecTest.btnSave.Enabled = False
    End If

    ' allocate initial buffer and make space for WAVE header
    recPtr = GlobalAlloc(GMEM_FIXED, BUFSTEP)
    reclen = 44

    ' fill the WAVE header
    wf.wFormatTag = 1
    wf.nChannels = 2
    wf.wBitsPerSample = 16
    wf.nSamplesPerSec = 44100
    wf.nBlockAlign = wf.nChannels * wf.wBitsPerSample / 8
    wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign

    ' Set WAV "fmt " header
    wf.wfBlockType = &H20746D66      ' "fmt "
    wf.wfBlockSize = 16

    ' Set WAV "RIFF" header
    wr.RIFF = &H46464952             ' "RIFF"
    wr.riffBlockSize = 0             ' after recording
    wr.riffBlockType = &H45564157    ' "WAVE"

    ' set WAV "data" header
    wd.dataBlockType = &H61746164    ' "data"
    wd.dataBlockSize = 0             ' after recording

    ' copy WAV Header to Memory
    Call CopyMemory(ByVal recPtr, wr, LenB(wr))        ' "RIFF"
    Call CopyMemory(ByVal recPtr + 12, wf, LenB(wf))   ' "fmt "
    Call CopyMemory(ByVal recPtr + 36, wd, LenB(wd))   ' "data"

    ' start recording @ 44100hz 16-bit stereo
    rchan = BASS_RecordStart(44100, 2, 0, AddressOf RecordingCallback, 0)

    If (rchan = 0) Then
        Call Error_("Couldn't start recording")
        Call GlobalFree(ByVal recPtr)
        recPtr = 0
        Exit Sub
    End If
    frmRecTest.btnRecord.Caption = "Stop"
End Sub

Public Sub StopRecording()
    Call BASS_ChannelStop(rchan)
    rchan = 0
    frmRecTest.btnRecord.Caption = "Record"

    ' complete the WAVE header
    wr.riffBlockSize = reclen - 8
    wd.dataBlockSize = reclen - 44

    Call CopyMemory(ByVal recPtr + 4, wr.riffBlockSize, LenB(wr.riffBlockSize))
    Call CopyMemory(ByVal recPtr + 40, wd.dataBlockSize, LenB(wd.dataBlockSize))

    ' create a stream from the recording
    chan = BASS_StreamCreateFile(BASSTRUE, recPtr, 0, reclen, 0)
    If (chan) Then
        ' enable "play" & "save" buttons
        frmRecTest.btnPlay.Enabled = True
        frmRecTest.btnSave.Enabled = True
    End If
End Sub

' write the recorded data to disk
Public Sub WriteToDisk()
    On Local Error Resume Next    ' if Cancel pressed...

    With frmRecTest.cmd
        .CancelError = True
        .flags = cdlOFNExplorer Or cdlOFNFileMustExist Or cdlOFNHideReadOnly
        .DialogTitle = "Save As..."
        .Filter = "WAV files|*.wav|All files|*.*"
        .DefaultExt = "wav"
        .ShowSave

        ' if cancel was pressed, exit the procedure
        If Err.Number = 32755 Then Exit Sub

        ' create a file .WAV, directly from Memory location
        Dim FileHandle As Long, ret As Long, OF As OFSTRUCT

        FileHandle = OpenFile(.filename, OF, OF_CREATE)

        If (FileHandle = 0) Then
            Call Error_("Can't create the file")
            Exit Sub
        End If

        Call WriteFile(FileHandle, ByVal recPtr, reclen, ret, ByVal 0&)
        Call CloseHandle(FileHandle)
    End With
End Sub

Public Sub UpdateInputInfo()
    Dim it As Long
    Dim level As Single
    
    it = BASS_RecordGetInput(input_, level) ' get info on the input
    If (it = -1 Or level < 0) Then ' failed
        Call BASS_RecordGetInput(-1, level) ' try master input instead
        If (level < 0) Then level = 1 ' that failed too, just display 100%
    End If
    frmRecTest.sldInputLevel.value = level * 100 ' set the level slider
    
    Dim type_ As String
    Select Case (it And BASS_INPUT_TYPE_MASK)
        Case BASS_INPUT_TYPE_DIGITAL:
            type_ = "digital"
        Case BASS_INPUT_TYPE_LINE:
            type_ = "line-in"
        Case BASS_INPUT_TYPE_MIC:
            type_ = "microphone"
        Case BASS_INPUT_TYPE_SYNTH:
            type_ = "midi synth"
        Case BASS_INPUT_TYPE_CD:
            type_ = "analog cd"
        Case BASS_INPUT_TYPE_PHONE:
            type_ = "telephone"
        Case BASS_INPUT_TYPE_SPEAKER:
            type_ = "pc speaker"
        Case BASS_INPUT_TYPE_WAVE:
            type_ = "wave/pcm"
        Case BASS_INPUT_TYPE_AUX:
            type_ = "aux"
        Case BASS_INPUT_TYPE_ANALOG:
            type_ = "analog"
        Case Else:
            type_ = "undefined"
    End Select
    frmRecTest.lblInputType.Caption = type_ ' display the type
End Sub
