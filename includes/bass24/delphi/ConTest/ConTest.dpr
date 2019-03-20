program ConTest;

{$APPTYPE CONSOLE}

uses
  Windows,
  SysUtils,
  MMSystem,
  Bass in '..\Bass.pas';

function IntToFixed(val, digits: Integer): string;
var
  s: string;
begin
  s := IntToStr(val);
  while Length(s) < digits do s := '0' + s;
  Result := s;
end;

// display error messages
procedure Error(text: string);
begin
  WriteLn('Error(' + IntToStr(BASS_ErrorGetCode) + '): ' + text);
  BASS_Free;
  Halt(0);
end;

var
  chn: DWORD;
  ismod: Boolean;
  time, pos, level, act: DWORD;
  a: Integer;
  AValue: Single;
begin
  WriteLn('Simple console mode BASS example : MOD/MPx/OGG/WAV player');
  Writeln('---------------------------------------------------------');

	// check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion) <> BASSVERSION) then
	begin
		Writeln('An incorrect version of BASS.DLL was loaded');
		Exit;
	end;
  if (ParamCount <> 1) then
  begin
    WriteLn(#9 + 'usage: contest <file>');
    Exit;
  end;

  // setup output - default device, 44100hz, stereo, 16 bits
  if not BASS_Init(-1, 44100, 0, 0, nil) then
    Error('Can''t initialize device');

  // try streaming the file
  chn := BASS_StreamCreateFile(FALSE, PChar(ParamStr(1)), 0, 0, BASS_SAMPLE_LOOP {$IFDEF UNICODE} or BASS_UNICODE {$ENDIF});
  if (chn = 0) then
    chn := BASS_StreamCreateURL(PAnsiChar(AnsiString(ParamStr(1))), 0, BASS_SAMPLE_LOOP, nil, nil);
  if (chn <> 0) then
  begin
    pos := BASS_ChannelGetLength(chn, BASS_POS_BYTE);
    if (BASS_StreamGetFilePosition(chn, BASS_FILEPOS_DOWNLOAD) <> QW_ERROR) then
    begin
      // streaming from the internet
      if (pos <> QW_ERROR) then
        Write('streaming internet file [' + IntToStr(pos) + ' bytes]')
      else
        Write('streaming internet file');
    end
    else
      Write('streaming file [' + IntToStr(pos) + ' bytes]');
    ismod := False;
  end
  else
  begin
    // load the MOD (with looping and sensitive ramping)
    chn := BASS_MusicLoad(FALSE, PChar(ParamStr(1)), 0, 0, BASS_MUSIC_LOOP or BASS_MUSIC_RAMPS or BASS_MUSIC_PRESCAN {$IFDEF UNICODE} or BASS_UNICODE {$ENDIF}, 1);
    if (chn = 0) then
      // not a MOD either
      Error('Can''t play the file');
    // count channels
    a := 0;
    while BASS_ChannelGetAttribute(chn, BASS_ATTRIB_MUSIC_VOL_CHAN + a, AValue) do
      a := a + 1;
    Write('playing MOD music "' + BASS_ChannelGetTags(chn, BASS_TAG_MUSIC_NAME) + '" [' + IntToStr(a) + ' chans, ' + IntToStr(BASS_ChannelGetLength(chn, BASS_POS_MUSIC_ORDER)) + ' orders]');
    pos := BASS_ChannelGetLength(chn, BASS_POS_BYTE);
    ismod := True;
  end;

	// display the time length
	if (pos > 0) then
  begin
		time := Trunc(BASS_ChannelBytes2Seconds(chn,pos));
		WriteLn(Format(' %d:%.2d', [time div 60, time mod 60]));
  end
  else // no time length available
		WriteLn('');

  BASS_ChannelPlay(chn, False);

  act := BASS_ChannelIsActive(chn);
  while (*not KeyPressed and*) (act > 0) do
  begin
    // display some stuff and wait a bit
		level := BASS_ChannelGetLevel(chn);
		pos := BASS_ChannelGetPosition(chn, BASS_POS_BYTE);
		time := Trunc(BASS_ChannelBytes2Seconds(chn,pos));
		Write(Format('pos %.9d', [pos]));
		if (ismod) then
    begin
			pos := BASS_ChannelGetPosition(chn, BASS_POS_MUSIC_ORDER);
			Write(Format(' (%.3d:%.3d)',[LOWORD(pos),HIWORD(pos)]));
		end;
		Write(Format(' - %d:%.2d - L ', [time div 60,time mod 60]));

		if (act=BASS_ACTIVE_STALLED) then // playback has stalled
    begin
			Write(Format('-- buffering : %.5d --',
				[BASS_StreamGetFilePosition(chn,BASS_FILEPOS_DOWNLOAD)-BASS_StreamGetFilePosition(chn,BASS_FILEPOS_DECODE)]));
		end
    else
    begin
      a := 27204;
      while (a > 200) do
      begin
        if (LOWORD(level) >= a) then
          Write('*')
        else
          Write('-');
        a := a * 2 div 3;
      end;
      Write(' ');
      a := 210;
      while (a < 32768) do
      begin
        if (HIWORD(level) >= a) then
          Write('*')
        else
          Write('-');
        a := a * 3 div 2;
      end;
		end;
		Write(Format(' R - cpu %.2f%%' + #13, [BASS_GetCPU]));
		Sleep(50);

    // Needs to update act to the current Active
    // status for the look to react accordingly
    act := BASS_ChannelIsActive(chn);
  end;
  WriteLn('                                                                   ');

  // wind the frequency down...
  BASS_ChannelSlideAttribute(chn, BASS_ATTRIB_FREQ, 1000, 500);
  Sleep(300);
  // ...and fade-out to avoid a "click"
  BASS_ChannelSlideAttribute(chn, BASS_ATTRIB_VOL, 0, 200);
  while BASS_ChannelIsSliding(chn, BASS_ATTRIB_VOL) do
    Sleep(1);

  BASS_Free();
end.
