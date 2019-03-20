unit Unit1;
{========================================================================
 Plugin Delphi Demo (translated from C PluginDemo Copyright by Ian Luck)
 by (Chris Troesken)(cst-tech@foni.net)http:www.cst-development.de


 Plugin Demo the shows how to get the Info about the supported loaded Plugins
 * Requires: BASS.DLL available @ www.un4seen.com
========================================================================}


interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  bass, ExtCtrls, StdCtrls, ComCtrls, Dialogs;

type
  TForm1 = class(TForm)
    Panel1: TPanel;
    GroupBox1: TGroupBox;
    Button1: TButton;
    open1: TOpenDialog;
    lblInfo: TLabel;
    ListView_PlugIns: TListView;
    Timer1: TTimer;
    ScrollBar1: TScrollBar;
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure ScrollBar1Scroll(Sender: TObject; ScrollCode: TScrollCode;
      var ScrollPos: Integer);
  private
    { Private-Deklarationen }
    Sliding: boolean;
    procedure Error(const Text: string);
    function GetCTypeString(ctype: DWORD; plugin: HPLUGIN): string;
  public
    { Public-Deklarationen }
  end;

var
  Form1: TForm1;
  Chan: HStream;

implementation

{$R *.dfm}

function ProgDir: string;
begin
  result := ExtractFilePath(ParamStr(0));
end;

function TForm1.GetCTypeString(ctype: DWORD; plugin: HPLUGIN): string;
var
  pInfo: ^BASS_PLUGININFO;
  a: integer;
begin
  if plugin <> 0 then
  begin
//    pInfo := BASS_PluginGetInfo(plugin);
    pInfo := pointer(BASS_PluginGetInfo(plugin));
    for a := 0 to PInfo.formatc - 1 do
      if pInfo.Formats[a].ctype = cType then
        Result := pInfo.Formats[a].name;
    end;
  // check built-in stream formats...
  case cType of
    BASS_CTYPE_STREAM_OGG: Result := Result + '"Ogg Vorbis"';
    BASS_CTYPE_STREAM_MP1: Result := Result + '"MPEG layer 1"';
    BASS_CTYPE_STREAM_MP2: Result := Result + '"MPEG layer 2"';
    BASS_CTYPE_STREAM_MP3: Result := Result + '"MPEG layer 3"';
    BASS_CTYPE_STREAM_AIFF: Result := Result + '"Audio IFF"';
    BASS_CTYPE_STREAM_WAV_PCM: Result := Result + '"PCM WAVE"';
    BASS_CTYPE_STREAM_WAV_FLOAT: Result := Result + '"Floating-point WAVE"';
    BASS_CTYPE_STREAM_WAV: Result := Result + '"Wave"';
  end;
end;

procedure TForm1.Error(const Text: string);
begin
  MessageBox(Handle, PChar(Text + '  Errorcode = ' +
    inttostr(Bass_ErrorGetCode)), PChar('Error'), 16);
end;

procedure TForm1.FormCreate(Sender: TObject);
var
  fd: TWin32FindData;
  fh: THandle;
  plug: HPLUGIN;
  Info: ^Bass_PluginInfo;
  s: string;
  a: integer;
  ListItem: TListItem;
begin
  if not Bass_Init(-1, 44100, 0, handle, nil) then
  begin
    Error('Can''t initialize device');
    Application.Terminate;
  end;
  // Set The OpenDialog Filter to the Bass build In Formats
  Open1.Filter :=
    'BASS built-in *.mp3;*.mp2;*.mp1;*.ogg;*.wav;*.aif)\0*.mp3;*.mp2;*.mp1;*.ogg;*.wav;*.aif' + '|'
    + '*.mp3;*.mp2;*.mp1;*.ogg;*.wav*;*.aif';

  // get the Modules from the currentDirectory
  fh := FindFirstFile(PChar(ProgDir + 'bass*.dll'), fd);
  if (fh <> INVALID_HANDLE_VALUE) then
  try
    repeat
      plug := BASS_PluginLoad(fd.cFileName, 0 {$IFDEF UNICODE} or BASS_UNICODE {$ENDIF});
      if Plug <> 0 then
      begin
//        Info := BASS_PluginGetInfo(Plug); // get plugin info to add to the file selector filter...
        Info := pointer(BASS_PluginGetInfo(Plug)); // get plugin info to add to the file selector filter...
        for a := 0 to Info.formatc - 1 do
        begin
          // Set The OpenDialog additional, to the supported PlugIn Formats
          open1.Filter := Open1.Filter + '|' + Info.Formats[a].name + ' ' + '(' +
            Info.Formats[a].exts + ') , ' + fd.cFileName + '|' + Info.Formats[a].exts;
          // Lets Show in the Listview the supported Formats

          if (Info.Formats[a].name <> nil) then
            if (a = 0) then
              s :=Info.Formats[a].name
            else
              s := s + ',' + Info.Formats[a].name;
        end;
        ListItem := ListView_PlugIns.Items.Add;
        ListItem.Caption := fd.cFileName;
        ListItem.SubItems.Add(s);
      end;
    until FindNextFile(fh, fd) = false;
  finally
    Windows.FindClose(fh);
  end;

  if Listview_PlugIns.Items.Count = 0 then
  begin
    ListItem := ListView_PlugIns.Items.Add;
    ListItem.SubItems.Add('no plugins found - visit the BASS webpage to get some');
  end;
end;

procedure TForm1.FormDestroy(Sender: TObject);
begin
  Bass_Free();
  BASS_PluginFree(0);
end;

procedure TForm1.Button1Click(Sender: TObject);
var
  Info: Bass_ChannelInfo;
begin
  if open1.Execute then
  begin
    Timer1.Enabled := false;
    Bass_StreamFree(Chan);
    Chan := Bass_StreamCreateFile(false, PChar(open1.FileName), 0, 0, BASS_SAMPLE_LOOP {$IFDEF UNICODE} or BASS_UNICODE {$ENDIF});
    if CHan <> 0 then
    begin
      BASS_ChannelGetInfo(chan, info);
      Button1.Caption := open1.FileName;
      lblInfo.Caption := 'channel type = ' + inttostr(info.ctype) + ' ' +
        getCTypeString(info.ctype, info.plugin);
      Scrollbar1.Max := round(BASS_ChannelBytes2Seconds(chan, BASS_ChannelGetLength(chan, BASS_POS_BYTE)));
      Bass_ChannelPlay(Chan, false);
      Timer1.Enabled := true;

    end
    else
    begin
      Button1.Caption := 'Click here to open a file';
      Error('Can''t play the file');
    end;
  end;
end;

procedure TForm1.Timer1Timer(Sender: TObject);
begin
  if not Sliding then
    Scrollbar1.Position := round(BASS_ChannelBytes2Seconds(chan, BASS_ChannelGetPosition(chan, BASS_POS_BYTE)));
end;

procedure TForm1.ScrollBar1Scroll(Sender: TObject; ScrollCode: TScrollCode;
  var ScrollPos: Integer);
begin
  Sliding := Scrollcode <> scEndScroll;
  if ScrollCode = scEndScroll then
    BASS_ChannelSetPosition(chan, BASS_ChannelSeconds2Bytes(chan, Scrollbar1.position), BASS_POS_BYTE);
end;

end.

