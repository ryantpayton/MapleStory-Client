unit STMain;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, BASS;

type
  TForm1 = class(TForm)
    Button1: TButton;
    Button2: TButton;
    Button3: TButton;
    GroupBox1: TGroupBox;
    ScrollBar1: TScrollBar;
    Label1: TLabel;
    Label2: TLabel;
    ScrollBar2: TScrollBar;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    Button4: TButton;
    Label7: TLabel;
    Label8: TLabel;
    procedure Button4Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure ScrollBar1Change(Sender: TObject);
    procedure ScrollBar2Change(Sender: TObject);
  private
    { Private-Deklarationen }
    SineStream: HSTREAM;
    procedure Error(msg: string);
  public
    { Public-Deklarationen }
  end;

var
  Form1: TForm1;
  SineCount, Frequency, Amplitude: Real;

implementation

{$R *.DFM}

function MakeSine(handle: HSTREAM; buffer: Pointer; length: DWORD; user: Pointer): DWORD; stdcall;
var
  buf: ^WORD;
  i, len: Integer;
begin
  buf := buffer;
  len := length div 2;
  // write the sine function to the output stream
  for i := 0 to len - 1 do begin
    buf^ := Trunc(Sin(SineCount * PI) * Amplitude);
    Inc(buf);
    SineCount := SineCount + (Frequency / 44100);
  end;
  Result := length;
end;

procedure TForm1.Error(msg: string);
var
  s: string;
begin
  // add the error code to the output string
  s := msg + #13#10 + '(error code: ' + IntToStr(BASS_ErrorGetCode) + ')';
  MessageBox(handle, PChar(s), 0, 0);
end;

procedure TForm1.Button4Click(Sender: TObject);
begin
  Close;
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
  // enable the BASS Init button
  Button1.Enabled := TRUE;
end;

procedure TForm1.Button1Click(Sender: TObject);
begin
	// check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion) <> BASSVERSION) then
	begin
		MessageBox(0,'An incorrect version of BASS.DLL was loaded',0,MB_ICONERROR);
		Halt;
	end;
  // Initialize BASS with the default device
  if not BASS_Init(-1, 44100, 0, handle, nil) then begin
    Error('Could not initialize BASS');
    Exit;
  end;
  // if all successful, enable the create stream button
  Button1.Enabled := FALSE;
  Button2.Enabled := TRUE;
end;

procedure TForm1.Button2Click(Sender: TObject);
begin
  (*
    create a stream with a sample rate of 44100Hz
    the max. output rate is sample rate / 2
    i.e. we have a 22050Hz stream!
    however, we'll set the max. output frequency
    of the sine wave lower becouse the human
    ear isn't able to hear waves above 16KHz...
  *)
  SineStream := BASS_StreamCreate(44100, 2, 0, @MakeSine, 0);
  if (SineStream = 0) then begin
    Error('Could not create user stream');
    Exit;
  end;
  // if successfully called, enable the play stream button
  Button2.Enabled := FALSE;
  Button3.Enabled := TRUE;
end;

procedure TForm1.Button3Click(Sender: TObject);
begin
  // reset the sine counter
  SineCount := 0;
  // initialize the amplitude and the frequency
  Frequency := ScrollBar1.Position;
  Amplitude := ScrollBar2.Position;
  if not BASS_ChannelPlay(SineStream, False) then begin
    Error('Could not start stream playback');
    Exit;
  end;
  // enable the potentiometers
  Button3.Enabled := FALSE;
  GroupBox1.Enabled := TRUE;
end;

procedure TForm1.FormClose(Sender: TObject; var Action: TCloseAction);
begin
  // release it
  // the stream will be released automatically
  BASS_Free;
end;

procedure TForm1.ScrollBar1Change(Sender: TObject);
begin
  // update the output frequency
  Frequency := ScrollBar1.Position;
  Label7.Caption := IntToStr(ScrollBar1.Position) + 'Hz';
end;

procedure TForm1.ScrollBar2Change(Sender: TObject);
begin
  // update the output amplitude
  Amplitude := ScrollBar2.Position;
  Label8.Caption := IntToStr(ScrollBar2.Position * 100 div 32767) + '%';
end;


end.
