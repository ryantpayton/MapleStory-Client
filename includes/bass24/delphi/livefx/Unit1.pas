// BASS Full-duplex test

// C++ to Delphi Translation by Chris Trösken
//Thanks to Ian Luck for some help
unit Unit1;

interface

uses
  Windows,Messages,Forms,bass, Controls, ExtCtrls, ComCtrls, StdCtrls,
  sysutils, Classes;

type
  TForm1 = class(TForm)
    ComboBox1: TComboBox;
    CheckBox1: TCheckBox;
    CheckBox2: TCheckBox;
    CheckBox3: TCheckBox;
    CheckBox4: TCheckBox;
    Label1: TLabel;
    TrackBar1: TTrackBar;
    p_latency: TPanel;
    procedure ComboBox1Change(Sender: TObject);
    procedure CheckBox1Click(Sender: TObject);
    procedure CheckBox2Click(Sender: TObject);
    procedure CheckBox3Click(Sender: TObject);
    procedure CheckBox4Click(Sender: TObject);
    procedure TrackBar1Change(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
    procedure WndProc(var Msg: TMessage); override;
  end;

var
  Form1: TForm1;
  rchan: HRECORD; // recording channel
  pchan: HStream;
  fx: array[1..4] of HFX;
  chunk: integer;
  latency: integer = 0; // Current latency
  win: hwnd;
  hTimer: DWORD;
implementation

{$R *.dfm}



procedure Error(es: string);
begin
  MessageBox(win, PChar(es + #13#10 + '(error code: ' + IntToStr(BASS_ErrorGetCode) +
    ')'), nil, 0);
end;

function RecordingCallback(Handle: HRECORD; buffer: Pointer; length: DWORD; user: Pointer): boolean; stdcall;
begin
  BASS_StreamPutData(pchan, buffer, length); // feed recorded data to output stream
  Result := True; // continue recording
end;


procedure TForm1.CheckBox1Click(Sender: TObject);
begin
  if CheckBox1.Checked = true then
    fx[1] := BASS_ChannelSetFX(pchan, BASS_FX_DX8_REVERB, 0)
  else
    BASS_ChannelRemoveFX(pchan, fx[1]);
end;

procedure TForm1.CheckBox2Click(Sender: TObject);
begin
  if CheckBox2.Checked = true then
    fx[2] := BASS_ChannelSetFX(pchan, BASS_FX_DX8_CHORUS, 0)
  else
    BASS_ChannelRemoveFX(pchan, fx[2]);
end;

procedure TForm1.CheckBox3Click(Sender: TObject);
begin
  if CheckBox3.Checked = true then
    fx[3] := BASS_ChannelSetFX(pchan, BASS_FX_DX8_FLANGER, 0)
  else
    BASS_ChannelRemoveFX(pchan, fx[3]);
end;

procedure TForm1.CheckBox4Click(Sender: TObject);
begin
  if CheckBox4.Checked = true then
    fx[4] := BASS_ChannelSetFX(pchan, BASS_FX_DX8_GARGLE, 0)
  else
    BASS_ChannelRemoveFX(pchan, fx[4]);
end;


procedure TForm1.FormCreate(Sender: TObject);
var
  bi: bass_Info;
  a: Integer;
  dName: PAnsiChar;
  r_Init: boolean;
  AVolume: Single;
  prebuf: DWORD;
begin
  win := handle;
  MessageBox(win, PChar('Do not set the input to "WAVE" or "What you hear"(etc...) with' +
  #13#10 + 'the level set high, as that is likely to result in nasty feedback.'),
   PChar('Feedback warning'), $00000030);

  if (HIWORD(BASS_GetVersion) <> BASSVERSION) then begin
    MessageBox(0, 'An incorrect version of BASS.DLL was loaded', nil, MB_ICONERROR);
    Halt;
  end;

  // setup output - get device latency
  if not BASS_Init(-1, 44100, BASS_DEVICE_LATENCY, win, nil) then begin
    Error('Can''t initialize device');
    Exit;
  end;

  BASS_GetInfo(bi);
    with checkbox1,checkbox2,checkbox3,checkbox4 do
      enabled := bi.dsver >= 8;
  pchan := BASS_StreamCreate(44100, 2, 0, STREAMPROC_PUSH, nil); //Playing Channel

  // start recording with 10ms period

  r_init := BASS_RecordInit(-1);
  rchan := BASS_RecordStart(44100, 2, MakeLong(0, 10), @RecordingCallback, nil); // Recording Channel
  if (not R_Init) or (rchan = 0) then begin
    Bass_Free();
    Error('Can''t initialize recording device');
    Halt;
  end;

  // Populate the list
  a := 0;
  dName := BASS_RecordGetInputName(a);
  while dName <> nil do begin
    ComboBox1.Items.Add(StrPas(dName));
    // is this one currently "on"?
    if (BASS_RecordGetInput(a, AVolume) and BASS_INPUT_OFF) = 0 then
      ComboBox1.ItemIndex := a;
    Inc(a);
    dName := BASS_RecordGetInputName(a);
  end;
  ComboBox1Change(Self); // display info
  BASS_RecordGetInput(ComboBox1.ItemIndex, AVolume);
  TrackBar1.Position := Trunc(AVolume * 100); // set the level slider

  // prebuffer at least "minbuf" amount of data before starting playback
  prebuf := BASS_ChannelSeconds2Bytes(pchan, bi.minbuf / 1000);
  while (BASS_ChannelGetData(pchan, nil, BASS_DATA_AVAILABLE) < prebuf) do
    sleep(1);

  BASS_ChannelPlay(pchan, False); // Start Playing
  hTimer := SetTimer(win, 1, 250, nil); // Start the latency winapi timer
  // our winapi Timer, we starting the timer without a callback so that we must
  // catch the WM_TIMER Message

end;

procedure TForm1.ComboBox1Change(Sender: TObject);
var
  i: Integer;
  r: Boolean;
  AVolume: Single;
begin
  // enable the selected input
  r := True;
  i := 0;
  // first disable all inputs, then...
  while r do begin
    r := BASS_RecordSetInput(i, BASS_INPUT_OFF, -1);
    Inc(i);
  end;
  // ...enable the selected.
  BASS_RecordSetInput(ComboBox1.ItemIndex, BASS_INPUT_ON, -1);
  BASS_RecordGetInput(ComboBox1.ItemIndex, AVolume);
  TrackBar1.Position := Trunc(AVolume * 100);
end;

procedure TForm1.TrackBar1Change(Sender: TObject);
begin
  if not (BASS_RecordSetInput(ComboBox1.ItemIndex, 0,
    TrackBar1.Position / 100)) then
    BASS_RecordSetInput(-1, 0, TrackBar1.Position / 100);
end;

procedure TForm1.WndProc(var Msg: TMessage);
begin
  inherited;
  case Msg.Msg of
    WM_TIMER:
       begin
         // display current latency (input+output buffer level)
         latency := (latency * 3 + BASS_ChannelGetData(pchan, nil,
         BASS_DATA_AVAILABLE) + BASS_ChannelGetData(rchan, nil,
         BASS_DATA_AVAILABLE)) div 4;
         p_latency.Caption := FormatFloat('0',
             BASS_ChannelBytes2Seconds(pchan, latency) * 1000);
       end;
  end;
end;

procedure TForm1.FormDestroy(Sender: TObject);
begin
  if hTimer <> 0 then
    KillTimer(win, 1);
  BASS_RecordFree();
  BASS_Free();
end;



end.

