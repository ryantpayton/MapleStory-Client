object Form1: TForm1
  Left = 238
  Top = 110
  Width = 206
  Height = 230
  Caption = 'BASS FX Demo'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 176
    Width = 25
    Height = 13
    Alignment = taCenter
    AutoSize = False
    Caption = '125'
    Transparent = True
  end
  object Label2: TLabel
    Left = 56
    Top = 176
    Width = 25
    Height = 17
    Alignment = taCenter
    AutoSize = False
    Caption = '1000'
    Transparent = True
  end
  object Label3: TLabel
    Left = 96
    Top = 176
    Width = 25
    Height = 17
    Alignment = taCenter
    AutoSize = False
    Caption = '8000'
    Transparent = True
  end
  object Label5: TLabel
    Left = 152
    Top = 176
    Width = 25
    Height = 17
    Alignment = taCenter
    AutoSize = False
    Caption = '0'
    Transparent = True
  end
  object Label4: TLabel
    Left = 24
    Top = 56
    Width = 97
    Height = 17
    Alignment = taCenter
    AutoSize = False
    Caption = 'Equalizer'
    Transparent = True
  end
  object Label6: TLabel
    Left = 140
    Top = 56
    Width = 49
    Height = 17
    Alignment = taCenter
    AutoSize = False
    Caption = 'Reverb'
    Transparent = True
  end
  object Button1: TButton
    Left = 16
    Top = 8
    Width = 169
    Height = 41
    Caption = 'Click here to Open and Play'
    TabOrder = 0
    WordWrap = True
    OnClick = Button1Click
  end
  object TrackBar1: TTrackBar
    Left = 16
    Top = 72
    Width = 33
    Height = 105
    Max = 30
    Orientation = trVertical
    Position = 15
    TabOrder = 1
    OnChange = TrackBar1Change
  end
  object TrackBar2: TTrackBar
    Tag = 1
    Left = 56
    Top = 72
    Width = 33
    Height = 105
    Max = 30
    Orientation = trVertical
    Position = 15
    TabOrder = 2
    OnChange = TrackBar2Change
  end
  object TrackBar3: TTrackBar
    Tag = 2
    Left = 96
    Top = 72
    Width = 33
    Height = 105
    Max = 30
    Orientation = trVertical
    Position = 15
    TabOrder = 3
    OnChange = TrackBar3Change
  end
  object TrackBar4: TTrackBar
    Tag = 3
    Left = 152
    Top = 72
    Width = 33
    Height = 105
    Max = 20
    Orientation = trVertical
    Position = 20
    TabOrder = 4
    OnChange = TrackBar4Change
  end
  object OpenDialog1: TOpenDialog
    Filter = 
      'Playable files|*.mo3;*.xm;*.mod;*.s3m;*.it;*.mtm;*.umx;*.mp3;*.mp2;*.mp1;*.ogg;*.wav;*.aif|All files|*' +
      '.*'
    Options = [ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Top = 72
  end
end
