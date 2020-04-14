// BASS spectrum analyser example, copyright (c) 2002-2006 Ian Luck.
//
// C++ to Delphi adapted by Evgeny Melnikov
//
// http://dsoft1961.narod.ru
// mail angvelem@gmail.com

unit Unit1;

{$I Common.inc}

{$IFDEF D7}
  {$WARN SYMBOL_PLATFORM OFF}
{$ENDIF}

interface

uses
  Windows, Messages, CommDlg, MMSystem, Bass;

const
  SPECWIDTH	= 368;	// display width
  SPECHEIGHT	= 127;	// height (changing requires palette adjustments too)
  BANDS		= 28;
  szAppName	= 'BASS-Spectrum';

  BASSErCodes	: array[-1..44] of String = (
    'Some other mystery error',
    'All is OK',
    'Memory error',
    'Can''t open the file',
    'Can''t find a free sound driver',
    'The sample buffer was lost',
    'Invalid handle',
    'Unsupported sample format',
    'Invalid playback position',
    'BASS_Init has not been successfully called',
    'BASS_Start has not been successfully called',
    'Unknown error',
    'Unknown error',
    'Unknown error',
    'Unknown error',
    'Already initialized/paused/whatever',
    'Unknown error',
    'Not paused',
    'Unknown error',
    'Can''t get a free channel',
    'An illegal type was specified',
    'An illegal parameter was specified',
    'No 3D support',
    'No EAX support',
    'Illegal device number',
    'Not playing',
    'Illegal sample rate',
    'Unknown error',
    'The stream is not a file stream',
    'Unknown error',
    'No hardware voices available',
    'Unknown error',
    'The MOD music has no sequence data',
    'No internet connection could be opened',
    'Couldn''t create the file',
    'Effects are not enabled',
    'The channel is playing',
    'Unknown error',
    'Requested data is not available',
    'The channel is a "decoding channel"',
    'A sufficient DirectX version is not installed',
    'Connection timedout',
    'Unsupported file format',
    'Unavailable speaker',
    'Invalid BASS version (used by add-ons)',
    'Codec is not available/supported');

var
  Window	: HWND = 0;
  Msg           : TMsg;
  WndClass      : TWndClassEX;

  PosX, PosY    : Integer;
  SizeX, SizeY  : Integer;

  Timer		: DWORD = 0;
  Chan		: DWORD;

  SpecDC	: HDC = 0;
  SpecBmp	: HBITMAP = 0;
  SpecBuf	: Pointer;
  SpecMode	: Integer = 0;
  SpecPos	: Integer = 0; // spectrum mode (and marker pos for 2nd mode)

  BI		: TBITMAPINFO;
  pal		: array[Byte] of TRGBQUAD;

procedure WinMain;

implementation

var
  Win32Platform		: Integer = 0;
  Win32MajorVersion	: Integer = 0;
  Win32MinorVersion	: Integer = 0;
  Win32BuildNumber	: Integer = 0;
  Win32CSDVersion	: string = '';

//---------------------------------------------------------

function IntPower(const Base: Extended; const Exponent: Integer): Extended;
asm
        mov     ecx, eax
        cdq
        fld1                      { Result := 1 }
        xor     eax, edx
        sub     eax, edx          { eax := Abs(Exponent) }
        jz      @@3
        fld     Base
        jmp     @@2
@@1:    fmul    ST, ST            { X := Base * Base }
@@2:    shr     eax,1
        jnc     @@1
        fmul    ST(1),ST          { Result := Result * X }
        jnz     @@1
        fstp    st                { pop X from FPU stack }
        cmp     ecx, 0
        jge     @@3
        fld1
        fdivrp                    { Result := 1 / Result }
@@3:
        fwait
end;

//---------------------------------------------------------

