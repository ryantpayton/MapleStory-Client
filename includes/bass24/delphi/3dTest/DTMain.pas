unit DTMain;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, BASS, Math, ComCtrls;

const

  XDIST = 70;
  YDIST = 65;
  XCENTER = 268;
  YCENTER = 92;
  DIAM = 10;
  TIMERPERIOD = 50; // timer period (ms)
  MAXDIST = 50; // maximum distance of the channels (m)
  SPEED = 12;

type
  PSource = ^TSource;
  TSource = record
    next: PSource;
    Channel: DWORD;
    pos, vel: BASS_3DVECTOR;
  end;

  TForm1 = class(TForm)
    GroupBox1: TGroupBox;
    ListBox1: TListBox;
    Button1: TButton;
    Button2: TButton;
    Button3: TButton;
    Button4: TButton;
    Bevel1: TBevel;
    StaticText1: TStaticText;
    GroupBox2: TGroupBox;
    ComboBox1: TComboBox;
    GroupBox3: TGroupBox;
    GroupBox4: TGroupBox;
    Timer1: TTimer;
    OpenDialog1: TOpenDialog;
    ScrollBar_Rolloff: TTrackBar;
    ScrollBar_Doppler: TTrackBar;
    ed_X: TEdit;
    ed_Z: TEdit;
    Label1: TLabel;
    Label2: TLabel;
    btnReset: TButton;
    Bevel2: TBevel;
    Bevel3: TBevel;
    procedure FormCreate(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure ListBox1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure ListBox1KeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure Button3Click(Sender: TObject);
    procedure Button4Click(Sender: TObject);
    procedure ComboBox1Change(Sender: TObject);
    procedure ScrollBar_DopplerChange(Sender: TObject);
    procedure ScrollBar_RollofChange(Sender: TObject);
    procedure btnResetClick(Sender: TObject);
    procedure ed_XKeyPress(Sender: TObject; var Key: Char);
    procedure ed_XChange(Sender: TObject);
    procedure FormPaint(Sender: TObject);
  private
    { Private-Deklarationen }
    sources: PSource;
    procedure Error(msg: string);
    procedure AddSource(name: string);
    procedure RemSource(num: Integer);
    function GetSource(num: Integer): PSource;
    procedure DrawSources;
    procedure FreeSources;
    procedure ActualizeSources(forceupdate: Boolean);
    procedure ActualizeButtons;

  public
    { Public-Deklarationen }
  end;

var
  Form1: TForm1;

implementation

{$R *.DFM}

procedure TForm1.Error(msg: string);
var
  s: string;
begin
  s := msg + #13#10 + '(error code: ' + IntToStr(BASS_ErrorGetCode) + ')';
  MessageBox(handle, PChar(s), nil, 0);
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
  sources := nil;
end;

procedure TForm1.AddSource(name: string);
var
  NewCHan: DWORD;
  p, last: PSource;
begin

  // Load a music from "file" with 3D enabled, and make it loop & use ramping
  newchan := BASS_MusicLoad(FALSE, PChar(name), 0, 0, BASS_MUSIC_RAMP or BASS_MUSIC_LOOP or
    BASS_SAMPLE_3D {$IFDEF UNICODE} or BASS_UNICODE {$ENDIF}, 1);
  if (newchan = 0) then
  begin
    // Load a sample from "file" with 3D enabled, and make it loop
    newchan := BASS_SampleLoad(FALSE, PChar(name), 0, 0, 1, BASS_SAMPLE_LOOP or BASS_SAMPLE_3D or
      BASS_SAMPLE_MONO {$IFDEF UNICODE} or BASS_UNICODE {$ENDIF});
  end;
  if (newchan = 0) then
  begin
    Error('Can''t load file (note samples must be mono)');
    Exit;
  end;

  New(p);
  FillChar(p^, SizeOf(P^), 0);
  p.Channel := newchan;
  BASS_SampleGetChannel(newchan, False); // initialize sample channel
  last := sources;
  if last <> nil then
    while (last.next <> nil) do
      last := last.next;
  if last = nil then
    sources := p
  else
    last.next := p;
  ListBox1.Items.Add(name);
  ActualizeButtons;
end;

procedure TForm1.RemSource(num: Integer);
var
  p, prev: PSource;
  i: Integer;
begin
  prev := nil;
  p := sources;
  i := 0;
  while (p <> nil) and (i < num) do
  begin
    Inc(i);
    prev := p;
    p := p.next;
  end;
  if (p <> nil) then
  begin
    if (prev <> nil) then
      prev.next := p.next
    else
      sources := p.next;
    BASS_SampleFree(p.channel);
    BASS_MusicFree(p.channel);
    Dispose(p);
  end;
  ListBox1.Items.Delete(num);
  ActualizeButtons;
end;

function TForm1.GetSource(num: Integer): PSource;
var
  p: PSource;
  i: Integer;
begin
  if num < 0 then
  begin
    Result := nil;
    Exit;
  end;
  p := sources;
  i := 0;
  while (p <> nil) and (i < num) do
  begin
    Inc(i);
    p := p.next;
  end;
  Result := p;
end;

procedure TForm1.DrawSources;
var
  p: PSource;
  i, j: Integer;
begin
  p := sources;
  with Canvas do
  begin
    Brush.Color := Form1.Color;
    Pen.Color := Form1.Color;
    Rectangle(XCENTER - XDIST - DIAM,
      YCENTER - YDIST - DIAM,
      XCENTER + XDIST + DIAM,
      YCENTER + YDIST + DIAM);
    Brush.Color := clGray;
    Pen.Color := clBlack;
    Ellipse(XCENTER - DIAM div 2,
      YCENTER - DIAM div 2,
      XCENTER + DIAM div 2,
      YCENTER + DIAM div 2);
    Pen.Color := Form1.Color;
    i := 0;
    j := ListBox1.ItemIndex;
    while (p <> nil) do
    begin
      if (i = j) then
        Brush.Color := clRed
      else
        Brush.Color := clBlack;

      Ellipse(XCENTER + Trunc(p.pos.x)+ DIAM div 2,
        YCENTER - Trunc(p.pos.z) + DIAM div 2,
        XCENTER + Trunc(p.pos.x)- DIAM div 2,
        YCENTER - Trunc(p.pos.z)- DIAM div 2);
      p := p.next;
      Inc(i);
    end;
  end;
end;

procedure TForm1.ActualizeSources(forceupdate: Boolean);
var
  p: PSource;
  chng, fchng: Boolean;
  tmp: BASS_3DVECTOR;
begin
  fchng := forceupdate;
  p := sources;
  while (p <> nil) do
  begin
    chng := forceupdate;
    if (BASS_ChannelIsActive(p.channel) = BASS_ACTIVE_PLAYING) then
    begin

      if (P.pos.z >= MAXDIST) or (P.pos.z <= -MAXDIST) then
        P.vel.z := -P.vel.z;
      if (p.pos.X >= MAXDIST) or (p.pos.X <= -MAXDIST) then
        P.vel.X := -P.vel.X;

      // Update channel position
      P.pos.z := P.pos.z + P.vel.z * TIMERPERIOD / 1000;
      P.pos.X := P.pos.X + P.vel.X * TIMERPERIOD / 1000;
      ZeroMemory(@tmp, sizeof(tmp));
      BASS_ChannelSet3DPosition(P.channel, P.pos, tmp, P.vel);
      chng := true;
    end;
    p := p.next;
    if chng then
      fchng := TRUE;
  end;
  if fchng then
  begin
    DrawSources;
    BASS_Apply3D;
  end;
end;

procedure TForm1.FreeSources;
var
  p, v: PSource;
begin
  p := sources;
  while (p <> nil) do
  begin
    v := p.next;
    Dispose(v);
    p := v;
  end;
  sources := nil;
end;

procedure TForm1.Timer1Timer(Sender: TObject);
begin
  ActualizeSources(false);

end;

procedure TForm1.Button1Click(Sender: TObject);
begin
  if OpenDialog1.Execute then
  begin
    AddSource(OpenDialog1.FileName);
  end;
end;

procedure TForm1.FormDestroy(Sender: TObject);
begin
  FreeSources;
  BASS_Stop;
  BASS_Free;
end;

procedure TForm1.ActualizeButtons;
var
  en: Boolean;
begin
  en := (ListBox1.ItemIndex >= 0);
  Button2.Enabled := en;
  Button3.Enabled := en;
  Button4.Enabled := en;
  ed_x.Enabled := en;
  ed_z.Enabled := en;
  btnReset.Enabled := en;
  if en then
   DrawSources;
end;

procedure TForm1.ListBox1Click(Sender: TObject);
begin
  ActualizeButtons;
end;

procedure TForm1.Button2Click(Sender: TObject);
begin
  if ListBox1.ItemIndex >= 0 then
    RemSource(ListBox1.ItemIndex);
end;

procedure TForm1.ListBox1KeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  ActualizeButtons;
end;

procedure TForm1.Button3Click(Sender: TObject);
var
  p: PSource;
begin
  if ListBox1.ItemIndex < 0 then
    Exit;
  p := GetSource(ListBox1.itemIndex);
  if p <> nil then
    BASS_ChannelPlay(p.channel, False);
end;

procedure TForm1.Button4Click(Sender: TObject);
var
  p: PSource;
begin
  if ListBox1.ItemIndex < 0 then
    Exit;
  p := GetSource(ListBox1.ItemIndex);
  if p = nil then
    Exit;
  BASS_ChannelPause(p.channel);
end;

procedure TForm1.ComboBox1Change(Sender: TObject);
begin
  case (ComboBox1.ItemIndex) of
    0: BASS_SetEAXParameters (-1, 0, -1, -1);
    1: BASS_SetEAXPreset(EAX_ENVIRONMENT_GENERIC);
    2: BASS_SetEAXPreset(EAX_ENVIRONMENT_PADDEDCELL);
    3: BASS_SetEAXPreset(EAX_ENVIRONMENT_ROOM);
    4: BASS_SetEAXPreset(EAX_ENVIRONMENT_BATHROOM);
    5: BASS_SetEAXPreset(EAX_ENVIRONMENT_LIVINGROOM);
    6: BASS_SetEAXPreset(EAX_ENVIRONMENT_STONEROOM);
    7: BASS_SetEAXPreset(EAX_ENVIRONMENT_AUDITORIUM);
    8: BASS_SetEAXPreset(EAX_ENVIRONMENT_CONCERTHALL);
    9: BASS_SetEAXPreset(EAX_ENVIRONMENT_CAVE);
    10: BASS_SetEAXPreset(EAX_ENVIRONMENT_ARENA);
    11: BASS_SetEAXPreset(EAX_ENVIRONMENT_HANGAR);
    12: BASS_SetEAXPreset(EAX_ENVIRONMENT_CARPETEDHALLWAY);
    13: BASS_SetEAXPreset(EAX_ENVIRONMENT_HALLWAY);
    14: BASS_SetEAXPreset(EAX_ENVIRONMENT_STONECORRIDOR);
    15: BASS_SetEAXPreset(EAX_ENVIRONMENT_ALLEY);
    16: BASS_SetEAXPreset(EAX_ENVIRONMENT_FOREST);
    17: BASS_SetEAXPreset(EAX_ENVIRONMENT_CITY);
    18: BASS_SetEAXPreset(EAX_ENVIRONMENT_MOUNTAINS);
    19: BASS_SetEAXPreset(EAX_ENVIRONMENT_QUARRY);
    20: BASS_SetEAXPreset(EAX_ENVIRONMENT_PLAIN);
    21: BASS_SetEAXPreset(EAX_ENVIRONMENT_PARKINGLOT);
    22: BASS_SetEAXPreset(EAX_ENVIRONMENT_SEWERPIPE);
    23: BASS_SetEAXPreset(EAX_ENVIRONMENT_UNDERWATER);
    24: BASS_SetEAXPreset(EAX_ENVIRONMENT_DRUGGED);
    25: BASS_SetEAXPreset(EAX_ENVIRONMENT_DIZZY);
    26: BASS_SetEAXPreset(EAX_ENVIRONMENT_PSYCHOTIC);
  end;
end;

procedure TForm1.ScrollBar_DopplerChange(Sender: TObject);
var
  a: integer;
begin
  a := ScrollBar_Doppler.Position;
  BASS_Set3DFactors(-1, -1, Power(2, a - 10 / 5.0));
end;

procedure TForm1.ScrollBar_RollofChange(Sender: TObject);
var
  a: integer;
begin
  a:= ScrollBar_Rolloff.Position;
  BASS_Set3DFactors(-1, Power(2, (a - 10) / 5.0), -1);
end;

procedure TForm1.btnResetClick(Sender: TObject);
var
  p: PSource;
  tmp : BASS_3DVECTOR;
begin
  ed_x.Text := '0';
  ed_z.Text := '0';
  if ListBox1.ItemIndex < 0 then
    Exit;
  p := GetSource(ListBox1.ItemIndex);
  if p <> nil then
  begin
    ZeroMemory(@tmp, sizeof(tmp));
    p.pos := tmp;
    p.vel := tmp;
    ActualizeSources(TRUE);
  end;
end;

procedure TForm1.ed_XKeyPress(Sender: TObject; var Key: Char);
begin
  if Key in [Char(VK_BACK)] then
    Key := '0'
  else if not (Key in ['0'..'9']) then
    Key := #0;
    //Only Numbers and Back key allowed
end;

procedure TForm1.ed_XChange(Sender: TObject);
var
  p: PSource;
  v_x, v_z: integer;
begin
  if ListBox1.ItemIndex < 0 then
    Exit;
  p := GetSource(ListBox1.ItemIndex);
  if p <> nil then
  begin
    v_x := strtointdef(ed_x.Text, 0);
    v_z := strtointdef(ed_z.Text, 0);
    // X velocity

    if abs(round(P.vel.x))<> v_x  then
      P.vel.x := v_x;
    // Z velocity
    if abs(round(P.vel.z)) <> v_z  then
      P.vel.z := v_z;
    ActualizeSources(TRUE);
  end;
end;

procedure TForm1.FormPaint(Sender: TObject);
begin
DrawSources;
end;

end.

