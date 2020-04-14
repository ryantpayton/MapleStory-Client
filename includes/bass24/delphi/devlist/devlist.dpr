{
	BASS device list example
	Copyright (c) 2014 Un4seen Developments Ltd.

        C++ to Delphi with use API adapted by Evgeny Melnikov
        Required Delphi 7 or above
}

program List;

{$APPTYPE CONSOLE}

uses
  Bass in '..\Bass.pas';

procedure DisplayDeviceInfo(di : BASS_DEVICEINFO);
var
  St : String;
begin
  Write(di.name, #13#10#9, 'driver: ', di.driver, #13#10#9'type: ');
  case (di.flags and BASS_DEVICE_TYPE_MASK) of
    BASS_DEVICE_TYPE_NETWORK :
      St := 'Remote Network';

    BASS_DEVICE_TYPE_SPEAKERS :
      St := 'Speakers';

    BASS_DEVICE_TYPE_LINE :
      St := 'Line';

    BASS_DEVICE_TYPE_HEADPHONES :
      St := 'Headphones';

    BASS_DEVICE_TYPE_MICROPHONE : 
      St := 'Microphone';

    BASS_DEVICE_TYPE_HEADSET :
      St := 'Headset';

    BASS_DEVICE_TYPE_HANDSET :
      St := 'Handset';

    BASS_DEVICE_TYPE_DIGITAL :
      St := 'Digital';

    BASS_DEVICE_TYPE_SPDIF :
      St := 'SPDIF';

    BASS_DEVICE_TYPE_HDMI :
      St := 'HDMI';

    BASS_DEVICE_TYPE_DISPLAYPORT :
      St := 'DisplayPort';
  else
    St := 'Unknown';
  end;

  St := St + #13#10#9'flags:';
  if (di.flags and BASS_DEVICE_LOOPBACK) = BASS_DEVICE_LOOPBACK then
    St := St + ' loopback';
  if (di.flags and BASS_DEVICE_ENABLED) = BASS_DEVICE_ENABLED then
    St := St + ' enabled';
  if (di.flags and BASS_DEVICE_DEFAULT) = BASS_DEVICE_DEFAULT then
    St := St + ' default';
  WriteLn(St, ' (', di.flags, ')');
end;

procedure Main;
var
  di : BASS_DEVICEINFO;
  i  : Integer;
begin
  WriteLn('Output Devices');
  i := 1;
  while BASS_GetDeviceInfo(i, di) do
  begin
    Write(i, ': ');
    DisplayDeviceInfo(di);
    inc(i);
  end;

  WriteLn(#13#10'Input Devices');
  
  i := 0;
  while BASS_RecordGetDeviceInfo(i, di) do
  begin
    Write(i, ': ');
    DisplayDeviceInfo(di);
    inc(i);
  end;
end;

begin
  Main;
end.