function Power(const Base, Exponent: Extended): Extended;
begin
  if Exponent = 0.0 then
    Result := 1.0               { n**0 = 1 }
  else if (Base = 0.0) and (Exponent > 0.0) then
    Result := 0.0               { 0**n = 0, n > 0 }
  else if (Frac(Exponent) = 0.0) and (Abs(Exponent) <= MaxInt) then
    Result := IntPower(Base, Integer(Trunc(Exponent)))
  else
    Result := Exp(Exponent * Ln(Base))
end;

//---------------------------------------------------------
// Log.10(X) := Log.2(X) * Log.10(2)

function Log10(const X : Extended) : Extended;
asm
	FLDLG2     { Log base ten of 2 }
	FLD	X
	FYL2X
	FWAIT
end;

//---------------------------------------------------------

function IntToStr(I : Integer) : String;
begin
  Str(I, Result);
end;

//---------------------------------------------------------

procedure GetOSInfo;
var
  OSVersionInfo	: TOSVersionInfo;
begin
  OSVersionInfo.dwOSVersionInfoSize := SizeOf(OSVersionInfo);
  if GetVersionEx(OSVersionInfo) then
    with OSVersionInfo do
    begin
      Win32Platform	:= dwPlatformId;
      Win32MajorVersion := dwMajorVersion;
      Win32MinorVersion := dwMinorVersion;
      Win32BuildNumber	:= dwBuildNumber;
      Win32CSDVersion	:= szCSDVersion;
    end;
end;

//---------------------------------------------------------
// display error messages

procedure Error(St : String);
var
  ErrCode : Integer;
  Mes     : String;
begin
  ErrCode := BASS_ErrorGetCode;
  if ErrCode > 44 then
    ErrCode := 10;

  Mes := St + #13#10 + BASSErCodes[ErrCode] + #13#10 + '(Error code: ' + IntToStr(ErrCode) + ')';
  MessageBox(0, PChar(Mes), '', MB_ICONERROR or MB_OK);
end;

//---------------------------------------------------------
// select a file to play, and play it

function PlayFile(Wnd : HWND) : Boolean; 
var
  FileName	: array[0..MAX_PATH - 1] of Char;
  ofn	 	: TOPENFILENAME;
  TempFileName	: String;
begin
  Result := False;
  FillChar(ofn, SizeOf(ofn), 0);
  FillChar(FileName, SizeOf(FileName), 0);
{$IFDEF D7}
  if (Win32MajorVersion >= 5) and (Win32Platform = VER_PLATFORM_WIN32_NT) or { Win2k }
     ((Win32Platform = VER_PLATFORM_WIN32_WINDOWS) and (Win32MajorVersion >= 4) and (Win32MinorVersion >= 90)) then { WinME }
    ofn.lStructSize := SizeOf(TOpenFilename)
  else
    ofn.lStructSize := SizeOf(TOpenFilename) - (SizeOf(DWORD) shl 1) - SizeOf(Pointer); { subtract size of added fields }
{$ELSE}
    ofn.lStructSize := SizeOf(TOpenFilename);
{$ENDIF}
  ofn.hwndOwner		:= Window;
  ofn.hInstance		:= hInstance;
  ofn.lpstrTitle	:= 'Select a file to play';
  ofn.lpstrFile		:= FileName;
  ofn.lpstrFilter	:= 'playable files'#0'*.mo3;*.xm;*.mod;*.s3m;*.it;*.mtm;*.umx;*.mp3;*.mp2;*.mp1;*.ogg;*.wav;*.aif'#0'All files'#0'*.*'#0#0;
  ofn.nMaxFile		:= MAX_PATH;
  ofn.Flags		:= OFN_FILEMUSTEXIST or OFN_HIDEREADONLY or OFN_EXPLORER;

  if not GetOpenFileName(ofn) then
    Exit;

  TempFileName := FileName;  
  chan := BASS_StreamCreateFile(False, PChar(TempFileName), 0, 0, BASS_SAMPLE_LOOP {$IFDEF UNICODE} or BASS_UNICODE {$ENDIF});
  if (chan = 0) then
  begin
    chan := BASS_MusicLoad(False, PChar(TempFileName), 0, 0, BASS_MUSIC_RAMP or BASS_SAMPLE_LOOP {$IFDEF UNICODE} or BASS_UNICODE {$ENDIF}, 1);
    if (chan = 0) then
    begin
      Error('Can''t play file'); // Can't load the file
      Exit;
    end;
  end;

  BASS_ChannelPlay(chan, False);
  Result := True;
