unit DTMain;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, BASS;

type
  TForm1 = class(TForm)
    Button1: TButton;
    CheckBox1: TCheckBox;
    CheckBox2: TCheckBox;
    CheckBox3: TCheckBox;
    OpenDialog1: TOpenDialog;
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure CheckBox1Click(Sender: TObject);
    procedure CheckBox2Click(Sender: TObject);
    procedure CheckBox3Click(Sender: TObject);
  private
    { Private-Deklarationen }
  public
    { Public-Deklarationen }
  end;

const
  ECHBUFLEN = 1200;  // buffer length
  FLABUFLEN = 350;   // buffer length

var
  Form1: TForm1;

  floatable: DWORD; // floating-point channel support?
  chan: DWORD;     // the channel... HMUSIC or HSTREAM

  rotdsp: HDSP = 0;  // DSP handle
  rotpos: Single;    // cur.pos

  echdsp: HDSP = 0;  // DSP handle
  echbuf: array[0..ECHBUFLEN - 1,0..1] of Single;  // buffer
  echpos: Integer;  // cur.pos

  fladsp: HDSP = 0;  // DSP handle
  flabuf: array[0..FLABUFLEN - 1,0..1] of Single;  // buffer
  flapos: Integer;  // cur.pos
  flas, flasinc: Single;  // sweep pos/increment

implementation

{$R *.DFM}

function fmod(a, b: Single): Single;
begin
  Result := a - (b * Trunc(a / b));
end;

procedure Rotate(handle: HDSP; channel: DWORD; buffer: Pointer; length: DWORD; user: Pointer); stdcall;
var
  a: DWORD;
  d: PSingle;
begin
  d := buffer;

  a := 0;
  while (a < (length div 4)) do
  begin
    d^ := d^ * Abs(Sin(rotpos));
    Inc(d);
    d^ := d^ * Abs(Cos(rotpos));

    rotpos := fmod(rotpos + 0.00003, Pi);

    Inc(d);
    a := a + 2;
  end;
end;

procedure Echo(handle: HDSP; channel: DWORD; buffer: Pointer; length: DWORD; user: Pointer); stdcall;
var
  a: DWORD;
  d: PSingle;
  l, r: Single;
begin
	d := buffer;

  a := 0;
  while (a < (length div 4)) do
  begin
    l := d^ + (echbuf[echpos,1] / 2);
    Inc(d);
    r := d^ + (echbuf[echpos,0] / 2);
    Dec(d);

    { Basic "bathroom" reverb }
    d^ := l;
    echbuf[echpos,0] := l;
    Inc(d);
    d^ := r;
    echbuf[echpos,1] := r;

    { Echo }
//    echbuf[echpos,0] := d^;
//    d^ := l;
//    Inc(d);
//    echbuf[echpos,1] := d^;
//    d^ := r;

		echpos := echpos + 1;
		if (echpos = ECHBUFLEN) then
      echpos := 0;

    Inc(d);
    a := a + 2;
  end;
end;

procedure Flange(handle: HDSP; channel: DWORD; buffer: Pointer; length: DWORD; user: Pointer); stdcall;
var
	a: DWORD;
	d: PSingle;
  f, s: Single;
  p1, p2: Integer;
