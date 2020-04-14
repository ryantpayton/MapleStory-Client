{
	BASS "live" spectrum analyser example
	Copyright (c) 2002-2008 Un4seen Developments Ltd.

        C++ to Delphi with use API adapted by Evgeny Melnikov
        Required Delphi 5 or above

        http://dsoft1961.narod.ru
        mail angvelem@gmail.com
}

program livespec;

uses
  Windows, Messages, MMSystem, Bass in '..\bass.pas';

const
  SPECWIDTH	= 368;	// display width
  SPECHEIGHT	= 127;	// height (changing requires palette adjustments too)
  BANDS		= 28;
  szAppName	= 'BASS-Spectrum';

var
  Window	: HWND = 0;
  WndGlobal	: HWND = 0;
  Msg           : TMsg;
  WndClass      : TWndClassEX;

  PosX, PosY    : Integer;
  SizeX, SizeY  : Integer;

  Timer		: DWORD = 0;
  Channel	: HRECORD;	// recording channel

  SpecDC	: HDC = 0;
  SpecBmp	: HBITMAP = 0;
  SpecBuf	: Pointer;
  SpecMode	: Integer = 0;
  SpecPos	: Integer = 0; // spectrum mode (and marker pos for 2nd mode)
  quietcount	: DWORD = 0;

  BI		: PBITMAPINFO;
  pal		: array[Byte] of TRGBQUAD;

{$DEFINE ScaleSqrt}

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

function IntPower(const Base : Extended; const Exponent : Integer) : Extended;
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

function Power(const Base, Exponent : Extended) : Extended;
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

//==========================================================
//==========================================================