end;

//---------------------------------------------------------
// update the spectrum display - the interesting bit :)

procedure UpdateSpectrum(uTimerID, uMsg, dwUser, dw1, dw2 : Integer); stdcall;
type
  TSingleArray	= array of Single;
var
  DC		: HDC;
  X, Y, Z,
  I, J, sc	: Integer;
  Sum		: Single;
  fft		: array[0..1023] of Single; // get the FFT data
  ci		: BASS_CHANNELINFO;
  Buf		: TSingleArray;
begin
  if SpecMode = 3 then  // waveform
  begin
    FillChar(SpecBuf^, SPECWIDTH * SPECHEIGHT, 0);
    BASS_ChannelGetInfo(chan, ci); // get number of channels
    SetLength(Buf, ci.chans * SPECWIDTH);
    Y := 0;
    BASS_ChannelGetData(chan, buf, (ci.chans * SPECWIDTH * SizeOf(Single)) or BASS_DATA_FLOAT); // get the sample data (floating-point to avoid 8 & 16 bit processing)
    for I := 0 to ci.chans - 1 do
    begin
      for X := 0 to SPECWIDTH - 1 do
      begin
	Z := Trunc((1 - Buf[X * Integer(ci.chans) + I]) * SPECHEIGHT / 2); // invert and scale to fit display
	if Z < 0 then
	  Z := 0
	else if Z >= SPECHEIGHT then
	  Z := SPECHEIGHT - 1;
	if X = 0 then
	  Y := Z;
	repeat  // draw line from previous sample...
	  if Y < Z then
	    inc(Y)
	  else if Y > Z then
	    dec(Y);
	  if (I and 1) = 1 then
	    Byte(Pointer(Longint(SpecBuf) + Y * SPECWIDTH + X)^) := 127
	  else
	    Byte(Pointer(Longint(SpecBuf) + Y * SPECWIDTH + X)^) := 1;
	until Y = Z;
      end;
    end;
  end
  else
  begin
    BASS_ChannelGetData(chan, @fft, BASS_DATA_FFT2048);
    case SpecMode of
      0 :  // "normal" FFT
      begin
	FillChar(SpecBuf^, SPECWIDTH * SPECHEIGHT, 0);
	Z := 0;
	for X := 0 to pred(SPECWIDTH) div 2 do
	begin
	  Y := Trunc(sqrt(fft[X + 1]) * 3 * SPECHEIGHT - 4); // scale it (sqrt to make low values more visible)
