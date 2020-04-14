VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsFileIo"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
' This ClassModule is written by Peter Hebels
Option Explicit

Private Const GENERIC_WRITE = &H40000000
Private Const FILE_ATTRIBUTE_NORMAL = &H80
Private Const CREATE_ALWAYS = 2
Private Const OPEN_ALWAYS = 4
Private Const INVALID_HANDLE_VALUE = -1

Private Declare Function MessageBox Lib "user32" Alias "MessageBoxA" (ByVal hwnd As Long, ByVal lpText As String, ByVal lpCaption As String, ByVal wType As Long) As Long
Private Declare Function ReadFile Lib "kernel32" (ByVal hFile As Long, lpBuffer As Any, ByVal nNumberOfBytesToRead As Long, lpNumberOfBytesRead As Long, ByVal lpOverlapped As Long) As Long
Private Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Long) As Long
Private Declare Function WriteFile Lib "kernel32" (ByVal hFile As Long, lpBuffer As Any, ByVal nNumberOfBytesToWrite As Long, lpNumberOfBytesWritten As Long, ByVal lpOverlapped As Long) As Long
Private Declare Function CreateFile Lib "kernel32" Alias "CreateFileA" (ByVal lpFileName As String, ByVal dwDesiredAccess As Long, ByVal dwShareMode As Long, ByVal lpSecurityAttributes As Long, ByVal dwCreationDisposition As Long, ByVal dwFlagsAndAttributes As Long, ByVal hTemplateFile As Long) As Long
Private Declare Function FlushFileBuffers Lib "kernel32" (ByVal hFile As Long) As Long
Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Destination As Any, Source As Any, ByVal length As Long)

Private fHandle As Long
Private fSuccess As Long
Private lFilePos As Long
Private File_Name As String

Public Function OpenFile(FileN As String) As Boolean
    fHandle = CreateFile(FileN, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0)
    If fHandle <> INVALID_HANDLE_VALUE Then
        File_Name = FileN
        OpenFile = True
    Else
        OpenFile = False
    End If
End Function

Public Function CloseFile() As Boolean
    If fHandle <> INVALID_HANDLE_VALUE And File_Name <> "" Then
        fSuccess = CloseHandle(fHandle)
        CloseFile = True
    Else
        CloseFile = False
    End If
End Function

Public Function WriteBytes(Pointer As Long, Size As Long) As Boolean
Dim hHdr As String
Dim NewHeader As String
Dim i As Long
  
Dim d() As Byte
ReDim d(Size) As Byte
  
If fHandle <> INVALID_HANDLE_VALUE And File_Name <> "" Then
    hHdr = Chr(255) & Chr(251)
    If GotHeader = False Then
        Call CopyMemory(d(0), ByVal Pointer, Size)
        For i = 1 To Size
            If d(i) = 255 Then
                If d(i + 1) = 251 Then
                    NewHeader = hHdr & Chr(d(i + 2))
                    fSuccess = WriteFile(fHandle, ByVal NewHeader, LenB(NewHeader), lFilePos, 0)
                    If fSuccess <> 0 Then
                        fSuccess = FlushFileBuffers(fHandle)
                        GotHeader = True
                        Exit For
                    End If
                End If
            End If
        Next i
    End If
    
    If GotHeader = True Then
        fSuccess = WriteFile(fHandle, ByVal Pointer, Size, lFilePos, 0)
        If fSuccess <> 0 Then
            fSuccess = FlushFileBuffers(fHandle)
            WriteBytes = True
        Else
            WriteBytes = False
            Exit Function
        End If
    End If
Else
    WriteBytes = False
    Exit Function
End If

End Function
