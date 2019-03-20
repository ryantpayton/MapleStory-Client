{
	BASS multiple output example
	Copyright (c) 2001-2008 Un4seen Developments Ltd.

        C++ to Delphi with use API adapted by Evgeny Melnikov
        Required Delphi 7 or above
}

program Multi;

uses
  Windows, Messages, CommCtrl, CommDlg, Bass in '..\Bass.pas';

const
  szFiles	= 'streamable files'#0'*.mp3;*.mp2;*.mp1;*.ogg;*.wav;*.aif'#0 +
		  'All files'#0'*.*'#0#0;
  szOpen	= 'Select a file to play';

var
  win		: HWND;
  ofn		: OPENFILENAME;
  FileName	: array[0..MAX_PATH - 1] of Char;

  outdev	: array[0..1] of DWORD;				// output devices
  latency	: array[0..1] of DWORD;				// latencies
  chan		: array[0..1] of HSTREAM;			// the streams

{$R 'multi.res' 'multi.rc'}

//------------------ Auxiliary functions -------------------

function Format(const Format : String; const Args : array of const ) : String;
var
  I		: Integer;
  FormatBuffer	: array[0..High(Word)] of Char;
  Arr, Arr1	: PDWORD;
  PP		: PDWORD;
begin
  Arr := NIL;
  if High(Args) >= 0 then
    GetMem(Arr, (High(Args) + 1) * SizeOf(Pointer));
  Arr1 := Arr;
  for I := 0 to High(Args) do
  begin
    PP := @Args[I];
    PP := Pointer(PP^);
    Arr1^ := DWORD(PP);
    inc(Arr1);
  end;
  I := wvsprintf(@FormatBuffer[0], PChar(Format), PChar(Arr));
  SetLength(Result, I);
  Result := FormatBuffer;
  if Arr <> NIL then
    FreeMem(Arr);
end;

//---------------------------------------------------------

