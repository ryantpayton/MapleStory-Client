unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Bass;

type
  TForm1 = class(TForm)
    GroupBox1: TGroupBox;
    Button1: TButton;
    GroupBox2: TGroupBox;
    Button2: TButton;
    GroupBox3: TGroupBox;
    Button3: TButton;
    GroupBox4: TGroupBox;
    Button4: TButton;
    OpenDialog1: TOpenDialog;
    Button5: TButton;
    Button6: TButton;
    Button7: TButton;
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button5Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

  flags: array[0..3] of DWORD = (BASS_SPEAKER_FRONT, BASS_SPEAKER_REAR, BASS_SPEAKER_CENLFE, BASS_SPEAKER_REAR2);
  chan: array[0..3] of HSTREAM;

implementation

{$R *.dfm}

procedure TForm1.FormCreate(Sender: TObject);
var
  i: BASS_INFO;
begin
	// check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion) <> BASSVERSION) then
	begin
		MessageBox(0,'An incorrect version of BASS.DLL was loaded',0,MB_ICONERROR);
		Halt;
	end;

  // initialize BASS - default device
  if (not BASS_Init(-1, 44100, 0, Handle, nil)) then
  begin
    MessageBox(0, 'Can''t initialize device', 0, 0);
    Halt;
  end;

  // check how many speakers the device supports
  BASS_GetInfo(i);
  if (i.speakers < 4) then // no extra speakers detected, enable them anyway?
  begin
    if (MessageBox(0, 'Do you wish to enable "speaker assignment" anyway?', 'No extra speakers detected', MB_ICONQUESTION or MB_YESNO) = IDYES) then
    begin
      // reinitialize BASS - forcing speaker assignment
      BASS_Free;
      if (not BASS_Init(-1, 44100, BASS_DEVICE_SPEAKERS, Handle, nil)) then
      begin
        MessageBox(0, 'Can''t initialize device', 0, 0);
        Halt;
      end;
      BASS_GetInfo(i); // get info again
    end;
  end;

  if (i.speakers < 8) then
  begin
    Button4.Enabled := False;
    Button7.Enabled := False;
  end;
  if (i.speakers < 6) then
  begin
    Button3.Enabled := False;
    Button6.Enabled := False;
  end;
  if (i.speakers < 4) then
  begin
    Button2.Enabled := False;
    Button5.Enabled := False;
  end;
end;

procedure TForm1.FormDestroy(Sender: TObject);
begin
  BASS_Free;
end;

procedure TForm1.Button1Click(Sender: TObject);
var
  speaker: Integer;
begin
  speaker := TButton(Sender).Tag;
  if (not OpenDialog1.Execute) then
    Exit;

  BASS_StreamFree(chan[speaker]); // free old stream before opening new
  chan[speaker] := BASS_StreamCreateFile(False, PChar(OpenDialog1.FileName), 0, 0, flags[speaker] or BASS_SAMPLE_LOOP {$IFDEF UNICODE} or BASS_UNICODE {$ENDIF});
  if (chan[speaker] = 0) then
  begin
    TButton(Sender).Caption := 'click here to open a file...';
    MessageBox(0, 'Can''t play the file', 0, 0);
    Exit;
  end;

  TButton(Sender).Caption := OpenDialog1.FileName;
  BASS_ChannelPlay(chan[speaker], False);
end;

procedure TForm1.Button5Click(Sender: TObject);
var
  i: BASS_CHANNELINFO;
  speaker: Integer;
  temp: HSTREAM;
  temp1, temp2: String;
begin
  speaker := TButton(Sender).Tag;

  // swap handles
  temp := chan[speaker];
  chan[speaker] := chan[speaker+1];
  chan[speaker+1] := temp;

  // swap text
  case speaker of
    0: // swap 1 and 2
    begin
      temp1 := Button1.Caption;
      temp2 := Button2.Caption;
      Button1.Caption := temp2;
      Button2.Caption := temp1;
    end;
    1: // swap 2 and 3
    begin
      temp1 := Button2.Caption;
      temp2 := Button3.Caption;
      Button2.Caption := temp2;
      Button3.Caption := temp1;
    end;
    2: // swap 3 and 4
    begin
      temp1 := Button3.Caption;
      temp2 := Button4.Caption;
      Button3.Caption := temp2;
      Button4.Caption := temp1;
    end;
  end;

  // update speaker flags
  BASS_ChannelFlags(chan[speaker],flags[speaker],BASS_SPEAKER_FRONT);
  BASS_ChannelFlags(chan[speaker+1],flags[speaker+1],BASS_SPEAKER_FRONT);
end;

end.
