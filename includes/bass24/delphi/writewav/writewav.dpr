program writewav;

uses
  Forms,
  UnitMain in 'UnitMain.pas' {Form1},
  Bass in '..\Bass.pas';

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
