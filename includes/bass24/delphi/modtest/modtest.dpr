{
	BASS MOD music test
	Copyright (c) 1999-2014 Un4seen Developments Ltd.

        C++ to Delphi with use API adapted by Evgeny Melnikov
        Required Delphi 7 or above
}

program modtest;

uses
  Windows, Messages, CommCtrl, CommDlg, Bass in '..\Bass.pas';

const
  szFiles	= 'Modules - mo3/xm/mod/s3m/it/mtm/umx'#0'*.mo3;*.xm;*.mod;*.s3m;*.it;*.mtm;*.umx'#0 +
		  'All files'#0'*.*'#0#0;
  szOpen	= 'Select a file to play';

var
  win		: HWND;
  music		: DWORD;				// the HMUSIC channel
  ofn		: OPENFILENAME;
  FileName	: array[0..MAX_PATH - 1] of Char;

{$R 'modtest.res' 'modtest.rc'}

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

function GetFlags : DWORD;
begin
  Result := BASS_MUSIC_POSRESET;				// stop notes when seeking

  case SendDlgItemMessage(win, 21, CB_GETCURSEL, 0, 0) of
    0 : Result := Result or BASS_MUSIC_NONINTER;		// no interpolation
    2 : Result := Result or BASS_MUSIC_SINCINTER;		// sinc interpolation
  end;

  case SendDlgItemMessage(win, 22, CB_GETCURSEL, 0, 0) of
    1 : Result := Result or BASS_MUSIC_RAMP;			// ramping
    2 : Result := Result or BASS_MUSIC_RAMPS;			// "sensitive" ramping
  end;

  case SendDlgItemMessage(win, 23, CB_GETCURSEL, 0, 0) of
    1 : Result := Result or BASS_MUSIC_SURROUND;		// surround
    2 : Result := Result or BASS_MUSIC_SURROUND2;		// "mode2"
  end;

{$IFDEF UNICODE}
  Result := Result or BASS_UNICODE;
{$ENDIF}
end;

//---------------------------------------------------------

procedure OnTimer;
var
  St  : String;
  Pos : QWORD;
begin
  Pos := BASS_ChannelGetPosition(music, BASS_POS_MUSIC_ORDER);
  if Pos <> QWORD(-1) then
  begin
    SendDlgItemMessage(win, 20, TBM_SETPOS, 1, LOWORD(Pos));
    St := Format('%03d.%03d', [LOWORD(Pos), HIWORD(Pos)]);
    SendDlgItemMessage(win, 15, WM_SETTEXT, 0, Integer(PChar(St)));
  end;
end;

//---------------------------------------------------------

procedure InitApp(Wnd : HWND);
begin
  win := Wnd;

  // initialize default output device
  if not BASS_Init(-1, 44100, 0, Wnd, NIL) then
  begin
    Error('Can''t initialize device');
    DestroyWindow(Wnd);
    Halt;
  end;

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
  
  SendDlgItemMessage(Wnd, 21, CB_ADDSTRING, 0, Integer(PChar('off')));
  SendDlgItemMessage(Wnd, 21, CB_ADDSTRING, 0, Integer(PChar('linear')));
  SendDlgItemMessage(Wnd, 21, CB_ADDSTRING, 0, Integer(PChar('sinc')));
  SendDlgItemMessage(Wnd, 21, CB_SETCURSEL, 1, 0);
  SendDlgItemMessage(Wnd, 22, CB_ADDSTRING, 0, Integer(PChar('off')));
  SendDlgItemMessage(Wnd, 22, CB_ADDSTRING, 0, Integer(PChar('normal')));
  SendDlgItemMessage(Wnd, 22, CB_ADDSTRING, 0, Integer(PChar('sensitive')));
  SendDlgItemMessage(Wnd, 22, CB_SETCURSEL, 2, 0);
  SendDlgItemMessage(Wnd, 23, CB_ADDSTRING, 0, Integer(PChar('off')));
  SendDlgItemMessage(Wnd, 23, CB_ADDSTRING, 0, Integer(PChar('mode1')));
  SendDlgItemMessage(Wnd, 23, CB_ADDSTRING, 0, Integer(PChar('mode2')));
  SendDlgItemMessage(Wnd, 23, CB_SETCURSEL, 0, 0);

  SetTimer(Wnd, 1, 100, NIL);
end;

//---------------------------------------------------------