// display error messages
procedure Error(const es : String);
begin
  MessageBox(win, PChar(Format('%s' + #13#10 + '(error code: %d)', [es, BASS_ErrorGetCode])), 'Error', MB_OK or MB_ICONERROR);
end;

//---------------------------------------------------------

// Cloning DSP function
procedure CloneDSP(Handle : HDSP; channel : DWORD; buffer : Pointer; Length : DWORD; user : Pointer); stdcall;
begin
  BASS_StreamPutData(HSTREAM(user), buffer, Length);			// user = clone
end;

//---------------------------------------------------------

procedure InitApp(Wnd : HWND);
var
  info : BASS_INFO;
  di   : BASS_DEVICEINFO;
  St   : AnsiString;
begin
  win := Wnd;

  // get a playable file
  FillChar(ofn, SizeOf(OPENFILENAME), 0);
  FillChar(FileName, SizeOf(FileName), 0);

  ofn.lStructSize := SizeOf(OPENFILENAME);
  ofn.hwndOwner   := Wnd;
  ofn.hInstance   := hInstance;
  ofn.lpstrTitle  := szOpen;
  ofn.lpstrFile   := FileName;
  ofn.lpstrFilter := szFiles;
  ofn.nMaxFile    := MAX_PATH;
  ofn.Flags       := OFN_FILEMUSTEXIST or OFN_HIDEREADONLY or OFN_EXPLORER;
  
  // initialize output devices
  if not BASS_Init(outdev[0], 44100, BASS_DEVICE_LATENCY, Wnd, NIL) then
  begin
    Error('Can''t initialize device 1');
    DestroyWindow(Wnd);
  end;

  BASS_GetInfo(info);
  latency[0] := info.latency;

  if not BASS_Init(outdev[1], 44100, BASS_DEVICE_LATENCY, Wnd, NIL) then
  begin
    Error('Can''t initialize device 2');
    DestroyWindow(Wnd);
  end;
  
  BASS_GetInfo(info);
  latency[1] := info.latency;

  BASS_GetDeviceInfo(outdev[0], di);
  St := di.name;
  SendDlgItemMessage(Wnd, 20, WM_SETTEXT, 0, Integer(PChar(String(St))));
  BASS_GetDeviceInfo(outdev[1], di);
  St := di.name;
  SendDlgItemMessage(Wnd, 21, WM_SETTEXT, 0, Integer(PChar(String(St))));
end;

//---------------------------------------------------------

function DialogProc(hWndDlg : HWND; Msg, wParam, lParam : Longint) : Integer; stdcall;
type
  TBuf = array of Byte;
var
  info  : BASS_CHANNELINFO;
  devn,
  devnx : Integer;
  c, d  : DWORD;
  temp  : HSTREAM;
  St    : String;
  Buf   : TBuf;
  Buf1  : array[0..MAX_PATH - 1] of AnsiChar;
  Buf2  : array[0..MAX_PATH - 1] of AnsiChar;
begin
  Result := 0;
  case Msg of
    WM_INITDIALOG :
    begin
      InitApp(hWndDlg);
      Result := 1;
      Exit;
    end;

    WM_COMMAND :
    case LOWORD(wParam) of
      10, 11 :								// open a file to play on device #1 or device #2
      begin
        devn := LOWORD(wParam) - 10;
        if GetOpenFileName(ofn) then
        begin
          St := FileName;
          BASS_StreamFree(chan[devn]);					// free old stream
          BASS_SetDevice(outdev[devn]);					// set the device to create stream on
          chan[devn] := BASS_StreamCreateFile(False, PChar(St), 0, 0, BASS_SAMPLE_LOOP {$IFDEF UNICODE} or BASS_UNICODE {$ENDIF});
          if chan[devn] = 0 then
          begin
            SendDlgItemMessage(hWndDlg, 10 + devn, WM_SETTEXT, 0, Integer(PChar('click here to open a file...')));
            Error('Can''t play the file');
            Exit;
          end;
          BASS_ChannelPlay(chan[devn], False);				// play new stream
          SendDlgItemMessage(hWndDlg, 10 + devn, WM_SETTEXT, 0, Integer(PChar(St)));
        end;
      end;

      15, 16 :								// clone on device #1 or device #2
      begin
        devn := LOWORD(wParam) - 15;
        devnx := devn xor 1;
        if not BASS_ChannelGetInfo(chan[devnx], info) then
        begin
          Error('Nothing to clone');
          Exit;
        end;

        BASS_StreamFree(chan[devn]);					// free old stream
        BASS_SetDevice(outdev[devn]);					// set the device to create stream on

        chan[devn] := BASS_StreamCreate(info.freq, info.chans, info.flags, STREAMPROC_PUSH, NIL);
        if chan[devn] = 0 then
        begin								// create a "push" stream
          SendDlgItemMessage(hWndDlg, 10 + devn, WM_SETTEXT, 0, Integer(PChar('click here to open a file...')));
          Error('Can''t create clone');
          Exit;
        end;

        BASS_ChannelLock(chan[devnx], True);				// lock source stream to synchonise buffer contents
        BASS_ChannelSetDSP(chan[devnx], @CloneDSP, Pointer(chan[devn]), 0);	// set DSP to feed data to clone

        // copy buffered data to clone
        d := BASS_ChannelSeconds2Bytes(chan[devn], latency[devn] / 1000); // playback delay
        c := BASS_ChannelGetData(chan[devnx], NIL, BASS_DATA_AVAILABLE);
        SetLength(Buf, c);
        c := BASS_ChannelGetData(chan[devnx], buf, c);
        if c > d then
          BASS_StreamPutData(chan[devn], Pointer(DWORD(buf) + d), c - d);

        BASS_ChannelLock(chan[devnx], False);				// unlock source stream
        BASS_ChannelPlay(chan[devn], False);				// play clone
        SendDlgItemMessage(hWndDlg, 10 + devn, WM_SETTEXT, 0, Integer(PChar('clone')));
      end;

      30 :								// swap channel devices
      begin
        // swap handles
        temp    := chan[0];
        chan[0] := chan[1];
        chan[1] := temp;

        // swap text
        SendDlgItemMessage(hWndDlg, 10, WM_GETTEXT, SizeOf(Buf1), Integer(@Buf1[0]));
        SendDlgItemMessage(hWndDlg, 11, WM_GETTEXT, SizeOf(Buf2), Integer(@Buf2[0]));
        SendDlgItemMessage(hWndDlg, 10, WM_SETTEXT, 0, Integer(@Buf2[0]));
        SendDlgItemMessage(hWndDlg, 11, WM_SETTEXT, 0, Integer(@Buf1[0]));

        // update the channel devices
        BASS_ChannelSetDevice(chan[0], outdev[0]);
        BASS_ChannelSetDevice(chan[1], outdev[1]);
      end;

      IDCANCEL : DestroyWindow(hWndDlg);
    end;

    WM_CLOSE   : DestroyWindow(hWndDlg);

    WM_DESTROY :
    begin
      // release both devices
      BASS_SetDevice(outdev[0]);
      BASS_Free;
      BASS_SetDevice(outdev[1]);
      BASS_Free;
    end;
  end;
end;

//---------------------------------------------------------

// Simple device selector dialog stuff begins here
function DeviceDialogProc(hWndDlg : HWND; Msg, wParam, lParam : Longint) : Integer; stdcall;
var
  i, idx,
  Device : Integer;
  di     : BASS_DEVICEINFO;
  St     : String;
  St1    : AnsiString;
begin
  Result := 0;
  case Msg of
    WM_INITDIALOG :
    begin
      St := Format('Select output device #%d', [lParam]);
      SetWindowText(hWndDlg, PChar(St));
      // device 1 = 1st real device
      i := 1;
      while BASS_GetDeviceInfo(I, di) do
      begin
        if di.flags and BASS_DEVICE_ENABLED = BASS_DEVICE_ENABLED then		// enabled, so add it...
        begin
          St1 := di.name;
          idx := SendDlgItemMessage(hWndDlg, 10, LB_ADDSTRING, 0, Integer(PChar(String(St1))));
          SendDlgItemMessage(hWndDlg, 10, LB_SETITEMDATA, idx, i);		// store device #
        end;
        inc(i);
      end;
      SendDlgItemMessage(hWndDlg, 10, LB_SETCURSEL, 0, 0);
      Result := 1;
      Exit;
    end;

    WM_COMMAND :
    case LOWORD(wParam) of
      10 :
      if HIWORD(wParam) <> LBN_DBLCLK then
        Exit;
      
      IDOK :
      begin
        Device := SendDlgItemMessage(hWndDlg, 10, LB_GETCURSEL, 0, 0);
        Device := SendDlgItemMessage(hWndDlg, 10, LB_GETITEMDATA, Device, 0);	// get device #
        EndDialog(hWndDlg, Device);
      end;
    end;
  end;
end;
// Device selector stuff ends here

//---------------------------------------------------------

begin
  // check the correct BASS was loaded
  if HIWORD(BASS_GetVersion) <> BASSVERSION then
  begin
    MessageBox(0, 'An incorrect version of BASS.DLL was loaded', 'Error', MB_OK or MB_ICONERROR);
    Halt(BASS_ERROR_VERSION);
  end;

  // Let the user choose the output devices
  outdev[0] := DialogBoxParam(hInstance, MakeIntResource(2000), 0, @DeviceDialogproc, 1);
  outdev[1] := DialogBoxParam(hInstance, MakeIntResource(2000), 0, @DeviceDialogproc, 2);

  // display the window
  DialogBox(hInstance, MakeIntResource(1000), 0, @DialogProc);
end.
