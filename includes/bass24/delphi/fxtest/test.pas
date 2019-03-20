unit test;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ComCtrls, StdCtrls, Bass;

type
  TForm1 = class(TForm)
    Button1: TButton;
    TrackBar1: TTrackBar;
    TrackBar2: TTrackBar;
    TrackBar3: TTrackBar;
    TrackBar4: TTrackBar;
    OpenDialog1: TOpenDialog;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Label5: TLabel;
    Label4: TLabel;
    Label6: TLabel;
    procedure Button1Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure TrackBar1Change(Sender: TObject);
    procedure TrackBar2Change(Sender: TObject);
    procedure TrackBar3Change(Sender: TObject);
    procedure TrackBar4Change(Sender: TObject);
  private
    { Private declarations }
    chan: DWORD;
  public
    { Public declarations }
  end;

var
  Form1: TForm1;
  p: BASS_DX8_PARAMEQ;
  pR: BASS_DX8_REVERB;
  fx: array[1..4] of integer;

implementation

{$R *.dfm}

procedure TForm1.Button1Click(Sender: TObject);
var
  p1: BASS_CHANNELINFO;
begin
  If OpenDialog1.Execute then begin
    // free both MOD and stream, it must be one of them! :)
    BASS_MusicFree(chan);
    BASS_StreamFree(chan);
    chan := BASS_StreamCreateFile(FALSE, PChar(OpenDialog1.FileName), 0, 0, BASS_SAMPLE_FX or BASS_SAMPLE_LOOP {$IFDEF UNICODE} or BASS_UNICODE {$ENDIF});
    if (chan = 0) then
      chan := BASS_MusicLoad(FALSE, PChar(OpenDialog1.FileName), 0, 0, BASS_MUSIC_LOOP or BASS_MUSIC_RAMP or BASS_SAMPLE_FX {$IFDEF UNICODE} or BASS_UNICODE {$ENDIF},1);
    if (chan = 0) then
    begin
      // not a WAV/MP3 or MOD
      Button1.Caption := 'Can''t play the file! Please select another.';
      Exit;
    end;
    if (p1.flags and BASS_SAMPLE_8BITS) > 0 then
    begin
      // not 16-bit stereo
      Button1.Caption := '16-bit stereo sources only. Please select another.';
      BASS_MusicFree(chan);
      BASS_StreamFree(chan);
      Exit;
    end;
    Button1.Caption := OpenDialog1.FileName;
    fx[1] := BASS_ChannelSetFX(chan, BASS_FX_DX8_PARAMEQ, 1);
    fx[2] := BASS_ChannelSetFX(chan, BASS_FX_DX8_PARAMEQ, 1);
    fx[3] := BASS_ChannelSetFX(chan, BASS_FX_DX8_PARAMEQ, 1);
    fx[4] := BASS_ChannelSetFX(chan, BASS_FX_DX8_REVERB, 1);
    // Set equalizer to flat and reverb off to start
    p.fGain := 0;
    p.fBandwidth := 18;
    p.fCenter := 125;
    BASS_FXSetParameters(fx[1], @p);
    p.fCenter := 1000;
    BASS_FXSetParameters(fx[2], @p);
    p.fCenter := 8000;
    BASS_FXSetParameters(fx[3], @p);
    BASS_FXGetParameters(fx[4], @pR);
    pR.fReverbMix := -96;
    pR.fReverbTime := 1200;
    pR.fHighFreqRTRatio := 0.1;
    BASS_FXSetParameters(fx[4], @pR);
    // play both MOD and stream, it must be one of them! :)
    BASS_ChannelPlay(chan, False);
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
  // setup output - default device, 44100hz, stereo, 16 bits
  if not BASS_Init(-1, 44100, 0, handle, nil) then
  begin
    Application.MessageBox('Can''t initialize device','Bass Initialize problem');
    Halt;
  end
  else
    BASS_Start;
  // Adjust buffer size to balance 'clicks' vs. response time, slower computers need bigger
  BASS_SetConfig(BASS_CONFIG_BUFFER,1000);
end;

procedure TForm1.FormDestroy(Sender: TObject);
begin
  Bass_Stop();
  BASS_StreamFree(chan);
  BASS_MusicFree(chan);
  BASS_Free();
end;

procedure TForm1.TrackBar1Change(Sender: TObject);
begin
    BASS_FXGetParameters(fx[1], @p);
    p.fgain := 15-TrackBar1.position;
    BASS_FXSetParameters(fx[1], @p);
end;

procedure TForm1.TrackBar2Change(Sender: TObject);
begin
    BASS_FXGetParameters(fx[2], @p);
    p.fgain := 15-TrackBar2.position;
    BASS_FXSetParameters(fx[2], @p);
end;

procedure TForm1.TrackBar3Change(Sender: TObject);
begin
    BASS_FXGetParameters(fx[3], @p);
    p.fgain := 15-TrackBar3.position;
    BASS_FXSetParameters(fx[3], @p);
end;

procedure TForm1.TrackBar4Change(Sender: TObject);
var
    v: integer;
begin
    v := TrackBar4.position;
    BASS_FXGetParameters(fx[4], @pR);
    // give exponential quality to trackbar as Bass more sensitive near 0
    pR.fReverbMix := -0.012*v*v*v; // gives -96 when bar at 20
    BASS_FXSetParameters(fx[4], @pR);
    Label5.Caption := inttostr(20-TrackBar4.position);
end;

end.