function DialogProc(hWndDlg : HWND; Msg, wParam, lParam : Longint) : Integer; stdcall;
var
  Len      : DWORD;
  info     : BASS_CHANNELINFO;
  channels,
  Pos      : Integer;
  cType,
  St       : String;
  Tag      : PAnsiChar;
  sTag,
  mo3      : AnsiString;
begin
  Result := 0;
  case Msg of
    WM_INITDIALOG :
    begin
      InitApp(hWndDlg);
      Result := 1;
      Exit;
    end;

    WM_TIMER      : OnTimer;						// update display

    WM_COMMAND    :
    case LOWORD(wParam) of
      10 :
      begin
        if GetOpenFileName(ofn) then
        begin
          St := FileName;
          BASS_MusicFree(music);					// free the current music
          music := BASS_MusicLoad(False, PChar(St), 0, 0, GetFlags, 1);	// load the new music
          if music <> 0 then						// success
          begin
            Len := BASS_ChannelGetLength(music, BASS_POS_MUSIC_ORDER);	// get the order length
            SendDlgItemMessage(hWndDlg, 10, WM_SETTEXT, 0, Integer(@FileName));
            begin
              channels := 0;
              while BASS_ChannelGetAttributeEx(music, BASS_ATTRIB_MUSIC_VOL_CHAN + channels, NIL, 0) > 0 do
                inc(channels);						// count channels

              BASS_ChannelGetInfo(music, info);

              cType := '';
              case (info.ctype and not BASS_CTYPE_MUSIC_MO3) of
                BASS_CTYPE_MUSIC_MOD : cType := 'MOD';

                BASS_CTYPE_MUSIC_MTM : cType := 'MTM';

                BASS_CTYPE_MUSIC_S3M : cType := 'S3M';

                BASS_CTYPE_MUSIC_XM  : cType := 'XM';

                BASS_CTYPE_MUSIC_IT  : cType := 'IT';
              end;

              Tag  := BASS_ChannelGetTags(music, BASS_TAG_MUSIC_NAME);
              sTag := Tag;
              mo3  := '';
              if info.ctype and BASS_CTYPE_MUSIC_MO3 = BASS_CTYPE_MUSIC_MO3 then
                mo3 := ' (MO3)';
              St := Format('name: %s, format: %dch %s%s', [String(sTag), channels, String(cType), String(mo3)]);
              SendDlgItemMessage(hWndDlg, 11, WM_SETTEXT, 0, Integer(PChar(St)));
            end;
            SendDlgItemMessage(hWndDlg, 20, TBM_SETRANGEMAX, 1, Len - 1); // update scroller range
            BASS_ChannelPlay(music, False);				// start it
          end
          else
          begin								// failed
            SendDlgItemMessage(hWndDlg, 10, WM_SETTEXT, 0, Integer(PChar('click here to open a file...')));
            SendDlgItemMessage(hWndDlg, 11, WM_SETTEXT, 0, Integer(NIL));
            SendDlgItemMessage(hWndDlg, 15, WM_SETTEXT, 0, Integer(NIL));
            Error('Can''t play the file');
          end;
        end;
      end;

      12 :
      if BASS_ChannelIsActive(music) = BASS_ACTIVE_PLAYING then
        BASS_ChannelPause(music)
      else
        BASS_ChannelPlay(music, False);

      21..23 : BASS_ChannelFlags(music, GetFlags, DWORD(-1));			// update flags

      IDCANCEL : DestroyWindow(hWndDlg);
    end;

    WM_HSCROLL :
    if (lparam > 0) and (LOWORD(wParam) <> SB_THUMBPOSITION) and (LOWORD(wParam) <> SB_ENDSCROLL) then
    begin
      Pos := SendMessage(lParam, TBM_GETPOS, 0, 0);
      BASS_ChannelSetPosition(music, pos, BASS_POS_MUSIC_ORDER);	// set the position
    end;

    WM_CLOSE   : DestroyWindow(hWndDlg);

    WM_DESTROY :
    begin
      BASS_Free;
      PostQuitMessage(0);
      Exit;
    end;
  end;
end;

//---------------------------------------------------------

begin
  // check the correct BASS was loaded
  if HIWORD(BASS_GetVersion) <> BASSVERSION then
  begin
    MessageBox(0, 'An incorrect version of BASS.DLL was loaded', 'Error', MB_OK or MB_ICONERROR);
    Halt(BASS_ERROR_VERSION);
  end;

  // display the window
  DialogBox(hInstance, MakeIntResource(1000), 0, @DialogProc);
end.
