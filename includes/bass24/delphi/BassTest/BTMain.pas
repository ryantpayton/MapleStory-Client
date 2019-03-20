(*
 *	BassTest - Simple BASS Test for Delphi
 *)

unit BTMain;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Bass, StdCtrls, ExtCtrls, Buttons;

type
  TForm1 = class(TForm)
    GroupBox1: TGroupBox;
    GroupBox2: TGroupBox;
    GroupBox3: TGroupBox;
    Button1: TButton;
    Button2: TButton;
    Label1: TLabel;
    ListBox1: TListBox;
    ListBox2: TListBox;
    Button4: TButton;
    Button5: TButton;
    Button6: TButton;
    Button7: TButton;
    Button8: TButton;
    Button9: TButton;
    Button10: TButton;
    Button11: TButton;
    OpenDialog1: TOpenDialog;
    Timer1: TTimer;
    OpenDialog2: TOpenDialog;
    OpenDialog3: TOpenDialog;
    ListBox3: TListBox;
    Button3: TButton;
    Button12: TButton;
    Button13: TButton;
    Button14: TButton;
    Button15: TButton;
    procedure FormCreate(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure Button4Click(Sender: TObject);
    procedure Button5Click(Sender: TObject);
    procedure Button6Click(Sender: TObject);
    procedure Button7Click(Sender: TObject);
    procedure Button8Click(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button10Click(Sender: TObject);
    procedure Button11Click(Sender: TObject);
    procedure Button9Click(Sender: TObject);
    procedure Button15Click(Sender: TObject);
    procedure Button14Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure Button12Click(Sender: TObject);
    procedure Button13Click(Sender: TObject);
  private
    mods: array[0..128] of HMUSIC;
    modc: Integer;
    sams: array[0..128] of HSAMPLE;
    samc: Integer;
    strs: array[0..128] of HSTREAM;
    strc: Integer;
    procedure Error(msg: string);
  public
  end;

var
  Form1: TForm1;

implementation

{$R *.DFM}


procedure TForm1.Error(msg: string);
var
	s: string;
begin
	s := msg + #13#10 + '(Error code: ' + IntToStr(BASS_ErrorGetCode) + ')';
	MessageBox(Handle, PChar(s), nil, 0);
end;


procedure TForm1.FormCreate(Sender: TObject);
begin
	modc := 0;		// music module count
	samc := 0;		// sample count
	strc := 0;		// stream count

	// check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion) <> BASSVERSION) then
	begin
		MessageBox(0,'An incorrect version of BASS.DLL was loaded',nil,MB_ICONERROR);
		Halt;
	end;

	// Initialize audio - default device, 44100hz, stereo, 16 bits
	if not BASS_Init(-1, 44100, 0, Handle, nil) then
		Error('Error initializing audio!');
end;


procedure TForm1.FormClose(Sender: TObject; var Action: TCloseAction);
var
  a: Integer;
begin
    (*
     (It's not actually necessary to free the streams, musics and
     samples because they are automatically freed by BASS_Free.)
	*)

	// Free stream
  if strc > 0 then
    for a := 0 to strc - 1 do
    	BASS_StreamFree(strs[a]);

    // Free music
	if modc > 0 then
		for a := 0 to modc - 1 do
			BASS_MusicFree(mods[a]);

	// Free samples
	if samc > 0 then
		for a := 0 to samc - 1 do
			BASS_SampleFree(sams[a]);

	// Close BASS
	BASS_Free();
end;


procedure TForm1.Button4Click(Sender: TObject);
var
	i: Integer;
begin
	i := ListBox1.ItemIndex;
	// Play the music (continuing from current position)
	if i >= 0 then
		if not BASS_ChannelPlay(mods[i], False) then
			Error('Error playing music!');
end;


procedure TForm1.Button5Click(Sender: TObject);
var
	i: Integer;
begin
	i := ListBox1.ItemIndex;
	// Stop the music
	if i >= 0 then
		BASS_ChannelStop(mods[i]);
end;


procedure TForm1.Button6Click(Sender: TObject);
var
	i: Integer;
begin
	i := ListBox1.ItemIndex;
	// Play the music from the beginning
	if i >= 0 then
		BASS_ChannelPlay(mods[i], True);
end;


procedure TForm1.Button7Click(Sender: TObject);
var
	f: PChar;
begin
	if not OpenDialog1.Execute then Exit;
	f := PChar(OpenDialog1.FileName);
	mods[modc] := BASS_MusicLoad(False, f, 0, 0, BASS_MUSIC_RAMP {$IFDEF UNICODE} or BASS_UNICODE {$ENDIF}, 1);
	if mods[modc] <> 0 then
	begin
		ListBox1.Items.Add(OpenDialog1.FileName);
		Inc(modc);
	end
	else
		Error('Error loading music!');
end;


procedure TForm1.Button8Click(Sender: TObject);
var
	a, i: Integer;
begin
	i := ListBox1.ItemIndex;
	if i >= 0 then
    begin
		BASS_MusicFree(mods[i]);
		if i < modc then
			for a := i to modc - 1 do
				mods[a] := mods[a + 1];
		Dec(modc);
		ListBox1.Items.Delete(i);
	end;
end;


procedure TForm1.Timer1Timer(Sender: TObject);
begin
	// update the CPU usage % display
	Label1.Caption := 'CPU%  ' + FloatToStrF(BASS_GetCPU, ffFixed, 4, 2);
end;


procedure TForm1.Button1Click(Sender: TObject);
begin
	// Pause audio output
	BASS_Pause();
end;

procedure TForm1.Button2Click(Sender: TObject);
begin
	// Resume audio output
	BASS_Start();
end;


procedure TForm1.Button10Click(Sender: TObject);
var
  f: PChar;
begin
	if not OpenDialog3.Execute then Exit;
	f := PChar(OpenDialog3.FileName);
	sams[samc] := BASS_SampleLoad(FALSE, f, 0, 0, 3, BASS_SAMPLE_OVER_POS {$IFDEF UNICODE} or BASS_UNICODE {$ENDIF});
	if sams[samc] <> 0 then
	begin
		ListBox2.Items.Add(OpenDialog3.FileName);
		Inc(samc);
	end
    else
		Error('Error loading sample!');
end;


procedure TForm1.Button11Click(Sender: TObject);
var
	a, i: Integer;
begin
  i := ListBox2.ItemIndex;
	if i >= 0 then
    begin
		BASS_SampleFree(sams[i]);
		if i < samc then
			for a := i to samc - 1 do
				sams[a] := sams[a + 1];
		Dec(samc);
		ListBox2.Items.Delete(i);
	end;
end;

procedure TForm1.Button9Click(Sender: TObject);
var
	i: Integer;
  ch: HCHANNEL;
begin
	i := ListBox2.ItemIndex;
  // Play the sample at default rate, volume=50%, random pan position
	if i >= 0 then
  begin
    ch := BASS_SampleGetChannel(sams[i], False);
    BASS_ChannelSetAttribute(ch, BASS_ATTRIB_PAN, (Random(201) - 100) / 100);
    BASS_ChannelSetAttribute(ch, BASS_ATTRIB_VOL, 0.5);
		if not BASS_ChannelPlay(ch, False) then
			Error('Error playing sample!');
  end;
end;


procedure TForm1.Button15Click(Sender: TObject);
var
	f: PChar;
begin
	if not OpenDialog2.Execute then Exit;
	f := PChar(OpenDialog2.FileName);
	strs[strc] := BASS_StreamCreateFile(False, f, 0, 0, 0 {$IFDEF UNICODE} or BASS_UNICODE {$ENDIF});
	if strs[strc] <> 0 then
	begin
		ListBox3.Items.Add(OpenDialog2.FileName);
		Inc(strc);
	end
	else
		Error('Error creating stream!');
end;

procedure TForm1.Button14Click(Sender: TObject);
var
	a, i: Integer;
begin
	i := ListBox3.ItemIndex;
	if i >= 0 then
    begin
		BASS_StreamFree(strs[i]);
		if i < strc then
			for a := i to strc - 1 do
				strs[a] := strs[a + 1];
		Dec(strc);
		ListBox3.Items.Delete(i);
	end;
end;

procedure TForm1.Button3Click(Sender: TObject);
var
	i: Integer;
begin
	i := ListBox3.ItemIndex;
	// Play the stream (continuing from current position)
	if i >= 0 then
		if not BASS_ChannelPlay(strs[i], False) then
			Error('Error playing stream!');
end;

procedure TForm1.Button12Click(Sender: TObject);
var
	i: Integer;
begin
	i := ListBox3.ItemIndex;
	// Stop the stream
	if i >= 0 then
		BASS_ChannelStop(strs[i]);
end;

procedure TForm1.Button13Click(Sender: TObject);
var
	i: Integer;
begin
	i := ListBox3.ItemIndex;
	// Play the stream from the beginning
	if i >= 0 then
		BASS_ChannelPlay(strs[i], True);
end;

end.
