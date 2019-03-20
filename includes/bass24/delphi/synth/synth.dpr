{
	BASS simple synth
	Copyright (c) 2001-2008 Un4seen Developments Ltd.

        C++ to Delphi with use API adapted by Evgeny Melnikov
        Required Delphi 5 or above

        http://dsoft1961.narod.ru
        mail angvelem@gmail.com
}

program synth;

{$APPTYPE CONSOLE}

uses
  Windows, Bass in '../bass.pas';

const
  PI		= 3.14159265358979323846;
  TABLESIZE	= 2048;
  KEYS		= 20;
  MAXVOL	= 4000;		// higher value = longer fadeout
  akey		: array[0..KEYS - 1] of Word = (
	ord('Q'), ord('2'), ord('W'), ord('3'), ord('E'),
	ord('R'), ord('5'), ord('T'), ord('6'), ord('Y'),
	ord('7'), ord('U'), ord('I'), ord('9'), ord('O'),
	ord('0'), ord('P'), 219,      187,      221);

var
  info      : BASS_INFO;
  SineTable	: array[0..TABLESIZE - 1] of Integer;	// sine table
  aVol		: array[0..KEYS - 1] of Integer = (	// keys' volume & pos
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
  aPos		: array[0..KEYS - 1] of Integer = (
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);

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

//==========================================================
//==========================================================

// display error messages
procedure Error(Text : String);
begin
  WriteLn(Format('Error(%d): %s', [BASS_ErrorGetCode, Text]));
  BASS_Free;
  ExitProcess(0);
end;

//---------------------------------------------------------

// stream writer
function WriteStream(Handle : HSTREAM; Buffer : Pointer; Len : DWORD; User : Pointer) : DWORD; stdcall;
type
  BufArray = array[0..0] of SmallInt;
var
  I, J, K : Integer;
  f       : Single;
  Buf     : ^BufArray absolute Buffer;
begin
  FillChar(Buffer^, Len, 0);
  for I := 0 to KEYS - 1 do
  begin
    if aVol[I] = 0 then
      Continue;
    f := Power(2.0, (I + 3) / 12.0) * TABLESIZE * 440.0 / info.freq;
    for K := 0 to (Len div 4 - 1) do
    begin
      if aVol[I] = 0 then
	Continue;
      inc(aPos[I]);
      J := Round(SineTable[Round(aPos[I] * f) and pred(TABLESIZE)] * aVol[I] / MAXVOL);
      inc(J, Buf[K * 2]);
      if J > 32767 then
	J := 32767
      else if J < -32768 then
	J := -32768;
      // left and right channels are the same
      Buf[K * 2 + 1] := J;
      Buf[K * 2]     := J;
      if aVol[I] < MAXVOL then
	dec(aVol[I]);
    end;
  end;
  Result := Len;
end;

//---------------------------------------------------------

var
  Stream    : HSTREAM;
  KeyIn     : INPUT_RECORD;
  bKey      : Integer;
  I, BufLen : DWORD;
  J         : HFX;
  St        : String;
  fx        : array[0..8] of HFX = (0, 0, 0, 0, 0, 0, 0, 0, 0);	// effect handles

const
  fxname  : array[0..8] of String = (
    'CHORUS',     'COMPRESSOR', 'DISTORTION', 
    'ECHO',       'FLANGER',    'GARGLE',
    'I3DL2REVERB','PARAMEQ',    'REVERB');

begin
  WriteLn('BASS Simple Sinewave Synth');
  WriteLn('--------------------------');

  // check the correct BASS was loaded
  if HIWORD(BASS_GetVersion) <> BASSVERSION then
  begin
    WriteLn('An incorrect version of BASS.DLL was loaded');
    Exit;
  end;

  // 10ms update period
  BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD, 10);

  // setup output - get latency
  if not BASS_Init(-1, 44100, BASS_DEVICE_LATENCY, 0, NIL) then
    Error('Can''t initialize device');

  // build sine table
  for I := 0 to TABLESIZE - 1 do
    SineTable[I] := Round((sin(2.0 * PI * I / TABLESIZE) * 7000.0));

  BASS_GetInfo(info);
  // default buffer size = update period + 'minbuf'
  BASS_SetConfig(BASS_CONFIG_BUFFER, 10 + info.minbuf);
  BufLen := BASS_GetConfig(BASS_CONFIG_BUFFER);
  // if the device's output rate is unknown default to 44100 Hz
  if info.freq = 0 then info.freq := 44100;
  // create a stream, stereo so that effects sound nice
  Stream := BASS_StreamCreate(info.freq, 2, 0, @WriteStream, NIL);

  WriteLn(Format('device latency: %dms', [info.latency]));
  WriteLn(Format('device minbuf: %dms', [info.minbuf]));
  if info.dsver < 8 then
    St := 'disabled'
  else
    St := 'enabled';
  WriteLn(Format('ds version: %d (effects %s)', [info.dsver, St]));
  WriteLn('press these keys to play:'#13#10);
  WriteLn('  2 3  5 6 7  9 0  =');
  WriteLn(' Q W ER T Y UI O P[ ]'#13#10);
  WriteLn('press -/+ to de/increase the buffer');
  WriteLn('press spacebar to quit'#13#10);
  if info.dsver >= 8 then	// DX8 effects available
    WriteLn('press F1-F9 to toggle effects'#13#10);
  Write(Format('using a %dms buffer'#13, [BufLen]));

  BASS_ChannelPlay(Stream, False);

  while (ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), KeyIn, 1, I)) do
  begin
    if KeyIn.EventType <> KEY_EVENT then
      Continue;

    if KeyIn.Event.KeyEvent.wVirtualKeyCode = VK_SPACE then
      Break;

    if KeyIn.Event.KeyEvent.bKeyDown then 
    begin
      case KeyIn.Event.KeyEvent.wVirtualKeyCode of
	VK_SUBTRACT,
	VK_ADD :
	begin   
	  // recreate stream with smaller/larger buffer
	  BASS_StreamFree(Stream);
	  if KeyIn.Event.KeyEvent.wVirtualKeyCode = VK_SUBTRACT then
	    // smaller buffer
	    BASS_SetConfig(BASS_CONFIG_BUFFER, BufLen - 1)
	  else 
	    // larger buffer
	    BASS_SetConfig(BASS_CONFIG_BUFFER, BufLen + 1);
	  BufLen := BASS_GetConfig(BASS_CONFIG_BUFFER);
	  Write(Format('using a %dms buffer'#9#9#13, [BufLen]));
	  Stream := BASS_StreamCreate(info.freq, 2, 0, @WriteStream, NIL);
	  // set effects on the new stream
	  for I := 0 to 8 do
	    if fx[I] > 0 then
	      fx[I] := BASS_ChannelSetFX(Stream, BASS_FX_DX8_CHORUS + I, 0);
	  BASS_ChannelPlay(Stream, False);
	end;
      
	VK_F1..VK_F9 :
	begin
	  I := KeyIn.Event.KeyEvent.wVirtualKeyCode - VK_F1;
	  if fx[I] > 0 then
	  begin
	    BASS_ChannelRemoveFX(Stream, fx[I]);
	    fx[I] := 0;
	    Write(Format('effect %s = OFF'#9#9#13, [fxname[I]]));
	  end
	  else
	  begin
	    // set the effect, not bothering with parameters (use defaults)
	    J := BASS_ChannelSetFX(Stream, BASS_FX_DX8_CHORUS + I, 0);
	    if J > 0 then
	    begin
	      fx[I] := J;
	      Write(Format('effect %s = ON'#9#9#13, [fxname[I]]));
	    end;  
	  end;
	end;
      end;
    end;

    for bKey := 0 to KEYS - 1 do
      if KeyIn.Event.KeyEvent.wVirtualKeyCode = aKey[bKey] then
      begin
	if KeyIn.Event.KeyEvent.bKeyDown and (aVol[bKey] <> MAXVOL) then
	begin
	  aPos[bKey] := 0;
	  aVol[bKey] := MAXVOL;	// start key
	end
	else if not KeyIn.Event.KeyEvent.bKeyDown and (aVol[bKey] > 0) then
	  dec(aVol[bKey]);	// trigger key fadeout
//	Break;
      end;
  end;
  BASS_Free;
end.
