unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Math, StdCtrls, Bass, ExtCtrls;

type
  TForm1 = class(TForm)
    OpenDialog1: TOpenDialog;
    Timer1: TTimer;
    PB: TPaintBox;
    procedure FormCreate(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure FormMouseDown(Sender: TObject; Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
    procedure FormMouseMove(Sender: TObject; Shift: TShiftState; X, Y: Integer);
    procedure FormMouseUp(Sender: TObject; Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
    procedure PBPaint(Sender: TObject);
    procedure FormKeyDown(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure FormCloseQuery(Sender: TObject; var CanClose: Boolean);
  private
    function PlayFile: boolean;
    procedure ErrorPop(str: string);
    procedure SetLoopStart(position: qword);
    procedure SetLoopEnd(position: qword);
    procedure ScanPeaks2(decoder: HSTREAM);
    procedure DrawSpectrum;
    procedure DrawTime_Line(position: QWORD; y : integer; cl : TColor);
  public
  end;

type TScanThread = class(TThread)
  private
    Fdecoder : HSTREAM;
  protected
    procedure Execute; override;
  public
    constructor Create(decoder:HSTREAM);
end;

procedure LoopSyncProc(handle: HSYNC; channel, data: DWORD; user: Pointer); stdcall;

var
  Form1: TForm1;
  lsync : HSYNC;		// looping synchronizer handle
  chan : HSTREAM;   // sample stream handle
  chan2: HSTREAM;
  loop : array[0..1] of DWORD;
  killscan : boolean;
  bpp : dword; // stream bytes per pixel
  wavebufL : array of smallint;
  wavebufR : array of smallint;
  mousedwn : integer;
  Buffer: TBitmap;

implementation

{$R *.dfm}

//------------------------------------------------------------------------------

procedure TForm1.FormCreate(Sender: TObject);
begin
	// check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion) <> BASSVERSION) then
	begin
		MessageBox(0,'An incorrect version of BASS.DLL was loaded', nil, MB_ICONERROR);
		Halt;
	end;

  //assigning layout properties
  ClientHeight := 201;
  ClientWidth := 600;
  Top := 100;
  Left := 100;
  Buffer := TBitmap.Create;
  Buffer.Width:= PB.Width;
  Buffer.Height:= PB.Height;
  PB.Parent.DoubleBuffered := true;

  //set array size
  setlength(wavebufL,ClientWidth);
  setlength(wavebufR,ClientWidth);

  //init vars
  loop[0] := 0;
  loop[1] := 0;
  
  //init BASS
  if not BASS_Init(-1,44100,0,Application.Handle,nil) then
    ErrorPop('Can''t initialize device');
  
  //init timer for updating
  Timer1.Interval := 20; //ms
  Timer1.Enabled := true;

  //main start play function
  if not PlayFile then
  begin
    BASS_Free();
    Application.Terminate;
  end;  
end;

function TForm1.PlayFile : boolean;
var
  filename : string;
  data : array[0..2000] of SmallInt;
  i : integer;
begin
  result := false;
  if OpenDialog1.Execute then
  begin
    filename := OpenDialog1.Filename;
    BringWindowToTop(Form1.Handle);
    SetForegroundWindow(Form1.Handle);

    //creating stream
    chan := BASS_StreamCreateFile(FALSE,pchar(filename),0,0,0 {$IFDEF UNICODE} or BASS_UNICODE {$ENDIF});
    if chan = 0 then
    begin
      chan := BASS_MusicLoad(False, pchar(filename), 0, 0, BASS_MUSIC_RAMPS or BASS_MUSIC_POSRESET or BASS_MUSIC_PRESCAN {$IFDEF UNICODE} or BASS_UNICODE {$ENDIF}, 1);
      if (chan = 0) then
      begin
        ErrorPop('Can''t play file');
        Exit;
      end;
    end;

    //playing stream and setting global vars
    for i:=0 to length(data)-1 do data[0] := 0;
    bpp := BASS_ChannelGetLength(chan,BASS_POS_BYTE) div ClientWidth; // stream bytes per pixel
    if (bpp < BASS_ChannelSeconds2Bytes(chan,0.02)) then // minimum 20ms per pixel (BASS_ChannelGetLevel scans 20ms)
      bpp := BASS_ChannelSeconds2Bytes(chan,0.02);
    BASS_ChannelSetSync(chan,BASS_SYNC_END or BASS_SYNC_MIXTIME,0,LoopSyncProc, nil); // set sync to loop at end
    BASS_ChannelPlay(chan,FALSE); // start playing

    //getting peak levels in seperate thread, stream handle as parameter
		chan2 := BASS_StreamCreateFile(FALSE,pchar(filename),0,0,BASS_STREAM_DECODE {$IFDEF UNICODE} or BASS_UNICODE {$ENDIF});
		if (chan2 = 0) then chan2 := BASS_MusicLoad(FALSE,pchar(filename),0,0,BASS_MUSIC_DECODE {$IFDEF UNICODE} or BASS_UNICODE {$ENDIF},1);
    TScanThread.Create(chan2); // start scanning peaks in a new thread
    result := true;
  end;
end;

procedure TForm1.DrawSpectrum;
var
  i,ht : integer;
begin
  //clear background
  Buffer.Canvas.Brush.Color := clBlack;
  Buffer.Canvas.FillRect(Rect(0,0,Buffer.Width,Buffer.Height));

  //draw peaks
  ht := ClientHeight div 2;
  for i:=0 to length(wavebufL)-1 do
  begin
    Buffer.Canvas.MoveTo(i,ht);
	  Buffer.Canvas.Pen.Color := clLime;
    Buffer.Canvas.LineTo(i,ht-trunc((wavebufL[i]/32768)*ht));
    Buffer.Canvas.Pen.Color := clLime;
    Buffer.Canvas.MoveTo(i,ht+2);
	  Buffer.Canvas.LineTo(i,ht+2+trunc((wavebufR[i]/32768)*ht));
  end;
end;

procedure TForm1.DrawTime_Line(position : QWORD; y : integer; cl : TColor);
var
  sectime : integer;
  str : string;
  x : integer;
begin
  sectime := trunc(BASS_ChannelBytes2Seconds(chan,position));
  x := position div bpp;

  //format time
  str := '';
  if (sectime mod 60 < 10) then str := '0';
  str := str+inttostr(sectime mod 60);
  str := inttostr(sectime div 60)+':'+str;

  //drawline
  Buffer.Canvas.Pen.Color := cl;
  Buffer.Canvas.MoveTo(x,0);
  Buffer.Canvas.LineTo(x,ClientHeight);

  //drawtext
  Buffer.Canvas.Font.Color := cl;
  Buffer.Canvas.Font.Style := [fsBold];
  if x > ClientWidth-20 then
    dec(x,40);
  SetBkMode(Buffer.Canvas.Handle,TRANSPARENT);
  Buffer.Canvas.TextOut(x+2,y,str);
end;               

procedure TForm1.ErrorPop(str:string);
begin
  //show last BASS errorcode when no argument is given, else show given text.
  if str = '' then
    Showmessage('Error code: '+inttostr(BASS_ErrorGetCode()))
  else
    Showmessage(str);
  Application.Terminate;
end;

procedure TForm1.SetLoopStart(position : qword);
begin
  loop[0] := position;
end;

procedure TForm1.SetLoopEnd(position : qword);
begin
  loop[1] := position;
  BASS_ChannelRemoveSync(chan,lsync); // remove old sync
  lsync := BASS_ChannelSetSync(chan,BASS_SYNC_POS or BASS_SYNC_MIXTIME,loop[1],LoopSyncProc, nil); // set new sync
end;

procedure LoopSyncProc(handle: HSYNC; channel, data: DWORD; user: Pointer); stdcall;
begin
	if not BASS_ChannelSetPosition(channel,loop[0],BASS_POS_BYTE) then // try seeking to loop start
		BASS_ChannelSetPosition(channel,0,BASS_POS_BYTE); // failed, go to start of file instead
end;

procedure TForm1.ScanPeaks2(decoder : HSTREAM);
var
  cpos,level : DWord;
  peak : array[0..1] of DWORD;
  position : DWORD;
  counter : integer;
begin
  cpos := 0;
  peak[0] := 0;
  peak[1] := 0;
  counter := 0;
  
  while not killscan do
  begin
    level := BASS_ChannelGetLevel(decoder); // scan peaks
    if (peak[0]<LOWORD(level)) then
      peak[0]:=LOWORD(level); // set left peak
		if (peak[1]<HIWORD(level)) then
      peak[1]:=HIWORD(level); // set right peak
    if BASS_ChannelIsActive(decoder) <> BASS_ACTIVE_PLAYING then
    begin
      position := cardinal(-1); // reached the end
		end else
      position := BASS_ChannelGetPosition(decoder,BASS_POS_BYTE) div bpp;

    if position > cpos then
    begin
      inc(counter);
      if counter <= length(wavebufL)-1 then
      begin
        wavebufL[counter] := peak[0];
        wavebufR[counter] := peak[1];
      end;

      if (position >= dword(ClientWidth)) then
        break;
      cpos := position;
     end;


    peak[0] := 0;
    peak[1] := 0;
  end;
  BASS_StreamFree(decoder); // free the decoder
end;

//------------------------------------------------------------------------------

{ TScanThread }

constructor TScanThread.Create(decoder: HSTREAM);
begin
  inherited create(false);
  Priority := tpNormal;
  FreeOnTerminate := true;
  FDecoder := decoder;
end;

procedure TScanThread.Execute;
begin
  inherited;
  Form1.ScanPeaks2(FDecoder);
  Terminate;
end;

//------------------------------------------------------------------------------

procedure TForm1.Timer1Timer(Sender: TObject);
begin
  if bpp = 0 then exit;
  DrawSpectrum; // draw peak waveform
  DrawTime_Line(loop[0],12,TColor($FFFF00)); // loop start
  DrawTime_Line(loop[1],24,TColor($00FFFF)); // loop end
  DrawTime_Line(BASS_ChannelGetPosition(chan,BASS_POS_BYTE),0,TColor($FFFFFF)); // current pos
  PB.Refresh;
end;

procedure TForm1.FormMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  if Button = mbLeft then
  begin
    mousedwn := 1;
    SetLoopStart(dword(x)*bpp)
  end
  else if Button = mbRight then
  begin
    mousedwn := 2;
    SetLoopEnd(dword(x)*bpp);
  end;
end;

procedure TForm1.FormMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
begin
  if mousedwn = 0 then
    exit;
  if mousedwn = 1 then
    SetLoopStart(dword(x)*bpp)
  else if mousedwn = 2 then
    SetLoopEnd(dword(x)*bpp);
end;

procedure TForm1.FormMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  mousedwn := 0;
end;

procedure TForm1.PBPaint(Sender: TObject);
begin
  if bpp = 0 then exit;
  PB.Canvas.Draw(0,0,Buffer);
end;

procedure TForm1.FormKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  if key = 27 then
    Application.Terminate;
end;

procedure TForm1.FormCloseQuery(Sender: TObject; var CanClose: Boolean);
begin
  Timer1.Enabled := false;
  bpp := 0;
  killscan := true;
  Buffer.Free;
  BASS_Free();
end;

end.