// display error messages
procedure Error(const es : String);
begin
  MessageBox(WndGlobal, PChar(Format('%s' + #13#10 + '(error code: %d)', [es, BASS_ErrorGetCode])), 'Error', MB_OK or MB_ICONERROR);
end;

//---------------------------------------------------------
// update the spectrum display - the interesting bit :)

procedure UpdateSpectrum(uTimerID, uMsg, dwUser, dw1, dw2 : Integer); stdcall;
type
  TSpecBuf   = array[0..0] of Byte;
  TBuf       = array of SmallInt;
var
  DC         : HDC;
  X, Y, Y1,
  V, B0, B1,
  SC         : Integer;
  Sum        : Single;
  aRect      : TRect;
  fft        : array[0..1023] of Single;
  Buf        : TBuf;
  SBuf       : ^TSpecBuf absolute SpecBuf;
begin
  if SpecMode = 3 then  // waveform
  begin
    FillChar(SpecBuf^, SPECWIDTH * SPECHEIGHT, 0);
    SetLength(Buf, SPECWIDTH);
    BASS_ChannelGetData(Channel, Buf, SizeOf(SmallInt) * SPECWIDTH); // get the sample data

    Y := 0;
    for X := 0 to SPECWIDTH - 1 do
    begin
      V := (32767 - Buf[X]) * SPECHEIGHT div 65536; // invert and scale to fit display
      if X = 0 then
	Y := V;
      repeat	// draw line from previous sample...
	if Y < V then
	  inc(Y)
	else if Y > V then
	  dec(Y);
	SBuf[Y * SPECWIDTH + X] := abs(Y - SPECHEIGHT div 2) * 2 + 1;
      until Y = V;
    end;
  end
  else
  begin
    BASS_ChannelGetData(Channel, @fft, BASS_DATA_FFT2048); // get the FFT data
    case SpecMode of
      0 :  // "normal" FFT
      begin
	FillChar(SpecBuf^, SPECWIDTH * SPECHEIGHT, 0);

	Y1 := 0;
	for X := 0 to (SPECWIDTH div 2) - 1 do
	begin
{$IFDEF ScaleSqrt}
	  Y := Trunc(sqrt(fft[X + 1]) * 3 * SPECHEIGHT - 4); // scale it (sqrt to make low values more visible)
{$ELSE}
	  Y := Trunc(fft[x + 1] * 10 * SPECHEIGHT); // scale it (linearly)
{$ENDIF}
	  if Y > SPECHEIGHT then
	    Y := SPECHEIGHT; // cap it

	  Y1 := (Y + Y1) div 2;  
	  if (X > 0) and (Y1 > 0) then // interpolate from previous to make the display smoother
	    while (Y1 >= 0) do
	    begin
	      SBuf[Y1 * SPECWIDTH + X * 2 - 1] := Y1 + 1;
	      dec(Y1);
	    end;
	    
	  Y1 := Y;
	  while (Y >= 0) do
	  begin
	    SBuf[Y * SPECWIDTH + X * 2] := Y + 1; // draw level
	    dec(Y);
	  end;  
	end;
      end;
      1 :  // logarithmic, acumulate & average bins
      begin
	B0 := 0;
	FillChar(SpecBuf^, SPECWIDTH * SPECHEIGHT, 0);
        for X := 0 to BANDS - 1 do
        begin
          Sum := 0;
          B1 := Trunc(Power(2, X * 10.0 / (BANDS - 1)));
          if B1 > 1023 then
            B1 := 1023;
          if B1 <= B0 then
            B1 := B0 + 1; // make sure it uses at least 1 FFT bin
          SC := 10 + B1 - B0;
          
          while B0 < B1 do
          begin
            Sum := Sum + fft[1 + B0];
            inc(B0);
          end;
          
          Y := Trunc((sqrt(Sum / log10(SC)) * 1.7 * SPECHEIGHT) - 4); // scale it
          if Y > SPECHEIGHT then
            Y := SPECHEIGHT; // cap it

          while (Y >= 0) do
          begin
	    FillChar(SBuf[Y * SPECWIDTH + X * (SPECWIDTH div BANDS)], Trunc(0.9 * (SPECWIDTH / BANDS)), Y + 1); // draw bar
	    dec(Y);
	  end;
	end;
      end;
      2 : // "3D"
      begin
        for X := 0 to SPECHEIGHT - 1 do
        begin
	  Y := Trunc(sqrt(fft[X + 1]) * 3 * 127); // scale it (sqrt to make low values more visible)
	  if Y > 127 then
	    Y := 127; // cap it
	  SBuf[X * SPECWIDTH + SpecPos] := 128 + Y; // plot it
	end;
	// move marker onto next position
	SpecPos := (SpecPos + 1) mod SPECWIDTH;
	for X := 0 to SPECHEIGHT - 1 do
          SBuf[X * SPECWIDTH + SpecPos] := 255;
      end;
    end;
  end;
  // update the display
  DC := GetDC(WndGlobal);
  try
    BitBlt(DC, 0, 0, SPECWIDTH, SPECHEIGHT, SpecDC, 0, 0, SRCCOPY);
    if LOWORD(BASS_ChannelGetLevel(Channel)) < 500 then
    begin // check if it's quiet
      inc(QuietCount);
      if (QuietCount > 40) and (QuietCount and 16 > 0) then
      begin // it's been quiet for over a second
        SetRect(aRect, 0, 0, SPECWIDTH, SPECHEIGHT);
        SetTextColor(DC, $ffffff);
        SetBkMode(DC, TRANSPARENT);
        DrawText(DC, 'make some noise!', -1, aRect, DT_CENTER or DT_VCENTER or DT_SINGLELINE);
      end;
    end
    else
      QuietCount := 0; // not quiet
  finally
    ReleaseDC(Window, DC);
  end;
end;

//---------------------------------------------------------
// Recording callback - not doing anything with the data

function DuffRecording(handle : HRECORD; const Buffer : Pointer; Length : DWORD; user : Pointer) : Boolean; stdcall;
begin
  Result := True;	// continue recording
end;

//---------------------------------------------------------

procedure InitApp(Wnd : HWND);
var
  I : Integer;
begin
  WndGlobal := Wnd;

  // initialize BASS recording (default device)
  if not BASS_RecordInit(-1) then
  begin
    Error('Can''t initialize device');
    Halt;
  end;

  // start recording (44100hz mono 16-bit)
  Channel := BASS_RecordStart(44100, 1, 0, @DuffRecording, NIL);
  if Channel = 0 then
  begin
    Error('Can''t start recording');
    Halt;
  end;

  // create bitmap to draw spectrum in - 8 bit for easy updating :)
  GetMem(BI, SizeOf(TBitmapInfo) + 256 * SizeOf(TRGBQuad));
  try
    with BI^.bmiHeader do		// fill structure with parameter bitmap
    begin
      biSize         := SizeOf(BI.bmiHeader);
      biWidth        := SPECWIDTH;
      biHeight       := SPECHEIGHT;	// upside down (line 0=bottom)
      biPlanes       := 1;
      biBitCount     := 8;
      biClrImportant := 256;
      biClrUsed	     := 256;
    end;

    // setup palette
    for I := 1 to 127 do
    begin
      pal[I].rgbGreen := 256 - 2 * I;
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

    // Move palette in BI
    Move(Pal, BI^.bmiColors, 256 * SizeOf(TRGBQuad));

    // create the bitmap
    SpecBmp := CreateDIBSection(0, BI^, DIB_RGB_COLORS, SpecBuf, 0, 0);
    SpecDC  := CreateCompatibleDC(0);
    SelectObject(SpecDC, SpecBmp);
  finally
    FreeMem(BI);
  end;  

  // setup update timer (40hz)
  timer := timeSetEvent(25, 25, @UpdateSpectrum, 0, TIME_PERIODIC);
end;

//---------------------------------------------------------
// window procedure

function SpectrumWindowProc(Wnd : HWND; Msg : Integer; wParam, lParam : Longint) : Integer; stdcall;
var
  Ps	: TPAINTSTRUCT;
  DC	: HDC;
begin
  Result := 0;
  
  case Msg of
    WM_CREATE : InitApp(Wnd);

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
      BASS_RecordFree;
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

  Window := CreateWindowEx(0, szAppName, 'BASS "live" spectrum (click to toggle mode)',
                        WS_POPUPWINDOW or WS_CAPTION,
			PosX, PosY, SizeX, SizeY, 0, 0, hInstance, NIL);

  ShowWindow(Window, SW_SHOWNORMAL);

  while (GetMessage(Msg, 0, 0, 0)) do
  begin
    TranslateMessage(Msg);
    DispatchMessage(Msg);
  end;

  Halt(Msg.wParam);
end.
