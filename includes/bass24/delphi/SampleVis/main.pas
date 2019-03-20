{
 Source code under Bass license
 by Alessandro Cappellozza

 http://digilander.libero.it/Kappe
 mail acappellozza@ieee.org

 Notice
  The body (this unit) only drive the classes and Bass,
  the rest is wrtitten in OOD for separate the draw code
 }

unit main;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, osc_vis, spectrum_vis,
  Dialogs, ExtCtrls, StdCtrls, ComCtrls, circle_vis, Bass, CommonTypes, ExtDlgs;

type
  TFormPlayer = class(TForm)
    Button1: TButton;
    OpenDialog: TOpenDialog;
    PaintFrame: TPaintBox;
    TimerRender: TTimer;
    BackImageRes: TImage;
    Button2: TButton;
    RadioMode: TRadioGroup;
    RadioDraw: TRadioGroup;
    GroupBox1: TGroupBox;
    TrackOff: TTrackBar;
    TrackRes: TTrackBar;
    TrackY: TTrackBar;
    TrackX: TTrackBar;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    PanelBkg: TPanel;
    PanelPen: TPanel;
    Label6: TLabel;
    Label7: TLabel;
    ColorDialog: TColorDialog;
    GroupBox2: TGroupBox;
    TrackRad: TTrackBar;
    Label5: TLabel;
    GroupBox3: TGroupBox;
    SpecTrackWidth: TTrackBar;
    Label8: TLabel;
    PanelPeakColor: TPanel;
    Label9: TLabel;
    Label10: TLabel;
    Label11: TLabel;
    Button3: TButton;
    CheckSpecPeaks: TCheckBox;
    OpenPictureDialog: TOpenPictureDialog;
    SpinLineFall: TUpDown;
    SpinPeakFall: TUpDown;
    EditLineFall: TEdit;
    EditPeakFall: TEdit;
    procedure FormCreate(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure TimerRenderTimer(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure RadioModeClick(Sender: TObject);
    procedure TrackOffChange(Sender: TObject);
    procedure TrackResChange(Sender: TObject);
    procedure TrackRadChange(Sender: TObject);
    procedure PanelBkgClick(Sender: TObject);
    procedure PanelPenClick(Sender: TObject);
    procedure SpecTrackWidthChange(Sender: TObject);
    procedure PanelPeakColorClick(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure CheckSpecPeaksClick(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure SpinLineFallClick(Sender: TObject; Button: TUDBtnType);
    procedure SpinPeakFallClick(Sender: TObject; Button: TUDBtnType);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  FormPlayer : TFormPlayer;
  Channel    : DWORD;

implementation

{$R *.dfm}

procedure TFormPlayer.FormCreate(Sender: TObject);
begin
  BASS_Init(-1, 44100, 0, Application.Handle, nil);

  CircleScope := TCircleScope.Create(PaintFrame.Width, PaintFrame.Height);
  OcilloScope := TOcilloScope.Create(PaintFrame.Width, PaintFrame.Height);
  Spectrum    := TSpectrum.Create(PaintFrame.Width, PaintFrame.Height);
end;


procedure TFormPlayer.Button1Click(Sender: TObject);
begin
  OpenDialog.Title  := 'Open Files';
  OpenDialog.Filter := 'mp3|*.mp3';
  if not OpenDialog.Execute then exit;
    Channel := BASS_StreamCreateFile(FALSE, PChar(OpenDialog.FileName), 0, 0, 0 {$IFDEF UNICODE} or BASS_UNICODE {$ENDIF});
    BASS_ChannelPlay(Channel, False);
end;

procedure TFormPlayer.TimerRenderTimer(Sender: TObject);
 var FFTFata : TFFTData; WaveData  : TWaveData;
begin

 if BASS_ChannelIsActive(Channel) <> BASS_ACTIVE_PLAYING then Exit;

case RadioDraw.ItemIndex of
  0 :
   begin
    BASS_ChannelGetData(Channel, @WaveData, 2048);
    CircleScope.Draw (PaintFrame.Canvas.Handle, WaveData, TrackX.Position, TrackY.Position);
   end;

  1 :
   begin
    BASS_ChannelGetData(Channel, @WaveData, 2048);
    OcilloScope.Draw (PaintFrame.Canvas.Handle, WaveData, TrackX.Position - 50, TrackY.Position);
   end;

  2 :
   begin
    BASS_ChannelGetData(Channel, @FFTFata, BASS_DATA_FFT1024);
    Spectrum.Draw (PaintFrame.Canvas.Handle, FFTFata, TrackX.Position - 40, TrackY.Position - 60);
   end;
end;
end;

procedure TFormPlayer.Button2Click(Sender: TObject);
begin
 OpenPictureDialog.Title  := 'Open Background Bitmap';
 OpenPictureDialog.Filter := 'BMP|*.bmp';
 if not OpenPictureDialog.Execute then exit;
 BackImageRes.Picture.LoadFromFile(OpenPictureDialog.FileName);

 CircleScope.SetBackGround(True, BackImageRes.Picture.Graphic);
 CircleScope.Pen := clBlack;

 OcilloScope.SetBackGround(True, BackImageRes.Picture.Graphic);
 OcilloScope.Pen := clBlack;

 Spectrum.SetBackGround(True, BackImageRes.Picture.Graphic);
 Spectrum.Pen := clBlack;
end;

procedure TFormPlayer.RadioModeClick(Sender: TObject);
begin
  CircleScope.Mode := RadioMode.ItemIndex;
  OcilloScope.Mode := RadioMode.ItemIndex;
  Spectrum.Mode := RadioMode.ItemIndex;
end;

procedure TFormPlayer.TrackOffChange(Sender: TObject);
begin
 CircleScope.Offset := TrackOff.Position;
 OcilloScope.Offset := TrackOff.Position;
 Spectrum.Height := TrackOff.Position;
end;

procedure TFormPlayer.TrackResChange(Sender: TObject);
begin
 CircleScope.Res := TrackRes.Position;
 OcilloScope.Res := TrackRes.Position;
 Spectrum.Res := TrackRes.Position;
end;

procedure TFormPlayer.TrackRadChange(Sender: TObject);
begin
 CircleScope.Radius := TrackRad.Position;
end;

procedure TFormPlayer.PanelBkgClick(Sender: TObject);
begin
 ColorDialog.Color := PanelBkg.Color;
  if not ColorDialog.Execute then Exit;
   PanelBkg.Color := ColorDialog.Color;
    CircleScope.BackColor := ColorDialog.Color;
    OcilloScope.BackColor := ColorDialog.Color;
    Spectrum.BackColor := ColorDialog.Color;
end;

procedure TFormPlayer.PanelPenClick(Sender: TObject);
begin
 ColorDialog.Color := PanelPen.Color;
  if not ColorDialog.Execute then Exit;
   PanelPen.Color := ColorDialog.Color;
    CircleScope.Pen := ColorDialog.Color;
    OcilloScope.Pen := ColorDialog.Color;
    Spectrum.Pen := ColorDialog.Color;
end;

procedure TFormPlayer.SpecTrackWidthChange(Sender: TObject);
begin
  Spectrum.Width := SpecTrackWidth.Position;
end;

procedure TFormPlayer.PanelPeakColorClick(Sender: TObject);
begin
 ColorDialog.Color := PanelPeakColor.Color;
  if not ColorDialog.Execute then Exit;
   PanelPeakColor.Color := ColorDialog.Color;
    Spectrum.Peak := ColorDialog.Color;
end;

procedure TFormPlayer.Button3Click(Sender: TObject);
begin
 CircleScope.SetBackGround(False, BackImageRes.Picture.Graphic);
 CircleScope.Pen := PanelPen.Color;

 OcilloScope.SetBackGround(False, BackImageRes.Picture.Graphic);
 OcilloScope.Pen := PanelPen.Color;

 Spectrum.SetBackGround(False, BackImageRes.Picture.Graphic);
 Spectrum.Pen := PanelPen.Color;
end;

procedure TFormPlayer.CheckSpecPeaksClick(Sender: TObject);
begin
Spectrum.DrawPeak := CheckSpecPeaks.Checked;
end;

procedure TFormPlayer.FormClose(Sender: TObject; var Action: TCloseAction);
begin
  Bass_Free;
end;

procedure TFormPlayer.SpinLineFallClick(Sender: TObject; Button: TUDBtnType);
begin
 Spectrum.LineFallOff := SpinLineFall.Position;
end;

procedure TFormPlayer.SpinPeakFallClick(Sender: TObject; Button: TUDBtnType);
begin
 Spectrum.PeakFallOff := SpinPeakFall.Position;
end;

end.
