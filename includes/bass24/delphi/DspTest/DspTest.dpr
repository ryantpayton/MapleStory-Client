{
BASS simple DSP test, copyright (c) 2000-2004 Ian Luck.
=======================================================
Other source: DTMain.pas; DTMain.dcu
Delphi version by Titus Miloi (titus.a.m@t-online.de)
}
program DspTest;

uses
  Forms,
  DTMain in 'DTMain.pas' {Form1},
  Bass in '..\Bass.pas';

begin
  Application.Initialize;
  Application.Title := 'BASS simple DSP Test';
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