//	Y := Trunc(fft[X + 1] * 10 * SPECHEIGHT); // scale it (linearly)
	  if Y > SPECHEIGHT then
	    Y := SPECHEIGHT; // cap it
      
	  if (X > 0) and (Z = (Y + Z) div 2) then // interpolate from previous to make the display smoother
	    while (Z >= 0) do
	    begin
	      Byte(Pointer(Longint(SpecBuf) + Z * SPECWIDTH + X * 2 - 1)^) := Z + 1;
	      dec(Z);
	    end;

	  Z := Y;
	  while (Y >= 0) do
	  begin
	    Byte(Pointer(Longint(SpecBuf) + Y * SPECWIDTH + X * 2)^) := Y + 1; // draw level
	    dec(Y);
	  end;  
        end;
      end;  
      1 :   // logarithmic, acumulate & average bins
      begin
        I := 0;
        FillChar(SpecBuf^, SPECWIDTH * SPECHEIGHT, 0);
	for X := 0 to BANDS - 1 do
	begin
	  Sum := 0;
	  J  := Trunc(Power(2, X * 10.0 / (BANDS - 1)));
	  if J > 1023 then
	    J := 1023;
	  if J <= I then
	    J := I + 1; // make sure it uses at least 1 FFT bin
	  sc := 10 + J - I;

	  while I < J do
	  begin
	    Sum := Sum + fft[1 + I];
	    inc(I);
	  end;

	  Y := Trunc((sqrt(Sum / log10(sc)) * 1.7 * SPECHEIGHT) - 4); // scale it
	  if Y > SPECHEIGHT then
	    Y := SPECHEIGHT; // cap it
	  while (Y >= 0) do
	  begin
	    FillChar(Pointer(Longint(SpecBuf) + Y * SPECWIDTH + X * (SPECWIDTH div BANDS))^, SPECWIDTH div BANDS - 2, Y + 1); // draw bar
	    dec(Y);
	  end;
	end;
      end;
      2 :  // "3D"
      begin
	for X := 0 to SPECHEIGHT - 1 do
	begin
	  Y := Trunc(sqrt(fft[x + 1]) * 3 * 127); // scale it (sqrt to make low values more visible)
	  if Y > 127 then
	    Y := 127; // cap it
	  Byte(Pointer(Longint(SpecBuf) + X * SPECWIDTH + SpecPos)^) := 128 + Y; // plot it
	end;
	// move marker onto next position
	SpecPos := (SpecPos + 1) mod SPECWIDTH;
	for X := 0 to SPECHEIGHT do
	  Byte(Pointer(Longint(SpecBuf) + X * SPECWIDTH + SpecPos)^) := 255;
      end;
    end;
  end;
  // update the display
  DC := GetDC(Window);
  try
    BitBlt(DC, 0, 0, SPECWIDTH, SPECHEIGHT, SpecDC, 0, 0, SRCCOPY);
  finally
    ReleaseDC(Window, DC);
  end;
end;

//---------------------------------------------------------
// window procedure

function SpectrumWindowProc(Wnd : HWND; Msg : Integer; wParam, lParam : Longint): Integer; stdcall;
var
  Ps	: TPAINTSTRUCT;
  DC	: HDC;
  I	: Integer;
begin
  Result := 0;
  
  case Msg of
    WM_CREATE :
    begin
      GetOSInfo;
    
      // initialize BASS
      if not BASS_Init(-1, 44100, 0, Wnd, NIL) then
      begin
	Error('Can''t initialize device');
	Result := -1;
	Exit;
      end;

      if not PlayFile(Wnd) then // start a file playing
      begin
	BASS_Free;
	Result := -1;
	Exit;
      end;

      // create bitmap to draw spectrum in - 8 bit for easy updating :)
      FillChar(BI, SizeOf(BI), 0);
      with BI.bmiHeader do        // fill structure with parameter bitmap
      begin
        biSize		:= SizeOf(BI.bmiHeader);
        biWidth		:= SPECWIDTH;
        biHeight	:= SPECHEIGHT; // upside down (line 0=bottom)
        biPlanes	:= 1;
        biBitCount	:= 8;
        biClrImportant	:= 256;
        biClrUsed	:= 256;
      end;

      // setup palette
      for I := 0 to 127 do
      begin
        pal[I].rgbGreen := 255 - 2 * I;
        pal[I].rgbRed   := 2 * I;
      end;

      for I := 0 to 31 do
      begin
        pal[128 + I].rgbBlue       := 8 * I;
        pal[128 + 32 + I].rgbBlue  := 255;
        pal[128 + 32 + I].rgbRed   := 8 * I;
        pal[128 + 64 + I].rgbRed   := 255;
        pal[128 + 64 + I].rgbBlue  := 8 * (31 - I);
        pal[128 + 64 + I].rgbGreen := 8 * I;
        pal[128 + 96 + I].rgbRed   := 255;
        pal[128 + 96 + I].rgbGreen := 255;
        pal[128 + 96 + I].rgbBlue  := 8 * I;
      end;

      // create the bitmap
      SpecBmp := CreateDIBSection(0, BI, DIB_RGB_COLORS, SpecBuf, 0, 0);
      SpecDC  := CreateCompatibleDC(0);
      SelectObject(SpecDC, SpecBmp);

      // setup update timer (40hz)
      timer := timeSetEvent(25, 25, @UpdateSpectrum, 0, TIME_PERIODIC);
    end;

    WM_PAINT :
    if GetUpdateRect(Wnd, PRect(NIL)^, False) then
    begin
      DC := BeginPaint(Wnd, Ps);
      if DC = 0 then
      begin
        Result := 0;
        Exit;
      end;

      BitBlt(DC, 0, 0, SPECWIDTH, SPECHEIGHT, SpecDC, 0, 0, SRCCOPY);
      EndPaint(Wnd, Ps);
      Result := 0;
      Exit;
    end; 

    WM_LBUTTONUP :
    begin
      SpecMode := (SpecMode + 1) mod 4; // swap spectrum mode
      if SpecMode = 2 then
        SpecPos := 0;
      FillChar(SpecBuf^, SPECWIDTH * SPECHEIGHT, 0);	// clear display
      Result := 0;
      Exit;
    end;

    WM_CLOSE :
    begin
      DestroyWindow(Wnd);
    end;

    WM_DESTROY :
    begin
      if timer <> 0 then
        timeKillEvent(timer);
      BASS_Free;
      if SpecDC <> 0 then
        DeleteDC(SpecDC);
      if SpecBmp <> 0 then
        DeleteObject(specbmp);
      PostQuitMessage(0);
      Exit;
    end;
  end;
  Result := DefWindowProc(Wnd, Msg, wParam, lParam);
