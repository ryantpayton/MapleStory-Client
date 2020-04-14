{
BASS simple stream test, copyright (c) Titus Miloi.
=====================================================
Other source: STMain.pas; STMain.dfm
}
program StreamTest;

uses
  Forms,
  STMain in 'STMain.pas' {Form1},
  Bass in '..\Bass.pas';

begin
  Application.Initialize;
  Application.Title := 'BASS - Stream Demo';
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
