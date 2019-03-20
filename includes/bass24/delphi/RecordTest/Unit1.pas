(*
 *  BASS Recording example for Delphi
 *)

unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Bass, ExtCtrls, ComCtrls;

type
  WAVHDR = packed record
    riff:			array[0..3] of AnsiChar;
    len:			DWord;
    cWavFmt:		array[0..7] of AnsiChar;
    dwHdrLen:		DWord;
    wFormat:		Word;
    wNumChannels:	Word;
    dwSampleRate:	DWord;
    dwBytesPerSec:	DWord;
    wBlockAlign:	Word;
    wBitsPerSample:	Word;
    cData:			array[0..3] of AnsiChar;
    dwDataLen:		DWord;
  end;

type
  TForm1 = class(TForm)
    ComboBox1: TComboBox;
    bRecord: TButton;
    bPlay: TButton;
    Label1: TLabel;
    TrackBar1: TTrackBar;
    Bevel1: TBevel;
    bSave: TButton;
    lPos: TLabel;
    PosTimer: TTimer;
    SaveDialog: TSaveDialog;
    Label2 : TLabel;
    procedure FormCreate(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure FormDestroy(Sender: TObject);
    procedure bRecordClick(Sender: TObject);
    procedure bPlayClick(Sender: TObject);
    procedure ComboBox1Change(Sender: TObject);
	procedure StartRecording;
	procedure StopRecording;
    procedure PosTimerTimer(Sender: TObject);
	procedure UpdateInputInfo;
    procedure TrackBar1Change(Sender: TObject);
    procedure bSaveClick(Sender: TObject);
  private
    { Private declarations }
    WaveStream: TMemoryStream;
  public
    { Public declarations }
  end;


var
  Form1:   TForm1;
  WaveHdr: WAVHDR;  // WAV header
  rchan:   HRECORD;	// recording channel
  chan:    HSTREAM;	// playback channel


implementation

{$R *.dfm}


(* This is called while recording audio *)
function RecordingCallback(Handle: HRECORD; buffer: Pointer; length: DWORD; user: Pointer): boolean; stdcall;
begin
    // Copy new buffer contents to the memory buffer
	Form1.WaveStream.Write(buffer^, length);
    // Allow recording to continue
	Result := True;
end;


(* Initialize BASS, form controls, memory stream *)
procedure TForm1.FormCreate(Sender: TObject);
var
  i: Integer;
  dName: PAnsiChar;
  level: Single;
begin
	// check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion) <> BASSVERSION) then
	begin
		MessageBox(0,'An incorrect version of BASS.DLL was loaded', nil,MB_ICONERROR);
		Halt;
	end;
	if (not BASS_RecordInit(-1)) or (not BASS_Init(-1, 44100, 0, Handle, nil)) then
	begin
		BASS_RecordFree;
		BASS_Free();
		MessageDlg('Cannot start default recording device!', mtError, [mbOk], 0);
		Halt;
	end;
	WaveStream := TMemoryStream.Create;
	i := 0;
	dName := BASS_RecordGetInputName(i);
	while dName <> nil do
	begin
		ComboBox1.Items.Add(StrPas(dName));
		// is this one currently "on"?
		if (BASS_RecordGetInput(i, level) and BASS_INPUT_OFF) = 0 then
        	ComboBox1.ItemIndex := i;
		Inc(i);
		dName := BASS_RecordGetInputName(i);
	end;
    ComboBox1Change(Self);	// display info
end;


procedure TForm1.FormClose(Sender: TObject; var Action: TCloseAction);
begin
	Action := caFree;
end;


(* Application closing, release stuff *)
procedure TForm1.FormDestroy(Sender: TObject);
begin
	WaveStream.Free;
	BASS_RecordFree;
	BASS_Free;
	BASS_Stop;
end;


(* Start recording to memory *)
procedure TForm1.StartRecording;
begin
	if ComboBox1.ItemIndex < 0 then Exit;
	if WaveStream.Size > 0 then
    begin	// free old recording
		BASS_StreamFree(chan);
		WaveStream.Clear;
	end;
	// generate header for WAV file
	with WaveHdr do
    begin
		riff := 'RIFF';
		len := 36;
		cWavFmt := 'WAVEfmt ';
		dwHdrLen := 16;
		wFormat := 1;
		wNumChannels := 2;
		dwSampleRate := 44100;
		wBlockAlign := 4;
		dwBytesPerSec := 176400;
		wBitsPerSample := 16;
		cData := 'data';
		dwDataLen := 0;
    end;
	WaveStream.Write(WaveHdr, SizeOf(WAVHDR));
	// start recording @ 44100hz 16-bit stereo
	rchan := BASS_RecordStart(44100, 2, 0, @RecordingCallback, nil);
	if rchan = 0 then
	begin
		MessageDlg('Couldn''t start recording!', mtError, [mbOk], 0);
		WaveStream.Clear;
	end
    else
    begin
		bRecord.Caption := 'Stop';
		bPlay.Enabled := False;
		bSave.Enabled := False;
    end;
end;


(* Stop recording *)
procedure TForm1.StopRecording;
var
	i: integer;
begin
	BASS_ChannelStop(rchan);
	bRecord.Caption := 'Record';
	// complete the WAV header
	WaveStream.Position := 4;
	i := WaveStream.Size - 8;
	WaveStream.Write(i, 4);
	i := i - $24;
	WaveStream.Position := 40;
	WaveStream.Write(i, 4);
	WaveStream.Position := 0;
	// create a stream from the recorded data
	chan := BASS_StreamCreateFile(True, WaveStream.Memory, 0, WaveStream.Size, 0);
	if chan <> 0 then
    begin
		// enable "Play" & "Save" buttons
        bPlay.Enabled := True;
        bSave.Enabled := True;
	end
    else
		MessageDlg('Error creating stream from recorded data!', mtError, [mbOk], 0);
end;


(* Start/stop recording *)
procedure TForm1.bRecordClick(Sender: TObject);
begin
	if BASS_ChannelIsActive(rchan) <> 0
		then StopRecording
		else StartRecording;
end;


(* Play the recorded data *)
procedure TForm1.bPlayClick(Sender: TObject);
begin
	BASS_ChannelPlay(chan, True);
end;


(* Change recording input *)
procedure TForm1.ComboBox1Change(Sender: TObject);
var
	i: Integer;
    r: Boolean;
begin
	// enable the selected input
    r := True;
    i := 0;
    // first disable all inputs, then...
	while r do
    begin
		r := BASS_RecordSetInput(i, BASS_INPUT_OFF, -1);
        Inc(i);
	end;
    // ...enable the selected.
	BASS_RecordSetInput(ComboBox1.ItemIndex, BASS_INPUT_ON, -1);
	UpdateInputInfo; 	// update info
end;


procedure TForm1.UpdateInputInfo;
var
	i: DWord;
	level: Single;
begin
	I := BASS_RecordGetInput(ComboBox1.ItemIndex, level);
	TrackBar1.Position := Round(level * 100);	// set the level slider
  Label2.Caption := IntToStr(TrackBar1.Position);

	case (i and BASS_INPUT_TYPE_MASK) of
		BASS_INPUT_TYPE_DIGITAL: Label1.Caption := 'digital';
		BASS_INPUT_TYPE_LINE: Label1.Caption := 'line-in';
		BASS_INPUT_TYPE_MIC: Label1.Caption := 'microphone';
		BASS_INPUT_TYPE_SYNTH: Label1.Caption := 'midi synth';
		BASS_INPUT_TYPE_CD: Label1.Caption := 'analog cd';
		BASS_INPUT_TYPE_PHONE: Label1.Caption := 'telephone';
		BASS_INPUT_TYPE_SPEAKER: Label1.Caption := 'pc speaker';
		BASS_INPUT_TYPE_WAVE: Label1.Caption := 'wave/pcm';
		BASS_INPUT_TYPE_AUX: Label1.Caption := 'aux';
		BASS_INPUT_TYPE_ANALOG: Label1.Caption := 'analog';
	else
		Label1.Caption := 'undefined';
	end;
end;


(* Update rec/playback position display *)
procedure TForm1.PosTimerTimer(Sender: TObject);
begin
	if WaveStream.Size < 1 then Exit;
    if BASS_ChannelIsActive(chan) = BASS_ACTIVE_STOPPED then
    	lPos.Caption := IntToStr(WaveStream.Size) else
        lPos.Caption := IntToStr(BASS_ChannelGetPosition(chan, BASS_POS_BYTE)) + ' / ' + IntToStr(WaveStream.Size);
end;


(* Set recording volume *)
procedure TForm1.TrackBar1Change(Sender: TObject);
begin
	BASS_RecordSetInput(ComboBox1.ItemIndex, 0, TrackBar1.Position / 100);
  Label2.Caption := IntToStr(TrackBar1.Position);

end;


(* Save recorded audio to WAV file *)
procedure TForm1.bSaveClick(Sender: TObject);
begin
	if SaveDialog.Execute then
		WaveStream.SaveToFile(SaveDialog.FileName);
end;


end.