end;

//---------------------------------------------------------

procedure WinMain;
begin
  Window := FindWindow(szAppName, NIL);
  if Window <> 0 then
  begin
    if IsIconic(Window) then
      ShowWindow(Window, SW_RESTORE);
    SetForegroundWindow(Window);
    Halt(254);
  end;

  // check the correct BASS was loaded
  if HIWORD(BASS_GetVersion) <> BASSVERSION then
  begin
    MessageBox(0, 'An incorrect version of BASS.DLL was loaded', '', MB_ICONERROR);
    Exit;
  end;

  // register window class and create the window
  FillChar(WndClass, SizeOf(TWndClassEx), 0);
  WndClass.cbSize        := SizeOf(TWndClassEx);
  WndClass.style         := CS_HREDRAW or CS_VREDRAW;
  WndClass.lpfnWndProc   := @SpectrumWindowProc;
  WndClass.cbClsExtra    := 0;
  WndClass.cbWndExtra    := 0;
  WndClass.hInstance     := hInstance;
  WndClass.hCursor       := LoadCursor(0, IDC_ARROW);
  WndClass.hbrBackGround := GetSysColorBrush(COLOR_BTNFACE);
  WndClass.lpszClassName := szAppName;

  if RegisterClassEx(WndClass) = 0 then
    Halt(255);

  SizeX := SPECWIDTH  + 2 * GetSystemMetrics(SM_CXDLGFRAME);
  SizeY := SPECHEIGHT + 2 * GetSystemMetrics(SM_CYDLGFRAME) + GetSystemMetrics(SM_CYCAPTION);

  PosX := (GetSystemMetrics(SM_CXSCREEN) - SizeX) div 2;
  PosY := (GetSystemMetrics(SM_CYSCREEN) - SizeY) div 2;

  Window := CreateWindowEx(0, szAppName, 'BASS spectrum example (click to toggle mode)',
                        WS_POPUPWINDOW or WS_CAPTION,
			PosX, PosY, SizeX, SizeY, 0, 0, hInstance, NIL);

  ShowWindow(Window, SW_SHOWNORMAL);

  while (GetMessage(Msg, 0, 0, 0)) do
  begin
    TranslateMessage(Msg);
    DispatchMessage(Msg);
  end;

  Halt(Msg.wParam);
end;

begin
  WinMain;
end.
