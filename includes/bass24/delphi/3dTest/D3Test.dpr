{
BASS 3D Test, copyright (c) 1999-2004 Ian Luck.
===============================================
Other source: DTMain.pas, DTMain.dfm, DTSelect.pas, DTSelect.dfm
Delphi version by Titus Miloi (titus.a.m@t-online.de)
}
program D3Test;

uses
  Windows,
  Forms,
  SysUtils,
  DTMain in 'DTMain.pas' {Form1},
  Bass in '..\Bass.pas';

var
  eaxon: Boolean;
begin
  // initialize application
  Application.Initialize;
  Application.Title := 'BASS - 3D Test';

	// check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion) <> BASSVERSION) then
	begin
		MessageBox(0,'An incorrect version of BASS.DLL was loaded',nil,MB_ICONERROR);
		Halt;
	end;

  Application.CreateForm(TForm1, Form1);
  Form1.Visible := True;

  // Initialize the default output device with 3D
  if not BASS_Init(-1, 44100, BASS_DEVICE_3D, Application.handle, nil) then
  begin
    MessageBox(0, 'Can''t initialize output device', nil, 0);
    Halt;
  end;

  // Use meters as distance unit, real world rolloff, real doppler effect
  BASS_Set3DFactors(1.0, 1.0, 1.0);
  // Turn EAX off (volume=0.0), if error then EAX is not supported
  eaxon := FALSE;
  if BASS_SetEAXParameters(-1, 0.0, -1.0, -1.0) then
    eaxon := TRUE;

  // create and start the main application form
  with Form1 do
  begin
    ComboBox1.Enabled := eaxon;
    ComboBox1.ItemIndex := 0;
  end;
  Application.Run;

  BASS_Free;
end.