begin
	d := buffer;

  a := 0;
  while (a < (length div 4)) do
  begin
    p1 := Trunc(flapos + flas) mod FLABUFLEN;
    p2 := (p1 + 1) mod FLABUFLEN;
    f := fmod(flas, 1);

    s := d^ + ((flabuf[p1, 0] * (1 - f)) + (flabuf[p2, 0] * f));
    flabuf[flapos, 0] := d^;
    d^ := s;

    Inc(d);
    s := d^ + ((flabuf[p1, 1] * (1 - f)) + (flabuf[p2, 1] * f));
    flabuf[flapos, 1] := d^;
    d^ := s;

    flapos := flapos + 1;
    if (flapos = FLABUFLEN) then
      flapos := 0;

    flas := flas + flasinc;
    if (flas < 0.0) or (flas > FLABUFLEN) then
      flasinc := -flasinc;

    Inc(d);
    a := a + 2;
  end;
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
	// check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion) <> BASSVERSION) then
	begin
		MessageBox(0,'An incorrect version of BASS.DLL was loaded',0,MB_ICONERROR);
		Halt;
	end;

  BASS_SetConfig(BASS_CONFIG_FLOATDSP, 1);
  if not BASS_Init(-1, 44100, 0, Handle, nil) then
  begin
    MessageBox(0, 'Can''t initialize device', 0, 0);
    Halt;
  end;

  floatable := BASS_StreamCreate(44100, 2, BASS_SAMPLE_FLOAT, nil, 0);
  if (floatable > 0) then
  begin
    BASS_StreamFree(floatable); // woohoo!
    floatable := BASS_SAMPLE_FLOAT;
  end
end;

procedure TForm1.FormDestroy(Sender: TObject);
begin
  BASS_Free;
end;

procedure TForm1.Button1Click(Sender: TObject);
var
  info: BASS_CHANNELINFO;
begin
  if not OpenDialog1.Execute then
    Exit;

  // free both MOD and stream, it must be one of them! :)
  BASS_MusicFree(chan);
  BASS_StreamFree(chan);

  chan := BASS_StreamCreateFile(False, PChar(OpenDialog1.FileName), 0, 0, floatable or BASS_SAMPLE_LOOP {$IFDEF UNICODE} or BASS_UNICODE {$ENDIF});
  if (chan = 0) then
    chan := BASS_MusicLoad(False, PChar(OpenDialog1.FileName), 0, 0, BASS_MUSIC_LOOP or BASS_MUSIC_RAMPS or floatable {$IFDEF UNICODE} or BASS_UNICODE {$ENDIF}, 1);
  if (chan = 0) then
  begin
    // whatever it is, it ain't playable
    Button1.Caption := 'click here to open a file...';
    MessageBox(0, 'Can''t play the file', 0, 0);
    Exit;
  end;

  BASS_ChannelGetInfo(chan, info);
  if (info.chans <> 2) then // only stereo is allowed
  begin
    Button1.Caption := 'click here to open a file...';
    BASS_MusicFree(chan);
    BASS_StreamFree(chan);
    MessageBox(0, 'Only stereo sources are supported', 0, 0);
    Exit;
  end;

  Button1.Caption := OpenDialog1.FileName;
  // setup DSPs on new channel
  CheckBox1.OnClick(Self);
  CheckBox2.OnClick(Self);
  CheckBox3.OnClick(Self);

  // play both MOD and stream, it must be one of them!
  BASS_ChannelPlay(chan, False);
end;

procedure TForm1.CheckBox1Click(Sender: TObject);
begin
  if (CheckBox1.Checked) then
  begin
    rotpos := 0.7853981;
    rotdsp := BASS_ChannelSetDSP(chan, @Rotate, 0, 2);
  end
  else
    BASS_ChannelRemoveDSP(chan, rotdsp);
end;

procedure TForm1.CheckBox2Click(Sender: TObject);
begin
  if (CheckBox2.Checked) then
  begin
    FillChar(echbuf, SizeOf(echbuf), 0);
    echpos := 0;
    echdsp := BASS_ChannelSetDSP(chan, @Echo, 0, 1);
  end
  else
    BASS_ChannelRemoveDSP(chan, echdsp);
end;

procedure TForm1.CheckBox3Click(Sender: TObject);
begin
  if (CheckBox3.Checked) then
  begin
    FillChar(flabuf, SizeOf(flabuf), 0);
    flapos := 0;
    flas := FLABUFLEN / 2;
    flasinc := 0.002;
    fladsp := BASS_ChannelSetDSP(chan, @Flange, 0, 0);
  end
  else
    BASS_ChannelRemoveDSP(chan, fladsp);
end;

end.

